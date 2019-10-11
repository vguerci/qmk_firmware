#include QMK_KEYBOARD_H
#ifdef AUDIO_ENABLE
#include "muse.h"
#endif
#include "eeprom.h"
#include "keymap_german.h"
#include "keymap_nordic.h"
#include "keymap_french.h"
#include "keymap_spanish.h"
#include "keymap_hungarian.h"
#include "keymap_swedish.h"
#include "keymap_br_abnt2.h"
#include "keymap_canadian_multilingual.h"
#include "keymap_german_ch.h"
#include "keymap_jp.h"

#define KC_MAC_UNDO LGUI(KC_Z)
#define KC_MAC_CUT LGUI(KC_X)
#define KC_MAC_COPY LGUI(KC_C)
#define KC_MAC_PASTE LGUI(KC_V)
#define KC_PC_UNDO LCTL(KC_Z)
#define KC_PC_CUT LCTL(KC_X)
#define KC_PC_COPY LCTL(KC_C)
#define KC_PC_PASTE LCTL(KC_V)
#define ES_LESS_MAC KC_GRAVE
#define ES_GRTR_MAC LSFT(KC_GRAVE)
#define ES_BSLS_MAC ALGR(KC_6)

enum planck_keycodes {
  RGB_SLD = EZ_SAFE_RANGE,
};

enum planck_layers {
  _BASE,
  _LOWER,
  _RAISE,
  _ADJUST,
  _LAYER4,
};


#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)

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
TD_MACRO_DOUBLE(sqo, KC_QUOTE, "''"SS_TAP(X_LEFT))
TD_MACRO_DOUBLE(dqo, KC_DOUBLE_QUOTE, "\"\""SS_TAP(X_LEFT))
TD_MACRO_DOUBLE(und, KC_UNDERSCORE, "__"SS_TAP(X_LEFT))
TD_MACRO_DOUBLE(str, KC_ASTERISK, "**"SS_TAP(X_LEFT))
TD_MACRO_DOUBLE(btk, KC_GRAVE, "``"SS_TAP(X_LEFT))

//Tap dance Actions

enum { TD_CMD_ESC = 0, TD_OPT_ESC, TD_CTR_ESC, TD_DOUBLE_PAR, TD_DOUBLE_BRK, TD_DOUBLE_CRL, TD_DOUBLE_SQO, TD_DOUBLE_DQO, TD_DOUBLE_UND, TD_DOUBLE_STR, TD_DOUBLE_BTK };

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_CMD_ESC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, chord_esc_cmd_finished, chord_esc_cmd_reset),
    [TD_OPT_ESC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, chord_esc_opt_finished, chord_esc_opt_reset),
    [TD_CTR_ESC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, chord_esc_ctrl_finished, chord_esc_ctrl_reset),
    [TD_DOUBLE_PAR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, double_macro_par_finished, double_macro_par_reset),
    [TD_DOUBLE_BRK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, double_macro_brk_finished, double_macro_brk_reset),
    [TD_DOUBLE_CRL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, double_macro_crl_finished, double_macro_crl_reset),
    [TD_DOUBLE_SQO] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, double_macro_sqo_finished, double_macro_sqo_reset),
    [TD_DOUBLE_DQO] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, double_macro_dqo_finished, double_macro_dqo_reset),
    [TD_DOUBLE_UND] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, double_macro_und_finished, double_macro_und_reset),
    [TD_DOUBLE_STR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, double_macro_str_finished, double_macro_str_reset),
    [TD_DOUBLE_BTK] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, double_macro_btk_finished, double_macro_btk_reset),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT_planck_grid(
      LCTL_T(KC_TAB),KC_Q,KC_W,KC_E,KC_R,KC_T,KC_Y,KC_U,KC_I,KC_O,KC_P,KC_BSPACE,
      LGUI_T(KC_ESCAPE),KC_A,KC_S,KC_D,KC_F,KC_G,KC_H,KC_J,KC_K,KC_L,KC_SCOLON,TD(TD_DOUBLE_SQO), //TODO cant bind shift there... DQO?
      KC_LSHIFT,KC_Z,KC_X,KC_C,KC_V,KC_B,KC_N,KC_M,KC_COMMA,KC_DOT,KC_SLASH,RSFT_T(KC_ENTER),
      MO(4),TD(TD_CTR_ESC),TD(TD_OPT_ESC),TD(TD_CMD_ESC),LOWER,KC_SPACE,KC_NO,RAISE,KC_LEFT,KC_DOWN,KC_UP,KC_RIGHT
  ),

  [_RAISE] = LAYOUT_planck_grid(
      KC_TILD,KC_EXLM,KC_AT,KC_HASH,KC_DLR,KC_PERC,KC_CIRC,KC_AMPR,TD(TD_DOUBLE_STR),TD(TD_DOUBLE_PAR),KC_RPRN,KC_TRANSPARENT,KC_TRANSPARENT,
      KC_F1,KC_F2,KC_F3,KC_F4,KC_F5,KC_F6,TD(TD_DOUBLE_UND),KC_PLUS,TD(TD_DOUBLE_CRL),KC_RCBR,KC_PIPE,KC_TRANSPARENT,
      KC_F7,KC_F8,KC_F9,KC_F10,KC_F11,KC_F12,KC_NONUS_HASH,KC_NONUS_BSLASH,KC_LBRACKET,KC_RBRACKET,KC_TRANSPARENT,KC_TRANSPARENT,
      KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_NO,KC_TRANSPARENT,KC_MEDIA_NEXT_TRACK,KC_AUDIO_VOL_DOWN,KC_AUDIO_VOL_UP,KC_MEDIA_PLAY_PAUSE
  ),

  [_LOWER] = LAYOUT_planck_grid(
      TD(TD_DOUBLE_BTK),KC_1,KC_2,KC_3,KC_4,KC_5,KC_6,KC_7,KC_8,KC_9,KC_0,KC_TRANSPARENT,
      KC_TRANSPARENT,KC_F1,KC_F2,KC_F3,KC_F4,KC_F5,KC_F6,KC_MINUS,KC_EQUAL,TD(TD_DOUBLE_BRK),KC_RBRACKET,KC_BSLASH,KC_TRANSPARENT,
      KC_F7,KC_F8,KC_F9,KC_F10,KC_F11,KC_F12,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,
      KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_NO,KC_TRANSPARENT,KC_HOME,KC_PGDOWN,KC_PGUP,KC_END
  ),

  [_ADJUST] = LAYOUT_planck_grid(
      KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,
      KC_TRANSPARENT,KC_TRANSPARENT,AU_ON,AU_OFF,AU_TOG,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,RESET,
      KC_TRANSPARENT,KC_TRANSPARENT,MU_ON,MU_OFF,MU_TOG,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,
      KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_NO,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT
  ),

  [_LAYER4] = LAYOUT_planck_grid(
      LGUI(KC_GRAVE),LGUI(KC_1),LGUI(KC_2),LGUI(KC_3),LGUI(KC_4),LGUI(KC_5),LGUI(KC_6),LGUI(KC_7),LGUI(KC_8),KC_LPRN,KC_RPRN,KC_DELETE,KC_TRANSPARENT,
      KC_F1,KC_F2,KC_F3,KC_F4,KC_F5,KC_F6,KC_MINUS,KC_EQUAL,KC_LBRACKET,KC_RBRACKET,KC_TRANSPARENT,KC_TRANSPARENT,
      KC_F7,KC_F8,KC_F9,KC_F10,KC_F11,KC_F12,KC_UNDS,KC_PLUS,KC_LCBR,KC_RCBR,KC_TRANSPARENT,KC_TRANSPARENT,
      KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_NO,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT
  ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
switch (keycode) {
}
return true;
}

#ifdef AUDIO_ENABLE
bool muse_mode = false;
uint8_t last_muse_note = 0;
uint16_t muse_counter = 0;
uint8_t muse_offset = 70;
uint16_t muse_tempo = 50;

void encoder_update(bool clockwise) {
    if (muse_mode) {
        if (IS_LAYER_ON(_RAISE)) {
            if (clockwise) {
                muse_offset++;
            } else {
                muse_offset--;
            }
        } else {
            if (clockwise) {
                muse_tempo+=1;
            } else {
                muse_tempo-=1;
            }
        }
    } else {
        if (clockwise) {
        #ifdef MOUSEKEY_ENABLE
            register_code(KC_MS_WH_DOWN);
            unregister_code(KC_MS_WH_DOWN);
        #else
            register_code(KC_PGDN);
            unregister_code(KC_PGDN);
        #endif
        } else {
        #ifdef MOUSEKEY_ENABLE
            register_code(KC_MS_WH_UP);
            unregister_code(KC_MS_WH_UP);
        #else
            register_code(KC_PGUP);
            unregister_code(KC_PGUP);
        #endif
        }
    }
}

void matrix_scan_user(void) {
#ifdef AUDIO_ENABLE
    if (muse_mode) {
        if (muse_counter == 0) {
            uint8_t muse_note = muse_offset + SCALE[muse_clock_pulse()];
            if (muse_note != last_muse_note) {
                stop_note(compute_freq_for_midi_note(last_muse_note));
                play_note(compute_freq_for_midi_note(muse_note), 0xF);
                last_muse_note = muse_note;
            }
        }
        muse_counter = (muse_counter + 1) % muse_tempo;
    }
#endif
}

bool music_mask_user(uint16_t keycode) {
    switch (keycode) {
    case RAISE:
    case LOWER:
        return false;
    default:
        return true;
    }
}
#endif

uint32_t layer_state_set_user(uint32_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}
