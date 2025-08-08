/**
 * @file unix.h
 * @author Krisna Pranav
 * @brief unix
 * @version 6.0
 * @date 2025-08-08
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

typedef struct kernel_heaps *kernel_heaps;
typedef struct unix_heaps *unix_heaps;
typedef struct process *process;
typedef struct thread *thread;

/**
 * @param kh 
 * @param root 
 * @param fs 
 * @return process 
 */
process init_unix(kernel_heaps kh, tuple root, filesystem fs);

/**
 * @brief Create a process object
 * 
 * @param uh 
 * @param root 
 * @param fs 
 * @return process 
 */
process create_process(unix_heaps uh, tuple root, filesystem fs);

/**
 * @param p 
 * @param cwd_fs 
 * @param cwd 
 */
void process_get_cwd(process p, filesystem *cwd_fs, inode *cwd);

/**
 * @brief Create a thread object
 * 
 * @param p 
 * @param tid 
 * @return thread 
 */
thread create_thread(process p, u64 tid);

/**
 * @param kp 
 * @param program_path 
 * @param complete 
 */
void exec_elf(process kp, string program_path, status_handler complete);

void unix_shutdown(void);

/**
 * @param root 
 * @param prog 
 */
void program_set_perms(tuple root, tuple prog);