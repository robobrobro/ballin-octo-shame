#ifndef __OCTO_UTILS_STRING_H__
#define __OCTO_UTILS_STRING_H__

#include <wchar.h>

#define char_to_wchar(c, w) swprintf((w), sizeof((w)) / sizeof(wchar_t), L"%s", (c))
#define char_to_wchar_len(c, w, l) swprintf((w), (l), L"%s", (c))

#endif

