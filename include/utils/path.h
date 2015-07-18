#ifndef __OCTO_UTILS_PATH_H__
#define __OCTO_UTILS_PATH_H__

#include <wchar.h>

wchar_t * path_get_cwd(void);
wchar_t * path_get_dir(wchar_t * path);
wchar_t * path_join(wchar_t * path, ...);

#endif

