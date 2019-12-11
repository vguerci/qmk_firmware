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

// Quad Function Tap-Dance
// from https://beta.docs.qmk.fm/features/feature_tap_dance

typedef struct {
  bool is_press_action;
  int state;
} tap;

enum {
  SINGLE_TAP = 1,
  SINGLE_HOLD = 2,
  DOUBLE_TAP = 3,
  DOUBLE_HOLD = 4,
  DOUBLE_SINGLE_TAP = 5, //send two single taps
  TRIPLE_TAP = 6,
  TRIPLE_HOLD = 7
};

int quad_dance (qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || !state->pressed)  return SINGLE_TAP;
    //key has not been interrupted, but they key is still held. Means you want to send a 'HOLD'.
    else return SINGLE_HOLD;
  }
  else if (state->count == 2) {
    /*
     * DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
     * action when hitting 'pp'. Suggested use case for this return value is when you want to send two
     * keystrokes of the key, and not the 'double tap' action/macro.
    */
    if (state->interrupted) return DOUBLE_SINGLE_TAP;
    else if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  //Assumes no one is trying to type the same letter three times (at least not quickly).
  //If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
  //an exception here to return a 'TRIPLE_SINGLE_TAP', and define that enum just like 'DOUBLE_SINGLE_TAP'
  if (state->count == 3) {
    if (state->interrupted || !state->pressed)  return TRIPLE_TAP;
    else return TRIPLE_HOLD;
  }
  else return 8; //magic number. At some point this method will expand to work for more presses
}

// Tri Function ESC > Tap=ESC / Hold=CTRL / Double Tap=CTRL+ESC
static tap esc_tap_state = {
  .is_press_action = true,
  .state = 0
};

void quad_esc_ctrl_finished (qk_tap_dance_state_t *state, void *user_data) {
  esc_tap_state.state = quad_dance(state);
  switch (esc_tap_state.state) {
    case SINGLE_TAP: register_code(KC_ESC); break;
    case SINGLE_HOLD: register_code(KC_LCTRL); break;
    case DOUBLE_TAP: register_code(KC_LCTRL); register_code(KC_ESC); break;
  }
}

void quad_esc_ctrl_reset (qk_tap_dance_state_t *state, void *user_data) {
  switch (esc_tap_state.state) {
    case SINGLE_TAP: unregister_code(KC_ESC); break;
    case SINGLE_HOLD: unregister_code(KC_LCTRL); break;
    case DOUBLE_TAP: unregister_code(KC_LCTRL); unregister_code(KC_ESC); break;
  }
  esc_tap_state.state = 0;
}

//Actions

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_CTR_ESC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, quad_esc_ctrl_finished, quad_esc_ctrl_reset),
    [TD_CMD_ESC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, chord_esc_cmd_finished, chord_esc_cmd_reset),
    [TD_OPT_ESC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, chord_esc_opt_finished, chord_esc_opt_reset),
    [TD_DBL_PAR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, double_macro_par_finished, double_macro_par_reset),
    [TD_DBL_BRK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, double_macro_brk_finished, double_macro_brk_reset),
    [TD_DBL_CRL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, double_macro_crl_finished, double_macro_crl_reset),
    [TD_DBL_UND] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, double_macro_und_finished, double_macro_und_reset),
    [TD_DBL_STR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, double_macro_str_finished, double_macro_str_reset),
    [TD_DBL_BTK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, double_macro_btk_finished, double_macro_btk_reset),
};

