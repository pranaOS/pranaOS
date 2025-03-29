/**
 * @file hyperv.c
 * @author Krisna Pranav
 * @brief hyperv
 * @version 6.0
 * @date 2025-03-29
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include <drivers/acpi.h>
#include <hyperv_internal.h>
#include <kernel.h>

#define HV_SMCCC_FUNC_NO 1

#define HV_FUNC_ID (0x46000000 | HV_SMCCC_FUNC_NO)

#define HVCALL_SET_VP_REGISTERS 0x0051
#define HVCALL_GET_VP_REGISTERS 0x0050
#define HV_HYPERCALL_FAST_BIT U64_FROM_BIT(16)
#define HV_HYPERCALL_REP_COMP_1 U64_FROM_BIT(32)

#define HV_PARTITION_ID_SELF (-1ull)

#define HV_VP_INDEX_SELF (-2u)

/**
 * @param kh
 * @return boolean
 */
boolean hyperv_arch_detect(kernel_heaps kh)
{
    u64 hv_id = acpi_get_hv_id();
    if(runtime_memcmp(&hv_id, "MsHyperV", sizeof(hv_id)))
        return false;
    return true;
}

/**
 * @param msr
 * @return u64
 */
u64 aarch64_hv_get_vreg(u32 msr)
{
    struct arm_hvc_full_ret ret = arm_hvc_full(HV_FUNC_ID,
                                               HVCALL_GET_VP_REGISTERS | HV_HYPERCALL_FAST_BIT |
                                                   HV_HYPERCALL_REP_COMP_1,
                                               HV_PARTITION_ID_SELF,
                                               HV_VP_INDEX_SELF,
                                               msr);
    return ret.x6;
}

/**
 * @param msr
 * @param val
 */
void aarch64_hv_set_vreg(u32 msr, u64 val)
{
    arm_hvc(HV_FUNC_ID, HVCALL_SET_VP_REGISTERS | HV_HYPERCALL_FAST_BIT | HV_HYPERCALL_REP_COMP_1, HV_PARTITION_ID_SELF, HV_VP_INDEX_SELF, msr, 0, val, 0);
}

/**
 * @param hctx
 */
void hypercall_create(struct hypercall_ctx* hctx)
{
}

/**
 * @param hc_addr
 * @param in_val
 * @param in_paddr
 * @param out_paddr
 * @return u64
 */
u64 hypercall_md(volatile void* hc_addr, u64 in_val, u64 in_paddr, u64 out_paddr)
{
    struct arm_hvc_ret ret = arm_hvc(HV_FUNC_ID, in_val, in_paddr, out_paddr, 0, 0, 0, 0);
    return ret.x0;
}