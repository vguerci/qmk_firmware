# https://thomasbaart.nl/2018/12/01/reducing-firmware-size-in-qmk/
EXTRAFLAGS += -flto
LTO_ENABLE = yes

SRC += tap_dances.c
