#include "utils/path.h"
#include "utils/string.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

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

    cwd = get_cwd(cwd, PATH_MAX_LEN);
    if (!cwd) return NULL;

    wcwd = (wchar_t *)malloc((PATH_MAX_LEN + 1) * sizeof(wchar_t));
    if (!wcwd) return NULL;

    char_to_wchar_len(cwd, wcwd, PATH_MAX_LEN);
    wcwd[PATH_MAX_LEN] = 0;

    return wcwd;
}

wchar_t * path_get_dir(const wchar_t * path)
{
    wchar_t * dir = NULL;
    int idx = 0;
    size_t len = 0;

    len = wcsnlen(path, PATH_MAX_LEN);
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

wchar_t * path_join(const wchar_t * path, ...)
{
    va_list args;
    wchar_t *s = NULL, *joined_path = 0;
    int idx = 0;
    size_t path_len = 0;

    joined_path = (wchar_t *)malloc((PATH_MAX_LEN + 1) * sizeof(wchar_t));
    if (!joined_path) return NULL;
    wcsncpy(joined_path, path, PATH_MAX_LEN);
    path_len = wcsnlen(path, PATH_MAX_LEN);
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

    for (s = va_arg(args, wchar_t *); s && idx < PATH_MAX_LEN; s = va_arg(args, wchar_t *))
    {
        path_len = wcsnlen(s, PATH_MAX_LEN);
        if (path_len > 0)
        {
            if (!is_path_sep(s[0]))
            {
                wcsncpy(&joined_path[idx], PATH_SEP, (size_t)(PATH_MAX_LEN - idx));
                ++idx;
            }

            wcsncpy(&joined_path[idx], s, (size_t)(PATH_MAX_LEN - idx));
            idx += (int)(wcsnlen(s, PATH_MAX_LEN)); 
        }
    }

    va_end(args);

    return joined_path;
}

int path_has_ext(const wchar_t * path, const wchar_t * ext)
{
    size_t path_len = 0, ext_len = 0;

    path_len = wcsnlen(path, PATH_MAX_LEN);
    ext_len = wcsnlen(ext, PATH_MAX_LEN);

    if (path_len < ext_len) return 0;

    return wcsncmp(&path[path_len - ext_len], ext, ext_len) == 0;
}

wchar_t * path_trim_ext(const wchar_t * path)
{
    wchar_t * new_path = NULL;
    size_t path_len = 0, idx = 0;

    path_len = wcsnlen(path, PATH_MAX_LEN);
    new_path = (wchar_t *)malloc((path_len + 1) * sizeof(wchar_t));
    if (!new_path) return NULL;

    wcsncpy(new_path, path, path_len);
    new_path[path_len] = 0;

    for (idx = path_len - 1; idx >= 0; --idx)
    {
        if (path[idx] == '.')
        {
            new_path[idx] = 0;
            return new_path;
        }
    }

    free(new_path);
    return NULL;
}

static int is_path_sep(wchar_t c)
{
    return (c == '/' || c == '\\');
}

