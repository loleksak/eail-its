/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

/*
* cc -o slideshow slideshow.c request.c `pkg-config --cflags elementary` -Wall `pkg-config --libs elementary`
*/

#include "utils.h"
#include <Elementary.h>
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"

#define DESCRITPION "Sample slideshow description"
#define NAME "Sample slideshow"

static Elm_Slideshow_Item_Class itc;

static Evas_Object *
_get(void		  *data,
	  Evas_Object *obj) {
	Evas_Object *photo = elm_photo_add(obj);
	elm_photo_file_set(photo, data);
	elm_photo_fill_inside_set(photo, EINA_TRUE);
	elm_object_style_set(photo, "shadow");
	return photo;
}

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}

Eina_Bool _response_start_cb (void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	if(elm_slideshow_timeout_get(obj) > 0.0)
		send_response(SUCCESS);
	return EINA_FALSE;
}

Eina_Bool _response_stop_cb (void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	if(elm_slideshow_timeout_get(obj) < 0.0001)
		send_response(SUCCESS);
	return EINA_FALSE;
}

void register_action_start(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	elm_slideshow_timeout_set(obj, 0.0);
	ecore_timer_add(1, _response_start_cb, data);
}

void register_action_stop(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	elm_slideshow_timeout_set(obj, 0.5);
	ecore_timer_add(1, _response_stop_cb, data);
}

void _response_next_prev_cb(void *data, Evas_Object *obj, void *event_info) {
	if(data == event_info)
		send_response(SUCCESS);
}

void register_action_next(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	evas_object_smart_callback_add(obj, "changed",
		 _response_next_prev_cb, elm_slideshow_item_nth_get(obj, 1));
}

void register_action_previous(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	elm_slideshow_next(obj);
	evas_object_smart_callback_add(obj, "changed",
		 _response_next_prev_cb, elm_slideshow_item_nth_get(obj, 0));
}


EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *bg = NULL;
	Evas_Object *slideshow = NULL;
	char *img_file = NULL;
	char *img2_file = NULL;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "slideshow");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);

	slideshow = elm_slideshow_add(win);

	itc.func.get = _get;
	itc.func.del = NULL;

	img_file = get_file(argv[0], "leon.jpg");
	elm_slideshow_item_add(slideshow, &itc, img_file);
	img2_file = get_file(argv[0], "lena2.jpg");
	elm_slideshow_item_add(slideshow, &itc, img2_file);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, slideshow);

	register_request_listener(EVENT_SET_FOCUS, set_focus, slideshow);
	register_request_listener(EVENT_UNSET_FOCUS, unset_focus, slideshow);
	register_request_listener(EVENT_SET_FOCUSABLE, set_focusable, slideshow);
	register_request_listener(EVENT_UNSET_FOCUSABLE, unset_focusable, slideshow);
	register_request_listener(EVENT_SHOW, show, slideshow);
	register_request_listener(EVENT_HIDE, hide, slideshow);
	register_request_listener(EVENT_SET_ENABLED, set_enabled, slideshow);
	register_request_listener(EVENT_SET_DISABLED, set_disabled, slideshow);

	register_request_listener(ACTION_NEXT, register_action_next, slideshow);
	register_request_listener(ACTION_PREVIOUS, register_action_previous, slideshow);
	register_request_listener(ACTION_START, register_action_start, slideshow);
	register_request_listener(ACTION_STOP, register_action_stop, slideshow);

	evas_object_resize(win, 240, 120);
	evas_object_resize(slideshow, 120, 60);
	evas_object_move(slideshow, 60, 40);

	evas_object_show(win);
	evas_object_show(bg);
	evas_object_show(slideshow);

	elm_run();
	request_shutdown();
	elm_shutdown();
	free(img_file);
	free(img2_file);
	return 0;
}
ELM_MAIN()
