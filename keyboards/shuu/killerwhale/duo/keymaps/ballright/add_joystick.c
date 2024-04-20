// Copyright 2021 Hayashi (@w_vwbw)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "add_joystick.h"
#include "quantum.h"
#include "analog.h"
#include "host.h"

// ファイル内のみ変数
int16_t gp27_newt;
int16_t gp28_newt;
bool joystick_attached;

keypos_t key_up;
keypos_t key_left;
keypos_t key_right;
keypos_t key_down;
bool pressed_up = false;
bool pressed_down = false;
bool pressed_left = false;
bool pressed_right = false;

// Change this
static int joystickThreshold = 6; // Value to prevent joystick drift
static int joystickResolution = 24; // Decide the movement speed of the joystick

// Don't chnage this
static uint8_t joystickMode = 0;
//static uint8_t joystick_modes = 3; // amount of modes

joystick_config_t joystick_axes[JOYSTICK_AXIS_COUNT] = {
    [0] = JOYSTICK_AXIS_VIRTUAL, // don't set these as JOYSTICK_AXIS_IN to be able to swich 
    [1] = JOYSTICK_AXIS_VIRTUAL  // to keycode mode and etc..
};

void matrix_init_addedjoystick(void) {
    gp27_newt = analogReadPin(GP27);
    gp28_newt = analogReadPin(GP28);
    if(gp27_newt < NO_STICK_VAL ||  gp28_newt < NO_STICK_VAL ){
        joystick_attached = false;
    }else{
        joystick_attached = true;
    }
    key_up.row = 3;
    key_up.col = 6;
    key_left.row = 2;
    key_left.col = 6;
    key_right.row = 4;
    key_right.col = 6;
    key_down.row = 1;
    key_down.col = 6;
}

void matrix_scan_addedjoystick(void) {
    if(joystick_attached){
        switch (joystickMode) {
            case 0: // keycodes
                int8_t layer = layer_switch_get_layer(key_up);
                int16_t keycode_up = keymap_key_to_keycode(layer, key_up);
                int16_t keycode_left = keymap_key_to_keycode(layer, key_left);
                int16_t keycode_right = keymap_key_to_keycode(layer, key_right);
                int16_t keycode_down = keymap_key_to_keycode(layer, key_down);

                if(!pressed_left && analogReadPin(GP27) - 512 > STICK_OFFSET){
                    pressed_left = true;
                    register_code(keycode_left);
                }else if(pressed_left && analogReadPin(GP27) - 512  < STICK_OFFSET){
                    pressed_left = false;
                    unregister_code(keycode_left);
                }else if(!pressed_right && analogReadPin(GP27) - 512  < -STICK_OFFSET){
                    pressed_right = true;
                    register_code(keycode_right);
                }else if (pressed_right && analogReadPin(GP27) - 512  > -STICK_OFFSET){
                    pressed_right = false;
                    unregister_code(keycode_right);
                }

                if(!pressed_up && analogReadPin(GP28) - 512  > STICK_OFFSET){
                    pressed_up = true;
                    register_code(keycode_up);
                }else if(pressed_up && analogReadPin(GP28) - 512  < STICK_OFFSET){
                    pressed_up = false;
                    unregister_code(keycode_up);
                }else if(!pressed_down && analogReadPin(GP28) - 512  < -STICK_OFFSET){
                    pressed_down = true;
                    register_code(keycode_down);
                }else if(pressed_down && analogReadPin(GP28) - 512  > -STICK_OFFSET){
                    pressed_down = false;
                    unregister_code(keycode_down);
                }
                break;
            case 1: // gamepad
                joystick_state.axes[0] = 128 - analogReadPin(GP27)/4;
                joystick_state.axes[1] = analogReadPin(GP28)/4 - 128;
                joystick_state.dirty = true;
                joystick_read_axes();
                void host_joystick_send(joystick_t *joystick);
                host_joystick_send(&joystick_state);
                break;
            case 2: // mouse
                report_mouse_t currentReport = pointing_device_get_report();
                currentReport.x = (512 - analogReadPin(GP27)) / joystickResolution;
                currentReport.y = (analogReadPin(GP28) - 512) / joystickResolution;
                if (currentReport.x < joystickThreshold && currentReport.x > -joystickThreshold){
                    currentReport.x = 0;
                }
                if (currentReport.y < joystickThreshold && currentReport.y > -joystickThreshold){
                    currentReport.y = 0;
                }
                pointing_device_set_report(currentReport);
                pointing_device_send();
                break;
        }
    }
}