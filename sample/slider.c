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
* cc -o slider slider.c request.c `pkg-config --cflags elementary` -Wall `pkg-config --libs elementary`
*/
#include <Elementary.h>
#include "utils.h"
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"

#define DESCRITPION "Sample slider description"
#define NAME "Sample slider"

Evas_Object *box = NULL;

void set_horizontal(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_box_unpack(box, obj);
	elm_slider_horizontal_set(obj, EINA_TRUE);
	elm_box_pack_end(box, obj);
}

void set_vertical(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_box_unpack(box, obj);
	elm_slider_horizontal_set(obj, EINA_FALSE);
	elm_box_pack_end(box, obj);
}

void resize(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_box_unpack(box, obj);
	evas_object_resize(obj,100,100);
}

void set_value(void *data) {
	Evas_Object *obj = (Evas_Object*) data;
	elm_slider_value_set(obj, 0.5);
	evas_object_smart_callback_call(obj, "changed", NULL);
}

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}


EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *bg = NULL;
	Evas_Object *slider = NULL;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "slider");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);

	box = elm_box_add(win);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, box);

	slider = elm_slider_add(win);

	evas_object_resize(win, 240, 120);
	evas_object_resize(slider, 120, 60);
	evas_object_move(slider, 60, 40);

	elm_box_pack_end(box, slider);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, slider);

	register_request_listener(EVENT_RESIZE, resize, slider);
	register_request_listener(EVENT_SET_DISABLED, set_disabled, slider);
	register_request_listener(EVENT_SET_ENABLED, set_enabled, slider);
	register_request_listener(EVENT_HIDE, hide, slider);
	register_request_listener(EVENT_SHOW, show, slider);
	register_request_listener(EVENT_SET_HORIZONTAL, set_horizontal, slider);
	register_request_listener(EVENT_SET_VERTICAL, set_vertical, slider);
	register_request_listener(EVENT_SET_FOCUS, set_focus, slider);
	register_request_listener(EVENT_UNSET_FOCUS, unset_focus, slider);
	register_request_listener(EVENT_SET_FOCUSABLE, set_focusable, slider);
	register_request_listener(EVENT_UNSET_FOCUSABLE, unset_focusable, slider);
	register_request_listener(EVENT_SET_VALUE, set_value, slider);

	evas_object_show(win);
	evas_object_show(bg);
	evas_object_show(box);
	evas_object_show(slider);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
