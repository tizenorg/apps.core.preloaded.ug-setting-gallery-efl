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

#ifndef __gallery_debug_H__
#define __gallery_debug_H__

#define ENABLE_LOG_SYSTEM

#ifdef ENABLE_LOG_SYSTEM
#define USE_DLOG_SYSTEM

#ifdef USE_DLOG_SYSTEM
#include <dlog.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "gallery-setting"

#define gallery_debug(fmt, arg...) do{ \
			LOGD("[%s :%05d] "fmt "\n",__FUNCTION__, __LINE__,##arg);     \
		} while (0)
#define gallery_info(fmt, arg...) do{ \
			LOGI("[%s :%05d] "fmt "\n",__FUNCTION__, __LINE__,##arg);     \
		} while (0)
#define gallery_warn(fmt, arg...) do{ \
			LOGW("[%s :%05d] "fmt "\n",__FUNCTION__, __LINE__,##arg);     \
		} while (0)
#define gallery_error(fmt, arg...) do{ \
			LOGE("[%s :%05d] "fmt "\n",__FUNCTION__, __LINE__,##arg);     \
		} while (0)

#define gallery_func_enter()	LOGD( "[%s: %05d] <<<< enter  \n", __func__, __LINE__)
#define gallery_func_leave()	LOGD( "[%s: %05d] >>>> leave  \n", __func__, __LINE__ )

#else //USE_DLOG_SYSTEM
#define gallery_debug(fmt, arg...) do { \
        fprintf(stderr,"[%s:%05d]  " fmt "\n",__func__, __LINE__, ##arg); \
} while(0)

#define gallery_info(fmt, arg...) do { \
        fprintf(stderr,"[%s:%05d]  " fmt "\n",__func__, __LINE__, ##arg); \
} while(0)

#define gallery_warn(fmt, arg...) do { \
        fprintf(stderr,"[%s:%05d]  " fmt "\n",__func__, __LINE__, ##arg); \
} while(0)

#define gallery_error(fmt, arg...) do { \
        fprintf(stderr,"[%s:%05d]  " fmt "\n",__func__, __LINE__, ##arg); \
} while(0)
#define gallery_func_enter()		fprintf(stdout, "[%s : %05d] <<< enter \n", __func__, __LINE__ , ##arg)
#define gallery_func_leave()		fprintf(stdout, "[%s : %05d] >>> leave \n", __func__, __LINE__ , ##arg)

#endif // #ifdef USE_DLOG_SYSTEM

#else //ENABLE_LOG_SYSTEM
#define gallery_debug(fmt, arg...)
#define gallery_info(fmt, arg...)
#define gallery_warn(fmt, arg...)
#define gallery_error(fmt, arg...)
#endif //ENABLE_LOG_SYSTEM


#define gallery_ret_if(expr) do { \
	if(expr) { \
		return; \
	} \
} while (0)
#define gallery_retv_if(expr, val) do { \
	if(expr) { \
		return (val); \
	} \
} while (0)

#define gallery_retm_if(expr, fmt, arg...) do { \
	if(expr) { \
		gallery_warn(fmt, ##arg); \
		return; \
	} \
} while (0)

#define gallery_retvm_if(expr, val, fmt, arg...) do { \
	if(expr) { \
		gallery_warn(fmt, ##arg); \
		return (val); \
	} \
} while (0)


#endif //__gallery_debug_H__

