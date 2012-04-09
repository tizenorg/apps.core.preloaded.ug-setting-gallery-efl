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

#ifndef __GALLERY_UG_EFL_H__
#define	__GALLERY_UG_EFL_H__


#include <Elementary.h>

#define PKGNAME "ug-setting-gallery-efl"

#define _EDJ(o)		elm_layout_edje_get(o)
//#define N_(s)			dgettext_noop(s)


/* Original code is in appcore-common.h */
#define _(str)	gettext(str)  /**< gettext alias */
#define gettext_noop(str) (str) /**< keyword for xgettext  to extract translatable strings */
#define N_(str)	gettext_noop(str)  /**< gettext_noop alias */

#define _SG_GETSYSTEMSTR(ID)         dgettext("sys_string", (ID))
#define _SG_GETLOCALSTR(ID)         dgettext("ug-setting-gallery-efl", (ID))


#define GALLERY_SUB_TIME_COUNT		5
#define GALLERY_SUB_EFFECT_COUNT	5

#define SGUG_TR_GALLERY	_SG_GETSYSTEMSTR("IDS_COM_BODY_GALLERY")

#define SGUG_TR_SLIDESHOW	_SG_GETLOCALSTR("IDS_MEDIABR_OPT_SLIDE_SHOW")
#define SGUG_TR_PESF	_SG_GETLOCALSTR("IDS_MEDIABR_BODY_SLIDESHOW_INTERVAL")
#define SGUG_TR_REPEAT	_SG_GETLOCALSTR("IDS_MEDIABR_BODY_REPEAT")
#define SGUG_TR_SHUFFLE	_SG_GETLOCALSTR("IDS_MEDIABR_BODY_SHUFFLE")
#define SGUG_TR_SUBTITLE_TITLE	_SG_GETSYSTEMSTR("IDS_COM_BODY_VIDEO")
#define SGUG_TR_SUBTITLE	_SG_GETLOCALSTR("IDS_MEDIABR_BODY_SUBTITLES")

#define SGUG_TR_2s	_SG_GETLOCALSTR("IDS_MEDIABR_OPT_2_SECONDS")
#define SGUG_TR_3s	_SG_GETLOCALSTR("IDS_MEDIABR_OPT_3_SECONDS")
#define SGUG_TR_5s	_SG_GETLOCALSTR("IDS_MEDIABR_BODY_5_SECONDS")
#define SGUG_TR_10s	_SG_GETLOCALSTR("IDS_MEDIABR_BODY_10_SECONDS")
#define SGUG_TR_20s	_SG_GETLOCALSTR("IDS_MEDIABR_BODY_20_SECONDS")
#define SGUG_TR_FAILED	_("Setting failed, please try again..")

#define VCONFKEY_GALLERY_ITV_TIME_2S	(2.0f)
#define VCONFKEY_GALLERY_ITV_TIME_3S	(3.0f)
#define VCONFKEY_GALLERY_ITV_TIME_5S	(5.0f)
#define VCONFKEY_GALLERY_ITV_TIME_10S	(10.0f)
#define VCONFKEY_GALLERY_ITV_TIME_20S	(20.0f)

typedef enum
{
	GALLERY_MAIN_MENU_TITLE = 0,
	GALLERY_MAIN_MENU_TIME,
	GALLERY_MAIN_MENU_REPEAT,
	GALLERY_MAIN_MENU_SHUFFLE,
	GALLERY_MAIN_MENU_SUBTITLE_TITLE,
	GALLERY_MAIN_MENU_SUBTITLE_ON_OFF,
	GALLERY_MAIN_MENU_ITEM_MAX,
}gallery_main_menu_item_t;


struct ug_data
{
	Evas_Object *main_layout;
	Evas_Object *bg;
	Evas_Object *naviframe;
	Evas_Object *time_layout;
	Evas_Object *genlist;
	Evas_Object *backbtn;

	Evas_Object *repeat_btn;
	Evas_Object *shuffle_btn;
	Evas_Object *subtitle_btn;
	int repeat_state;
	int shuffle_state;
	int subtitle_state;
	Evas_Object *radio_group;
	Evas_Object *effect_radio_group;
	Evas_Object *popup;
	Elm_Object_Item *gl_it[GALLERY_MAIN_MENU_ITEM_MAX];

	Elm_Genlist_Item_Class check_time_itc;
	Elm_Genlist_Item_Class expandable_itc;

	Elm_Genlist_Item_Class txt_icon_itc;

	Elm_Genlist_Item_Class title_itc;
	Elm_Genlist_Item_Class effect_itc;

	struct ui_gadget *ug;
};


#endif

