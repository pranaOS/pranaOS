#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <libpranaos/syscalls.h>

struct FILE {
    int flags;
    off_t offset;
    char* buff;
    int buff_size;
    int fd;
    int status;
};

static FILE _stdin = { .flags = O_RDONLY, .offset = 0, .buff = nullptr, .buff_size = 0, .fd = STDIN_FILENO, .status = 0 };
static FILE _stdout = { .flags = O_WRONLY, .offset = 0, .buff = nullptr, .buff_size = 0, .fd = STDOUT_FILENO, .status = 0 };
static FILE _stderr = { .flags = O_WRONLY, .offset = 0, .buff = nullptr, .buff_size = 0, .fd = STDERR_FILENO, .status = 0  };

FILE* stdint = &_stdin;
FILE* stdout = &_stdout;
FILE* stderr = &_stderr;

FILE* fdopen(int fd, const char* mode)
{
    FILE* file = (FILE*)malloc(sizeof(FILE));

    file->flags = O_RDWR; 
    file->offset = 0;
    file->buff = nullptr;
    file->buff_size = 0;
    file->fd = fd;
    file->status = 0;

    return file;
}

size_t fread(void* buff, size_t size, size_t count, FILE* file)
{
    size_t bytes = count * size;
    size_t bytes_read = read(file->fd, buff, bytes);

    return count;
}

size_t fwrite(const void* buff, size_t size, size_t count, FILE* file)
{
    size_t bytes = count * size;
    size_t bytes_written = write(file->fd, buff, bytes);

    if (bytes == bytes_written)
    {
        return count;
    }

    return 0;
}

int fputc(int c, FILE* file)
{
    fwrite((const void*)&c, 1, 1, file);
    return c;
}

int fgetc(FILE* file)
{
    char c;

    return (fread(&c, 1, 1, file) == 1) ? (int)c : EOF;
}

int fputs(const char* str, FILE* file)
{
    fwrite(str, sizeof(char), strlen(str), file);

    return 0;
}

int putc(int c, FILE* file)
{
    return fputc(c, file);
}

int getc(FILE* file)
{
    return fgetc(file);
}

int putchar(int c)
{
    return fputc(c, stdout);
}

int getchar()
{
    return fgetc(stdin);
}