VIA_ENABLE = yes
ENCODER_MAP_ENABLE = yes
OS_DETECTION_ENABLE = yes

JOYSTICK_ENABLE = yes
JOYSTICK_DRIVER = analog

SRC += add_keycodes.c
SRC += add_oled.c
SRC += add_trackball.c
SRC += add_joystick.c
SRC += analog.c