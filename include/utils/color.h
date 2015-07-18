#ifndef __OCTO_UTILS_COLOR_H__
#define __OCTO_UTILS_COLOR_H__

#define COLOR(c)         L"\033[" c L"m"
#define COLOR_END        COLOR(L"0")
#define COLOR_BOLD       COLOR(L"1")
#define COLOR_BLACK      COLOR(L"30")
#define COLOR_RED        COLOR(L"31")
#define COLOR_GREEN      COLOR(L"32")
#define COLOR_YELLOW     COLOR(L"33")
#define COLOR_BLUE       COLOR(L"34")
#define COLOR_MAGENTA    COLOR(L"35")
#define COLOR_CYAN       COLOR(L"36")
#define COLOR_WHITE      COLOR(L"37")

#endif

