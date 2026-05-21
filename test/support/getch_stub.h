#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int test_getch(void);

#ifdef __cplusplus
}
#endif

#define getch test_getch
