/**
 * @file debugsession.h
 * @author Krisna Pranav
 * @brief debug session
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

#include <mods/demangle.h>
#include <mods/function.h>
#include <mods/hashmap.h>
#include <mods/nonnullrefptr.h>
#include <mods/optional.h>
#include <mods/ownptr.h>
#include <mods/string.h>
#include <libc/sys/arch/i386/regs.h>
#include <libcore/mappedfile.h>
#include <libdebug/debuginfo.h>
#include <libdebug/processinspector.h>
#include <signal.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>

namespace Debug 
{

    class DebugSession : public ProcessInspector 
    {
    public:
        /**
         * @param command 
         * @param source_root 
         * @param setup_child 
         * @return OwnPtr<DebugSession> 
         */
        static OwnPtr<DebugSession> exec_and_attach(String const& command, String source_root = {}, Function<ErrorOr<void>()> setup_child = {});

        virtual ~DebugSession() override;

        /**
         * @param address 
         * @param data 
         * @return true 
         * @return false 
         */
        virtual bool poke(FlatPtr address, FlatPtr data) override;

        /**
         * @param address 
         * @return Optional<FlatPtr> 
         */
        virtual Optional<FlatPtr> peek(FlatPtr address) const override;

        /**
         * @brief Get the registers object
         * 
         * @return PtraceRegisters 
         */
        virtual PtraceRegisters get_registers() const override;
        
        virtual void set_registers(PtraceRegisters const&) override;

        virtual void for_each_loaded_library(Function<IterationDecision(LoadedLibrary const&)>) const override;

        /**
         * @return int 
         */
        int pid() const 
        { 
            return m_debuggee_pid; 
        }

        /**
         * @param register_index 
         * @param data 
         * @return true 
         * @return false 
         */
        bool poke_debug(u32 register_index, FlatPtr data) const;

        /**
         * @param register_index 
         * @return Optional<FlatPtr> 
         */
        Optional<FlatPtr> peek_debug(u32 register_index) const;

        enum class BreakPointState {
            Enabled,
            Disabled,
        }; // enum class BreakPointState

        struct BreakPoint {
            FlatPtr address { 0 };
            FlatPtr original_first_word { 0 };
            BreakPointState state { BreakPointState::Disabled };
        }; // struct BreakPoint

        struct InsertBreakpointAtSymbolResult {
            String library_name;
            FlatPtr address { 0 };
        }; // struct InsertBreakpointAtSymbolResult

        /**
         * @param symbol_name 
         * @return Optional<InsertBreakpointAtSymbolResult> 
         */
        Optional<InsertBreakpointAtSymbolResult> insert_breakpoint(String const& symbol_name);

        struct InsertBreakpointAtSourcePositionResult {
            String library_name;
            String filename;
            size_t line_number { 0 };
            FlatPtr address { 0 };
        }; // struct InsertBreakpointAtSourcePositionResult

        /**
         * @param filename 
         * @param line_number 
         * @return Optional<InsertBreakpointAtSourcePositionResult> 
         */
        Optional<InsertBreakpointAtSourcePositionResult> insert_breakpoint(String const& filename, size_t line_number);

        /**
         * @param address 
         * @return true 
         * @return false 
         */
        bool insert_breakpoint(FlatPtr address);

        /**
         * @param address 
         * @return true 
         * @return false 
         */
        bool disable_breakpoint(FlatPtr address);

        /**
         * @param address 
         * @return true 
         * @return false 
         */
        bool enable_breakpoint(FlatPtr address);

        /**
         * @param address 
         * @return true 
         * @return false 
         */
        bool remove_breakpoint(FlatPtr address);

        /**
         * @param address 
         * @return true 
         * @return false 
         */
        bool breakpoint_exists(FlatPtr address) const;

        struct WatchPoint {
            FlatPtr address { 0 };
            u32 debug_register_index { 0 };
            u32 ebp { 0 };
        }; // struct WatchPoint

        /**
         * @param address 
         * @param ebp 
         * @return true 
         * @return false 
         */
        bool insert_watchpoint(FlatPtr address, u32 ebp);

        /**
         * @param address 
         * @return true 
         * @return false 
         */
        bool remove_watchpoint(FlatPtr address);

        /**
         * @param address 
         * @return true 
         * @return false 
         */
        bool disable_watchpoint(FlatPtr address);

        /**
         * @param address 
         * @return true 
         * @return false 
         */
        bool watchpoint_exists(FlatPtr address) const;

        void dump_breakpoints()
        {
            for (auto addr : m_breakpoints.keys()) {
                dbgln("{}", addr);
            }
        }

        enum class ContinueType {
            FreeRun,
            Syscall,
        }; // enum class ContinueType

        /**
         * @param type 
         */
        void continue_debuggee(ContinueType type = ContinueType::FreeRun);

        /**
         * @param type 
         * @return int 
         */
        int continue_debuggee_and_wait(ContinueType type = ContinueType::FreeRun);

        /**
         * @return FlatPtr 
         */
        FlatPtr single_step();

        void detach();

        enum DesiredInitialDebugeeState {
            Running,
            Stopped
        }; // enum DesiredInitialDebugeeState

        /**
         * @tparam Callback 
         */
        template<typename Callback>
        void run(DesiredInitialDebugeeState, Callback);

        enum DebugDecision {
            Continue,
            SingleStep,
            ContinueBreakAtSyscall,
            Detach,
            Kill,
        }; // enum DebugDecision

        enum DebugBreakReason {
            Breakpoint,
            Syscall,
            Exited, 
        }; // enum DebugBreakReason

    private:
        /**
         * @brief Construct a new Debug Session object
         * 
         * @param source_root 
         */
        explicit DebugSession(pid_t, String source_root);

        static constexpr u8 BREAKPOINT_INSTRUCTION = 0xcc;

        void update_loaded_libs();

        int m_debuggee_pid { -1 };
        String m_source_root;
        bool m_is_debuggee_dead { false };

        HashMap<FlatPtr, BreakPoint> m_breakpoints;
        HashMap<FlatPtr, WatchPoint> m_watchpoints;

        HashMap<String, NonnullOwnPtr<LoadedLibrary>> m_loaded_libraries;
    }; // class DebugSession : public ProcessInspector

    /**
     * @tparam Callback 
     * @param initial_debugee_state 
     * @param callback 
     */
    template<typename Callback>
    void DebugSession::run(DesiredInitialDebugeeState initial_debugee_state, Callback callback)
    {
        enum class State {
            FirstIteration,
            FreeRun,
            Syscall,
            ConsecutiveBreakpoint,
            SingleStep,
        }; // enum class State

        State state { State::FirstIteration };

        auto do_continue_and_wait = [&]() {
            int wstatus = continue_debuggee_and_wait((state == State::Syscall) ? ContinueType::Syscall : ContinueType::FreeRun);

            if (WSTOPSIG(wstatus) != SIGTRAP) {
                callback(DebugBreakReason::Exited, Optional<PtraceRegisters>());
                m_is_debuggee_dead = true;
                return true;
            }
            return false;
        };

        for (;;) {
            if ((state == State::FirstIteration && initial_debugee_state == DesiredInitialDebugeeState::Running) || state == State::FreeRun || state == State::Syscall) {
                if (do_continue_and_wait())
                    break;
            }
            if (state == State::FirstIteration)
                state = State::FreeRun;

            auto regs = get_registers();

    #if ARCH(I386)
            FlatPtr current_instruction = regs.eip;
    #else
            FlatPtr current_instruction = regs.rip;
    #endif

            auto debug_status = peek_debug(DEBUG_STATUS_REGISTER);
            if (debug_status.has_value() && (debug_status.value() & 0b1111) > 0) {
                auto watchpoint_index = debug_status.value() & 0b1111;
                Optional<WatchPoint> watchpoint {};
                for (auto wp : m_watchpoints) {
                    if ((watchpoint_index & (1 << wp.value.debug_register_index)) == 0)
                        continue;
                    watchpoint = wp.value;
                    break;
                }
                if (watchpoint.has_value()) {
                    auto required_ebp = watchpoint.value().ebp;
                    auto found_ebp = false;

    #if ARCH(I386)
                    FlatPtr current_ebp = regs.ebp;
    #else
                    FlatPtr current_ebp = regs.rbp;
    #endif

                    do {
                        if (current_ebp == required_ebp) {
                            found_ebp = true;
                            break;
                        }
                        auto return_address = peek(current_ebp + sizeof(FlatPtr));
                        auto next_ebp = peek(current_ebp);
                        VERIFY(return_address.has_value());
                        VERIFY(next_ebp.has_value());
                        current_instruction = return_address.value();
                        current_ebp = next_ebp.value();
                    } while (current_ebp && current_instruction);

                    if (!found_ebp) {
                        dbgln("Removing watchpoint at {:p} because it went out of scope!", watchpoint.value().address);
                        remove_watchpoint(watchpoint.value().address);
                        continue;
                    }
                }
            }

            Optional<BreakPoint> current_breakpoint;

            if (state == State::FreeRun || state == State::Syscall) {
                current_breakpoint = m_breakpoints.get(current_instruction - 1);
                if (current_breakpoint.has_value())
                    state = State::FreeRun;
            } else {
                current_breakpoint = m_breakpoints.get(current_instruction);
            }

            if (current_breakpoint.has_value()) {
                auto breakpoint_addr = bit_cast<FlatPtr>(current_breakpoint.value().address);
    #if ARCH(I386)
                regs.eip = breakpoint_addr;
    #else
                regs.rip = breakpoint_addr;
    #endif
                set_registers(regs);
                disable_breakpoint(current_breakpoint.value().address);
            }

            DebugBreakReason reason = (state == State::Syscall && !current_breakpoint.has_value()) ? DebugBreakReason::Syscall : DebugBreakReason::Breakpoint;

            DebugDecision decision = callback(reason, regs);

            if (reason == DebugBreakReason::Syscall) {
                if (do_continue_and_wait())
                    break;
            }

            if (decision == DebugDecision::Continue) {
                state = State::FreeRun;
            } else if (decision == DebugDecision::ContinueBreakAtSyscall) {
                state = State::Syscall;
            }

            bool did_single_step = false;

            auto current_breakpoint_address = bit_cast<FlatPtr>(current_breakpoint.value().address);

            if (current_breakpoint.has_value() && m_breakpoints.contains(current_breakpoint_address)) {
                if (m_breakpoints.contains(current_breakpoint_address) && m_breakpoints.get(current_breakpoint_address).value().state == BreakPointState::Enabled) {
                    disable_breakpoint(current_breakpoint.value().address);
                }
                auto stopped_address = single_step();
                enable_breakpoint(current_breakpoint.value().address);
                did_single_step = true;
                
                auto breakpoint_at_next_instruction = m_breakpoints.get(stopped_address);
                if (breakpoint_at_next_instruction.has_value()
                    && breakpoint_at_next_instruction.value().state == BreakPointState::Enabled) {
                    state = State::ConsecutiveBreakpoint;
                }
            }

            if (decision == DebugDecision::SingleStep) {
                state = State::SingleStep;
            }

            if (decision == DebugDecision::Detach) {
                detach();
                break;
            }
            if (decision == DebugDecision::Kill) {
                kill(m_debuggee_pid, SIGTERM);
                break;
            }

            if (state == State::SingleStep && !did_single_step) {
                single_step();
            }
        }
    }

} // namespace Debug
