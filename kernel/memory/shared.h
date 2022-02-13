/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <tasking/process.h>
#include <stdint.h>

namespace Kernel {

  /**
   * @brief shared memory class
   * 
   */
  class sharedMemory {
  public:

    /**
     * @brief Create a Shared Region object
     * 
     * @param proc1 
     * @param proc2 
     * @param virtStart 
     * @param len 
     * @return true 
     * @return false 
     */
    static bool createSharedRegion(Process* proc1, Process* proc2, uint32_t virtStart, uint32_t len);
    static bool createSharedRegion(Process* proc1, Process* proc2, uint32_t virtStart1, uint32_t virtStart2, uint32_t len);

    /**
     * @brief remove shared region object
     * 
     * @param proc1 
     * @param proc2 
     * @param virtStart 
     * @param len 
     * @return true 
     * @return false 
     */
    static bool removeSharedRegion(Process* proc1, Process* proc2, uint32_t virtStart, uint32_t len);
    static bool removeSharedRegion(Process* proc1, Process* proc2, uint32_t virtStart1, uint32_t virtStart2, uint32_t len);
  };
}