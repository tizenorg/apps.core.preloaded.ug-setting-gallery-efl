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

#include <vconf.h>

#include "sg-keys.h"
#include "sg-debug.h"
#include "ug-setting-gallery.h"

#define POPUP_TIMEOUT 3

static void
_gallery_key_text_popuptimeout_cb( void *data, Evas_Object *obj, void *event_info )
{
	if(!data)
	{
		return;
	}

	struct ug_data * ugd = (struct ug_data *)data;

	if(ugd->popup)
	{
		evas_object_del(ugd->popup);
		ugd->popup = NULL;
	}
}

void
gallery_key_text_popup(void *data, char *msg)
{
	if(!data)
	{
		return;
	}

	struct ug_data * ugd = (struct ug_data *)data;

	Evas_Object *popup = elm_popup_add(ugd->main_layout);

	if(!ugd->popup)
	{
		ugd->popup = popup;
		evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

		elm_object_text_set(popup, msg);
		elm_popup_timeout_set(popup, POPUP_TIMEOUT);
		evas_object_smart_callback_add(popup, "timeout", _gallery_key_text_popuptimeout_cb, data);
		evas_object_show(popup);
	}
}

int
gallery_key_init_current_time(double *val)
{
	if(vconf_get_dbl(VCONFKEY_GALLERY_INTERVAL_TIME, val))
	{
		if(vconf_set_dbl(VCONFKEY_GALLERY_INTERVAL_TIME, DEFAULT_TIMER))
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	return 0;
}

int
gallery_key_init_repeat_state(int *b_val)
{
	if(vconf_get_bool(VCONFKEY_GALLERY_REPEAT_STATE, b_val))
	{
		if(vconf_set_bool(VCONFKEY_GALLERY_REPEAT_STATE, DEFAULT_REPEAT))
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	return 0;
}

int
gallery_key_init_shuffle_state(int *b_val)
{
	if(vconf_get_bool(VCONFKEY_GALLERY_SHUFFLE_STATE, b_val))
	{
		if(vconf_set_bool(VCONFKEY_GALLERY_SHUFFLE_STATE, DEFAULT_SHUFFLE))
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	return 0;
}

int
gallery_key_init_subtitle_state(int *b_val)
{
	if(vconf_get_bool(VCONFKEY_GALLERY_SUBTITLE_STATE, b_val))
	{
		if(vconf_set_bool(VCONFKEY_GALLERY_SUBTITLE_STATE, DEFAULT_SUBTITLE))
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	return 0;
}

int
gallery_key_init_slideshow_effect(void)
{
	if(!vconf_get_str(VCONFKEY_GALLERY_TRANSITION_EFFECT))
	{
		if(vconf_set_str(VCONFKEY_GALLERY_TRANSITION_EFFECT, DEFAULT_EFFECT))
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	return 0;
}

double
gallery_key_get_time_vconf_value(int index)
{
	switch(index)
	{
		case 0:
			return VCONFKEY_GALLERY_ITV_TIME_2S;
			break;

		case 1:
			return VCONFKEY_GALLERY_ITV_TIME_3S;
			break;

		case 2:
			return VCONFKEY_GALLERY_ITV_TIME_5S;
			break;

		case 3:
			return VCONFKEY_GALLERY_ITV_TIME_10S;
			break;

		case 4:
			return VCONFKEY_GALLERY_ITV_TIME_20S;
			break;

		default:
			return 0;
	}

}

char *
gallery_key_menu_get(int index)
{
	char *text = NULL;

	switch(index)
	{
		case 0:
			text = SGUG_TR_2s;
			break;

		case 1:
			text = SGUG_TR_3s;
			break;

		case 2:
			text = SGUG_TR_5s;
			break;

		case 3:
			text = SGUG_TR_10s;
			break;

		case 4:
			text = SGUG_TR_20s;
			break;

		default:
			break;

	}

	return text;

}

int
gallery_key_reset_current_time(void)
{
	if(vconf_set_dbl(VCONFKEY_GALLERY_INTERVAL_TIME, DEFAULT_TIMER))
	{
		gallery_error("vconf_set_dbl:interval time failed");
		return -1;
	}

	return 0;
}

int
gallery_key_reset_repeat_state(void)
{
	if(vconf_set_bool(VCONFKEY_GALLERY_REPEAT_STATE, DEFAULT_REPEAT))
	{
		gallery_error("vconf_set_bool:repeat state failed");
		return -1;
	}

	return 0;
}

int
gallery_key_reset_shuffle_state(void)
{
	if(vconf_set_bool(VCONFKEY_GALLERY_SHUFFLE_STATE, DEFAULT_SHUFFLE))
	{
		gallery_error("vconf_set_bool:shuffle state failed");
		return -1;
	}

	return 0;
}

int
gallery_key_reset_subtitle_state(void)
{
	if(vconf_set_bool(VCONFKEY_GALLERY_SUBTITLE_STATE, DEFAULT_SUBTITLE))
	{
		gallery_error("vconf_set_bool:subtitle state failed");
		return -1;
	}

	return 0;
}

int
gallery_key_reset_slideshow_effect(void)
{
	if(vconf_set_str(VCONFKEY_GALLERY_TRANSITION_EFFECT, DEFAULT_EFFECT))
	{
		gallery_error("vconf_set_str:transition effect failed");
		return -1;
	}

	return 0;
}
