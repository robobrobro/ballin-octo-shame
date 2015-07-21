#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#   include <direct.h>
#   define _get_cwd(b, l) _getcwd((b), (int)(l))
#   define PATH_SEP L"\\"
#else
#   include <unistd.h>
#   define _get_cwd(b, l) getcwd((b), (size_t)(l))
#   define PATH_SEP L"/"
#endif

#include "utils/path.h"
#include "utils/string.h"

static int is_path_sep(wchar_t c);

std::wstring utils::path::get_cwd(void)
{
    char * cwd = _get_cwd(NULL, utils::path::MAX_LEN + 1);
    if (!cwd) return std::wstring(L"");

    std::wstring wcwd = utils::string::cstr_to_wstr(cwd);
    free(cwd);
    return wcwd;
}

std::wstring utils::path::get_dir(const std::wstring & path)
{
    wprintf(L"path=%ls\n", path.c_str());
    auto pos = path.find_last_of(PATH_SEP);
    wprintf(L"path=%ls\n", path.c_str());
    if (pos == std::wstring::npos) return path;

    wprintf(L"path=%ls\n", path.substr(0, pos).c_str());
    return path.substr(0, pos);
}

std::wstring utils::path::join(const wchar_t * path, ...)
{
    va_list args;
    wchar_t *s = NULL, *joined_path = 0;
    int idx = 0;
    size_t path_len = 0;

    joined_path = (wchar_t *)malloc((utils::path::MAX_LEN + 1) * sizeof(wchar_t));
    if (!joined_path) return NULL;
    wcsncpy(joined_path, path, utils::path::MAX_LEN);
    path_len = wcsnlen(path, utils::path::MAX_LEN);
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

    for (s = va_arg(args, wchar_t *); s && idx < (int)utils::path::MAX_LEN; s = va_arg(args, wchar_t *))
    {
        path_len = wcsnlen(s, utils::path::MAX_LEN);
        if (path_len > 0)
        {
            if (!is_path_sep(s[0]))
            {
                wcsncpy(&joined_path[idx], PATH_SEP, (size_t)(utils::path::MAX_LEN - idx));
                ++idx;
            }

            wcsncpy(&joined_path[idx], s, (size_t)(utils::path::MAX_LEN - idx));
            idx += (int)(wcsnlen(s, utils::path::MAX_LEN)); 
        }
    }

    va_end(args);
    
    std::wstring p = joined_path;
    free(joined_path);
    return p;
}

bool utils::path::has_ext(const std::wstring & path, const std::wstring & ext)
{
    if (path.size() < ext.size()) return false;

    return path.substr(path.size() - 1 - ext.size(), ext.size()) == ext;
}

std::wstring utils::path::trim_ext(const std::wstring & path)
{
    return path.substr(0, path.find_last_of(L"."));
}

static int is_path_sep(wchar_t c)
{
    return (c == '/' || c == '\\');
}

