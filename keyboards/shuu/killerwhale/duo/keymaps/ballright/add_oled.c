// Copyright 2021 Hayashi (@w_vwbw)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "add_oled.h"
#include "quantum.h"
#include "config.h"
#include "add_trackball.h"
#include "../../lib/oled_lagtrain.h"
#include "../../lib/oled_lagtrain2.h"
#include "../../lib/oled_layer.h"

// ファイル外参照変数
bool oled_mode;
// ファイル内のみ変数
uint8_t pre_layer;
uint8_t cur_layer;

// 初期化
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    oled_mode = false;
    pre_layer = 0;
    cur_layer = 0;
    return !is_keyboard_master() ? OLED_ROTATION_90 : OLED_ROTATION_180;
}

void oled_render_aniamtion(const char animation[][1024], const uint8_t framenum){
    uint8_t frame = (timer_read() / 60) % (framenum * 2);
    oled_write_raw_P(animation[frame/2], sizeof(animation[frame/2]));
}

// OLED表示  10 characters per row
bool oled_task_user(void) { 

    cur_layer = get_highest_layer(layer_state);
    oled_set_cursor(0, 0);
    if(!is_keyboard_master()){
        if (oled_mode) {
            if (is_keyboard_left()) {
                //oled_write_raw_P(number[cur_layer], sizeof(number[cur_layer]));
                oled_render_aniamtion(lagtrain2, lagtrain2_length);
            }else{
                oled_render_aniamtion(lagtrain, lagtrain_length);
            }
        } else{
            oled_set_cursor(0, 0);
            oled_write_P(PSTR("CPI:"), false);
            oled_write(get_u16_str(cpi_array[ballconfig.cpi_idx], ' '), false);


            oled_set_cursor(0, 1);
            oled_write_P(PSTR("AUTO M: "), false);
            if(ballconfig.auto_mouse){
                oled_write_P(PSTR("1"), false);
            }else{
                oled_write_P(PSTR("0"), false);
            }

            oled_set_cursor(0, 2);
            oled_write_P(PSTR("ANGLE:"), false);
            oled_write(get_u16_str(angle_array[ballconfig.angle_idx], ' '), false);

            oled_set_cursor(0, 3);
            oled_write_P(PSTR("INV AXIS:"), false);
            if (ballconfig.inv){
                oled_write_P(PSTR("1"), false);
            }else{
                oled_write_P(PSTR("0"), false);
            }
            if(ballconfig.scmode){
                oled_write_P(PSTR(" SCRL"), false);
            }else{
                oled_write_P(PSTR(" CRSL"), false);
            }

            oled_set_cursor(0, 4);
            oled_write_P(PSTR("INV SCRL:"), false);
            if (ballconfig.inv_sc){
                oled_write_P(PSTR("1"), false);
            }else{
                oled_write_P(PSTR("0"), false);
            }

            oled_set_cursor(0, 5);
            oled_write_P(PSTR("LAYER:"), false);
            oled_write(get_u16_str(cur_layer, ' '), false);

            oled_set_cursor(0, 6);
            oled_write_P(PSTR("OLED MODE:"), false);
            if (oled_mode){
                oled_write_P(PSTR("1"), false);
            }else{
                oled_write_P(PSTR("2"), false);
            }
        }
    }else{
        if (oled_mode) {
            //oled_write_raw_P(number[cur_layer], sizeof(number[cur_layer]));
            oled_render_aniamtion(lagtrain, lagtrain_length);
        }else{
            //oled_write_raw_P(reverse_number[cur_layer], sizeof(reverse_number[cur_layer]));
            oled_render_aniamtion(lagtrain2, lagtrain2_length);
        }
    }
    return false;
}



