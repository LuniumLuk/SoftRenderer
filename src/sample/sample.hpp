#ifndef __SAMPLE_HPP__
#define __SAMPLE_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <utility>
#include "../api.hpp"
#include "../platform/platform.hpp"

#define FPS_SETUP() long _frame_count_since_last_update = 0; \
                    clock_t _last_update = clock()
#define FPS_UPDATE(x) _frame_count_since_last_update++; \
                      do { if (clock() - _last_update > CLOCKS_PER_SEC) { \
                      _last_update = clock();                             \
                      x = _frame_count_since_last_update;                 \
                      _frame_count_since_last_update = 0; } } while(0)

int main_demo();
int blank_demo();
int colormap_demo();
int normal_mapping_demo();

#endif