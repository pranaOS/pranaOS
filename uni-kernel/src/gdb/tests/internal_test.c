/**
 * @file test_gdb_internal.c
 * @author Krisna Pranav
 * @brief Unit tests for gdb_internal.h and gdbutil.c
 * @version 1.0
 * @date 2025-09-03
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include <gdb_internal.h>

void test_get_char()
{
    printf("Testing get_char...\n");
    buffer b = allocate_buffer(10);
    strcpy(b->contents, "abc");
    b->end = 3;

    assert(get_char(b) == 'a');
    assert(b->start == 1);
    assert(get_char(b) == 'b');
    assert(b->start == 2);
    assert(get_char(b) == 'c');
    assert(b->start == 3);
    assert(get_char(b) == 0); // Empty buffer
    assert(b->start == 3);

    deallocate_buffer(b);
    printf("test_get_char passed.\n");
}

void test_check()
{
    printf("Testing check...\n");
    buffer b = allocate_buffer(10);
    strcpy(b->contents, "abc");
    b->end = 3;

    assert(check(b, 'a') == true);
    assert(b->start == 1);
    assert(check(b, 'c') == false);
    assert(b->start == 1);
    assert(check(b, 'b') == true);
    assert(b->start == 2);

    buffer_consume(b, 1);
    assert(buffer_length(b) == 0);
    assert(check(b, 'd') == false);

    deallocate_buffer(b);
    printf("test_check passed.\n");
}

void test_reset_buffer()
{
    printf("Testing reset_buffer...\n");
    string s = allocate_buffer(10);
    s->start = 2;
    s->end = 8;

    reset_buffer(s);
    assert(s->start == 0);
    assert(s->end == 0);

    deallocate_buffer(s);
    printf("test_reset_buffer passed.\n");
}

void test_parse_hex_pair()
{
    printf("Testing parse_hex_pair...\n");
    buffer b = allocate_buffer(20);
    strcpy(b->contents, "1a,2b");
    b->end = strlen(b->contents);

    u64 first, second;
    boolean result = parse_hex_pair(b, &first, &second);

    assert(result == true);
    assert(first == 0x1a);
    assert(second == 0x2b);
    assert(buffer_length(b) == 0);

    deallocate_buffer(b);
    printf("test_parse_hex_pair passed.\n");
}

void test_mem2hex()
{
    printf("Testing mem2hex...\n");
    string b = allocate_buffer(20);
    unsigned char mem[] = {0x12, 0xab, 0xff};

    boolean result = mem2hex(b, mem, sizeof(mem));

    assert(result == true);
    b->contents[b->end] = '\0'; // Null-terminate for strcmp
    assert(strcmp(b->contents, "12abff") == 0);

    deallocate_buffer(b);
    printf("test_mem2hex passed.\n");
}

void test_hex2mem()
{
    printf("Testing hex2mem...\n");
    buffer b = allocate_buffer(20);
    strcpy(b->contents, "12abff");
    b->end = strlen(b->contents);

    unsigned char mem[3];
    boolean result = hex2mem(b, mem, sizeof(mem));

    assert(result == true);
    assert(mem[0] == 0x12);
    assert(mem[1] == 0xab);
    assert(mem[2] == 0xff);

    deallocate_buffer(b);
    printf("test_hex2mem passed.\n");
}

int main()
{
    test_get_char();
    test_check();
    test_reset_buffer();
    test_parse_hex_pair();
    test_mem2hex();
    test_hex2mem();

    printf("\nAll gdb utility tests passed!\n");
    return 0;
}
