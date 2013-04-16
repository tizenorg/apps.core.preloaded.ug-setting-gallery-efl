/*
 * Copyright 2012  Samsung Electronics Co., Ltd
 *
 * Licensed under the Flora License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	  http://floralicense.org/license/
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __GALLERY_KEY_H__
#define __GALLERY_KEY_H__

#include <Elementary.h>

#include <vconf-keys.h>
#include <stdbool.h>

#define VCONFKEY_GALLERY_INTERVAL_TIME		"db/gallery/setting/interval_time"  //int

#define VCONFKEY_GALLERY_REPEAT_STATE		"db/gallery/setting/repeat_state" //boolean

#define VCONFKEY_GALLERY_SHUFFLE_STATE		"db/gallery/setting/shuffle_state" //boolean


int gallery_key_init_current_time(double *val);
int gallery_key_init_repeat_state(int *b_val);
int gallery_key_init_shuffle_state(int *b_val);

void gallery_key_text_popup(void *data, char *msg);

char* gallery_key_time_menu_get(int index);
char* gallery_key_size_menu_get(int index);

double gallery_key_get_time_vconf_value(int index);
char* gallery_key_get_size_vconf_value(int index);

int gallery_key_set_current_time(double value);
int gallery_key_set_repeat_state(bool value);
int gallery_key_set_shuffle_state(bool value);


#endif

