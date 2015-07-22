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
    auto pos = path.find_last_of(PATH_SEP);
    if (pos == std::wstring::npos) return path;
    return path.substr(0, pos);
}

std::wstring utils::path::join(const wchar_t * path, ...)
{
    va_list args;
    wchar_t *s = NULL;

    // Remove trailing slash, if there is one
    std::wstring joined_path = path;
    if (joined_path.compare(joined_path.size() - 1, 1, PATH_SEP) == 0)
        joined_path = joined_path.substr(0, joined_path.size() - 1);

    // Append each path in the varargs list
    va_start(args, path);

    for (s = va_arg(args, wchar_t *); s; s = va_arg(args, wchar_t *))
    {
        size_t path_len = wcsnlen(s, utils::path::MAX_LEN);
        if (path_len > 0)
        {
            if (wcsncmp(s, PATH_SEP, 1) != 0)
                joined_path += PATH_SEP;

            joined_path += s;
        }
    }

    va_end(args);
    
    return joined_path;
}

bool utils::path::has_ext(const std::wstring & path, const std::wstring & ext)
{
    if (path.size() < ext.size()) return false;

    return path.substr(path.size() - ext.size(), ext.size()) == ext;
}

std::wstring utils::path::trim_ext(const std::wstring & path)
{
    return path.substr(0, path.find_last_of(L"."));
}

