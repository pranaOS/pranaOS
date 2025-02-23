/**
 * @file stubs.cpp
 * @author Krisna Pranav
 * @brief stubs
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

extern "C" {

#define DO_STUB(name) \
    void name();      \
    void name() { }

DO_STUB(__register_frame_info);
DO_STUB(__deregister_frame_info);
DO_STUB(_ITM_registerTMCloneTable);
DO_STUB(_ITM_deregisterTMCloneTable);
}
