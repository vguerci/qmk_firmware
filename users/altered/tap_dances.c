#include "tap_dances.h"

//Macros

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
      if (key & QK_LSFT) register_code(KC_LSHIFT);                                    \
      register_code((uint8_t)key);                                                    \
    } else {                                                                          \
      SEND_STRING(macro);                                                             \
      reset_tap_dance(state);                                                         \
    }                                                                                 \
  }                                                                                   \
  void double_macro_##name##_reset(qk_tap_dance_state_t *state, void *user_data) {    \
    if (state->count == 1) {                                                          \
      if (key & QK_LSFT) unregister_code(KC_LSHIFT);                                  \
      unregister_code((uint8_t)key);                                                  \
    }                                                                                 \
  }

TD_MACRO_DOUBLE(par, KC_LEFT_PAREN, "()"SS_TAP(X_LEFT))
TD_MACRO_DOUBLE(brk, KC_LBRACKET, "[]"SS_TAP(X_LEFT))
TD_MACRO_DOUBLE(crl, KC_LEFT_CURLY_BRACE, "{}"SS_TAP(X_LEFT))
TD_MACRO_DOUBLE(und, KC_UNDERSCORE, "__"SS_TAP(X_LEFT))
TD_MACRO_DOUBLE(str, KC_ASTERISK, "**"SS_TAP(X_LEFT))
TD_MACRO_DOUBLE(btk, KC_GRAVE, "``"SS_TAP(X_LEFT))

//Actions

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

