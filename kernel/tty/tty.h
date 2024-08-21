/**
 * @file tty.h
 * @author Krisna Pranav
 * @brief tty
 * @version 6.0
 * @date 2023-08-13
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/doublebuffer.h>
#include <kernel/processgroup.h>
#include <kernel/unixtypes.h>
#include <mods/circular_deque.h>
#include <mods/weakptr.h>
#include <kernel/device/characterdevice.h>

namespace Kernel 
{

    class TTY : public CharacterDevice 
    {
    public:
        /// @brief Destroy the TTY object
        virtual ~TTY() override;

        /**
         * @return KResultOr<size_t> 
         */
        virtual KResultOr<size_t> read(FileDescription&, size_t, UserOrKernelBuffer&, size_t) override;
        virtual KResultOr<size_t> write(FileDescription&, size_t, const UserOrKernelBuffer&, size_t) override;

        /**
         * @return true 
         * @return false 
         */
        virtual bool can_read(const FileDescription&, size_t) const override;
        virtual bool can_write(const FileDescription&, size_t) const override;

        /**
         * @param request 
         * @param arg 
         * @return int 
         */
        virtual int ioctl(FileDescription&, unsigned request, FlatPtr arg) override final;

        /**
         * @return String 
         */
        virtual String absolute_path(const FileDescription&) const override 
        { 
            return tty_name(); 
        }

        /**
         * @return String 
         */
        virtual String tty_name() const = 0;

        /**
         * @return unsigned short 
         */
        unsigned short rows() const 
        { 
            return m_rows; 
        }

        unsigned short columns() const 
        { 
            return m_columns; 
        }

        /**
         * @return ProcessGroupID 
         */
        ProcessGroupID pgid() const
        {
            if (auto pg = m_pg.strong_ref())
                return pg->pgid();
            return 0;
        }

        /// @brief Set the termios object
        void set_termios(const termios&);

        /**
         * @return true 
         * @return false 
         */
        bool should_generate_signals() const 
        { 
            return m_termios.c_lflag & ISIG; 
        }

        bool should_flush_on_signal() const 
        { 
            return !(m_termios.c_lflag & NOFLSH); 
        }

        bool should_echo_input() const 
        { 
            return m_termios.c_lflag & ECHO; 
        }

        bool in_canonical_mode() const 
        { 
            return m_termios.c_lflag & ICANON; 
        }

        void set_default_termios();
        void hang_up();

    protected:
        
        /**
         * @return ssize_t 
         */
        virtual ssize_t on_tty_write(const UserOrKernelBuffer&, ssize_t) = 0;

        /**
         * @param columns 
         * @param rows 
         */
        void set_size(unsigned short columns, unsigned short rows);

        /**
         * @param major 
         * @param minor 
         */
        TTY(unsigned major, unsigned minor);

        /**
         * @param do_evaluate_block_conditions 
         */
        void emit(u8, bool do_evaluate_block_conditions = false);

        /// @brief: echo
        virtual void echo(u8) = 0;

        /**
         * @return true 
         * @return false 
         */
        bool can_do_backspace() const;
        void do_backspace();
        void erase_word();
        void erase_character();
        void kill_line();
        void flush_input();

        /**
         * @return true 
         * @return false 
         */
        bool is_eol(u8) const;
        bool is_eof(u8) const;
        bool is_kill(u8) const;
        bool is_erase(u8) const;
        bool is_werase(u8) const;

        /**
         * @param signal 
         */
        void generate_signal(int signal);

        int m_available_lines { 0 };

    private:
        
        /**
         * @return true 
         * @return false 
         */
        virtual bool is_tty() const final override 
        { 
            return true; 
        }

        CircularDeque<u8, 1024> m_input_buffer;

        WeakPtr<Process> m_original_process_parent;
        WeakPtr<ProcessGroup> m_pg;

        termios m_termios;
        
        unsigned short m_rows { 0 };
        unsigned short m_columns { 0 };
    }; // class TTY

} // namespace Kernel