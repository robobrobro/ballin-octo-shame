#ifndef __OCTO_UTILS_PATH_H__
#define __OCTO_UTILS_PATH_H__

#include <wchar.h>

#define PATH_MAX_LEN 260

wchar_t * path_get_cwd(void);
wchar_t * path_get_dir(const wchar_t * path);
wchar_t * path_join(const wchar_t * path, ...);
int path_has_ext(const wchar_t * path, const wchar_t * ext);
wchar_t * path_trim_ext(const wchar_t * path);

#endif

