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
  RGBRST
};
