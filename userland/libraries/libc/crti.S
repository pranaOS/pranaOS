/**
 * @file crti.S
 * @author Krisna Pranav
 * @brief CRTI
 * @version 6.0
 * @date 2023-08-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

.global _init
.section .init
_init:
    push %ebp

.global _fini
.section .fini
_fini:
    push %ebp
