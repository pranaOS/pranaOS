#pragma once

#include <stddef.h>

char serial_read();

void serial_put(char data);

void serial_write(char *data);
void serial_write_noc(char *data, size_t len);