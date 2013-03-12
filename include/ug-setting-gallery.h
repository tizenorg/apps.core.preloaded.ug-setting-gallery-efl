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

#ifndef __GALLERY_UG_EFL_H__
#define	__GALLERY_UG_EFL_H__


#include <Elementary.h>
#include <ui-gadget-module.h>


#define PKGNAME "ug-setting-gallery-efl"

#define _EDJ(o)		elm_layout_edje_get(o)

/* Original code is in appcore-common.h */
#define _(str)	gettext(str)  /**< gettext alias */
#define gettext_noop(str) (str) /**< keyword for xgettext  to extract translatable strings */
#define N_(str)	gettext_noop(str)  /**< gettext_noop alias */

#define _SG_GETSYSTEMSTR(ID)         dgettext("sys_string", (ID))
#define _SG_GETLOCALSTR(ID)         dgettext("ug-setting-gallery-efl", (ID))

#define GALLERY_SUB_TIME_COUNT		5
#define GALLERY_SUB_EFFECT_COUNT	10//5

#define SGUG_TR_GALLERY	_SG_GETSYSTEMSTR("IDS_COM_BODY_GALLERY")

#define SGUG_TR_SLIDESHOW	_SG_GETLOCALSTR("IDS_MEDIABR_OPT_SLIDE_SHOW")

#define SGUG_TR_PESF		_SG_GETLOCALSTR("IDS_MEDIABR_BODY_SLIDESHOW_INTERVAL")
#define SGUG_TR_REPEAT		_SG_GETLOCALSTR("IDS_MEDIABR_BODY_REPEAT")
#define SGUG_TR_SHUFFLE		_SG_GETLOCALSTR("IDS_MEDIABR_BODY_SHUFFLE")


#define SGUG_TR_1s	_SG_GETSYSTEMSTR("IDS_COM_BODY_1_SECOND")
#define SGUG_TR_2s	_SG_GETLOCALSTR("IDS_MEDIABR_OPT_2_SECONDS")
#define SGUG_TR_3s	_SG_GETLOCALSTR("IDS_MEDIABR_OPT_3_SECONDS")
#define SGUG_TR_5s	_SG_GETLOCALSTR("IDS_MEDIABR_BODY_5_SECONDS")
#define SGUG_TR_10s	_SG_GETLOCALSTR("IDS_MEDIABR_BODY_10_SECONDS")
#define SGUG_TR_20s	_SG_GETLOCALSTR("IDS_MEDIABR_BODY_20_SECONDS")
#define SGUG_TR_FAILED	_("Setting failed, please try again..")


#define VCONFKEY_GALLERY_ITV_TIME_1S	(1.0f)
#define VCONFKEY_GALLERY_ITV_TIME_2S	(2.0f)
#define VCONFKEY_GALLERY_ITV_TIME_3S	(3.0f)
#define VCONFKEY_GALLERY_ITV_TIME_5S	(5.0f)
#define VCONFKEY_GALLERY_ITV_TIME_10S	(10.0f)
#define VCONFKEY_GALLERY_ITV_TIME_20S	(20.0f)


#define DEFAULT_TIMER			VCONFKEY_GALLERY_ITV_TIME_3S
#define DEFAULT_REPEAT			false
#define DEFAULT_SHUFFLE			false

#ifdef TARGET
typedef enum
{
	GALLERY_MAIN_MENU_TITLE = 0,
	GALLERY_MAIN_MENU_TIME,
	GALLERY_MAIN_MENU_REPEAT,
	GALLERY_MAIN_MENU_SHUFFLE,
	GALLERY_MAIN_MENU_ITEM_MAX,
}gallery_main_menu_item_t;
#else
typedef enum
{
	GALLERY_MAIN_MENU_TITLE = 0,
	GALLERY_MAIN_MENU_TIME,
	GALLERY_MAIN_MENU_REPEAT,
	GALLERY_MAIN_MENU_SHUFFLE,
	GALLERY_MAIN_MENU_ITEM_MAX,
}gallery_main_menu_item_t;
#endif

typedef struct ug_data
{
	Evas_Object *main_layout;
	Evas_Object *bg;
	Evas_Object *naviframe;
	Evas_Object *time_layout;
	Evas_Object *genlist;
	Evas_Object *backbtn;

	Evas_Object *repeat_btn;
	Evas_Object *shuffle_btn;
	Evas_Object *inline_video_btn;
	int repeat_state;
	int shuffle_state;
	int inline_video_state;
	Evas_Object *radio_group;
	Evas_Object *popup;
	Elm_Object_Item *gl_it[GALLERY_MAIN_MENU_ITEM_MAX];


	Elm_Genlist_Item_Class check_time_itc;
	Elm_Genlist_Item_Class expandable_itc;

	Elm_Genlist_Item_Class txt_icon_itc;

	Elm_Genlist_Item_Class title_itc;

	Elm_Genlist_Item_Class seperator_itc;
	Elm_Genlist_Item_Class seperator_end_itc;
	Elm_Genlist_Item_Class two_txt_icon_itc;

	ui_gadget_h	ug;
	ui_gadget_h ug_loading;
}st_ug_data;


#endif

