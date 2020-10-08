#include QMK_KEYBOARD_H
#include "altered.h"

#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

#ifdef RGB_MATRIX_ENABLE
  #include "rgb_matrix_types.h"
  #include "lib/lib8tion/lib8tion.h"

  typedef enum rgb_fade_states { ENABLED, FADEOUT, DISABLED, FADEIN } rgb_fade_states;
  static rgb_fade_states rgb_fade_state = ENABLED;
  extern uint32_t rgb_anykey_timer;

  rgb_config_t user_rgb_enabled_config;
  rgb_config_t user_rgb_fadein_config;
  static uint32_t user_rgb_fadein_timer = 0;
#endif

#define XX_F1 LCTL_T(KC_F1)
#define XX_F7 LSFT_T(KC_F7)

#define CG_F1 C(G(KC_F1))
#define CG_F2 C(G(KC_F2))
#define CG_F3 C(G(KC_F3))
#define CG_F4 C(G(KC_F4))
#define CG_F5 C(G(KC_F5))
#define CG_F6 C(G(KC_F6))

extern uint8_t is_master;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_BSPC,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       XX_ESC,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  XX_ENT,\
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                           XX_OPT,  XX_CMD,  KC_SPC,     KC_ENT,   LOWER,   RAISE\
                                      //`--------------------------'  `--------------------------'

  ),

  [_LOWER] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       XX_BTK,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, _______,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        XX_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                       XX_BRK, KC_LEFT, KC_DOWN,   KC_UP,KC_RIGHT, KC_RBRC,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        XX_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,                      KC_MINS,  KC_EQL, _______, _______, KC_PIPE, _______,\
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______, _______, _______,    _______, _______, _______\
                                      //`--------------------------'  `--------------------------'
    ),

  [_RAISE] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TILD,KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, XX_PAR,  KC_RPRN,  KC_DEL,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        XX_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                      XX_CRL,  KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_RCBR,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        XX_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,                      KC_UNDS, KC_PLUS, _______, _______, KC_BSLS, _______,\
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______, _______, _______,    _______, _______, _______\
                                      //`--------------------------'  `--------------------------'
  ),

  [_ADJUST] = LAYOUT_split_3x6_3( \
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        RESET,  RGBRST, _______, _______, _______, _______,                       KC_F13,  KC_F14, KC_BRID, KC_BRIU,  KC_F15,  KC_F16,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI,  RGBLYR,                      _______, KC_MPLY, KC_VOLD, KC_VOLU, KC_MNXT, _______,\
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, RGB_SPD, _______,                        CG_F1,   CG_F2,   CG_F3,   CG_F4,   CG_F5,   CG_F6,\
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______, _______, _______,    _______, _______, _______\
                                      //`--------------------------'  `--------------------------'
  )
};

int RGB_current_mode, RGB_current_layer;

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

// Setting ADJUST layer RGB back to default
void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
    layer_on(layer3);
  } else {
    layer_off(layer3);
  }
}

void matrix_init_user(void) {
    #ifdef RGBLIGHT_ENABLE
      RGB_current_mode = rgblight_config.mode;
    #endif
    //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
    #ifdef SSD1306OLED
      #ifdef SSD1306OLED_SLOW
        wait_ms(2000);
      #endif
      iota_gfx_init(!has_usb()); // turns on the display
    #endif
}

#ifdef RGB_MATRIX_ENABLE
void rgb_matrix_indicators_user(void) {
  switch (RGB_current_layer) {
    case RGB_LYR_KEYS_ONLY:
      for (uint8_t i = 0; i < 6; i++) {
        rgb_matrix_set_color(i, 0x00, 0x00, 0x00);
      }
      break;
    case RGB_LYR_UNDER_ONLY:
      for (uint8_t i = 6; i < 6 + 21; i++) {
        rgb_matrix_set_color(i, 0x00, 0x00, 0x00);
      }
      break;
  }
}
#endif

void matrix_scan_user(void) {
#ifdef SSD1306OLED
    iota_gfx_task();
#endif
#ifdef RGB_MATRIX_ENABLE
    int32_t rgb_disabled_time = rgb_anykey_timer - RGB_MATRIX_DISABLE_TIMEOUT * 60 * 20;
    bool    rgb_enabled       = rgb_disabled_time < 0;
    // State change
    switch (rgb_fade_state) {
        case ENABLED:
        case FADEIN:
            if (!rgb_enabled) {
                // uprintf("RGB FADEOUT\n");
                user_rgb_enabled_config = rgb_matrix_config;
                rgb_fade_state          = FADEOUT;
            }
            break;
        case DISABLED:
        case FADEOUT:
            if (rgb_enabled) {
                // uprintf("RGB FADEIN\n");
                user_rgb_fadein_timer  = timer_read32();
                user_rgb_fadein_config = rgb_matrix_config;
                rgb_fade_state         = FADEIN;
            }
            break;
    }
    // State effect
    switch (rgb_fade_state) {
        case ENABLED:
            break;
        case FADEOUT:
            if (rgb_matrix_config.hsv.v > 0) {
                uint8_t dec             = (uint8_t)(rgb_disabled_time / RGB_MATRIX_DISABLE_FADEOUT);
                rgb_matrix_config.hsv.v = qsub8(user_rgb_enabled_config.hsv.v, dec);
            } else {
                // uprintf("RGB DISABLED\n");
                rgb_matrix_config.hsv.v = 0;
                rgb_fade_state          = DISABLED;
            }
            break;
        case DISABLED:
            break;
        case FADEIN:
            if (rgb_matrix_config.hsv.v < user_rgb_enabled_config.hsv.v) {
                uint8_t inc             = (uint8_t)(timer_elapsed32(user_rgb_fadein_timer) / RGB_MATRIX_DISABLE_FADEIN);
                rgb_matrix_config.hsv.v = qadd8(user_rgb_fadein_config.hsv.v, inc);
            }
            if (rgb_matrix_config.hsv.v > user_rgb_enabled_config.hsv.v) {
                // uprintf("RGB ENABLED\n");
                rgb_matrix_config.hsv.v = user_rgb_enabled_config.hsv.v;
                rgb_fade_state          = ENABLED;
            }
            break;
    }
#endif
}

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_master) {
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  }
  return rotation;
}

#define L_BASE 0
#define L_LOWER 2
#define L_RAISE 4
#define L_ADJUST 8

void oled_render_layer_state(void) {
    oled_write_P("Layer: ", false);
    switch (layer_state) {
        case L_BASE:
            oled_write_ln_P("Default", false);
            break;
        case L_LOWER:
            oled_write_ln_P("Lower", false);
            break;
        case L_RAISE:
            oled_write_ln_P("Raise", false);
            break;
        case L_ADJUST:
        case L_ADJUST|L_LOWER:
        case L_ADJUST|L_RAISE:
        case L_ADJUST|L_LOWER|L_RAISE:
            oled_write_ln_P("Adjust", false);
            break;
    }
}


char keylog_str[24] = {};
char keylogs_str[21] = {};
int keylogs_str_idx = 0;

const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\',
    '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};

void set_keylog(uint16_t keycode, keyrecord_t *record) {
  char name = ' ';
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
        (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) { keycode = keycode & 0xFF; }
  if (keycode < 60) {
    name = code_to_name[keycode];
  }

  // update keylog
  snprintf(keylog_str, sizeof(keylog_str), "%dx%d, k%2d : %c",
           record->event.key.row, record->event.key.col,
           keycode, name);

  // update keylogs
  if (keylogs_str_idx == sizeof(keylogs_str) - 1) {
    keylogs_str_idx = 0;
    for (int i = 0; i < sizeof(keylogs_str) - 1; i++) {
      keylogs_str[i] = ' ';
    }
  }

  keylogs_str[keylogs_str_idx] = name;
  keylogs_str_idx++;
}

void oled_render_keylog(void) {
    oled_write(keylog_str, false);
}
void oled_render_keylogs(void) {
    oled_write(keylogs_str, false);
}

void render_bootmagic_status(bool status) {
    /* Show Ctrl-Gui Swap options */
    static const char PROGMEM logo[][2][3] = {
        {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
        {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
    };
    if (status) {
        oled_write_ln_P(logo[0][0], false);
        oled_write_ln_P(logo[0][1], false);
    } else {
        oled_write_ln_P(logo[1][0], false);
        oled_write_ln_P(logo[1][1], false);
    }
}

void oled_render_logo(void) {
    static const char PROGMEM crkbd_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
        0};
    oled_write_P(crkbd_logo, false);
}

void oled_task_user(void) {
/*
    if (is_master) {
        oled_render_layer_state();
        oled_render_keylog();
        // oled_render_keylogs();
        // oled_render_bootmagic_icon(keymap_config.swap_lalt_lgui);
    } else {
*/
        oled_render_logo();
//    }
}

// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//   if (record->event.pressed) {
//     set_keylog(keycode, record);
//     // set_timelog();
//   }
//   return true;
// }
#endif // OLED_DRIVER_ENABLE


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
#ifdef OLED_DRIVER_ENABLE
    set_keylog(keycode, record);
#endif
    // set_timelog();
  }

  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        persistent_default_layer_set(1UL<<_QWERTY);
      }
      return false;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer_RGB(_LOWER, _RAISE, _ADJUST);
      }
      return false;
    case ADJUST:
        if (record->event.pressed) {
          layer_on(_ADJUST);
        } else {
          layer_off(_ADJUST);
        }
        return false;
    // case RGB_MOD:
    //   #ifdef RGBLIGHT_ENABLE
    //     if (record->event.pressed) {
    //       rgblight_mode(RGB_current_mode);
    //       rgblight_step();
    //       RGB_current_mode = rgblight_config.mode;
    //     }
    //   #endif
    //   return false;
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgblight_default();
          rgblight_enable();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      #ifdef RGB_MATRIX_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgb_matrix_default();
          rgb_matrix_enable();
        }
      #endif
      break;
    case RGBLYR:
      if (record->event.pressed) {
        RGB_current_layer = (RGB_current_layer + 1) % RGB_LYR_MAX;
        #ifdef RGB_MATRIX_ENABLE
        if (RGB_current_layer == RGB_LYR_UNDER_ONLY &&
            rgb_matrix_config.hsv.v == RGB_MATRIX_MAXIMUM_BRIGHTNESS) {
            rgb_matrix_config.hsv.v = 255;
        } else if (RGB_current_layer != RGB_LYR_UNDER_ONLY &&
            rgb_matrix_config.hsv.v > RGB_MATRIX_MAXIMUM_BRIGHTNESS) {
            rgb_matrix_config.hsv.v = RGB_MATRIX_MAXIMUM_BRIGHTNESS;
        }
        #endif
      }
      break;
  }
  return true;
}

void keyboard_post_init_user(void) {
  /* DEBUG Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
  //*/
#ifdef RGB_MATRIX_ENABLE
  user_rgb_enabled_config = rgb_matrix_config;
#endif
}
