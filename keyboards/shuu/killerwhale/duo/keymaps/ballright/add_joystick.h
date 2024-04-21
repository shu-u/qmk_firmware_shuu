// Copyright 2021 Hayashi (@w_vwbw)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once
#include "quantum.h"
#include "Joystick.h"

// ジョイスティックの定数
#define STICK_OFFSET 220
#define STICK_WAIT 50
#define NO_STICK_VAL 100

// Change this
//static int joystickThreshold = 6; // Value to prevent joystick drift
//static int joystickResolution = 24; // Decide the movement speed of the joystick


void matrix_init_addedjoystick(void);
void matrix_scan_addedjoystick(uint8_t joystickMode);