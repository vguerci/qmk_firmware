#include QMK_KEYBOARD_H
#ifdef AUDIO_ENABLE
#include "muse.h"
#endif

enum altered_layers {
  _QWERTY,
  _LOWER,
  _RAISE,
  _ADJUST
};

enum altered_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE
};

// Tap Dance Macros

#define TD_CHORD_ESC(name, key)                                                    \
  void chord_esc_##name##_finished(qk_tap_dance_state_t *state, void *user_data) { \
    if (state->count == 1) {                                                       \
      register_code(key);                                                          \
    } else {                                                                       \
      register_code(key);                                                          \
      register_code(KC_ESC);                                                       \
    }                                                                              \
  }                                                                                \
  void chord_esc_##name##_reset(qk_tap_dance_state_t *state, void *user_data) {    \
    if (state->count == 1) {                                                       \
      unregister_code(key);                                                        \
    } else {                                                                       \
      unregister_code(key);                                                        \
      unregister_code(KC_ESC);                                                     \
    }                                                                              \
  }

TD_CHORD_ESC(cmd, KC_LGUI)
TD_CHORD_ESC(opt, KC_LALT)
TD_CHORD_ESC(ctrl, KC_LCTRL)

#define TD_MACRO_DOUBLE(name, key, macro)                                             \
  void double_macro_##name##_finished(qk_tap_dance_state_t *state, void *user_data) { \
    if (state->count == 1) {                                                          \
      if (key & QK_LSFT) register_code(KC_LSHIFT);                             \
      register_code((uint8_t)key);                                                    \
    } else {                                                                          \
      SEND_STRING(macro);                                                             \
      reset_tap_dance(state);                                                         \
    }                                                                                 \
  }                                                                                   \
  void double_macro_##name##_reset(qk_tap_dance_state_t *state, void *user_data) {    \
    if (state->count == 1) {                                                          \
      if (key & QK_LSFT) unregister_code(KC_LSHIFT);                           \
      unregister_code((uint8_t)key);                                                  \
    }                                                                                 \
  }

TD_MACRO_DOUBLE(par, KC_LEFT_PAREN, "()"SS_TAP(X_LEFT))
TD_MACRO_DOUBLE(brk, KC_LBRACKET, "[]"SS_TAP(X_LEFT))
TD_MACRO_DOUBLE(crl, KC_LEFT_CURLY_BRACE, "{}"SS_TAP(X_LEFT))
TD_MACRO_DOUBLE(und, KC_UNDERSCORE, "__"SS_TAP(X_LEFT))
TD_MACRO_DOUBLE(str, KC_ASTERISK, "**"SS_TAP(X_LEFT))
TD_MACRO_DOUBLE(btk, KC_GRAVE, "``"SS_TAP(X_LEFT))

//Tap dance Actions

enum { TD_CMD_ESC = 0, TD_OPT_ESC, TD_CTR_ESC, TD_DBL_PAR, TD_DBL_BRK, TD_DBL_CRL, TD_DBL_UND, TD_DBL_STR, TD_DBL_BTK };

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_CMD_ESC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, chord_esc_cmd_finished, chord_esc_cmd_reset),
    [TD_OPT_ESC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, chord_esc_opt_finished, chord_esc_opt_reset),
    [TD_CTR_ESC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, chord_esc_ctrl_finished, chord_esc_ctrl_reset),
    [TD_DBL_PAR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, double_macro_par_finished, double_macro_par_reset),
    [TD_DBL_BRK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, double_macro_brk_finished, double_macro_brk_reset),
    [TD_DBL_CRL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, double_macro_crl_finished, double_macro_crl_reset),
    [TD_DBL_UND] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, double_macro_und_finished, double_macro_und_reset),
    [TD_DBL_STR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, double_macro_str_finished, double_macro_str_reset),
    [TD_DBL_BTK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, double_macro_btk_finished, double_macro_btk_reset),
};

#define XX_ESC LCTL_T(KC_ESCAPE)
#define XX_ENT RSFT_T(KC_ENTER)
#define XX_CTL TD(TD_CTR_ESC)
#define XX_OPT TD(TD_OPT_ESC)
#define XX_CMD TD(TD_CMD_ESC)
#define XX_STR TD(TD_DBL_STR)
#define XX_PAR TD(TD_DBL_PAR)
#define XX_UND TD(TD_DBL_UND)
#define XX_BRK TD(TD_DBL_BRK)
#define XX_CRL TD(TD_DBL_CRL)
#define XX_BTK TD(TD_DBL_BTK)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_QWERTY] = LAYOUT_ortho_5x15(
  XX_BTK,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,  /**/ KC_MINS, KC_EQL,  KC_BSPC, \
  KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC, /**/ KC_7,    KC_8,    KC_9,    \
  XX_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, /**/ KC_4,    KC_5,    KC_6,    \
  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, XX_ENT,  /**/ KC_1,    KC_2,    KC_3,    \
  MO(4),   XX_CTL,  XX_OPT,  XX_CMD,  LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, /**/ KC_0,    KC_PGDN, KC_PGUP  \
),

[_LOWER] = LAYOUT_ortho_5x15(
  XX_BTK,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______, /**/ _______, _______, _______, \
  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______, /**/ _______, _______, _______, \
  KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_MINS, KC_EQL,  XX_BRK,  KC_RBRC, KC_BSLS, /**/ _______, _______, _______, \
  _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_NUHS, KC_NUBS, KC_PGUP, KC_PGDN, _______, /**/ _______, _______, _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY, /**/ _______, _______, _______  \
),

[_RAISE] = LAYOUT_ortho_5x15(
  KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, XX_STR,  XX_PAR,  KC_RPRN, _______, /**/ _______, _______, _______, \
  KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______, /**/ _______, _______, _______, \
  KC_DEL,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   XX_UND,  KC_PLUS, XX_CRL,  KC_RCBR, KC_PIPE, /**/ _______, _______, _______, \
  _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, KC_HOME, KC_END,  _______, /**/ _______, _______, _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY, /**/ _______, _______, _______  \
),

[_ADJUST] = LAYOUT_ortho_5x15(
  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  /**/ _______, _______, _______, \
  _______, RESET,   DEBUG,   _______, _______, _______, _______, TERM_ON, TERM_OFF,_______, _______, _______, /**/ _______, _______, _______, \
  _______, _______, MU_MOD,  AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, _______, _______, _______,  _______, RESET,  /**/ _______, _______, _______, \
  _______, MUV_DE,  MUV_IN,  MU_ON,   MU_OFF,  MI_ON,   MI_OFF,  _______, _______, _______, _______, _______, /**/ _______, _______, _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, /**/ _______, _______, _______  \
),

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_QWERTY);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
  }
  return true;
}

void matrix_init_user(void) {}
void matrix_scan_user(void) {}
void led_set_user(uint8_t usb_led) {}
