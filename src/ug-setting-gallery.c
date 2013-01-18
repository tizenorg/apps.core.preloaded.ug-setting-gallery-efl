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

#ifndef UG_MODULE_API
#define UG_MODULE_API __attribute__ ((visibility("default")))
#endif

#define USE_DIALOGUE_STYLE	// new style has bug

#include <stdio.h>
#include <stdbool.h>

#include <Elementary.h>
#include <ui-gadget-module.h>
#include <vconf.h>

#include "sg-keys.h"
#include "sg-debug.h"
#include "ug-setting-gallery.h"

#define MAX_PRECISION	0.1

typedef int (*Init_State_Func) (int *data);

static void
_gallery_quit_cb(void *data, Evas_Object *obj, void *event_info)
{
	gallery_ret_if(!data);

	struct ug_data *ugd;
	ugd = (struct ug_data *)data;

	if(ugd->ug)
	{
		ug_destroy_me(ugd->ug);
	}
}

static char *_gallery_get_file_name(char *path)
{
	if (NULL == path || '\0' == path[0]) {
		return NULL;	/* invalid arguement */
	}
	char *p = strrchr(path, '/');
	if (!p) {
		return (char *)g_strdup(path);	/*  cannot find '/' */
	}
	if ('\0' == p[1]) {
		return NULL;	/* end with '/' */
	}
	return (char *)g_strdup(p + 1);
}

static char *_gallery_get_menu_elm_text(int param)
{
	char *text = NULL;

	switch(param)
	{
		case GALLERY_MAIN_MENU_TIME:
			text = SGUG_TR_PESF;
			break;

		case GALLERY_MAIN_MENU_REPEAT:
			text = SGUG_TR_REPEAT;
			break;

		case GALLERY_MAIN_MENU_SHUFFLE:
			text = SGUG_TR_SHUFFLE;
			break;

		case GALLERY_MAIN_MENU_TITLE:
			text = SGUG_TR_SLIDESHOW;
			break;
		default:
			break;
	}

	return text;
}

static int _gallery_get_time_radio_value(void)
{
	int radio_value = 0;
	double time_value = 0;

	gallery_key_init_current_time(&time_value);

	for(radio_value = 0; radio_value < GALLERY_SUB_TIME_COUNT; radio_value++)
	{
		if(abs(time_value - gallery_key_get_time_vconf_value(radio_value)) < MAX_PRECISION)
			break;
	}

	return radio_value;
}

static char *_gallery_get_menu_time_text(void)
{
	int radio_value = 0;
	radio_value =_gallery_get_time_radio_value();

	return gallery_key_time_menu_get(radio_value);
}

static char *_gallery_get_menu_sub_elm_text(int param)
{
	char *text = NULL;

	switch(param)
	{
		case GALLERY_MAIN_MENU_TIME:
			text = _gallery_get_menu_time_text();
			break;
		default:
			break;
	}

	return text;
}

static char *
_gallery_genlist_text_get(void *data, Evas_Object *obj, const char *part)
{
	gallery_retv_if(!part, NULL);

	int param = (int )data;

	char *txt = NULL;
	int input_len = 0;
	input_len = strlen(part);

	if (input_len == strlen("elm.text.1") && strncmp(part, "elm.text.1", input_len) == 0 )
	{
		txt = _gallery_get_menu_elm_text(param);
	}
	else if (input_len == strlen("elm.text.2") && strncmp(part, "elm.text.2", input_len) == 0 )
	{
		txt = _gallery_get_menu_sub_elm_text(param);
	}
	else if (input_len == strlen("elm.text") && strncmp(part, "elm.text", input_len) == 0 )
	{
		txt = _gallery_get_menu_elm_text(param);
	}

	if(txt)
		return strdup(txt);
	else
		return NULL;
}

static char *
_gallery_expand_common_text_get(const char *part, void *data, gallery_main_menu_item_t mitem)
{
	gallery_retv_if(!part, NULL);

	int param = (int )data;
	char *txt = NULL;
	int input_len = 0;

	input_len = strlen(part);

	if (input_len == strlen("elm.text") && strncmp(part, "elm.text", input_len) == 0 )
	{
		switch(mitem)
		{
			case GALLERY_MAIN_MENU_TIME:
				txt = gallery_key_time_menu_get(param);
				break;
			default:
				break;
		}

		if(txt)
			return strdup(txt);
	}

	return NULL;
}

static char *
_gallery_time_text_get(void *data, Evas_Object *obj, const char *part)
{
	gallery_retv_if(!part, NULL);

	return _gallery_expand_common_text_get(part, data, GALLERY_MAIN_MENU_TIME);
}

static Evas_Object*
_gallery_expand_common_icon_get(Evas_Object *obj, const char *part, int param, int radio_value, Evas_Object *radio_group)
{
	gallery_retv_if(!part, NULL);
	gallery_retv_if(!obj, NULL);

	if (strncmp(part, "elm.icon", strlen(part)) == 0 )
	{
		Evas_Object *radio_btn = elm_radio_add(obj);
		evas_object_propagate_events_set(radio_btn, EINA_TRUE);
		elm_radio_state_value_set(radio_btn, param);
		elm_radio_group_add(radio_btn, radio_group);
		elm_radio_value_set(radio_group, radio_value);
		evas_object_show(radio_btn);

		return radio_btn;
	}

	return NULL;
}

static Evas_Object*
_gallery_time_content_get(void *data, Evas_Object *obj, const char *part)
{
	gallery_retv_if(!part, NULL);

	struct ug_data *ugd = evas_object_data_get(obj, "ugd");
	gallery_retvm_if(!ugd, NULL, "INVALID param");

	int radio_value = 0;
	radio_value = _gallery_get_time_radio_value();

	return _gallery_expand_common_icon_get(obj, part, (int )data, radio_value, ugd->radio_group);
}

static void
_gallery_genlist_icon_cb(void *data,  Evas_Object *obj, void *event_info)
{
	gallery_ret_if(!data);
	struct ug_data *ugd = evas_object_data_get(obj, "ugd");

	int state = 0;
	int index = (int )data;

	if (index == GALLERY_MAIN_MENU_REPEAT)
	{
		gallery_key_init_repeat_state(&state);

		if (gallery_key_set_repeat_state(!state))
		{
			gallery_key_text_popup(ugd,SGUG_TR_FAILED);
		}
	}
	else if(index == GALLERY_MAIN_MENU_SHUFFLE)
	{
		gallery_key_init_shuffle_state(&state);

		if(gallery_key_set_shuffle_state(!state))
		{
			gallery_key_text_popup(ugd,SGUG_TR_FAILED);
		}
	}
}

static Evas_Object *_gallery_create_check(Evas_Object *obj, struct ug_data *ugd, int index, Init_State_Func func, int *state)
{
	gallery_retvm_if(!obj, NULL, "obj is NULL");
	gallery_retvm_if(!ugd, NULL, "ugd is NULL");
	gallery_retvm_if(!state, NULL, "state is NULL");

	if(func && state)
		func(state);

	Evas_Object *check = NULL;
	check  = elm_check_add(obj);
	evas_object_data_set(check, "ugd", ugd);
	elm_check_state_set( check, (bool)*state);
	elm_object_style_set(check, "on&off");
	evas_object_propagate_events_set(check, EINA_FALSE);
	evas_object_smart_callback_add(check, "changed", _gallery_genlist_icon_cb, (void *)index);

	return check;
}

static Evas_Object*
_gallery_genlist_icon_get(void *data, Evas_Object *obj, const char *part)
{
	gallery_retv_if(!data, NULL);
	gallery_retv_if(!part, NULL);

	struct ug_data *ugd = NULL;

	ugd = evas_object_data_get(obj, "ugd");
	gallery_retvm_if(!ugd, NULL, "INVALID param");

	int index = (int )data;
	Evas_Object *check = NULL;

	if (strncmp(part, "elm.icon", strlen(part)) == 0)
	{
		if (index == GALLERY_MAIN_MENU_REPEAT)
		{
			check = _gallery_create_check(obj, ugd, index, gallery_key_init_repeat_state, &ugd->repeat_state);
			ugd->repeat_btn = check;
		}
		else if(index == GALLERY_MAIN_MENU_SHUFFLE)
		{
			check = _gallery_create_check(obj, ugd, index, gallery_key_init_shuffle_state, &ugd->shuffle_state);
			ugd->shuffle_btn = check;
		}
	}

	return check;
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

	if(gallery_key_set_current_time(gallery_key_get_time_vconf_value(param)))
	{
		gallery_key_text_popup(ugd,SGUG_TR_FAILED);
	}

	elm_radio_value_set(ugd->radio_group, param);
	elm_genlist_item_update(ugd->gl_it[index]);

}

static void
_gallery_genlist_con(void *data, Evas_Object *obj, void *event_info)
{
	gallery_ret_if(!event_info);

	Elm_Object_Item *item = event_info;
	elm_genlist_item_subitems_clear(item);
}

static void
_gallery_genlist_time_select_cb(void *data, Evas_Object *obj, void *event_info)
{
	struct ug_data *ugd = evas_object_data_get(obj, "ugd");
	gallery_ret_if(!ugd);

	gallery_info("_gallery_genlist_time_select_cb");

	int index = 0;
	Elm_Object_Item *item = (Elm_Object_Item *)event_info;

	elm_genlist_item_selected_set(item, EINA_FALSE);

	Evas_Object *gl = elm_object_item_widget_get(item);

	ugd->check_time_itc.item_style = "dialogue/1text.1icon.3";
	ugd->check_time_itc.func.text_get = _gallery_time_text_get;
	ugd->check_time_itc.func.content_get = _gallery_time_content_get;
	ugd->check_time_itc.func.state_get = NULL;
	ugd->check_time_itc.func.del = NULL;

	ugd->radio_group = elm_radio_add(gl);
	for (index = 0; gallery_key_time_menu_get(index); index++)
	{
		elm_genlist_item_append(gl,&ugd->check_time_itc, (void *)index, item,
				ELM_GENLIST_ITEM_NONE, _gallery_time_select_cb,  (void *)index);
	}

}

static void
_gallery_genlist_exp(void *data, Evas_Object *obj, void *event_info)
{
	gallery_ret_if(!obj);

	struct ug_data *ugd = evas_object_data_get(obj, "ugd");
	gallery_ret_if(!ugd);

	gallery_info("_gallery_genlist_exp");

	Elm_Object_Item *item = (Elm_Object_Item *)event_info;

	elm_genlist_item_selected_set(item, EINA_FALSE);

	if(item == ugd->gl_it[GALLERY_MAIN_MENU_TIME])
	{
		_gallery_genlist_time_select_cb(data, obj, event_info);
	}
}
static void
_gallery_genlist_realized(void *data, Evas_Object *obj, void *event_info)
{
	gallery_ret_if(!obj);
	gallery_ret_if(!event_info);

	struct ug_data *ugd = (struct ug_data *)data;
	gallery_ret_if(!ugd);

	Elm_Object_Item *item = (Elm_Object_Item *)event_info;

	int index = (int)elm_object_item_data_get(item);

	if(index-1 == GALLERY_MAIN_MENU_TITLE)
	{
		elm_object_item_signal_emit(item, "elm,state,top", "");
	}
	else if(index+1 == GALLERY_MAIN_MENU_ITEM_MAX)
	{
		elm_object_item_signal_emit(item, "elm,state,bottom", "");
	}
	else
	{
		elm_object_item_signal_emit(item, "elm,state,center", "");
	}
}

static void
_gallery_main_layout_ug_cb(ui_gadget_h ug, enum ug_mode mode,
			     void *priv)
{
	gallery_ret_if(priv == NULL);
	gallery_ret_if(ug == NULL);
	Evas_Object *base = NULL;
	gallery_info("_gallery_main_layout_ug_cb");

	base = (Evas_Object *) ug_get_layout(ug);
	if (!base)
		return;

	switch (mode) {
		case UG_MODE_FULLVIEW:
			evas_object_size_hint_weight_set(base, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
			elm_win_resize_object_add(ug_get_window(), base);
			evas_object_show(base);
			break;
		default:
			break;
	}
	gallery_info("end _gallery_main_layout_ug_cb");
}


static void
_gallery_genlist_select_cb(void *data, Evas_Object *obj, void *event_info)
{
	struct ug_data *ugd = evas_object_data_get(obj, "ugd");
	gallery_ret_if(!ugd);

	int param = (int )data;

	int icon_state = 0;
	bool expand_state = false;

	Elm_Object_Item *item = (Elm_Object_Item *)event_info;

	elm_genlist_item_selected_set(item, EINA_FALSE);

	if(param == GALLERY_MAIN_MENU_TIME)
	{
		expand_state = elm_genlist_item_expanded_get(item);
		elm_genlist_item_expanded_set(item, !expand_state);
	}
	else if(param == GALLERY_MAIN_MENU_REPEAT)
	{
		gallery_key_init_repeat_state(&icon_state);
		icon_state = !icon_state;

		if(gallery_key_set_repeat_state(icon_state))
		{
			gallery_key_text_popup(ugd,SGUG_TR_FAILED);
		}

		elm_check_state_set(ugd->repeat_btn, (bool)icon_state);
	}
	else if(param == GALLERY_MAIN_MENU_SHUFFLE)
	{
		gallery_key_init_shuffle_state(&icon_state);
		icon_state = !icon_state;

		if(gallery_key_set_shuffle_state(icon_state))
		{
			gallery_key_text_popup(ugd,SGUG_TR_FAILED);
		}

		elm_check_state_set(ugd->shuffle_btn,(bool)icon_state);
	}
}

static Evas_Object *
_gallery_genlist_items_add (Evas_Object *parent, struct ug_data *ugd)
{
	Evas_Object *main_genlist;
	Elm_Object_Item *sep;

	int index = 0;

	main_genlist = elm_genlist_add(parent);

#ifdef USE_DIALOGUE_STYLE
	elm_object_style_set(main_genlist, "dialogue");
#else
	evas_object_smart_callback_add(main_genlist, "realized", _gallery_genlist_realized, ugd);
#endif

	evas_object_size_hint_weight_set(main_genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(main_genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);

	ugd->genlist = main_genlist;

	evas_object_data_set(ugd->genlist, "ugd", ugd);

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
	ugd->txt_icon_itc.func.content_get = _gallery_genlist_icon_get;
	ugd->txt_icon_itc.func.state_get = NULL;
	ugd->txt_icon_itc.func.del = NULL;

	ugd->two_txt_icon_itc.item_style = "dialogue/2text.1icon.6";
	ugd->two_txt_icon_itc.func.text_get = _gallery_genlist_text_get;
	ugd->two_txt_icon_itc.func.content_get = _gallery_genlist_icon_get;
	ugd->two_txt_icon_itc.func.state_get = NULL;
	ugd->two_txt_icon_itc.func.del = NULL;

	ugd->seperator_itc.item_style = "grouptitle.dialogue.seperator";
	ugd->seperator_itc.func.text_get = NULL;
	ugd->seperator_itc.func.content_get = NULL;
	ugd->seperator_itc.func.state_get = NULL;
	ugd->seperator_itc.func.del = NULL;

	ugd->seperator_end_itc.item_style = "dialogue/separator/end";
	ugd->seperator_end_itc.func.text_get = NULL;
	ugd->seperator_end_itc.func.content_get = NULL;
	ugd->seperator_end_itc.func.state_get = NULL;
	ugd->seperator_end_itc.func.del = NULL;

	sep = elm_genlist_item_append(main_genlist, &ugd->seperator_itc, NULL, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
	elm_genlist_item_select_mode_set(sep, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);

	for (index = GALLERY_MAIN_MENU_TITLE; index < GALLERY_MAIN_MENU_ITEM_MAX; index++)
	{
		//expandable genlist
		if (index == GALLERY_MAIN_MENU_TIME)
		{
			ugd->gl_it[index] = elm_genlist_item_append(main_genlist, &ugd->expandable_itc,
								    (void *)index, NULL,
								    ELM_GENLIST_ITEM_TREE,
								    _gallery_genlist_select_cb,
								    (void *)index);
		}
		// repeat&shuffle 1txt .1icon
		else if (index == GALLERY_MAIN_MENU_REPEAT ||
			 index == GALLERY_MAIN_MENU_SHUFFLE)
		{
			ugd->gl_it[index] = elm_genlist_item_append(main_genlist, &ugd->txt_icon_itc,
								    (void *)index, NULL,
								    ELM_GENLIST_ITEM_NONE,
								    _gallery_genlist_select_cb,
								    (void *)index);
		}
	}

	sep = elm_genlist_item_append(main_genlist, &ugd->seperator_end_itc, NULL, NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
	elm_genlist_item_select_mode_set(sep, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY);

	evas_object_smart_callback_add(main_genlist, "expanded", _gallery_genlist_exp, ugd);
	evas_object_smart_callback_add(main_genlist, "contracted", _gallery_genlist_con, ugd);

	return main_genlist;
}


static Evas_Object *
_gallery_create_fullview(Evas_Object *parent, struct ug_data *ugd)
{
	gallery_retv_if(!parent, NULL);

	Evas_Object *layout;
	layout = elm_layout_add(parent);
	gallery_retv_if( layout == NULL, NULL );

	Eina_Bool ret = EINA_FALSE;

	const char *profile = elm_config_profile_get();
	if (!strcmp(profile,"mobile"))
	{
		ret = elm_layout_theme_set(layout, "layout", "application", "default");
		gallery_debug("layout/application/default");
	}
	else if (!strcmp(profile,"desktop"))
	{
		ret = elm_layout_theme_set(layout, "layout", "application", "noindicator");
		gallery_debug("layout/application/noindicator");
	}

	evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	return layout;
}

static Evas_Object*
_gallery_create_naviframe(Evas_Object* parent)
{
	gallery_retv_if(!parent, NULL);

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
	elm_object_part_content_set(parent, "elm.swallow.bg", bg);
	evas_object_show(bg);

    return bg;
}

static void *
_on_create(ui_gadget_h ug, enum ug_mode mode, service_h data, void *priv)
{
	gallery_retvm_if(!ug, NULL, "ug is NULL, check it !!");
	gallery_retvm_if(!priv, NULL, "priv is NULL, check it !!");

	Evas_Object *parent = NULL;
	struct ug_data *ugd = NULL;

	ugd = (struct ug_data *)priv;
	ugd->ug = ug;

	/* Bind text domain for internalization */
	bindtextdomain("ug-setting-gallery-efl" , "/usr/ug/res/locale");

	parent = (Evas_Object *)ug_get_parent_layout(ug);
	gallery_retvm_if(parent == NULL, NULL, "parent layout is NULL");

	if (mode == UG_MODE_FULLVIEW)
	{
		ugd->main_layout = _gallery_create_fullview(parent, ugd);
	}
	else
	{
		gallery_info("It is not UG_MODE_FULLVIEW");
		return NULL;
	}

	ugd->bg = _gallery_create_bg(ugd->main_layout);

	if(ugd->main_layout)
	{
		ugd->naviframe = _gallery_create_naviframe(ugd->main_layout);
		evas_object_data_set(ugd->naviframe, "ugd", ugd);
	}

	ugd->genlist = _gallery_genlist_items_add(ugd->naviframe,ugd);

	//evas_object_data_set(ugd->genlist, "ugd", ugd);

	ugd->backbtn = elm_button_add(ugd->naviframe);
	evas_object_smart_callback_add(ugd->backbtn, "clicked", _gallery_quit_cb, ugd);
	elm_object_style_set(ugd->backbtn, "naviframe/end_btn/default");

	elm_naviframe_item_push(ugd->naviframe, SGUG_TR_GALLERY, ugd->backbtn,
				NULL, ugd->genlist, NULL);

	evas_object_show(ugd->main_layout);

	return ugd->main_layout;
}

static void
_on_message(ui_gadget_h ug, service_h msg, service_h data, void *priv)
{
}

static void
_on_event(ui_gadget_h ug, enum ug_event event, service_h data, void *priv)
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
_on_start(ui_gadget_h ug, service_h data, void *priv)
{
}

static void
_on_pause(ui_gadget_h ug, service_h data, void *priv)
{

}

static void
_on_resume(ui_gadget_h ug, service_h data, void *priv)
{

}

static void
_on_destroy(ui_gadget_h ug, service_h data, void *priv)
{
	struct ug_data *ugd;

	gallery_retm_if(!ug, "ug is NULL, check it !!");
	gallery_retm_if(!priv, "priv is NULL, check it !!");

	ugd = priv;
	if(ugd->main_layout)
	{
		evas_object_del(ugd->main_layout);
		ugd->main_layout = NULL;
	}
}

static void
_on_key_event(ui_gadget_h ug, enum ug_key_event event, service_h data, void *priv)
{

}

UG_MODULE_API int
UG_MODULE_INIT(struct ug_module_ops *ops)
{
	gallery_retvm_if(!ops, -1, "ops is NULL");

	struct ug_data *ugd = NULL;
	ugd = calloc(1, sizeof(struct ug_data));
	gallery_retvm_if(!ugd, -1, "ugd is NULL");

	ops->create = _on_create;
	ops->start = _on_start;
	ops->pause = _on_pause;
	ops->resume = _on_resume;
	ops->destroy = _on_destroy;
	ops->message = _on_message;
	ops->key_event = _on_key_event;
	ops->event = _on_event;
	ops->priv = ugd;
	ops->opt = UG_OPT_INDICATOR_ENABLE;

	return 0;
}

UG_MODULE_API int setting_plugin_reset(service_h data, void *priv)
{
	gallery_info("Reset the settings start");

	int ret = 0;
	ret += gallery_key_set_current_time(DEFAULT_TIMER);
	ret += gallery_key_set_repeat_state(DEFAULT_REPEAT);
	ret += gallery_key_set_shuffle_state(DEFAULT_SHUFFLE);
	gallery_info("Finished");

	return ret;
}

UG_MODULE_API void
UG_MODULE_EXIT(struct ug_module_ops *ops)
{
	gallery_retm_if(!ops, "ops is NULL");

	struct ug_data *ugd;

	ugd = ops->priv;
	if (ugd)
	{
		free(ugd);
	}
}
