#ifndef _LIBC_STDIO_H
#define _LIBC_STDIO_H

// includes
#include <stddef.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

#define _IOFBF 0 
#define _IOLBF 1 
#define _IONBF 2 
#define BUFSIZ 1024 
#define EOF (-1)
#define UNGOTC_EMPTY (-0x400)

#define STDIN 0
#define STDOUT 1
#define STDERR 2

typedef struct __fbuf __fbuf_t;
typedef struct __rwbuf __rwbuf_t;
typedef struct __file FILE;

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

FILE* fopen(const char* filename, const char* mode);

int fclose(FILE* stream);

size_t fread(void* buf, size_t size, size_t count, FILE* stream);

size_t fwrite(const void* buf, size_t size, size_t count, FILE* stream);

int fseek(FILE* stream, uint32_t offset, int origin);

int fflush(FILE* stream);

int fputc(int c, FILE* stream);
int putc(int c, FILE* stream) __use_instead("fputc");
int putchar(int c);

int fputs(const char* s, FILE* stream);
int puts(const char* s);

int fgetc(FILE* stream);
int getc(FILE* stream) __use_instead("fgets");
int getchar();
int ungetc(int c, FILE* stream);

char* fgets(char* str, int size, FILE* stream);

int setvbuf(FILE* stream, char* buf, int mode, size_t size);

void setbuf(FILE* stream, char* buf);

void setlinebuf(FILE* stream);

int vsnprintf(char* buf, size_t n, const char* fmt, va_list arg);
int vsprintf(char* buf, const char* fmt, va_list arg);
int snprintf(char* buf, size_t n, const char* fmt, ...);
int sprintf(char* buf, const char* fmt, ...);
int fprintf(FILE* stream, const char* fmt, ...);
int printf(const char* fmt, ...);

int vsscanf(const char* buf, const char* fmt, va_list arg);
int sscanf(const char* buf, const char* fmt, ...);
int scanf(const char* fmt, ...);

__END_DECLS

#endif 