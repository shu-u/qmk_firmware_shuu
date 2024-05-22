// Copyright 2021 Hayashi (@w_vwbw)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "add_keycodes.h"
#include "add_joystick.h"
#include "add_oled.h"
#include "add_trackball.h"

static uint8_t joystickMode = 0;

// レイヤー名
enum layer_number {
    BASE = 0,
    ONOFF, OFFON, ONON,                       // トグルスイッチで変更するレイヤー
    LOWER, UPPER, UTIL,                       // 長押しで変更するレイヤー
    MOUSE, BALL_SETTINGS, LIGHT_SETTINGS // 自動マウスレイヤー切り替えや設定用のレイヤー
};

// キーマップの設定
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [BASE] = LAYOUT(
        // 左手
        // 天面スイッチ
        KC_ESC,  KC_1,         KC_2,            KC_3, LT(BALL_SETTINGS, KC_4), LT(LIGHT_SETTINGS, KC_5),
        KC_LEFT_SHIFT,  KC_Q,         KC_W,            KC_E, KC_R, KC_T,
        CMD_CTL, KC_A,         KC_S, KC_D, KC_F, KC_G,
                 KC_Z, KC_X,            KC_C, KC_V, KC_B,
                               LT(KC_LEFT_ALT ,KC_TAB),
        // 側面スイッチ
        LALT(KC_LEFT_SHIFT), KC_LNG1,                
        // 十字キーorジョイスティック                // ジョイスティックスイッチ
        KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,         KC_ENT,      
        // 追加スイッチ                             // トグルスイッチ
        LT(MO(MOUSE), KC_SPC), MO(LOWER),                    MO(ONOFF),

        // 右手
        LT(LIGHT_SETTINGS, KC_6), LT(BALL_SETTINGS, KC_7), KC_8, KC_9, KC_0, KC_MINS,
        KC_Y, KC_U, KC_I,    KC_O,             KC_P,    KC_EQL,
        KC_H, KC_J, KC_K,    KC_L,  KC_SCLN, KC_QUOT,
        KC_N, KC_M, KC_COMM, KC_DOT,           KC_RBRC,
                             KC_LBRC,
        KC_BSPC, KC_ENT,                   
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,         XXXXXXX,     
        AUTO_MOUSE, KC_DEL,                    MO(OFFON)
    ),
    [ONOFF] = LAYOUT( // GAME LAYER 1
        // left
        // Top
        KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,
        KC_LEFT_SHIFT, KC_1, KC_2, KC_3, KC_4, KC_5,
        KC_LEFT_CTRL, KC_LEFT_SHIFT, KC_Q, KC_R, KC_E, KC_F,
        KC_LEFT_CTRL, KC_Z, KC_X, KC_C, KC_V,
        KC_G,
        // Side (left is upper)
        KC_TAB, KC_LEFT_ALT,
        // Cross or Joystick
        _______, _______, _______, _______, KC_MS_BTN3,
        // Additional
        KC_B, KC_H, XXXXXXX,

        // Right
        // Top
        KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11,
        KC_6, KC_7, KC_8, KC_9, KC_0, KC_F12,
        KC_MS_BTN5, KC_MS_BTN1, KC_MS_BTN2, KC_MS_BTN3, KC_SPC, KC_ENT,
        KC_MS_BTN4, KC_MS_BTN6, KC_MS_BTN7, KC_MS_BTN8, MOD_SCRL,
        KC_ENT,
        // Side (Right is upper)
        KC_J, KC_M,
        // Cross or Joystick
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        // Additional
        KC_I, KC_K, MO(ONON)
    ),
#if JOYSTICK_BUTTON_COUNT > 7
    [OFFON] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, GPD_LB,  GPD_LT,  GPD_RB,  GPD_RT,
                 XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                          XXXXXXX,
        XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
        XXXXXXX, XXXXXXX,                            MO(ONON),

        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        GPD_A,   GPD_B,   GPD_X,   GPD_Y,   XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                   XXXXXXX,
        XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
        XXXXXXX, XXXXXXX,                            XXXXXXX
    ),
#endif
    //[ONON] = LAYOUT(
        //// left
        //// Top
        //KC_, KC_, KC_, KC_, KC_, KC_,
        //KC_, KC_, KC_, KC_, KC_, KC_,
        //KC_, KC_, KC_, KC_, KC_, KC_,
        //KC_, KC_, KC_, KC_, KC_,
        //KC_,
        //// Side (left is upper)
        //KC_, KC_,
        //// Cross or Joystick
        //_______, _______, _______, _______, KC_,
        //// Additional
        //KC_, KC_,
//
        //// Right
        //// Top
        //KC_, KC_, KC_, KC_, KC_, KC_,
        //KC_, KC_, KC_, KC_, KC_, KC_,
        //KC_, KC_, KC_, KC_, KC_, KC_,
        //KC_, KC_, KC_, KC_, KC_,
        //KC_,
        //// Side (Right is upper)
        //KC_, KC_,
        //// Cross or Joystick
        //XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        //// Additional
        //KC_, KC_
    //),
    [MOUSE] = LAYOUT(
        // left
        // Top
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,
        _______,
        // Side (left is upper)
        _______, _______,
        // Cross or Joystick
        _______, _______, _______, _______, _______,
        // Additional
        _______, _______, XXXXXXX,
        // Right
        // Top
        _______, _______, KC_UP, _______, _______, _______,
        KC_HOME, KC_LEFT, KC_DOWN, KC_RIGHT, KC_END, _______,
        KC_MS_BTN5, KC_MS_BTN1, KC_MS_BTN2, KC_MS_BTN3, MOD_SCRL, _______,
        KC_MS_BTN4, KC_MS_BTN6, KC_MS_BTN7, KC_MS_BTN8, _______,
        _______,
        // Side (Right is upper)
        _______, _______,
        // Cross or Joystick
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        // Additional
        _______, _______, XXXXXXX
    ),
    [BALL_SETTINGS] = LAYOUT(
        XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, _______, L_CHMOD,
        XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, CPI_I, XXXXXXX,
        AUTO_MOUSE, XXXXXXX, XXXXXXX, L_ANG_D, L_INV,   L_ANG_I,
                    XXXXXXX, XXXXXXX, XXXXXXX, CPI_D, XXXXXXX,
                             INV_SCRL,
        XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
        XXXXXXX, INV_SCRL,                           XXXXXXX,
        R_CHMOD, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, CPI_I,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        R_ANG_D, R_INV,   R_ANG_I, XXXXXXX, XXXXXXX, AUTO_MOUSE,
        XXXXXXX, CPI_D,   XXXXXXX, XXXXXXX, XXXXXXX,
                                     INV_SCRL,
        XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
        XXXXXXX, XXXXXXX,                            XXXXXXX
    ),
    [LIGHT_SETTINGS] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, RGB_MOD, RGB_MOD, _______,
        OLED_MOD, RGB_SPI, RGB_VAI, RGB_SAI, RGB_HUI, RGB_TOG,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                 RGB_SPD, RGB_VAD, RGB_SAD, RGB_HUD, XXXXXXX,
                          XXXXXXX,
        RGB_MOD, RGB_MOD,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
        XXXXXXX, XXXXXXX,                            XXXXXXX,
        _______, RGB_MOD, RGB_MOD, XXXXXXX, XXXXXXX, XXXXXXX,
        RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI, OLED_MOD,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, RGB_HUD, RGB_SAD, RGB_VAD, RGB_SPD,
                                   XXXXXXX,
        RGB_MOD, RGB_MOD,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
        XXXXXXX, XXXXXXX,                            XXXXXXX
    )
};

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [BASE] =   { 
        ENCODER_CCW_CW(KC_ESC, KC_TAB),
        ENCODER_CCW_CW(REDO, UNDO),
        ENCODER_CCW_CW(KC_WH_U, KC_WH_D),
        ENCODER_CCW_CW(KC_WH_U, KC_WH_D),
        ENCODER_CCW_CW(XXXXXXX, XXXXXXX),
        ENCODER_CCW_CW(KC_DEL, KC_BSPC),
        ENCODER_CCW_CW(KC_UP, KC_DOWN),
        ENCODER_CCW_CW(KC_WH_U, KC_WH_D),
        ENCODER_CCW_CW(KC_WH_U, KC_WH_D),
        ENCODER_CCW_CW(XXXXXXX, XXXXXXX)
    },
    [ONOFF] =   { 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______)
    },
    [OFFON] =   { 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______)
    },
    [ONON] =   { 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______)
    },
    [LOWER] =   { 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______)
    },
    [UPPER] =   { 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______)
    },
    [UTIL] =   { 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______)
    },
    [MOUSE] =   { 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______)
    },
    [BALL_SETTINGS] =   { 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______), 
        ENCODER_CCW_CW(_______, _______),
        ENCODER_CCW_CW(_______, _______)
    },
    [LIGHT_SETTINGS] =   { 
        ENCODER_CCW_CW(RGB_SPI, RGB_SPD), 
        ENCODER_CCW_CW(RGB_VAI, RGB_VAD), 
        ENCODER_CCW_CW(RGB_SAI, RGB_SAD), 
        ENCODER_CCW_CW(RGB_HUI, RGB_HUD),
        ENCODER_CCW_CW(XXXXXXX, XXXXXXX),
        ENCODER_CCW_CW(RGB_SPI, RGB_SPD), 
        ENCODER_CCW_CW(RGB_VAI, RGB_VAD), 
        ENCODER_CCW_CW(RGB_SAI, RGB_SAD), 
        ENCODER_CCW_CW(RGB_HUI, RGB_HUD),
        ENCODER_CCW_CW(XXXXXXX, XXXXXXX)
    },
};

// 初期化関係
void matrix_init_user(void) {
    matrix_init_addedjoystick();
    joystick_init();
}

// キースキャン関係
bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    process_record_addedkeycodes(keycode, record);
    return true;
}

void matrix_scan_user(void) {
    //matrix_scan_addedjoystick();
}

bool should_process_keypress(void) { return true; }

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case ONOFF:
        case OFFON:
        case ONON:
            joystickMode = 1;
            ballconfig.auto_mouse = false;
            set_auto_mouse_enable(ballconfig.auto_mouse);
            eeconfig_update_user(ballconfig.raw);
            break;
        default:
            joystickMode = 0;
    }

    return state;
}

typedef struct _master_to_slave_t {
    bool oled_mode;
    uint8_t joystickMode;
    ballconfig_t ballconfig;
} master_to_slave_t;


#if defined(JOYSTICK_ENABLE)

void joystick_task(){
    matrix_scan_addedjoystick(joystickMode);
}

#endif

