#include "utils/path.h"
#include "utils/string.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH_LEN 260
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#   include <direct.h>
#   define get_cwd(b, l) _getcwd((b), (int)(l))
#   define PATH_SEP L"\\"
#else
#   include <unistd.h>
#   define get_cwd(b, l) getcwd((b), (size_t)(l))
#   define PATH_SEP L"/"
#endif

static int is_path_sep(wchar_t c);

wchar_t * path_get_cwd(void)
{
    wchar_t * wcwd = NULL;
    char * cwd = NULL;

    cwd = get_cwd(cwd, MAX_PATH_LEN);
    if (!cwd) return NULL;

    wcwd = (wchar_t *)malloc((MAX_PATH_LEN + 1) * sizeof(wchar_t));
    if (!wcwd) return NULL;

    char_to_wchar_len(cwd, wcwd, MAX_PATH_LEN);
    wcwd[MAX_PATH_LEN] = 0;

    return wcwd;
}

wchar_t * path_get_dir(wchar_t * path)
{
    wchar_t * dir = NULL;
    int idx = 0;
    size_t len = 0;

    len = wcsnlen(path, MAX_PATH_LEN);
    dir = (wchar_t *)malloc((len + 1) * sizeof(wchar_t));
    if (!dir) return NULL;

    wcsncpy(dir, path, len);
    dir[len] = 0;

    for (idx = (int)(len - 1); idx >= 0; --idx)
    {
        if (is_path_sep(dir[idx]))
        {
            dir[idx + 1] = 0;
            break;
        }
    }

    return dir;
}

wchar_t * path_join(wchar_t * path, ...)
{
    va_list args;
    wchar_t *s = NULL, *joined_path = 0;
    int idx = 0;
    size_t path_len = 0;

    joined_path = (wchar_t *)malloc((MAX_PATH_LEN + 1) * sizeof(wchar_t));
    if (!joined_path) return NULL;
    wcsncpy(joined_path, path, MAX_PATH_LEN);
    path_len = wcsnlen(path, MAX_PATH_LEN);
    if (is_path_sep(joined_path[path_len - 1]))
    {
        joined_path[path_len - 1] = 0;
        idx = (int)(path_len - 1);
    }
    else
    {
        joined_path[path_len] = 0;
        idx = (int)path_len;
    }

    va_start(args, path);

    for (s = va_arg(args, wchar_t *); s && idx < MAX_PATH_LEN; s = va_arg(args, wchar_t *))
    {
        path_len = wcsnlen(s, MAX_PATH_LEN);
        if (path_len > 0)
        {
            if (!is_path_sep(s[0]))
            {
                wcsncpy(&joined_path[idx], PATH_SEP, (size_t)(MAX_PATH_LEN - idx));
                ++idx;
            }

            wcsncpy(&joined_path[idx], s, (size_t)(MAX_PATH_LEN - idx));
            idx += (int)(wcsnlen(s, MAX_PATH_LEN)); 
        }
    }

    va_end(args);

    return joined_path;
}

static int is_path_sep(wchar_t c)
{
    return (c == '/' || c == '\\');
}

