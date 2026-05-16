TAP_DANCE_ENABLE = no # Allow tab dance

# Other dependency are defined in info.json
LTO_ENABLE = yes  # Makes hex file smaller

# Optimize for size
OPT_DEFS += -Os

RGB_DISABLE_WHEN_USB_SUSPENDED=true

# Suspend RGB leds
SLEEP_LED_ENABLE = no

POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = analog_joystick
POINTING_DEVICE_LEFT = no
POINTING_DEVICE_RIGHT = yes
