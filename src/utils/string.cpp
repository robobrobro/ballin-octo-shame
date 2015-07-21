#include <string>
#include "utils/string.h"

std::wstring utils::string::cstr_to_wstr(const char * cstr)
{
    wchar_t mbstr[300] = {0};
    swprintf(mbstr, sizeof(mbstr) / sizeof(wchar_t) - 1, L"%s", cstr);
    std::wstring wstr = mbstr;
    return wstr;
}

