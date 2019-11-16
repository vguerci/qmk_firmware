#pragma once
#include "altered.h"

enum { TD_CMD_ESC = 0, TD_OPT_ESC, TD_CTR_ESC, TD_DBL_PAR, TD_DBL_BRK, TD_DBL_CRL, TD_DBL_UND, TD_DBL_STR, TD_DBL_BTK };

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
