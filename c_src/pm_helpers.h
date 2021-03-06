#ifndef PM_HELPERS_H
#define PM_HELPERS_H

#define DEFAULT_PATH "/bin:/usr/bin:/usr/local/bin:/sbin;"

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <paths.h>
#include <ctype.h>
#include <errno.h>

/* Defines */
#ifndef MIN_BUFFER_SIZE
#define MIN_BUFFER_SIZE 128
#endif
#ifndef BUFFER_SZ
#define BUFFER_SZ 1024
#endif

#ifndef MAX_BUFFER_SZ
#define MAX_BUFFER_SZ 65536
#endif

#ifndef MAX_ENV
#define MAX_ENV 256
#endif

#ifndef PREFIX_LEN
#define PREFIX_LEN 8
#endif

#ifndef FATAL_ERROR
#define FATAL_ERROR(m,x) { perror(m); exit(x); }
#endif

/* prototypes */
char* read_from_file(const char *filename);
pid_t get_pid_from_file_or_retry(const char* filename, int retries);
int pm_abs_path(const char *path);
const char *find_binary(const char *file, const char* path);
int string_index(const char* cmds[], const char *cmd);
int argify(const char *line, char ***argv_ptr);
char* str_chomp(const char *string);
char* str_safe_quote(const char *str);

#endif
