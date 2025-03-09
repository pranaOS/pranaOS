/**
 * @file debugsession.cpp
 * @author Krisna Pranav
 * @brief debug session
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#include "debugsession.h"
#include <mods/jsonobject.h>
#include <mods/jsonvalue.h>
#include <mods/lexicalpath.h>
#include <mods/optional.h>
#include <mods/platform.h>
#include <libcore/file.h>
#include <libregex/regex.h>
#include <stdlib.h>
#include <sys/mman.h>

namespace Debug 
{

    /**
     * @brief Construct a new DebugSession::DebugSession object
     * 
     * @param pid 
     * @param source_root 
     */
    DebugSession::DebugSession(pid_t pid, String source_root)
        : m_debuggee_pid(pid)
        , m_source_root(source_root)

    {
    }

    /**
     * @brief Destroy the DebugSession::DebugSession object
     * 
     */
    DebugSession::~DebugSession()
    {
        if (m_is_debuggee_dead)
            return;

        for (auto const& bp : m_breakpoints) {
            disable_breakpoint(bp.key);
        }
        m_breakpoints.clear();

        for (auto const& wp : m_watchpoints) {
            disable_watchpoint(wp.key);
        }
        m_watchpoints.clear();

        if (ptrace(PT_DETACH, m_debuggee_pid, 0, 0) < 0) {
            perror("PT_DETACH");
        }
    }

    /**
     * @param func 
     */
    void DebugSession::for_each_loaded_library(Function<IterationDecision(LoadedLibrary const&)> func) const
    {
        for (auto const& lib_name : m_loaded_libraries.keys()) {
            auto const& lib = *m_loaded_libraries.get(lib_name).value();
            if (func(lib) == IterationDecision::Break)
                break;
        }
    }

    /**
     * @param command 
     * @param source_root 
     * @param setup_child 
     * @return OwnPtr<DebugSession> 
     */
    OwnPtr<DebugSession> DebugSession::exec_and_attach(String const& command,
        String source_root,
        Function<ErrorOr<void>()> setup_child)
    {
        auto pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(1);
        }

        if (!pid) {

            if (setup_child) {
                if (setup_child().is_error()) {
                    perror("DebugSession::setup_child");
                    exit(1);
                }
            }

            if (ptrace(PT_TRACE_ME, 0, 0, 0) < 0) {
                perror("PT_TRACE_ME");
                exit(1);
            }

            auto parts = command.split(' ');
            VERIFY(!parts.is_empty());
            char const** args = bit_cast<char const**>(calloc(parts.size() + 1, sizeof(char const*)));
            for (size_t i = 0; i < parts.size(); i++) {
                args[i] = parts[i].characters();
            }
            char const** envp = bit_cast<char const**>(calloc(2, sizeof(char const*)));

            envp[0] = "_LOADER_BREAKPOINT=1";
            int rc = execvpe(args[0], const_cast<char**>(args), const_cast<char**>(envp));
            if (rc < 0) {
                perror("execvp");
                exit(1);
            }
        }

        if (waitpid(pid, nullptr, WSTOPPED) != pid) {
            perror("waitpid");
            return {};
        }

        if (ptrace(PT_ATTACH, pid, 0, 0) < 0) {
            perror("PT_ATTACH");
            return {};
        }

        if (waitpid(pid, nullptr, WSTOPPED) != pid) {
            perror("wait_pid");
            return {};
        }

        auto debug_session = adopt_own(*new DebugSession(pid, source_root));

        int wstatus = debug_session->continue_debuggee_and_wait();
        if (WSTOPSIG(wstatus) != SIGTRAP) {
            dbgln("expected SIGTRAP");
            return {};
        }

        debug_session->update_loaded_libs();

        return debug_session;
    }

    /**
     * @param address 
     * @param data 
     * @return true 
     * @return false 
     */
    bool DebugSession::poke(FlatPtr address, FlatPtr data)
    {
        if (ptrace(PT_POKE, m_debuggee_pid, bit_cast<void*>(address), bit_cast<void*>(data)) < 0) {
            perror("PT_POKE");
            return false;
        }
        return true;
    }

    /**
     * @param address 
     * @return Optional<FlatPtr> 
     */
    Optional<FlatPtr> DebugSession::peek(FlatPtr address) const
    {
        Optional<FlatPtr> result;
        auto rc = ptrace(PT_PEEK, m_debuggee_pid, bit_cast<void*>(address), nullptr);
        if (errno == 0)
            result = static_cast<FlatPtr>(rc);
        return result;
    }

    /**
     * @param register_index 
     * @param data 
     * @return true 
     * @return false 
     */
    bool DebugSession::poke_debug(u32 register_index, FlatPtr data) const
    {
        if (ptrace(PT_POKEDEBUG, m_debuggee_pid, bit_cast<void*>(static_cast<FlatPtr>(register_index)), bit_cast<void*>(data)) < 0) {
            perror("PT_POKEDEBUG");
            return false;
        }
        return true;
    }

    /**
     * @param register_index 
     * @return Optional<FlatPtr> 
     */
    Optional<FlatPtr> DebugSession::peek_debug(u32 register_index) const
    {
        auto rc = ptrace(PT_PEEKDEBUG, m_debuggee_pid, bit_cast<void*>(static_cast<FlatPtr>(register_index)), nullptr);
        if (errno == 0)
            return static_cast<FlatPtr>(rc);

        return {};
    }

    /**
     * @param address 
     * @return true 
     * @return false 
     */
    bool DebugSession::insert_breakpoint(FlatPtr address)
    {
        if (m_breakpoints.contains(address))
            return false;

        auto original_bytes = peek(address);

        if (!original_bytes.has_value())
            return false;

        VERIFY((original_bytes.value() & 0xff) != BREAKPOINT_INSTRUCTION);

        BreakPoint breakpoint { address, original_bytes.value(), BreakPointState::Disabled };

        m_breakpoints.set(address, breakpoint);

        enable_breakpoint(breakpoint.address);

        return true;
    }

    /**
     * @param address 
     * @return true 
     * @return false 
     */
    bool DebugSession::disable_breakpoint(FlatPtr address)
    {
        auto breakpoint = m_breakpoints.get(address);
        VERIFY(breakpoint.has_value());
        if (!poke(breakpoint.value().address, breakpoint.value().original_first_word))
            return false;

        auto bp = m_breakpoints.get(breakpoint.value().address).value();
        bp.state = BreakPointState::Disabled;
        m_breakpoints.set(bp.address, bp);
        return true;
    }

    /**
     * @param address 
     * @return true 
     * @return false 
     */
    bool DebugSession::enable_breakpoint(FlatPtr address)
    {
        auto breakpoint = m_breakpoints.get(address);
        VERIFY(breakpoint.has_value());

        VERIFY(breakpoint.value().state == BreakPointState::Disabled);

        if (!poke(breakpoint.value().address, (breakpoint.value().original_first_word & ~static_cast<FlatPtr>(0xff)) | BREAKPOINT_INSTRUCTION))
            return false;

        auto bp = m_breakpoints.get(breakpoint.value().address).value();
        bp.state = BreakPointState::Enabled;
        m_breakpoints.set(bp.address, bp);
        return true;
    }

    /**
     * @param address 
     * @return true 
     * @return false 
     */
    bool DebugSession::remove_breakpoint(FlatPtr address)
    {
        if (!disable_breakpoint(address))
            return false;

        m_breakpoints.remove(address);
        return true;
    }

    /**
     * @param address 
     * @return true 
     * @return false 
     */
    bool DebugSession::breakpoint_exists(FlatPtr address) const
    {
        return m_breakpoints.contains(address);
    }

    /**
     * @param address 
     * @param ebp 
     * @return true 
     * @return false 
     */
    bool DebugSession::insert_watchpoint(FlatPtr address, u32 ebp)
    {
        auto current_register_status = peek_debug(DEBUG_CONTROL_REGISTER);
        if (!current_register_status.has_value())
            return false;
        
        u32 dr7_value = static_cast<u32>(current_register_status.value());
        u32 next_available_index;
        for (next_available_index = 0; next_available_index < 4; next_available_index++) {
            auto bitmask = 1 << (next_available_index * 2);
            if ((dr7_value & bitmask) == 0)
                break;
        }
        if (next_available_index > 3)
            return false;
        WatchPoint watchpoint { address, next_available_index, ebp };

        if (!poke_debug(next_available_index, bit_cast<FlatPtr>(address)))
            return false;

        dr7_value |= (1u << (next_available_index * 2)); 
        auto condition_shift = 16 + (next_available_index * 4);
        dr7_value &= ~(0b11u << condition_shift);
        dr7_value |= 1u << condition_shift; 
        auto length_shift = 18 + (next_available_index * 4);
        dr7_value &= ~(0b11u << length_shift);
        
        dr7_value |= 0b11u << length_shift; 
        if (!poke_debug(DEBUG_CONTROL_REGISTER, dr7_value))
            return false;

        m_watchpoints.set(address, watchpoint);
        return true;
    }

    /**
     * @param address 
     * @return true 
     * @return false 
     */
    bool DebugSession::remove_watchpoint(FlatPtr address)
    {
        if (!disable_watchpoint(address))
            return false;
        return m_watchpoints.remove(address);
    }

    /**
     * @param address 
     * @return true 
     * @return false 
     */
    bool DebugSession::disable_watchpoint(FlatPtr address)
    {
        VERIFY(watchpoint_exists(address));
        auto watchpoint = m_watchpoints.get(address).value();
        if (!poke_debug(watchpoint.debug_register_index, 0))
            return false;
        auto current_register_status = peek_debug(DEBUG_CONTROL_REGISTER);
        if (!current_register_status.has_value())
            return false;
        u32 dr7_value = current_register_status.value();
        dr7_value &= ~(1u << watchpoint.debug_register_index * 2);
        if (!poke_debug(watchpoint.debug_register_index, dr7_value))
            return false;
        return true;
    }

    /**
     * @param address 
     * @return true 
     * @return false 
     */
    bool DebugSession::watchpoint_exists(FlatPtr address) const
    {
        return m_watchpoints.contains(address);
    }

    /**
     * @return PtraceRegisters 
     */
    PtraceRegisters DebugSession::get_registers() const
    {
        PtraceRegisters regs;
        if (ptrace(PT_GETREGS, m_debuggee_pid, &regs, 0) < 0) {
            perror("PT_GETREGS");
            VERIFY_NOT_REACHED();
        }
        return regs;
    }

    /**
     * @param regs 
     */
    void DebugSession::set_registers(PtraceRegisters const& regs)
    {
        if (ptrace(PT_SETREGS, m_debuggee_pid, bit_cast<void*>(&regs), 0) < 0) {
            perror("PT_SETREGS");
            VERIFY_NOT_REACHED();
        }
    }

    /**
     * @param type 
     */
    void DebugSession::continue_debuggee(ContinueType type)
    {
        int command = (type == ContinueType::FreeRun) ? PT_CONTINUE : PT_SYSCALL;
        if (ptrace(command, m_debuggee_pid, 0, 0) < 0) {
            perror("continue");
            VERIFY_NOT_REACHED();
        }
    }

    /**
     * @param type 
     * @return int 
     */
    int DebugSession::continue_debuggee_and_wait(ContinueType type)
    {
        continue_debuggee(type);
        int wstatus = 0;
        if (waitpid(m_debuggee_pid, &wstatus, WSTOPPED | WEXITED) != m_debuggee_pid) {
            perror("waitpid");
            VERIFY_NOT_REACHED();
        }
        return wstatus;
    }

    /**
     * @return FlatPtr 
     */
    FlatPtr DebugSession::single_step()
    {
        auto regs = get_registers();
        constexpr u32 TRAP_FLAG = 0x100;
    #if ARCH(I386)
        regs.eflags |= TRAP_FLAG;
    #else
        regs.rflags |= TRAP_FLAG;
    #endif
        set_registers(regs);

        continue_debuggee();

        if (waitpid(m_debuggee_pid, 0, WSTOPPED) != m_debuggee_pid) {
            perror("waitpid");
            VERIFY_NOT_REACHED();
        }

        regs = get_registers();
    #if ARCH(I386)
        regs.eflags &= ~(TRAP_FLAG);
    #else
        regs.rflags &= ~(TRAP_FLAG);
    #endif
        set_registers(regs);
        return regs.ip();
    }

    void DebugSession::detach()
    {
        for (auto& breakpoint : m_breakpoints.keys()) {
            remove_breakpoint(breakpoint);
        }
        for (auto& watchpoint : m_watchpoints.keys())
            remove_watchpoint(watchpoint);
        continue_debuggee();
    }

    /**
     * @param symbol_name 
     * @return Optional<DebugSession::InsertBreakpointAtSymbolResult> 
     */
    Optional<DebugSession::InsertBreakpointAtSymbolResult> DebugSession::insert_breakpoint(String const& symbol_name)
    {
        Optional<InsertBreakpointAtSymbolResult> result;
        for_each_loaded_library([this, symbol_name, &result](auto& lib) {
            if (lib.name == "Loader.so")
                return IterationDecision::Continue;

            auto symbol = lib.debug_info->elf().find_demangled_function(symbol_name);
            if (!symbol.has_value())
                return IterationDecision::Continue;

            FlatPtr breakpoint_address = symbol->value() + lib.base_address;
            bool rc = this->insert_breakpoint(breakpoint_address);
            if (!rc)
                return IterationDecision::Break;

            result = InsertBreakpointAtSymbolResult { lib.name, breakpoint_address };
            return IterationDecision::Break;
        });
        return result;
    }

    /**
     * @param filename 
     * @param line_number 
     * @return Optional<DebugSession::InsertBreakpointAtSourcePositionResult> 
     */
    Optional<DebugSession::InsertBreakpointAtSourcePositionResult> DebugSession::insert_breakpoint(String const& filename, size_t line_number)
    {
        auto address_and_source_position = get_address_from_source_position(filename, line_number);
        if (!address_and_source_position.has_value())
            return {};

        auto address = address_and_source_position.value().address;
        bool rc = this->insert_breakpoint(address);
        if (!rc)
            return {};

        auto lib = library_at(address);
        VERIFY(lib);

        return InsertBreakpointAtSourcePositionResult { lib->name, address_and_source_position.value().file, address_and_source_position.value().line, address };
    }

    void DebugSession::update_loaded_libs()
    {
        auto file = Core::File::construct(String::formatted("/proc/{}/vm", m_debuggee_pid));
        bool rc = file->open(Core::OpenMode::ReadOnly);
        VERIFY(rc);

        auto file_contents = file->read_all();
        auto json = JsonValue::from_string(file_contents).release_value_but_fixme_should_propagate_errors();

        auto const& vm_entries = json.as_array();
        Regex<PosixExtended> segment_name_re("(.+): ");

        auto get_path_to_object = [&segment_name_re](String const& vm_name) -> Optional<String> {
            if (vm_name == "/usr/lib/Loader.so")
                return vm_name;
            RegexResult result;
            auto rc = segment_name_re.search(vm_name, result);
            if (!rc)
                return {};
            auto lib_name = result.capture_group_matches.at(0).at(0).view.string_view().to_string();
            if (lib_name.starts_with("/"))
                return lib_name;
            return String::formatted("/usr/lib/{}", lib_name);
        };

        vm_entries.for_each([&](auto& entry) {
            auto vm_name = entry.as_object().get("name").as_string();

            auto object_path = get_path_to_object(vm_name);
            if (!object_path.has_value())
                return IterationDecision::Continue;

            String lib_name = object_path.value();
            if (Core::File::looks_like_shared_library(lib_name))
                lib_name = LexicalPath::basename(object_path.value());

            FlatPtr base_address = entry.as_object().get("address").to_addr();
            if (auto it = m_loaded_libraries.find(lib_name); it != m_loaded_libraries.end()) {
                
                VERIFY(base_address >= it->value->base_address);
                return IterationDecision::Continue;
            }

            auto file_or_error = Core::MappedFile::map(object_path.value());
            if (file_or_error.is_error())
                return IterationDecision::Continue;

            auto image = make<ELF::Image>(file_or_error.value()->bytes());
            auto debug_info = make<DebugInfo>(*image, m_source_root, base_address);
            auto lib = make<LoadedLibrary>(lib_name, file_or_error.release_value(), move(image), move(debug_info), base_address);
            m_loaded_libraries.set(lib_name, move(lib));

            return IterationDecision::Continue;
        });
    }

} // namespace Debug
