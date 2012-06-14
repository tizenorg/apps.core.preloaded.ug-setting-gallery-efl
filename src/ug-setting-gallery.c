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

#ifndef UG_MODULE_API
#define UG_MODULE_API __attribute__ ((visibility("default")))
#endif


#include <stdio.h>
#include <stdbool.h>

#include <Elementary.h>
#include <bundle.h>
#include <ui-gadget-module.h>
#include <vconf.h>


#include "ui-gadget.h"
#include "sg-keys.h"
#include "sg-debug.h"
#include "ug-setting-gallery.h"


static void
_gallery_quit_cb(void *data, Evas_Object *obj, void *event_info)
{
	gallery_ret_if( data == NULL);

	struct ug_data *ugd;
	ugd = (struct ug_data *)data;

	if(ugd->ug)
	{
		ug_destroy_me(ugd->ug);
	}
}

static double
_gallery_get_fabs(double vconf_value1, double vconf_value2)
{
	double temp = 0;
	temp = vconf_value1 - vconf_value2;
	if(temp < 0)
	{
		return  vconf_value2 - vconf_value1 ;
	}

	return temp;
}

static char *
_gallery_genlist_text_get(void *data, Evas_Object *obj, const char *part)
{
	gallery_retv_if( part == NULL, NULL);

	int param = (int )data;

	char *txt = NULL;
	double time_value = 0;
	int input_len = 0;
	input_len = strlen(part);

	if (input_len == strlen("elm.text.1") && strncmp(part, "elm.text.1", input_len) == 0 )
	{
		if(param == GALLERY_MAIN_MENU_TIME)
		{
			txt = SGUG_TR_PESF;
		}
	}
	// get sub item of interval-timer
	else if (input_len == strlen("elm.text.2") && strncmp(part, "elm.text.2", input_len) == 0 )
	{
		if(param == GALLERY_MAIN_MENU_TIME)
		{
			gallery_key_init_current_time(&time_value);

			int radio_value =0;
			for(radio_value =0;radio_value < GALLERY_SUB_TIME_COUNT; radio_value++)
			{
				if(_gallery_get_fabs(time_value, gallery_key_get_time_vconf_value(radio_value))<0.1)
				{
					break;
				}
			}
			if(radio_value >= 0)
			{
				txt = gallery_key_menu_get(radio_value);
			}
		}
	}
	else if (input_len == strlen("elm.text") && strncmp(part, "elm.text", input_len) == 0 )
	{
		if(param == GALLERY_MAIN_MENU_REPEAT)
		{
			txt = SGUG_TR_REPEAT;
		}
		else if(param == GALLERY_MAIN_MENU_SHUFFLE)
		{
			txt = SGUG_TR_SHUFFLE;
		}
		else if(param == GALLERY_MAIN_MENU_TITLE)
		{
			txt = SGUG_TR_SLIDESHOW;
		}
		else if(param == GALLERY_MAIN_MENU_SUBTITLE_TITLE)
		{
			txt = SGUG_TR_SUBTITLE_TITLE;
		}
		else if(param == GALLERY_MAIN_MENU_SUBTITLE_ON_OFF)
		{
			txt = SGUG_TR_SUBTITLE;
		}
	}

	if(txt)
	{
		return strdup(txt);
	}
	else
	{
		return NULL;
	}
}

static char *
_gallery_time_text_get(void *data, Evas_Object *obj, const char *part)
{
	gallery_retv_if(!part, NULL);

	int param = (int )data;

	char *txt = NULL;
	int input_len = 0;

	input_len = strlen(part);

	if (input_len == strlen("elm.text") && strncmp(part, "elm.text", input_len) == 0 )
	{
		if(param>=0 && param <GALLERY_SUB_TIME_COUNT)
		{
			txt = gallery_key_menu_get(param);
		}
		if(txt)
		{
			return strdup(txt);
		}
	}

	return NULL;
}

static Evas_Object*
_gallery_time_content_get(void *data, Evas_Object *obj, const char *part)
{
	gallery_retv_if(!part, NULL);

	int param = (int )data;

	struct ug_data *ugd = evas_object_data_get(obj, "ugd");
	gallery_retvm_if(!ugd, NULL, "INVALID param");

	int radio_value = 0;

	if (strncmp(part, "elm.icon", strlen(part)) == 0 )
	{
		double time_value = 0;

		Evas_Object *radio_btn = elm_radio_add(obj);
		evas_object_propagate_events_set(radio_btn, EINA_TRUE);

		elm_radio_state_value_set(radio_btn, param);
		elm_radio_group_add(radio_btn, ugd->radio_group);

		gallery_key_init_current_time(&time_value);

		for(radio_value =0;radio_value <GALLERY_SUB_TIME_COUNT; radio_value++)
		{
			if(_gallery_get_fabs(time_value, gallery_key_get_time_vconf_value(radio_value))<0.1)
			{
				break;
			}
		}

		if(ugd->radio_group)
		{
			elm_radio_value_set(ugd->radio_group, radio_value);
		}

		evas_object_smart_callback_add(radio_btn, "changed", NULL, NULL);//gallery_key_set_time_vconf
		evas_object_show(radio_btn);

		return radio_btn;
	}

	return NULL;
}

static void
_gallery_genlist_icon_cb(void *data,  Evas_Object *obj, void *event_info)
{
	gallery_ret_if(NULL == data);
	struct ug_data *ugd = (struct ug_data *)data;

	int state = 0;
	int index = (int )data;

	if (index == GALLERY_MAIN_MENU_REPEAT)
	{
		gallery_key_init_repeat_state(&state);

		if (vconf_set_bool(VCONFKEY_GALLERY_REPEAT_STATE, !state))
		{
			gallery_key_text_popup(ugd,SGUG_TR_FAILED);
		}
	}
	else if(index == GALLERY_MAIN_MENU_SHUFFLE)
	{
		gallery_key_init_shuffle_state(&state);

		if(vconf_set_bool(VCONFKEY_GALLERY_SHUFFLE_STATE, !state))
		{
			gallery_key_text_popup(ugd,SGUG_TR_FAILED);
		}
	}
	else if(index == GALLERY_MAIN_MENU_SUBTITLE_ON_OFF)
	{
		gallery_key_init_subtitle_state(&state);

		if(vconf_set_bool(VCONFKEY_GALLERY_SUBTITLE_STATE, !state))
		{
			gallery_key_text_popup(ugd, SGUG_TR_FAILED);
		}
	}
}

static Evas_Object*
_gallery_genlist_content_get(void *data, Evas_Object *obj, const char *part)
{

	gallery_retv_if(NULL == data , NULL);
	gallery_retv_if(NULL == part, NULL);

	struct ug_data *ugd = NULL;

	ugd = evas_object_data_get(obj, "ugd");
	gallery_retvm_if(!ugd, NULL, "INVALID param");

	int index = (int )data;

	if (0 == strncmp(part, "elm.icon", strlen(part)))
	{
		if (index == GALLERY_MAIN_MENU_REPEAT)
		{
			Evas_Object *check;
			check  = elm_check_add(obj);

			ugd->repeat_btn= check;

			gallery_key_init_repeat_state(&ugd->repeat_state);
			elm_check_state_set( ugd->repeat_btn, (bool)ugd->repeat_state);

			elm_object_style_set(check, "on&off");

			evas_object_propagate_events_set(check, EINA_FALSE);
			evas_object_smart_callback_add(check, "changed", _gallery_genlist_icon_cb, (void *)index);

			return ugd->repeat_btn;

		}
		else if(index == GALLERY_MAIN_MENU_SHUFFLE)
		{
			Evas_Object *check;
			check  = elm_check_add(obj);

			ugd->shuffle_btn= check;

			gallery_key_init_shuffle_state(&ugd->shuffle_state);
			elm_check_state_set( ugd->shuffle_btn, (bool)ugd->shuffle_state);

			elm_object_style_set(check, "on&off");

			evas_object_propagate_events_set(check, EINA_FALSE);
			evas_object_smart_callback_add(check, "changed", _gallery_genlist_icon_cb, (void *)index);

			return ugd->shuffle_btn;
		}
		else if(index == GALLERY_MAIN_MENU_SUBTITLE_ON_OFF)
		{
			Evas_Object *check;
			check  = elm_check_add(obj);

			ugd->subtitle_btn= check;

			gallery_key_init_subtitle_state(&ugd->subtitle_state);
			elm_check_state_set( ugd->subtitle_btn, (bool)ugd->subtitle_state);

			elm_object_style_set(check, "on&off");

			evas_object_propagate_events_set(check, EINA_FALSE);
			evas_object_smart_callback_add(check, "changed", _gallery_genlist_icon_cb, (void *)index);

			return ugd->subtitle_btn;
		}
	}

	return NULL;

	}

static void
_gallery_time_select_cb(void *data, Evas_Object *obj, void *event_info)
{

	struct ug_data *ugd = evas_object_data_get(obj, "ugd");
	gallery_ret_if(!ugd);

	int param = (int )data;
	Elm_Object_Item *item = (Elm_Object_Item *)event_info;

	int index = GALLERY_MAIN_MENU_TIME;

	elm_genlist_item_selected_set(item, EINA_FALSE);

	if(vconf_set_dbl(VCONFKEY_GALLERY_INTERVAL_TIME, gallery_key_get_time_vconf_value(param)))
	{
		gallery_key_text_popup(ugd,SGUG_TR_FAILED);
	}

	elm_radio_value_set(ugd->radio_group, param);
	elm_genlist_item_update(ugd->gl_it[index]);

}



static void
_gallery_genlist_con(void *data, Evas_Object *obj, void *event_info)
{
	gallery_ret_if( event_info == NULL);

	Elm_Object_Item *item = event_info;
	elm_genlist_item_subitems_clear(item);
}

static void
_gallery_genlist_time_select_cb(void *data, Evas_Object *obj, void *event_info)
{
	struct ug_data *ugd = evas_object_data_get(obj, "ugd");
	gallery_ret_if(ugd == NULL);

	int index = 0;
	Elm_Object_Item *item = (Elm_Object_Item *)event_info;

	elm_genlist_item_selected_set(item, EINA_FALSE);

	Evas_Object *gl = elm_object_item_widget_get(item);

	ugd->check_time_itc.item_style = "dialogue/1text.1icon/expandable2";
	ugd->check_time_itc.func.text_get = _gallery_time_text_get;
	ugd->check_time_itc.func.content_get = _gallery_time_content_get;
	ugd->check_time_itc.func.state_get = NULL;
	ugd->check_time_itc.func.del = NULL;

	ugd->radio_group = elm_radio_add(gl);
	for (index = 0; gallery_key_menu_get(index); index++)
	{
		elm_genlist_item_append(gl,&ugd->check_time_itc, (void *)index, item,
				ELM_GENLIST_ITEM_NONE, _gallery_time_select_cb,  (void *)index);
	}

}

static void
_gallery_genlist_exp(void *data, Evas_Object *obj, void *event_info)
{
	gallery_ret_if( NULL == obj);

	struct ug_data *ugd = evas_object_data_get(obj, "ugd");
	gallery_ret_if(NULL == ugd);

	Elm_Object_Item *item = (Elm_Object_Item *)event_info;

	elm_genlist_item_selected_set(item, EINA_FALSE);

	if(item == ugd->gl_it[GALLERY_MAIN_MENU_TIME])
	{
		_gallery_genlist_time_select_cb(data, obj, event_info);

	}
}

static void
_gallery_genlist_select_cb(void *data, Evas_Object *obj, void *event_info)
{
	struct ug_data *ugd = evas_object_data_get(obj, "ugd");
	gallery_ret_if(ugd == NULL);

	int param = (int )data;

	int icon_state = 0;
	bool expand_state = false;

	Elm_Object_Item *item = (Elm_Object_Item *)event_info;

	elm_genlist_item_selected_set(item, EINA_FALSE);

	if(param == GALLERY_MAIN_MENU_TIME)
	{
		expand_state = elm_genlist_item_expanded_get(item);

		if(!expand_state)
		{
			elm_genlist_item_expanded_set(item,EINA_TRUE);
		}
		else
		{
			elm_genlist_item_expanded_set(item,EINA_FALSE);
		}
	}
	else if(param == GALLERY_MAIN_MENU_REPEAT)
	{
		gallery_key_init_repeat_state(&icon_state);
		icon_state = !icon_state;

		if(vconf_set_bool(VCONFKEY_GALLERY_REPEAT_STATE, icon_state))
		{
			gallery_key_text_popup(ugd,SGUG_TR_FAILED);

		}

		elm_check_state_set(ugd->repeat_btn, (bool)icon_state);
	}
	else if(param == GALLERY_MAIN_MENU_SHUFFLE)
	{
		gallery_key_init_shuffle_state(&icon_state);
		icon_state = !icon_state;

		if(vconf_set_bool(VCONFKEY_GALLERY_SHUFFLE_STATE, icon_state))
		{
			gallery_key_text_popup(ugd,SGUG_TR_FAILED);

		}

		elm_check_state_set(ugd->shuffle_btn,(bool)icon_state);
	}
	else if(param == GALLERY_MAIN_MENU_SUBTITLE_ON_OFF)
	{
		gallery_key_init_subtitle_state(&icon_state);
		icon_state = !icon_state;

		if(vconf_set_bool(VCONFKEY_GALLERY_SUBTITLE_STATE, icon_state))
		{
			gallery_key_text_popup(ugd,SGUG_TR_FAILED);
			return;

		}

		elm_check_state_set(ugd->subtitle_btn,(bool)icon_state);
	}
}

static Evas_Object *
_gallery_genlist_items_add (Evas_Object *parent, struct ug_data *ugd)
{
	Evas_Object *main_genlist;
	Elm_Object_Item *it = NULL;

	int index = 0;

	main_genlist = elm_genlist_add(parent);

	evas_object_size_hint_weight_set(main_genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(main_genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);

	ugd->genlist = main_genlist;

	ugd->title_itc.item_style = "dialogue/title";
	ugd->title_itc.func.text_get = _gallery_genlist_text_get;
	ugd->title_itc.func.content_get = NULL;
	ugd->title_itc.func.state_get = NULL;
	ugd->title_itc.func.del = NULL;

	ugd->expandable_itc.item_style = "dialogue/2text.3/expandable";
	ugd->expandable_itc.func.text_get = _gallery_genlist_text_get;
	ugd->expandable_itc.func.content_get = NULL;
	ugd->expandable_itc.func.state_get = NULL;
	ugd->expandable_itc.func.del = NULL;

	ugd->txt_icon_itc.item_style = "dialogue/1text.1icon";
	ugd->txt_icon_itc.func.text_get = _gallery_genlist_text_get;
	ugd->txt_icon_itc.func.content_get = _gallery_genlist_content_get;
	ugd->txt_icon_itc.func.state_get = NULL;
	ugd->txt_icon_itc.func.del = NULL;

	for (index = GALLERY_MAIN_MENU_TITLE; index < GALLERY_MAIN_MENU_ITEM_MAX; index++)
	{
		//expandable genlist
		if (index == GALLERY_MAIN_MENU_TIME)
		{
			ugd->gl_it[index] = elm_genlist_item_append(main_genlist, &ugd->expandable_itc,
											(void *)index, NULL, ELM_GENLIST_ITEM_TREE,
											_gallery_genlist_select_cb, (void *)index);
		}
		// repeat&shuffle 1txt .1icon
		else if (index == GALLERY_MAIN_MENU_REPEAT ||
					index == GALLERY_MAIN_MENU_SHUFFLE ||
					index == GALLERY_MAIN_MENU_SUBTITLE_ON_OFF)
		{
			ugd->gl_it[index] = elm_genlist_item_append(main_genlist, &ugd->txt_icon_itc,
											(void *)index, NULL, ELM_GENLIST_ITEM_NONE,
											_gallery_genlist_select_cb, (void *)index);
		}
		//title
		else if (index == GALLERY_MAIN_MENU_TITLE)
		{
			it = elm_genlist_item_append(main_genlist, &ugd->title_itc,
											(void *)index, NULL, ELM_GENLIST_ITEM_NONE,
											NULL, NULL);
			elm_genlist_item_select_mode_set(it, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);
		}
		//subtitle title
		else if (index == GALLERY_MAIN_MENU_SUBTITLE_TITLE)
		{
			it = elm_genlist_item_append(main_genlist, &ugd->title_itc,
											(void *)index, NULL, ELM_GENLIST_ITEM_NONE,
											NULL, NULL);
			elm_genlist_item_select_mode_set(it, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);
		}
	}

	evas_object_smart_callback_add(main_genlist, "expanded", _gallery_genlist_exp, ugd);
	evas_object_smart_callback_add(main_genlist, "contracted", _gallery_genlist_con, ugd);

	return main_genlist;
}


static Evas_Object *
_gallery_create_fullview(Evas_Object *parent, struct ug_data *ugd)
{
	gallery_retv_if( parent == NULL, NULL);

	Evas_Object *base;
	base = elm_layout_add(parent);
	gallery_retv_if( base == NULL, NULL );

	elm_layout_theme_set(base, "layout", "application", "default");
	edje_object_signal_emit(_EDJ(base), "elm,state,show,indicator", "elm");
	elm_object_part_content_set(base, "elm.swallow.bg", ugd->bg);

	return base;
}

static Evas_Object*
_gallery_create_naviframe(Evas_Object* parent)
{
	gallery_retv_if( parent == NULL, NULL);

	Evas_Object *naviframe = NULL;
	naviframe = elm_naviframe_add( parent );
	elm_object_part_content_set( parent, "elm.swallow.content", naviframe );
	evas_object_show(naviframe);

	return naviframe;
}

static Evas_Object *_gallery_create_bg(Evas_Object *parent)
{
	Evas_Object *bg = elm_bg_add(parent);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(parent, bg);
	evas_object_show(bg);

    return bg;
}

static void *
_on_create(struct ui_gadget *ug, enum ug_mode mode, bundle *data, void *priv)
{
	Evas_Object *parent = NULL;
	struct ug_data *ugd = NULL;

	gallery_retvm_if((!ug || !priv), NULL, "handle or ui_gadget pointer is NULL, check it !!");

	ugd = (struct ug_data *)priv;
	ugd->ug = ug;

	/* Bind text domain for internalization */
	bindtextdomain("ug-setting-gallery-efl" , "/opt/ug/res/locale");

	parent = (Evas_Object *)ug_get_parent_layout(ug);
	gallery_retvm_if(parent == NULL, NULL, "parent layout is NULL");

	ugd->bg = _gallery_create_bg(parent);

	if (mode == UG_MODE_FULLVIEW)
	{
		ugd->main_layout = _gallery_create_fullview(parent, ugd);
	}
	else
	{
		ugd->main_layout = NULL;
	}

	if(ugd->main_layout)
	{
		ugd->naviframe = _gallery_create_naviframe(ugd->main_layout);
		evas_object_data_set(ugd->naviframe, "ugd", ugd);
	}

	ugd->genlist = _gallery_genlist_items_add(ugd->naviframe,ugd);

	evas_object_data_set(ugd->genlist, "ugd", ugd);

	ugd->backbtn = elm_button_add(ugd->naviframe);
	evas_object_smart_callback_add(ugd->backbtn, "clicked", _gallery_quit_cb, ugd);
	elm_naviframe_item_push(ugd->naviframe, SGUG_TR_GALLERY, ugd->backbtn,
				NULL, ugd->genlist, NULL);
	elm_object_style_set(ugd->backbtn, "naviframe/end_btn/default");

	evas_object_show(ugd->main_layout);

	return ugd->main_layout;
}

static void
_on_message(struct ui_gadget *ug, bundle *msg, bundle *data, void *priv)
{
}

static void
_on_event(struct ui_gadget *ug, enum ug_event event, bundle *data, void *priv)
{

	//struct ug_data *ugd = (struct ug_data *)priv;

	switch (event) {
	case UG_EVENT_LOW_MEMORY:
		break;
	case UG_EVENT_LOW_BATTERY:
		break;
	case UG_EVENT_LANG_CHANGE:
		break;
	case UG_EVENT_ROTATE_PORTRAIT:
		break;
	case UG_EVENT_ROTATE_PORTRAIT_UPSIDEDOWN:
		break;
	case UG_EVENT_ROTATE_LANDSCAPE:
		break;
	case UG_EVENT_ROTATE_LANDSCAPE_UPSIDEDOWN:
		break;
	default:
		break;
	}
}

static void
_on_start(struct ui_gadget *ug, bundle *data, void *priv)
{
}

static void
_on_pause(struct ui_gadget *ug, bundle *data, void *priv)
{

}

static void
_on_resume(struct ui_gadget *ug, bundle *data, void *priv)
{

}

static void
_on_destroy(struct ui_gadget *ug, bundle *data, void *priv)
{
	struct ug_data *ugd;

	if (!ug || !priv)
	{
		return;
	}

	ugd = priv;
	if(ugd->main_layout)
	{
		evas_object_del(ugd->main_layout);
		ugd->main_layout = NULL;
	}
}

static void
_on_key_event(struct ui_gadget *ug, enum ug_key_event event, bundle *data, void *priv)
{

}


UG_MODULE_API int
UG_MODULE_INIT(struct ug_module_ops *ops)
{
	struct ug_data *ugd;

	if (!ops)
	{
		return -1;
	}

	ugd = calloc(1, sizeof(struct ug_data));
	if (!ugd)
	{
		return -1;
	}

	ops->create = _on_create;
	ops->start = _on_start;
	ops->pause = _on_pause;
	ops->resume = _on_resume;
	ops->destroy = _on_destroy;
	ops->message = _on_message;
	ops->key_event = _on_key_event;
	ops->event = _on_event;
	ops->priv = ugd;
	ops->opt = UG_OPT_INDICATOR_PORTRAIT_ONLY;

	return 0;
}

UG_MODULE_API int setting_plugin_reset(bundle *data, void *priv)
{
	gallery_info("Reset the settings start");

	int ret = 0;
	ret += gallery_key_reset_current_time();
	ret += gallery_key_reset_repeat_state();
	ret += gallery_key_reset_shuffle_state();
	ret += gallery_key_reset_subtitle_state();
	ret += gallery_key_reset_slideshow_effect();

	gallery_info("Finished");

	return ret;
}

UG_MODULE_API void
UG_MODULE_EXIT(struct ug_module_ops *ops)
{
	struct ug_data *ugd;

	if (!ops)
	{
		return;
	}

	ugd = ops->priv;
	if (ugd)
	{
		free(ugd);
	}
}
