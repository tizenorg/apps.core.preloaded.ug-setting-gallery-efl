/*
Copyright (c) 2000 - 2012 Samsung Electronics Co., Ltd All Rights Reserved

This file is part of ug-setting-gallery-efl
Written by	Sangjin Han <sjhan.han@samsung.com>,
			Jonghyuk Lee <jhyuk47.lee@samsung.com>,
			Li Suxia <suxia.li@samsung.com>,
			Hyunwoo Kim <hw4444.kim@samsung.com>,
			Jisung Ahn <jcastle.ahn@samsung.com>,

PROPRIETARY/CONFIDENTIAL

This software is the confidential and proprietary information of
SAMSUNG ELECTRONICS ("Confidential Information").
You shall not disclose such Confidential Information and shall use it only
in accordance with the terms of the license agreement you entered into with
SAMSUNG ELECTRONICS.
SAMSUNG make no representations or warranties about the suitability of
the software, either express or implied, including but not limited to
the implied warranties of merchantability, fitness for a particular purpose,
or non-infringement.
SAMSUNG shall not be liable for any damages suffered by licensee as a result
of using, modifying or distributing this software or its derivatives.
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


