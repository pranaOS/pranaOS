/**
 * @file debuginfo.h
 * @author Krisna Pranav
 * @brief debug info
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/nonnullownptrvector.h>
#include <mods/nonnullrefptr.h>
#include <mods/optional.h>
#include <mods/ownPtr.h>
#include <mods/vector.h>
#include <libdebug/dwarf/die.h>
#include <libdebug/dwarf/dwarfinfo.h>
#include <libdebug/dwarf/lineprogram.h>
#include <libelf/image.h>
#include <sys/arch/i386/regs.h>

namespace Debug 
{

    class DebugInfo 
    {
        MOD_MAKE_NONCOPYABLE(DebugInfo);
        MOD_MAKE_NONMOVABLE(DebugInfo);

    public:
        /**
         * @brief Construct a new Debug Info object
         * 
         * @param source_root 
         * @param base_address 
         */
        explicit DebugInfo(ELF::Image const&, String source_root = {}, FlatPtr base_address = 0);

        /**
         * @return ELF::Image const& 
         */
        ELF::Image const& elf() const 
        { 
            return m_elf; 
        }

        struct SourcePosition {
            FlyString file_path;
            size_t line_number { 0 };
            Optional<FlatPtr> address_of_first_statement;

            /**
             * @brief Construct a new SourcePosition object
             * 
             */
            SourcePosition()
                : SourcePosition(String::empty(), 0)
            {
            }

            /**
             * @brief Construct a new SourcePosition object
             * 
             * @param file_path 
             * @param line_number 
             */
            SourcePosition(String file_path, size_t line_number)
                : file_path(file_path)
                , line_number(line_number)
            {
            }

            /**
             * @brief Construct a new SourcePosition object
             * 
             * @param file_path 
             * @param line_number 
             * @param address_of_first_statement 
             */
            SourcePosition(String file_path, size_t line_number, FlatPtr address_of_first_statement)
                : file_path(file_path)
                , line_number(line_number)
                , address_of_first_statement(address_of_first_statement)
            {
            }

            /**
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator==(SourcePosition const& other) const 
            { 
                return file_path == other.file_path && line_number == other.line_number; 
            }

            /**
             * @param other 
             * @return true 
             * @return false 
             */
            bool operator!=(SourcePosition const& other) const 
            { 
                return !(*this == other); 
            }

            /**
             * @return SourcePosition 
             */
            static SourcePosition from_line_info(Dwarf::LineProgram::LineInfo const&);
        }; // struct SourcePosition

        struct VariableInfo {

            enum class LocationType {
                None,
                Address,
                Register,
            }; // enum class LocationType

            String name; 
            String type_name;
            LocationType location_type { LocationType::None };
            union {
                FlatPtr address;
            } location_data { 0 };

            union {
                u32 as_u32;
                u32 as_i32;
                char const* as_string;
            } constant_data { 0 };

            Dwarf::EntryTag type_tag;
            OwnPtr<VariableInfo> type;
            NonnullOwnPtrVector<VariableInfo> members;
            VariableInfo* parent { nullptr };
            Vector<u32> dimension_sizes;

            /**
             * @return true 
             * @return false 
             */
            bool is_enum_type() const 
            { 
                return type && type->type_tag == Dwarf::EntryTag::EnumerationType; 
            }
        }; // struct VariableInfo

        struct VariablesScope {
            bool is_function { false };
            String name;
            FlatPtr address_low { 0 };
            FlatPtr address_high { 0 }; 
            Vector<Dwarf::DIE> dies_of_variables;
        }; // struct VariablesScope

        /**
         * @brief Get the variables in current scope object
         * 
         * @return NonnullOwnPtrVector<VariableInfo> 
         */
        NonnullOwnPtrVector<VariableInfo> get_variables_in_current_scope(PtraceRegisters const&) const;

        /**
         * @brief Get the source position object
         * 
         * @param address 
         * @return Optional<SourcePosition> 
         */
        Optional<SourcePosition> get_source_position(FlatPtr address) const;

        struct SourcePositionWithInlines {
            Optional<SourcePosition> source_position;
            Vector<SourcePosition> inline_chain;
        }; // struct SourcePositionWithInlines

        /**
         * @brief Get the source position with inlines object
         * 
         * @param address 
         * @return SourcePositionWithInlines 
         */
        SourcePositionWithInlines get_source_position_with_inlines(FlatPtr address) const;

        struct SourcePositionAndAddress {
            String file;
            size_t line;
            FlatPtr address;
        }; // struct SourcePositionAndAddress

        /**
         * @brief Get the address from source position object
         * 
         * @param file 
         * @param line 
         * @return Optional<SourcePositionAndAddress> 
         */
        Optional<SourcePositionAndAddress> get_address_from_source_position(String const& file, size_t line) const;

        /**
         * @param address 
         * @return String 
         */
        String name_of_containing_function(FlatPtr address) const;

        /**
         * @return Vector<SourcePosition> 
         */
        Vector<SourcePosition> source_lines_in_scope(VariablesScope const&) const;

        /**
         * @brief Get the containing function object
         * 
         * @param address 
         * @return Optional<VariablesScope> 
         */
        Optional<VariablesScope> get_containing_function(FlatPtr address) const;

    private:
        void prepare_variable_scopes();
        void prepare_lines();

        /**
         * @param die 
         */
        void parse_scopes_impl(Dwarf::DIE const& die);

        /**
         * @brief Create a variable info object
         * 
         * @param variable_die 
         * @param address_offset 
         * @return OwnPtr<VariableInfo> 
         */
        OwnPtr<VariableInfo> create_variable_info(Dwarf::DIE const& variable_die, PtraceRegisters const&, u32 address_offset = 0) const;

        /**
         * @param tag 
         * @return true 
         * @return false 
         */
        static bool is_variable_tag_supported(Dwarf::EntryTag const& tag);

        /**
         * @param type_die 
         * @param regs 
         * @param parent_variable 
         */
        void add_type_info_to_variable(Dwarf::DIE const& type_die, PtraceRegisters const& regs, DebugInfo::VariableInfo* parent_variable) const;

        /**
         * @brief Get the source path of inline object
         * 
         * @return Optional<Dwarf::LineProgram::DirectoryAndFile> 
         */
        Optional<Dwarf::LineProgram::DirectoryAndFile> get_source_path_of_inline(Dwarf::DIE const&) const;

        /**
         * @brief Get the line of inline object
         * 
         * @return Optional<uint32_t> 
         */
        Optional<uint32_t> get_line_of_inline(Dwarf::DIE const&) const;

        ELF::Image const& m_elf;
        String m_source_root;
        FlatPtr m_base_address { 0 };
        Dwarf::DwarfInfo m_dwarf_info;

        Vector<VariablesScope> m_scopes;
        Vector<Dwarf::LineProgram::LineInfo> m_sorted_lines;
    }; // class DebugInfo

} // namespace Debug
