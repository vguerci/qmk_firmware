#pragma once

#include <print.h>
#include "quantum.h"
#include "tap_dances.h"

enum altered_layers {
  _QWERTY,
  _LOWER,
  _RAISE,
  _FUNCT,
  _ADJUST,
};

enum altered_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
  BACKLIT,
  RGBRST,
  RGBLYR
};

enum altered_rgb_matrix_layers {
  RGB_LYR_ALL,
  RGB_LYR_KEYS_ONLY,
  RGB_LYR_UNDER_ONLY,
  RGB_LYR_MAX
};
