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

#include <vconf.h>

#include "sg-keys.h"
#include "sg-debug.h"
#include "ug-setting-gallery.h"

#define POPUP_TIMEOUT 3

static void
_gallery_key_text_popupresponse_cb( void *data, Evas_Object *obj, void *event_info )
{
	gallery_retm_if(!data, "data is NULL");

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
	gallery_retm_if(!data, "data is NULL");

	struct ug_data * ugd = (struct ug_data *)data;

	Evas_Object *popup = elm_popup_add(ugd->main_layout);

	if(!ugd->popup)
	{
		ugd->popup = popup;
		evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

		elm_object_text_set(popup, msg);
//		elm_popup_mode_set(popup, ELM_POPUP_TYPE_ALERT);
		elm_popup_timeout_set(popup, POPUP_TIMEOUT);
		evas_object_smart_callback_add(popup, "response", _gallery_key_text_popupresponse_cb, data);
		evas_object_show(popup);
	}
}

int
gallery_key_init_current_time(double *val)
{
	if(vconf_get_dbl(VCONFKEY_GALLERY_INTERVAL_TIME, val))
	{
		if(vconf_set_dbl(VCONFKEY_GALLERY_INTERVAL_TIME, DEFAULT_TIMER))
			return -1;
	}

	return 0;
}

int
gallery_key_init_repeat_state(int *b_val)
{
	if(vconf_get_bool(VCONFKEY_GALLERY_REPEAT_STATE, b_val))
	{
		if(vconf_set_bool(VCONFKEY_GALLERY_REPEAT_STATE, DEFAULT_REPEAT))
			return -1;
	}

	return 0;
}

int
gallery_key_init_shuffle_state(int *b_val)
{
	if(vconf_get_bool(VCONFKEY_GALLERY_SHUFFLE_STATE, b_val))
	{
		if(vconf_set_bool(VCONFKEY_GALLERY_SHUFFLE_STATE, DEFAULT_SHUFFLE))
			return -1;
	}

	return 0;
}

double
gallery_key_get_time_vconf_value(int index)
{
	double value = 0.0;

	switch(index)
	{
		case 0:
			value = VCONFKEY_GALLERY_ITV_TIME_1S;
			break;

		case 1:
			value = VCONFKEY_GALLERY_ITV_TIME_3S;
			break;

		case 2:
			value = VCONFKEY_GALLERY_ITV_TIME_5S;
			break;

		case 3:
			value = VCONFKEY_GALLERY_ITV_TIME_10S;
			break;

		case 4:
			value = VCONFKEY_GALLERY_ITV_TIME_20S;
			break;

		default:
			break;
	}

	return value;
}

char *
gallery_key_time_menu_get(int index)
{
	char *text = NULL;

	switch(index)
	{
		case 0:
			text = SGUG_TR_1s;
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
gallery_key_set_current_time(double value)
{
	if(vconf_set_dbl(VCONFKEY_GALLERY_INTERVAL_TIME, value))
	{
		gallery_error("vconf_set_dbl:interval time failed");
		return -1;
	}

	return 0;
}

int
gallery_key_set_repeat_state(bool value)
{
	if(vconf_set_bool(VCONFKEY_GALLERY_REPEAT_STATE, value))
	{
		gallery_error("vconf_set_bool:repeat state failed");
		return -1;
	}

	return 0;
}

int
gallery_key_set_shuffle_state(bool value)
{
	if(vconf_set_bool(VCONFKEY_GALLERY_SHUFFLE_STATE, value))
	{
		gallery_error("vconf_set_bool:shuffle state failed");
		return -1;
	}

	return 0;
}


