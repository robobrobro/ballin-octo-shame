#ifndef __OCTO_UTILS_PATH_H__
#define __OCTO_UTILS_PATH_H__

#include <string>
#include <wchar.h>

namespace utils
{

namespace path
{

const size_t MAX_LEN = 260;

std::wstring get_cwd(void);
std::wstring get_dir(const std::wstring & path);
std::wstring join(const wchar_t * path, ...);
bool has_ext(const std::wstring & path, const std::wstring & ext);
std::wstring trim_ext(const std::wstring & path);

}   // namespace path

}   // namespace utils

#endif

