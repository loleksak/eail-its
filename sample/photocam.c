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
* cc -o photocam photocam.c request.c `pkg-config --cflags elementary` -Wall `pkg-config --libs elementary`
*/
#include <Elementary.h>
#include "utils.h"
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"

#define DESCRITPION "Sample photocam description"
#define NAME "Sample photocam"

static int zoom;


void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}

void resize(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	evas_object_resize(obj,100,100);
}

void _zoom_in_response_cb (void *data, Evas_Object *obj, void *event_info) {
	if(zoom > elm_photocam_zoom_get(obj))
		send_response(SUCCESS);
}

void _zoom_out_response_cb (void *data, Evas_Object *obj, void *event_info) {
	if( zoom < elm_photocam_zoom_get(obj))
		send_response(SUCCESS);
}

void register_action_zoom_in(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	zoom = elm_photocam_zoom_get(obj);
	evas_object_smart_callback_add(obj, "zoom,stop", _zoom_in_response_cb, NULL);
}

void register_action_zoom_out(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	zoom = elm_photocam_zoom_get(obj);
	evas_object_smart_callback_add(obj, "zoom,stop", _zoom_out_response_cb, NULL);
}


EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *bg = NULL;
	Evas_Object *photocam = NULL;
	char *img_file = NULL;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "photocam");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);

	photocam = elm_photocam_add(win);
	img_file = get_file(argv[0], "leon.jpg");
	elm_photocam_file_set(photocam, img_file);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, photocam);

	register_request_listener(EVENT_RESIZE, resize, photocam);
	register_request_listener(EVENT_SET_DISABLED, set_disabled, photocam);
	register_request_listener(EVENT_SET_ENABLED, set_enabled, photocam);
	register_request_listener(EVENT_HIDE, hide, photocam);
	register_request_listener(EVENT_SHOW, show, photocam);
	register_request_listener(EVENT_CLICK, click, photocam);

	register_request_listener(ACTION_CLICK, register_action_click, photocam);
	register_request_listener(ACTION_ZOOM_IN, register_action_zoom_in, photocam);
	register_request_listener(ACTION_ZOOM_OUT, register_action_zoom_out, photocam);
	register_request_listener(ACTION_SCROLL_LEFT, register_action_scroll_left, photocam);
	register_request_listener(ACTION_SCROLL_RIGHT, register_action_scroll_right, photocam);
	register_request_listener(ACTION_SCROLL_UP, register_action_scroll_up, photocam);
	register_request_listener(ACTION_SCROLL_DOWN, register_action_scroll_down, photocam);

	evas_object_resize(win, 240, 120);
	evas_object_resize(photocam, 120, 60);
	evas_object_move(photocam, 60, 40);

	evas_object_show(win);
	evas_object_show(bg);
	evas_object_show(photocam);

	elm_run();
	request_shutdown();
	elm_shutdown();
	free(img_file);
	return 0;
}
ELM_MAIN()
