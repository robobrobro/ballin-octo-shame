#ifndef __OCTO_UTILS_STRING_H__
#define __OCTO_UTILS_STRING_H__

#include <string>
#include <wchar.h>

namespace utils
{

namespace string
{

std::wstring cstr_to_wstr(const char * cstr);

}   // namespace string

}   // namespace utils

#endif

