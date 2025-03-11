/**
 * @file regexbytecode.h
 * @author Krisna Pranav
 * @brief Regex Byte Code
 * @version 6.0
 * @date 2024-11-04
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

#include <mods/disjointchunks.h>
#include <mods/format.h>
#include <mods/forward.h>
#include <mods/hashMap.h>
#include <mods/nonnullOwnPtr.h>
#include <mods/ownPtr.h>
#include <mods/traits.h>
#include <mods/typecast.h>
#include <mods/types.h>
#include <mods/vector.h>
#include <libunicode/forward.h>
#include "regexbytecodestreamoptimizer.h"
#include "regexmatch.h"
#include "regexoptions.h"

namespace regex 
{

    using ByteCodeValueType = u64;

    #define ENUMERATE_OPCODES                          \
        __ENUMERATE_OPCODE(Compare)                    \
        __ENUMERATE_OPCODE(Jump)                       \
        __ENUMERATE_OPCODE(JumpNonEmpty)               \
        __ENUMERATE_OPCODE(ForkJump)                   \
        __ENUMERATE_OPCODE(ForkStay)                   \
        __ENUMERATE_OPCODE(ForkReplaceJump)            \
        __ENUMERATE_OPCODE(ForkReplaceStay)            \
        __ENUMERATE_OPCODE(FailForks)                  \
        __ENUMERATE_OPCODE(SaveLeftCaptureGroup)       \
        __ENUMERATE_OPCODE(SaveRightCaptureGroup)      \
        __ENUMERATE_OPCODE(SaveRightNamedCaptureGroup) \
        __ENUMERATE_OPCODE(CheckBegin)                 \
        __ENUMERATE_OPCODE(CheckEnd)                   \
        __ENUMERATE_OPCODE(CheckBoundary)              \
        __ENUMERATE_OPCODE(Save)                       \
        __ENUMERATE_OPCODE(Restore)                    \
        __ENUMERATE_OPCODE(GoBack)                     \
        __ENUMERATE_OPCODE(ClearCaptureGroup)          \
        __ENUMERATE_OPCODE(Repeat)                     \
        __ENUMERATE_OPCODE(ResetRepeat)                \
        __ENUMERATE_OPCODE(Checkpoint)                 \
        __ENUMERATE_OPCODE(Exit)

    enum class OpCodeId : ByteCodeValueType 
    {
    #define __ENUMERATE_OPCODE(x) x,
        ENUMERATE_OPCODES
    #undef __ENUMERATE_OPCODE

        First = Compare,
        Last = Exit,
    }; // enum class OpCodeId : ByteCodeValueType
    

    #define ENUMERATE_CHARACTER_COMPARE_TYPES                    \
        __ENUMERATE_CHARACTER_COMPARE_TYPE(Undefined)            \
        __ENUMERATE_CHARACTER_COMPARE_TYPE(Inverse)              \
        __ENUMERATE_CHARACTER_COMPARE_TYPE(TemporaryInverse)     \
        __ENUMERATE_CHARACTER_COMPARE_TYPE(AnyChar)              \
        __ENUMERATE_CHARACTER_COMPARE_TYPE(Char)                 \
        __ENUMERATE_CHARACTER_COMPARE_TYPE(String)               \
        __ENUMERATE_CHARACTER_COMPARE_TYPE(CharClass)            \
        __ENUMERATE_CHARACTER_COMPARE_TYPE(CharRange)            \
        __ENUMERATE_CHARACTER_COMPARE_TYPE(Reference)            \
        __ENUMERATE_CHARACTER_COMPARE_TYPE(Property)             \
        __ENUMERATE_CHARACTER_COMPARE_TYPE(GeneralCategory)      \
        __ENUMERATE_CHARACTER_COMPARE_TYPE(Script)               \
        __ENUMERATE_CHARACTER_COMPARE_TYPE(ScriptExtension)      \
        __ENUMERATE_CHARACTER_COMPARE_TYPE(RangeExpressionDummy) \
        __ENUMERATE_CHARACTER_COMPARE_TYPE(LookupTable)

    enum class CharacterCompareType : ByteCodeValueType 
    {
    #define __ENUMERATE_CHARACTER_COMPARE_TYPE(x) x,
        ENUMERATE_CHARACTER_COMPARE_TYPES
    #undef __ENUMERATE_CHARACTER_COMPARE_TYPE
    }; // enum class CharacterCompareType : ByteCodeValueType 

    #define ENUMERATE_CHARACTER_CLASSES    \
        __ENUMERATE_CHARACTER_CLASS(Alnum) \
        __ENUMERATE_CHARACTER_CLASS(Cntrl) \
        __ENUMERATE_CHARACTER_CLASS(Lower) \
        __ENUMERATE_CHARACTER_CLASS(Space) \
        __ENUMERATE_CHARACTER_CLASS(Alpha) \
        __ENUMERATE_CHARACTER_CLASS(Digit) \
        __ENUMERATE_CHARACTER_CLASS(Print) \
        __ENUMERATE_CHARACTER_CLASS(Upper) \
        __ENUMERATE_CHARACTER_CLASS(Blank) \
        __ENUMERATE_CHARACTER_CLASS(Graph) \
        __ENUMERATE_CHARACTER_CLASS(Punct) \
        __ENUMERATE_CHARACTER_CLASS(Word)  \
        __ENUMERATE_CHARACTER_CLASS(Xdigit)

    enum class CharClass : ByteCodeValueType 
    {
    #define __ENUMERATE_CHARACTER_CLASS(x) x,
        ENUMERATE_CHARACTER_CLASSES
    #undef __ENUMERATE_CHARACTER_CLASS
    }; // enum class CharClass : ByteCodeValueType 

    #define ENUMERATE_BOUNDARY_CHECK_TYPES    \
        __ENUMERATE_BOUNDARY_CHECK_TYPE(Word) \
        __ENUMERATE_BOUNDARY_CHECK_TYPE(NonWord)

    enum class BoundaryCheckType : ByteCodeValueType 
    {
    #define __ENUMERATE_BOUNDARY_CHECK_TYPE(x) x,
        ENUMERATE_BOUNDARY_CHECK_TYPES
    #undef __ENUMERATE_BOUNDARY_CHECK_TYPE
    }; // enum class BoundaryCheckType : ByteCodeValueType 

    struct CharRange 
    {
        u32 const from;
        u32 const to;

        /**
         * @brief Construct a new Char Range object
         * 
         * @param value 
         */
        CharRange(u64 value)
            : from(value >> 32)
            , to(value & 0xffffffff)
        {
        }

        /**
         * @brief Construct a new Char Range object
         * 
         * @param from 
         * @param to 
         */
        CharRange(u32 from, u32 to)
            : from(from)
            , to(to)
        {
        }

        operator ByteCodeValueType() const 
        { 
            return ((u64)from << 32) | to; 
        }
    }; // struct CharRange 

    struct CompareTypeAndValuePair 
    {
        CharacterCompareType type;
        ByteCodeValueType value;
    }; // struct CompareTypeAndValuePair 

    class OpCode;

    class ByteCode : public DisjointChunks<ByteCodeValueType> 
    {
        using Base = DisjointChunks<ByteCodeValueType>;

    public: 
        /**
         * @brief Construct a new Byte Code object
         * 
         */
        ByteCode()
        {
            ensure_opcodes_initialized();
        }

        ByteCode(ByteCode const&) = default;

        virtual ~ByteCode() = default;

        ByteCode& operator=(ByteCode&&) = default;

        /**
         * @param value 
         * @return ByteCode& 
         */
        ByteCode& operator=(Base&& value)
        {
            static_cast<Base&>(*this) = move(value);
            return *this;
        }

        /**
         * @tparam Args 
         * @param args 
         */
        template<typename... Args>
        void empend(Args&&... args)
        {
            if (is_empty())
                Base::append({});
            Base::last_chunk().empend(forward<Args>(args)...);
        }

        /**
         * @tparam T 
         * @param value 
         */
        template<typename T>
        void append(T&& value)
        {
            if (is_empty())
                Base::append({});
            Base::last_chunk().append(forward<T>(value));
        }

        /**
         * @tparam T 
         * @param value 
         */
        template<typename T>
        void prepend(T&& value)
        {
            if (is_empty())
                return append(forward<T>(value));
            Base::first_chunk().prepend(forward<T>(value));
        }

        void last_chunk() const = delete;
        void first_chunk() const = delete;

        /**
         * @param pairs 
         */
        void insert_bytecode_compare_values(Vector<CompareTypeAndValuePair>&& pairs)
        {
            Optimizer::append_character_class(*this, move(pairs));
        }

        /**
         * @param type 
         */
        void insert_bytecode_check_boundary(BoundaryCheckType type)
        {
            ByteCode bytecode;
            bytecode.empend((ByteCodeValueType)OpCodeId::CheckBoundary);
            bytecode.empend((ByteCodeValueType)type);

            extend(move(bytecode));
        }

        /**
         * @param index 
         */
        void insert_bytecode_clear_capture_group(size_t index)
        {
            empend(static_cast<ByteCodeValueType>(OpCodeId::ClearCaptureGroup));
            empend(index);
        }

        /**
         * @param view 
         */
        void insert_bytecode_compare_string(StringView view)
        {
            ByteCode bytecode;

            bytecode.empend(static_cast<ByteCodeValueType>(OpCodeId::Compare));
            bytecode.empend(static_cast<u64>(1)); // number of arguments

            ByteCode arguments;

            arguments.empend(static_cast<ByteCodeValueType>(CharacterCompareType::String));
            arguments.insert_string(view);

            bytecode.empend(arguments.size()); // size of arguments
            bytecode.extend(move(arguments));

            extend(move(bytecode));
        }

        /**
         * @param capture_groups_count 
         */
        void insert_bytecode_group_capture_left(size_t capture_groups_count)
        {
            empend(static_cast<ByteCodeValueType>(OpCodeId::SaveLeftCaptureGroup));
            empend(capture_groups_count);
        }

        /**
         * @param capture_groups_count 
         */
        void insert_bytecode_group_capture_right(size_t capture_groups_count)
        {
            empend(static_cast<ByteCodeValueType>(OpCodeId::SaveRightCaptureGroup));
            empend(capture_groups_count);
        }

        /**
         * @param capture_groups_count 
         * @param name 
         */
        void insert_bytecode_group_capture_right(size_t capture_groups_count, StringView name)
        {
            empend(static_cast<ByteCodeValueType>(OpCodeId::SaveRightNamedCaptureGroup));
            empend(reinterpret_cast<ByteCodeValueType>(name.characters_without_null_termination()));
            empend(name.length());
            empend(capture_groups_count);
        }

        enum class LookAroundType 
        {
            LookAhead,
            LookBehind,
            NegatedLookAhead,
            NegatedLookBehind,
        }; // enum class LookAroundType

        /**
         * @param lookaround_body 
         * @param type 
         * @param match_length 
         */
        void insert_bytecode_lookaround(ByteCode&& lookaround_body, LookAroundType type, size_t match_length = 0)
        {
            switch (type) {
            case LookAroundType::LookAhead: {
                empend((ByteCodeValueType)OpCodeId::Save);
                extend(move(lookaround_body));
                empend((ByteCodeValueType)OpCodeId::Restore);
                return;
            }
            case LookAroundType::NegatedLookAhead: {
                auto body_length = lookaround_body.size();
                empend((ByteCodeValueType)OpCodeId::Jump);
                empend((ByteCodeValueType)body_length + 1); 
                extend(move(lookaround_body));
                empend((ByteCodeValueType)OpCodeId::FailForks);
                empend((ByteCodeValueType)OpCodeId::Save);
                empend((ByteCodeValueType)OpCodeId::ForkJump);
                empend((ByteCodeValueType) - (body_length + 4)); 
                empend((ByteCodeValueType)OpCodeId::Restore);
                return;
            }
            case LookAroundType::LookBehind:
                empend((ByteCodeValueType)OpCodeId::Save);
                empend((ByteCodeValueType)OpCodeId::GoBack);
                empend((ByteCodeValueType)match_length);
                extend(move(lookaround_body));
                empend((ByteCodeValueType)OpCodeId::Restore);
                return;
            case LookAroundType::NegatedLookBehind: {
                auto body_length = lookaround_body.size();
                empend((ByteCodeValueType)OpCodeId::Jump);
                empend((ByteCodeValueType)body_length + 3); 
                empend((ByteCodeValueType)OpCodeId::GoBack);
                empend((ByteCodeValueType)match_length);
                extend(move(lookaround_body));
                empend((ByteCodeValueType)OpCodeId::FailForks);
                empend((ByteCodeValueType)OpCodeId::Save);
                empend((ByteCodeValueType)OpCodeId::ForkJump);
                empend((ByteCodeValueType) - (body_length + 6)); 
                empend((ByteCodeValueType)OpCodeId::Restore);
                return;
            }
            }

            VERIFY_NOT_REACHED();
        }

        /**
         * @param left 
         * @param right 
         */
        void insert_bytecode_alternation(ByteCode&& left, ByteCode&& right)
        {
            Optimizer::append_alternation(*this, move(left), move(right));
        }

        /**
         * @tparam T 
         */
        template<typename T>
        static void transform_bytecode_repetition_min_max(ByteCode& bytecode_to_repeat, T minimum, Optional<T> maximum, size_t min_repetition_mark_id, size_t max_repetition_mark_id, bool greedy = true) requires(IsIntegral<T>)
        {
            if (!maximum.has_value()) {
                if (minimum == 0)
                    return transform_bytecode_repetition_any(bytecode_to_repeat, greedy);
                if (minimum == 1)
                    return transform_bytecode_repetition_min_one(bytecode_to_repeat, greedy);
            }

            ByteCode new_bytecode;
            new_bytecode.insert_bytecode_repetition_n(bytecode_to_repeat, minimum, min_repetition_mark_id);

            if (maximum.has_value()) {
                auto jump_kind = static_cast<ByteCodeValueType>(greedy ? OpCodeId::ForkStay : OpCodeId::ForkJump);
                if (maximum.value() > minimum) {
                    new_bytecode.empend(jump_kind);
                    new_bytecode.empend((ByteCodeValueType)0); 
                    auto pre_loop_fork_jump_index = new_bytecode.size();
                    new_bytecode.extend(bytecode_to_repeat);
                    auto repetitions = maximum.value() - minimum;
                    auto fork_jump_address = new_bytecode.size();
                    if (repetitions > 1) {
                        new_bytecode.empend((ByteCodeValueType)OpCodeId::Repeat);
                        new_bytecode.empend(bytecode_to_repeat.size() + 2);
                        new_bytecode.empend(static_cast<ByteCodeValueType>(repetitions - 1));
                        new_bytecode.empend(max_repetition_mark_id);
                        new_bytecode.empend(jump_kind);
                        new_bytecode.empend((ByteCodeValueType)0);
                        auto post_loop_fork_jump_index = new_bytecode.size();
                        new_bytecode.extend(bytecode_to_repeat);
                        fork_jump_address = new_bytecode.size();

                        new_bytecode[post_loop_fork_jump_index - 1] = (ByteCodeValueType)(fork_jump_address - post_loop_fork_jump_index);

                        new_bytecode.empend((ByteCodeValueType)OpCodeId::ResetRepeat);
                        new_bytecode.empend((ByteCodeValueType)max_repetition_mark_id);
                    }
                    new_bytecode[pre_loop_fork_jump_index - 1] = (ByteCodeValueType)(fork_jump_address - pre_loop_fork_jump_index);
                }
            } else {
                new_bytecode.insert(new_bytecode.size() - bytecode_to_repeat.size(), (ByteCodeValueType)OpCodeId::Checkpoint);

                auto jump_kind = static_cast<ByteCodeValueType>(greedy ? OpCodeId::ForkJump : OpCodeId::ForkStay);
                new_bytecode.empend((ByteCodeValueType)OpCodeId::JumpNonEmpty);
                new_bytecode.empend(-bytecode_to_repeat.size() - 4 - 1); 
                new_bytecode.empend(-bytecode_to_repeat.size() - 4 - 1); 
                new_bytecode.empend(jump_kind);
            }

            bytecode_to_repeat = move(new_bytecode);
        }

        /**
         * @tparam T 
         */
        template<typename T>
        void insert_bytecode_repetition_n(ByteCode& bytecode_to_repeat, T n, size_t repetition_mark_id) requires(IsIntegral<T>)
        {
            if (n == 0)
                return;

            extend(bytecode_to_repeat);

            if (n > 1) {
                empend(static_cast<ByteCodeValueType>(OpCodeId::Repeat));
                empend(bytecode_to_repeat.size());
                empend(static_cast<ByteCodeValueType>(n - 1));
                empend(repetition_mark_id);

                extend(bytecode_to_repeat);
            }
        }

        /**
         * @param bytecode_to_repeat 
         * @param greedy 
         */
        static void transform_bytecode_repetition_min_one(ByteCode& bytecode_to_repeat, bool greedy)
        {

            bytecode_to_repeat.prepend((ByteCodeValueType)OpCodeId::Checkpoint);

            bytecode_to_repeat.empend((ByteCodeValueType)OpCodeId::JumpNonEmpty);
            bytecode_to_repeat.empend(-bytecode_to_repeat.size() - 3); 
            bytecode_to_repeat.empend(-bytecode_to_repeat.size() - 2); 

            if (greedy)
                bytecode_to_repeat.empend(static_cast<ByteCodeValueType>(OpCodeId::ForkJump));
            else
                bytecode_to_repeat.empend(static_cast<ByteCodeValueType>(OpCodeId::ForkStay));
        }

        /**
         * @param bytecode_to_repeat 
         * @param greedy 
         */
        static void transform_bytecode_repetition_any(ByteCode& bytecode_to_repeat, bool greedy)
        {
            ByteCode bytecode;

            if (greedy)
                bytecode.empend(static_cast<ByteCodeValueType>(OpCodeId::ForkStay));
            else
                bytecode.empend(static_cast<ByteCodeValueType>(OpCodeId::ForkJump));

            bytecode.empend(bytecode_to_repeat.size() + 1 + 4); 

            auto c_label = bytecode.size();
            bytecode.empend(static_cast<ByteCodeValueType>(OpCodeId::Checkpoint));

            bytecode.extend(bytecode_to_repeat);

            bytecode.empend(static_cast<ByteCodeValueType>(OpCodeId::JumpNonEmpty));
            bytecode.empend(-bytecode.size() - 3);          
            bytecode.empend(c_label - bytecode.size() - 2); 
            bytecode.empend((ByteCodeValueType)OpCodeId::Jump);

            bytecode_to_repeat = move(bytecode);
        }

        /**
         * @param bytecode_to_repeat 
         * @param greedy 
         */
        static void transform_bytecode_repetition_zero_or_one(ByteCode& bytecode_to_repeat, bool greedy)
        {
            ByteCode bytecode;

            if (greedy)
                bytecode.empend(static_cast<ByteCodeValueType>(OpCodeId::ForkStay));
            else
                bytecode.empend(static_cast<ByteCodeValueType>(OpCodeId::ForkJump));

            bytecode.empend(bytecode_to_repeat.size()); // Jump to the _END label

            bytecode.extend(move(bytecode_to_repeat));

            bytecode_to_repeat = move(bytecode);
        }

        /**
         * @brief Get the opcode object
         * 
         * @param state 
         * @return OpCode& 
         */
        OpCode& get_opcode(MatchState& state) const;

    private:

        /**
         * @param view 
         */
        void insert_string(StringView view)
        {
            empend((ByteCodeValueType)view.length());
            for (size_t i = 0; i < view.length(); ++i)
                empend((ByteCodeValueType)view[i]);
        }

        void ensure_opcodes_initialized();

        /**
         * @brief Get the opcode by id object
         * 
         * @param id 
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE OpCode& get_opcode_by_id(OpCodeId id) const;

        static OwnPtr<OpCode> s_opcodes[(size_t)OpCodeId::Last + 1];

        static bool s_opcodes_initialized;
    }; // class ByteCode : public DisjointChunks<ByteCodeValueType> 

    #define ENUMERATE_EXECUTION_RESULTS                          \
        __ENUMERATE_EXECUTION_RESULT(Continue)                   \
        __ENUMERATE_EXECUTION_RESULT(Fork_PrioHigh)              \
        __ENUMERATE_EXECUTION_RESULT(Fork_PrioLow)               \
        __ENUMERATE_EXECUTION_RESULT(Failed)                     \
        __ENUMERATE_EXECUTION_RESULT(Failed_ExecuteLowPrioForks) \
        __ENUMERATE_EXECUTION_RESULT(Succeeded)

    enum class ExecutionResult : u8 
    {
    #define __ENUMERATE_EXECUTION_RESULT(x) x,
        ENUMERATE_EXECUTION_RESULTS
    #undef __ENUMERATE_EXECUTION_RESULT
    }; // enum class ExecutionResult : u8 

    /**
     * @param result 
     * @return char const* 
     */
    char const* execution_result_name(ExecutionResult result);

    /**
     * @param opcode_id 
     * @return char const* 
     */
    char const* opcode_id_name(OpCodeId opcode_id);
    char const* boundary_check_type_name(BoundaryCheckType);
    char const* character_compare_type_name(CharacterCompareType result);

    class OpCode 
    {
    public:
        /**
         * @brief Construct a new Op Code object
         * 
         */
        OpCode() = default;

        /**
         * @brief Destroy the Op Code object
         * 
         */
        virtual ~OpCode() = default;

        virtual OpCodeId opcode_id() const = 0;
        virtual size_t size() const = 0;

        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        virtual ExecutionResult execute(MatchInput const& input, MatchState& state) const = 0;

        /**
         * @param offset 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE ByteCodeValueType argument(size_t offset) const
        {
            VERIFY(state().instruction_position + offset <= m_bytecode->size());
            return m_bytecode->at(state().instruction_position + 1 + offset);
        }

        ALWAYS_INLINE char const* name() const;
        
        static char const* name(OpCodeId);

        /**
         * @brief Set the state object
         * 
         * @param state 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void set_state(MatchState& state) 
        { 
            m_state = &state; 
        }

        /**
         * @brief Set the bytecode object
         * 
         * @param bytecode 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE void set_bytecode(ByteCode& bytecode) { m_bytecode = &bytecode; }

        ALWAYS_INLINE MatchState const& state() const
        {
            VERIFY(m_state);
            return *m_state;
        }

        String to_string() const
        {
            return String::formatted("[{:02X}] {}", (int)opcode_id(), name(opcode_id()));
        }

        virtual String arguments_string() const = 0;

        ALWAYS_INLINE ByteCode const& bytecode() const 
        { 
            return *m_bytecode; 
        }

    protected:
        ByteCode* m_bytecode { nullptr };
        MatchState* m_state { nullptr };
    }; // class OpCode

    class OpCode_Exit final : public OpCode 
    {
    public:
        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        ExecutionResult execute(MatchInput const& input, MatchState& state) const override;

        ALWAYS_INLINE OpCodeId opcode_id() const override 
        { 
            return OpCodeId::Exit; 
        }

        ALWAYS_INLINE size_t size() const override 
        { 
            return 1; 
        }

        String arguments_string() const override 
        { 
            return String::empty(); 
        }
    }; // class OpCode_Exit final : public OpCode 

    class OpCode_FailForks final : public OpCode 
    {
    public:
        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        ExecutionResult execute(MatchInput const& input, MatchState& state) const override;

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE OpCodeId opcode_id() const override 
        { 
            return OpCodeId::FailForks; 
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE size_t size() const override 
        { 
            return 1; 
        }

        String arguments_string() const override 
        { 
            return String::empty(); 
        }
    };

    class OpCode_Save final : public OpCode 
    {
    public:
        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        ExecutionResult execute(MatchInput const& input, MatchState& state) const override;

        ALWAYS_INLINE OpCodeId opcode_id() const override 
        { 
            return OpCodeId::Save; 
        }

        ALWAYS_INLINE size_t size() const override 
        { 
            return 1; 
        }

        String arguments_string() const override 
        { 
            return String::empty(); 
        }
    }; // class OpCode_Save final : public OpCode 

    class OpCode_Restore final : public OpCode 
    {
    public:
        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        ExecutionResult execute(MatchInput const& input, MatchState& state) const override;

        ALWAYS_INLINE OpCodeId opcode_id() const override 
        { 
            return OpCodeId::Restore; 
        }

        ALWAYS_INLINE size_t size() const override 
        { 
            return 1; 
        }

        String arguments_string() const override 
        { 
            return String::empty(); 
        }
    }; // class OpCode_Restore final : public OpCode 

    class OpCode_GoBack final : public OpCode 
    {
    public:
        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        ExecutionResult execute(MatchInput const& input, MatchState& state) const override;

        ALWAYS_INLINE OpCodeId opcode_id() const override 
        { 
            return OpCodeId::GoBack; 
        }

        ALWAYS_INLINE size_t size() const override 
        { 
            return 2; 
        }

        ALWAYS_INLINE size_t count() const 
        { 
            return argument(0); 
        }

        String arguments_string() const override 
        { 
            return String::formatted("count={}", count()); 
        }
    }; // class OpCode_GoBack final : public OpCode 

    class OpCode_Jump final : public OpCode 
    {
    public:
        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        ExecutionResult execute(MatchInput const& input, MatchState& state) const override;

        ALWAYS_INLINE OpCodeId opcode_id() const override 
        { 
            return OpCodeId::Jump; 
        }

        ALWAYS_INLINE size_t size() const override 
        { 
            return 2; 
        }

        ALWAYS_INLINE ssize_t offset() const 
        { 
            return argument(0); 
        }

        String arguments_string() const override
        {
            return String::formatted("offset={} [&{}]", offset(), state().instruction_position + size() + offset());
        }
    }; // class OpCode_Jump final : public OpCode 

    class OpCode_ForkJump : public OpCode 
    {
    public:
        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        ExecutionResult execute(MatchInput const& input, MatchState& state) const override;

        ALWAYS_INLINE OpCodeId opcode_id() const override 
        { 
            return OpCodeId::ForkJump; 
        }

        ALWAYS_INLINE size_t size() const override 
        { 
            return 2; 
        }

        ALWAYS_INLINE ssize_t offset() const 
        { 
            return argument(0); 
        }

        String arguments_string() const override
        {
            return String::formatted("offset={} [&{}], sp: {}", offset(), state().instruction_position + size() + offset(), state().string_position);
        }
    }; // class OpCode_ForkJump : public OpCode 

    class OpCode_ForkReplaceJump final : public OpCode_ForkJump 
    {
    public:
        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        ExecutionResult execute(MatchInput const& input, MatchState& state) const override;

        ALWAYS_INLINE OpCodeId opcode_id() const override 
        { 
            return OpCodeId::ForkReplaceJump; 
        }
    }; // class OpCode_ForkReplaceJump final : public OpCode_ForkJump 

    class OpCode_ForkStay : public OpCode 
    {
    public:
        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        ExecutionResult execute(MatchInput const& input, MatchState& state) const override;

        ALWAYS_INLINE OpCodeId opcode_id() const override 
        { 
            return OpCodeId::ForkStay; 
        }

        ALWAYS_INLINE size_t size() const override 
        { 
            return 2;
        }

        ALWAYS_INLINE ssize_t offset() const 
        { 
            return argument(0); 
        }

        String arguments_string() const override
        {
            return String::formatted("offset = {} [&{}], sp: {}", offset(), state().instruction_position + size() + offset(), state().string_position);
        }
    }; // class OpCode_ForkStay : public OpCode 

    class OpCode_ForkReplaceStay final : public OpCode_ForkStay 
    {
    public:
        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        ExecutionResult execute(MatchInput const& input, MatchState& state) const override;

        ALWAYS_INLINE OpCodeId opcode_id() const override 
        { 
            return OpCodeId::ForkReplaceStay; 
        }
    }; // class OpCode_ForkReplaceStay final : public OpCode_ForkStay 

    class OpCode_CheckBegin final : public OpCode 
    {
    public:
        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        ExecutionResult execute(MatchInput const& input, MatchState& state) const override;

        ALWAYS_INLINE OpCodeId opcode_id() const override 
        { 
            return OpCodeId::CheckBegin; 
        }

        ALWAYS_INLINE size_t size() const override 
        { 
            return 1; 
        }

        String arguments_string() const override 
        { 
            return String::empty(); 
        }
    }; // class OpCode_CheckBegin final : public OpCode 

    class OpCode_CheckEnd final : public OpCode 
    {
    public:
        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        ExecutionResult execute(MatchInput const& input, MatchState& state) const override;

        ALWAYS_INLINE OpCodeId opcode_id() const override 
        { 
            return OpCodeId::CheckEnd; 
        }

        ALWAYS_INLINE size_t size() const override 
        { 
            return 1; 
        }

        String arguments_string() const override 
        { 
            return String::empty(); 
        }
    }; // class OpCode_CheckEnd final : public OpCode 

    class OpCode_CheckBoundary final : public OpCode 
    {
    public:
        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        ExecutionResult execute(MatchInput const& input, MatchState& state) const override;

        ALWAYS_INLINE OpCodeId opcode_id() const override 
        { 
            return OpCodeId::CheckBoundary; 
        }

        ALWAYS_INLINE size_t size() const override 
        { 
            return 2; 
        }
        
        ALWAYS_INLINE size_t arguments_count() const 
        { 
            return 1; 
        }

        ALWAYS_INLINE BoundaryCheckType type() const 
        { 
            return static_cast<BoundaryCheckType>(argument(0)); 
        }
        
        String arguments_string() const override 
        { 
            return String::formatted("kind={} ({})", (long unsigned int)argument(0), boundary_check_type_name(type())); 
        }
    }; // class OpCode_CheckBoundary final : public OpCode

    class OpCode_ClearCaptureGroup final : public OpCode 
    {
    public:
        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        ExecutionResult execute(MatchInput const& input, MatchState& state) const override;

        ALWAYS_INLINE OpCodeId opcode_id() const override 
        { 
            return OpCodeId::ClearCaptureGroup; 
        }

        ALWAYS_INLINE size_t size() const override 
        { 
            return 2; 
        }

        ALWAYS_INLINE size_t id() const 
        { 
            return argument(0); 
        }
        
        String arguments_string() const override 
        { 
            return String::formatted("id={}", id()); 
        }
    }; // class OpCode_ClearCaptureGroup final : public OpCode

    class OpCode_SaveLeftCaptureGroup final : public OpCode 
    {
    public:
        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        ExecutionResult execute(MatchInput const& input, MatchState& state) const override;


        ALWAYS_INLINE OpCodeId opcode_id() const override 
        { 
            return OpCodeId::SaveLeftCaptureGroup; 
        }

        ALWAYS_INLINE size_t size() const override 
        { 
            return 2; 
        }
        
        ALWAYS_INLINE size_t id() const 
        { 
            return argument(0); 
        }

        String arguments_string() const override 
        { 
            return String::formatted("id={}", id()); 
        }
    }; // class OpCode_SaveLeftCaptureGroup final : public OpCode

    class OpCode_SaveRightCaptureGroup final : public OpCode 
    {
    public:
        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        ExecutionResult execute(MatchInput const& input, MatchState& state) const override;

        ALWAYS_INLINE OpCodeId opcode_id() const override 
        { 
            return OpCodeId::SaveRightCaptureGroup; 
        }

        ALWAYS_INLINE size_t size() const override 
        { 
            return 2; 
        }

        ALWAYS_INLINE size_t id() const 
        { 
            return argument(0); 
        }

        String arguments_string() const override 
        { 
            return String::formatted("id={}", id()); 
        }
    }; // class OpCode_SaveRightCaptureGroup final : public OpCode

    class OpCode_SaveRightNamedCaptureGroup final : public OpCode 
    {
    public:
        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        ExecutionResult execute(MatchInput const& input, MatchState& state) const override;

        ALWAYS_INLINE OpCodeId opcode_id() const override 
        { 
            return OpCodeId::SaveRightNamedCaptureGroup; 
        }
        
        ALWAYS_INLINE size_t size() const override 
        { 
            return 4; 
        }
        
        ALWAYS_INLINE StringView name() const 
        { 
            return { reinterpret_cast<char*>(argument(0)), length() }; 
        }

        ALWAYS_INLINE size_t length() const 
        { 
            return argument(1); 
        }

        ALWAYS_INLINE size_t id() const 
        { 
            return argument(2); 
        }

        String arguments_string() const override
        {
            return String::formatted("name={}, length={}", name(), length());
        }
    }; // class OpCode_SaveRightNamedCaptureGroup final : public OpCode

    class OpCode_Compare final : public OpCode 
    {
    public:
        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        ExecutionResult execute(MatchInput const& input, MatchState& state) const override;

        ALWAYS_INLINE OpCodeId opcode_id() const override 
        { 
            return OpCodeId::Compare; 
        }

        ALWAYS_INLINE size_t size() const override 
        { 
            return arguments_size() + 3; 
        }

        ALWAYS_INLINE size_t arguments_count() const 
        { 
            return argument(0); 
        }

        ALWAYS_INLINE size_t arguments_size() const 
        { 
            return argument(1);
        }

        String arguments_string() const override;

        Vector<String> variable_arguments_to_string(Optional<MatchInput> input = {}) const;

        Vector<CompareTypeAndValuePair> flat_compares() const;

    private:
        /**
         * @param input 
         * @param state 
         * @param ch1 
         * @param inverse 
         * @param inverse_matched 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static void compare_char(MatchInput const& input, MatchState& state, u32 ch1, bool inverse, bool& inverse_matched);

        /**
         * @param input 
         * @param state 
         * @param str 
         * @param had_zero_length_match 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static bool compare_string(MatchInput const& input, MatchState& state, RegexStringView str, bool& had_zero_length_match);

        /**
         * @param input 
         * @param state 
         * @param character_class 
         * @param ch 
         * @param inverse 
         * @param inverse_matched 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static void compare_character_class(MatchInput const& input, MatchState& state, CharClass character_class, u32 ch, bool inverse, bool& inverse_matched);

        /**
         * @param input 
         * @param state 
         * @param from 
         * @param to 
         * @param ch 
         * @param inverse 
         * @param inverse_matched 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static void compare_character_range(MatchInput const& input, MatchState& state, u32 from, u32 to, u32 ch, bool inverse, bool& inverse_matched);

        /**
         * @param input 
         * @param state 
         * @param property 
         * @param inverse 
         * @param inverse_matched 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static void compare_property(MatchInput const& input, MatchState& state, Unicode::Property property, bool inverse, bool& inverse_matched);

        /**
         * @param input 
         * @param state 
         * @param general_category 
         * @param inverse 
         * @param inverse_matched 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static void compare_general_category(MatchInput const& input, MatchState& state, Unicode::GeneralCategory general_category, bool inverse, bool& inverse_matched);

        /**
         * @param input 
         * @param state 
         * @param script 
         * @param inverse 
         * @param inverse_matched 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static void compare_script(MatchInput const& input, MatchState& state, Unicode::Script script, bool inverse, bool& inverse_matched);

        /**
         * @param input 
         * @param state 
         * @param script 
         * @param inverse 
         * @param inverse_matched 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static void compare_script_extension(MatchInput const& input, MatchState& state, Unicode::Script script, bool inverse, bool& inverse_matched);
    }; // class OpCode_Compare final : public OpCode 

    class OpCode_Repeat : public OpCode 
    {
    public:
        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        ExecutionResult execute(MatchInput const& input, MatchState& state) const override;

        ALWAYS_INLINE OpCodeId opcode_id() const override 
        { 
            return OpCodeId::Repeat; 
        }

        ALWAYS_INLINE size_t size() const override 
        { 
            return 4; 
        }

        ALWAYS_INLINE size_t offset() const 
        { 
            return argument(0); 
        }

        ALWAYS_INLINE u64 count() const 
        { 
            return argument(1); 
        }

        ALWAYS_INLINE size_t id() const 
        { 
            return argument(2); 
        }

        String arguments_string() const override
        {
            auto reps = id() < state().repetition_marks.size() ? state().repetition_marks.at(id()) : 0;
            return String::formatted("offset={} count={} id={} rep={}, sp: {}", offset(), count() + 1, id(), reps + 1, state().string_position);
        }
    }; // class OpCode_Repeat : public OpCode 

    class OpCode_ResetRepeat : public OpCode 
    {
    public:
        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        ExecutionResult execute(MatchInput const& input, MatchState& state) const override;

        ALWAYS_INLINE OpCodeId opcode_id() const override 
        { 
            return OpCodeId::ResetRepeat; 
        }

        ALWAYS_INLINE size_t size() const override 
        { 
            return 2; 
        }

        ALWAYS_INLINE size_t id() const 
        { 
            return argument(0); 
        }

        String arguments_string() const override
        {
            auto reps = id() < state().repetition_marks.size() ? state().repetition_marks.at(id()) : 0;
            return String::formatted("id={} rep={}", id(), reps + 1);
        }
    }; // class OpCode_ResetRepeat : public OpCode 

    class OpCode_Checkpoint final : public OpCode 
    {
    public:
        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        ExecutionResult execute(MatchInput const& input, MatchState& state) const override;

        ALWAYS_INLINE OpCodeId opcode_id() const override 
        { 
            return OpCodeId::Checkpoint; 
        }

        ALWAYS_INLINE size_t size() const override 
        { 
            return 1; 
        }

        String arguments_string() const override 
        { 
            return String::empty(); 
        }
    }; // class OpCode_Checkpoint final : public OpCode 

    class OpCode_JumpNonEmpty final : public OpCode 
    {
    public:
        /**
         * @param input 
         * @param state 
         * @return ExecutionResult 
         */
        ExecutionResult execute(MatchInput const& input, MatchState& state) const override;

        ALWAYS_INLINE OpCodeId opcode_id() const override 
        { 
            return OpCodeId::JumpNonEmpty; 
        }

        ALWAYS_INLINE size_t size() const override 
        { 
            return 4; 
        }

        ALWAYS_INLINE ssize_t offset() const 
        { 
            return argument(0); 
        }
        
        ALWAYS_INLINE ssize_t checkpoint() const 
        { 
            return argument(1); 
        }

        ALWAYS_INLINE OpCodeId form() const 
        { 
            return (OpCodeId)argument(2); 
        }

        String arguments_string() const override
        {
            return String::formatted("{} offset={} [&{}], cp={} [&{}]",
                opcode_id_name(form()),
                offset(), state().instruction_position + size() + offset(),
                checkpoint(), state().instruction_position + size() + checkpoint());
        }
    }; // class OpCode_JumpNonEmpty final : public OpCode 

    /**
     * @tparam T 
     * @return true 
     * @return false 
     */
    template<typename T>
    bool is(OpCode const&);

    /**
     * @tparam T 
     * @return ALWAYS_INLINE 
     */
    template<typename T>
    ALWAYS_INLINE bool is(OpCode const&)
    {
        return false;
    }

    /**
     * @tparam T 
     * @param opcode 
     * @return ALWAYS_INLINE 
     */
    template<typename T>
    ALWAYS_INLINE bool is(OpCode const* opcode)
    {
        return is<T>(*opcode);
    }

    /**
     * @tparam  
     * @param opcode 
     * @return ALWAYS_INLINE 
     */
    template<>
    ALWAYS_INLINE bool is<OpCode_ForkStay>(OpCode const& opcode)
    {
        return opcode.opcode_id() == OpCodeId::ForkStay;
    }

    /**
     * @tparam  
     * @param opcode 
     * @return ALWAYS_INLINE 
     */
    template<>
    ALWAYS_INLINE bool is<OpCode_Exit>(OpCode const& opcode)
    {
        return opcode.opcode_id() == OpCodeId::Exit;
    }

    /**
     * @tparam  
     * @param opcode 
     * @return ALWAYS_INLINE 
     */
    template<>
    ALWAYS_INLINE bool is<OpCode_Compare>(OpCode const& opcode)
    {
        return opcode.opcode_id() == OpCodeId::Compare;
    }

    /**
     * @tparam T 
     * @param opcode 
     * @return ALWAYS_INLINE const& 
     */
    template<typename T>
    ALWAYS_INLINE T const& to(OpCode const& opcode)
    {
        return verify_cast<T>(opcode);
    }

    /**
     * @tparam T 
     * @param opcode 
     * @return ALWAYS_INLINE* 
     */
    template<typename T>
    ALWAYS_INLINE T* to(OpCode* opcode)
    {
        return verify_cast<T>(opcode);
    }

    /**
     * @tparam T 
     * @param opcode 
     * @return ALWAYS_INLINE const* 
     */
    template<typename T>
    ALWAYS_INLINE T const* to(OpCode const* opcode)
    {
        return verify_cast<T>(opcode);
    }

    /**
     * @tparam T 
     * @param opcode 
     * @return ALWAYS_INLINE& 
     */
    template<typename T>
    ALWAYS_INLINE T& to(OpCode& opcode)
    {
        return verify_cast<T>(opcode);
    }
} // namespace regex