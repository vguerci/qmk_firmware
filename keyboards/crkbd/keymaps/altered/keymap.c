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
  [_QWERTY] = LAYOUT( \
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

  [_LOWER] = LAYOUT( \
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

  [_RAISE] = LAYOUT( \
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

  [_ADJUST] = LAYOUT( \
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

void matrix_scan_user(void) {
#ifdef SSD1306OLED
    iota_gfx_task();
#endif
#ifdef RGB_MATRIX_ENABLE
    int32_t rgb_disabled_time = g_rgb_counters.any_key_hit - RGB_MATRIX_DISABLE_TIMEOUT * 60 * 20;
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

//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED

// When add source files to SRC in rules.mk, you can use functions.
const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);

// const char *read_mode_icon(bool swap);
// const char *read_host_led_state(void);
// void set_timelog(void);
// const char *read_timelog(void);

void matrix_render_user(struct CharacterMatrix *matrix) {
  if (is_master) {
    // If you want to change the display of OLED, you need to change here
    matrix_write_ln(matrix, read_layer_state());
    matrix_write_ln(matrix, read_keylog());
    //matrix_write_ln(matrix, read_keylogs());
    //matrix_write_ln(matrix, read_mode_icon(keymap_config.swap_lalt_lgui));
    //matrix_write_ln(matrix, read_host_led_state());
    //matrix_write_ln(matrix, read_timelog());
  } else {
    matrix_write(matrix, read_logo());
  }
}

void matrix_update(struct CharacterMatrix *dest, const struct CharacterMatrix *source) {
  if (memcmp(dest->display, source->display, sizeof(dest->display))) {
    memcpy(dest->display, source->display, sizeof(dest->display));
    dest->dirty = true;
  }
}

void iota_gfx_task_user(void) {
  struct CharacterMatrix matrix;
  matrix_clear(&matrix);
  matrix_render_user(&matrix);
  matrix_update(&display, &matrix);
}
#endif//SSD1306OLED

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
#ifdef SSD1306OLED
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
