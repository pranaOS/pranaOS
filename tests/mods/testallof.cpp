/**
 * @file testallof.cpp
 * @author Krisna Pranav
 * @brief test all of 
 * @version 6.0
 * @date 2025-08-29
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <libtest/testcase.h>
#include <mods/allof.h>
#include <mods/array.h>
#include <mods/vector.h>

using namespace Test::Randomized;

TEST_CASE(vacuous_truth)
{
    constexpr Array<int, 0> a {};
    static_assert(all_of(a.begin(), a.end(), [](auto) { return false; }));
    EXPECT(all_of(a.begin(), a.end(), [](auto) { return false; }));
}

TEST_CASE(all_but_one_false)
{
    constexpr Array<int, 5> a { 0, 1, 2, 3, 4 };
    static_assert(!all_of(a.begin(), a.end(), [](auto n) { return n != 3; }));
    EXPECT(!all_of(a.begin(), a.end(), [](auto n) { return n != 3; }));
}

RANDOMIZED_TEST_CASE(trivial_all_true)
{
    GEN(vec, Gen::vector(0, 10, []() { return Gen::number_u64(); }));
    EXPECT(all_of(vec.begin(), vec.end(), [](auto) { return true; }));
}

RANDOMIZED_TEST_CASE(trivial_all_false)
{
    GEN(vec, Gen::vector(1, 10, []() { return Gen::number_u64(); }));
    EXPECT(!all_of(vec.begin(), vec.end(), [](auto) { return false; }));
}

/**
 * @brief Construct a new test case object
 * 
 */
TEST_CASE(should_determine_if_predicate_applies_to_all_elements_in_container)
{
    constexpr Array<int, 10> a {};

    static_assert(all_of(a.begin(), a.end(), [](auto elem) { return elem == 0; }));
    static_assert(!all_of(a.begin(), a.end(), [](auto elem) { return elem == 1; }));

    EXPECT(all_of(a.begin(), a.end(), [](auto elem) { return elem == 0; }));
    EXPECT(!all_of(a.begin(), a.end(), [](auto elem) { return elem == 1; }));
}

TEST_CASE(container_form)
{
    constexpr Array a { 10, 20, 30 };
    static_assert(all_of(a, [](auto elem) { return elem > 0; }));
    static_assert(!all_of(a, [](auto elem) { return elem > 10; }));
    EXPECT(all_of(a, [](auto elem) { return elem > 0; }));
    EXPECT(!all_of(a, [](auto elem) { return elem > 10; }));

    Vector b { 10, 20, 30 };
    EXPECT(all_of(b, [](auto elem) { return elem > 0; }));
    EXPECT(!all_of(b, [](auto elem) { return elem > 10; }));

    struct ArbitraryIterable 
    {
        struct ArbitraryIterator 
        {
            /**
             * @brief Construct a new Arbitrary Iterator object
             * 
             * @param v 
             */
            ArbitraryIterator(int v)
                : value(v)
            {
            }

            /**
             * @return true 
             * @return false 
             */
            bool operator==(ArbitraryIterator const&) const = default;

            /**
             * @return int 
             */
            int operator*() const 
            { 
                return value; 
            }

            /**
             * @return ArbitraryIterator& 
             */
            ArbitraryIterator& operator++()
            {
                ++value;
                return *this;
            }

            int value;
        }; // struct ArbitraryIterator

        /**
         * @return ArbitraryIterator 
         */
        ArbitraryIterator begin() const 
        { 
            return 0; 
        }

        ArbitraryIterator end() const 
        { 
            return 20; 
        }
    }; // struct ArbitraryIterable

    ArbitraryIterable c;

    /**
     * @brief Construct a new EXPECT object
     * 
     */
    EXPECT(all_of(c, [](auto elem) { return elem < 20; }));
    EXPECT(!all_of(c, [](auto elem) { return elem > 10; }));
}
