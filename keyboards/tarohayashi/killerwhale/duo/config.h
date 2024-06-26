// Copyright 2021 Hayashi (@w_vwbw)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define PMW33XX_CS_PIN  GP0
#define SPI_SCK_PIN GP18
#define SPI_MISO_PIN GP16
#define SPI_MOSI_PIN GP19

#define POINTING_DEVICE_AUTO_MOUSE_ENABLE
#define AUTO_MOUSE_DEFAULT_LAYER 7
#define AUTO_MOUSE_TIME 800
#define AUTO_MOUSE_DELAY 500

#define I2C_DRIVER I2CD0
#define I2C1_SCL_PIN        GP9
#define I2C1_SDA_PIN        GP8
#define OLED_FONT_H "./lib/glcdfont.c"

#define SPLIT_HAND_PIN GP10
#define SPLIT_POINTING_ENABLE
#define SPLIT_HAND_PIN_LOW_IS_LEFT

#define SPLIT_LAYER_STATE_ENABLE

#define SELECT_SOFT_SERIAL_SPEED 0