/**
 * @file cdefs.h
 * @author Krisna Pranav
 * @brief CDEFS
 * @version 0.1
 * @date 2023-05-25
 * 
 * @copyright Copyright (c) 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#ifndef PRANAOS_CDEFS_H
#define PRANAOS_CDEFS_H

#ifdef __cplusplus
#ifndef __DECL_BEGIN
#define __DECL_BEGIN extern "C" {
#define __DECL_END }
#endif
#else
#ifndef __DECL_BEGIN
		#define __DECL_BEGIN
		#define __DECL_END
	#endif
#endif

#endif