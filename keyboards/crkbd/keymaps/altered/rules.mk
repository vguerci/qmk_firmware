# BOOTLOADER = atmel-dfu # for Elite C

# RGBLIGHT_ENABLE = yes
RGBLIGHT_ENABLE = no
RGB_MATRIX_ENABLE = WS2812

# Reduce firmware size
MOUSEKEY_ENABLE = no

AUTO_SHIFT_ENABLE = yes
TAP_DANCE_ENABLE = yes
EXTRAKEY_ENABLE = yes
CONSOLE_ENABLE = no # DEBUG
OLED_DRIVER_ENABLE = yes

# If you want to change the display of OLED, you need to change here
SRC +=  ./lib/glcdfont.c \
        ./lib/rgb_state_reader.c \
        ./lib/layer_state_reader.c \
        ./lib/logo_reader.c \
        ./lib/keylogger.c \
        # ./lib/mode_icon_reader.c \
        # ./lib/host_led_state_reader.c \
        # ./lib/timelogger.c \
