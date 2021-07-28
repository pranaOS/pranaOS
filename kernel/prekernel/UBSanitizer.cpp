/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <base/UBSanitizer.h>
#include <kernel/arch/x86/Processor.h>
#include <kernel/KSyms.h>

using namespace AK::UBSanitizer;

bool AK::UBSanitizer::g_ubsan_is_deadly { true };

extern "C" {

static void print_location(const SourceLocation&)
{
    asm volatile("cli; hlt");
}

void __ubsan_handle_load_invalid_value(const InvalidValueData&, ValueHandle) __attribute__((used));
void __ubsan_handle_load_invalid_value(const InvalidValueData& data, ValueHandle)
{
    print_location(data.location);
}

void __ubsan_handle_nonnull_arg(const NonnullArgData&) __attribute__((used));
void __ubsan_handle_nonnull_arg(const NonnullArgData& data)
{
    print_location(data.location);
}

void __ubsan_handle_nullability_arg(const NonnullArgData&) __attribute__((used));
void __ubsan_handle_nullability_arg(const NonnullArgData& data)
{
    print_location(data.location);
}

void __ubsan_handle_nonnull_return_v1(const NonnullReturnData&, const SourceLocation&) __attribute__((used));
void __ubsan_handle_nonnull_return_v1(const NonnullReturnData&, const SourceLocation& location)
{
    print_location(location);
}

void __ubsan_handle_nullability_return_v1(const NonnullReturnData& data, const SourceLocation& location) __attribute__((used));
void __ubsan_handle_nullability_return_v1(const NonnullReturnData&, const SourceLocation& location)
{
    print_location(location);
}

void __ubsan_handle_vla_bound_not_positive(const VLABoundData&, ValueHandle) __attribute__((used));
void __ubsan_handle_vla_bound_not_positive(const VLABoundData& data, ValueHandle)
{
    print_location(data.location);
}

void __ubsan_handle_add_overflow(const OverflowData&, ValueHandle lhs, ValueHandle rhs) __attribute__((used));
void __ubsan_handle_add_overflow(const OverflowData& data, ValueHandle, ValueHandle)
{
    print_location(data.location);
}

void __ubsan_handle_sub_overflow(const OverflowData&, ValueHandle lhs, ValueHandle rhs) __attribute__((used));
void __ubsan_handle_sub_overflow(const OverflowData& data, ValueHandle, ValueHandle)
{
    print_location(data.location);
}

void __ubsan_handle_negate_overflow(const OverflowData&, ValueHandle) __attribute__((used));
void __ubsan_handle_negate_overflow(const OverflowData& data, ValueHandle)
{
    print_location(data.location);
}

void __ubsan_handle_mul_overflow(const OverflowData&, ValueHandle lhs, ValueHandle rhs) __attribute__((used));
void __ubsan_handle_mul_overflow(const OverflowData& data, ValueHandle, ValueHandle)
{
    print_location(data.location);
}

void __ubsan_handle_shift_out_of_bounds(const ShiftOutOfBoundsData&, ValueHandle lhs, ValueHandle rhs) __attribute__((used));
void __ubsan_handle_shift_out_of_bounds(const ShiftOutOfBoundsData& data, ValueHandle, ValueHandle)
{
    print_location(data.location);
}

void __ubsan_handle_divrem_overflow(const OverflowData&, ValueHandle lhs, ValueHandle rhs) __attribute__((used));
void __ubsan_handle_divrem_overflow(const OverflowData& data, ValueHandle, ValueHandle)
{
    print_location(data.location);
}

void __ubsan_handle_out_of_bounds(const OutOfBoundsData&, ValueHandle) __attribute__((used));
void __ubsan_handle_out_of_bounds(const OutOfBoundsData& data, ValueHandle)
{
    print_location(data.location);
}

void __ubsan_handle_type_mismatch_v1(const TypeMismatchData&, ValueHandle) __attribute__((used));
void __ubsan_handle_type_mismatch_v1(const TypeMismatchData& data, ValueHandle)
{
    print_location(data.location);
}


}