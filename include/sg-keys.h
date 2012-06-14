/*
  * Copyright 2012  Samsung Electronics Co., Ltd
  *
  * Licensed under the Flora License, Version 1.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  *	http://www.tizenopensource.org/license
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


#define DEFAULT_TIMER		(3.0f)
#define DEFAULT_REPEAT		false
#define DEFAULT_SHUFFLE		false
#define DEFAULT_SUBTITLE	false
#define DEFAULT_EFFECT		"Slide"



#define VCONFKEY_GALLERY_INTERVAL_TIME		"db/setting/gallery/interval_time"  //int

#define VCONFKEY_GALLERY_REPEAT_STATE		"db/setting/gallery/repeat_state" //boolean

#define VCONFKEY_GALLERY_SHUFFLE_STATE		"db/setting/gallery/shuffle_state" //boolean

#define VCONFKEY_GALLERY_SUBTITLE_STATE		"db/setting/gallery/subtitle_state" //boolean

#define VCONFKEY_GALLERY_TRANSITION_EFFECT	"db/setting/gallery/ss_effect"  //char


//void _gallery_key_set_time_vconf(void *data, Evas_Object *obj, void *event_info);


int
gallery_key_init_current_time(double *val);

int
gallery_key_init_repeat_state(int *b_val);

int
gallery_key_init_shuffle_state(int *b_val);

int
gallery_key_init_subtitle_state(int *b_val);

void
gallery_key_text_popup(void *data, char *msg);

int
gallery_key_init_slideshow_effect(void);

char *
gallery_key_menu_get(int index);


double
gallery_key_get_time_vconf_value(int index);

int gallery_key_reset_current_time(void);
int gallery_key_reset_repeat_state(void);
int gallery_key_reset_shuffle_state(void);
int gallery_key_reset_subtitle_state(void);
int gallery_key_reset_slideshow_effect(void);

#endif


