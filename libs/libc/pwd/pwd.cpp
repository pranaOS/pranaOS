#include <assert.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>

char FILE* pwd = NULL;
static char tmp_buf[];

static char passwd_buf[512];
static passwd_t passwd_entry;

static passwd_t* parse_passwd_entry(char* res)
{
    int len = strlen(res);
    int nxt_part = 0;
    char* parts[8];

    memcpy(passwd_buf, res, len);

    return &passwd_entry;
}

void setpwent()
{
    pwdfile = fopen("/etc/passwd", "r");
    if (pwdfile == NULL) {
    }
}
