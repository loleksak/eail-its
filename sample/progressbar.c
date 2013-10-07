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
* cc -o progressbar progressbar.c `pkg-config --cflags elementary` -Wall `pkg-config --libs elementary`
*/
#include <Elementary.h>
#include "utils.h"
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"

#define DESCRITPION "Sample progressbar description"
#define NAME "Sample progressbar"

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}

void set_horizontal(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_progressbar_horizontal_set(obj, EINA_TRUE);
}

void unset_horizontal(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_progressbar_horizontal_set(obj, EINA_FALSE);
}

void set_animated(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_progressbar_pulse_set(obj, EINA_TRUE);
	elm_progressbar_pulse(obj, EINA_TRUE);
}

void unset_animated(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_progressbar_pulse(obj, EINA_FALSE);
}

void set_value(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_progressbar_value_set(obj, 0.5);
	evas_object_smart_callback_call(obj, "changed", NULL);
}

void resize(void *data) {
	Evas_Object *obj = (Evas_Object*) data;
	evas_object_resize(obj, 240, 120);
}


EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *progressbar = NULL;
	Evas_Object *bg = NULL;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "progressbar");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	bg = elm_bg_add(win);
	elm_win_resize_object_add(win, bg);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	progressbar = elm_progressbar_add(win);

	request_init();

	evas_object_smart_callback_add(win, "focus,in", _register_cb, progressbar);

	register_request_listener(EVENT_HIDE, hide, progressbar);
	register_request_listener(EVENT_SHOW, show, progressbar);
	register_request_listener(EVENT_RESIZE, resize, progressbar);
	register_request_listener(EVENT_SET_ENABLED, set_enabled, progressbar);
	register_request_listener(EVENT_SET_DISABLED, set_disabled, progressbar);
	register_request_listener(EVENT_SET_FOCUS, set_focus, progressbar);
	register_request_listener(EVENT_UNSET_FOCUS, unset_focus, progressbar);
	register_request_listener(EVENT_SET_FOCUSABLE, set_focusable, progressbar);
	register_request_listener(EVENT_UNSET_FOCUSABLE, unset_focusable, progressbar);
	register_request_listener(EVENT_SET_HORIZONTAL, set_horizontal, progressbar);
	register_request_listener(EVENT_UNSET_HORIZONTAL, unset_horizontal, progressbar);
	register_request_listener(EVENT_SET_ANIMATED, set_animated, progressbar);
	register_request_listener(EVENT_UNSET_ANIMATED, unset_animated, progressbar);
	register_request_listener(EVENT_SET_VALUE, set_value, progressbar);

	evas_object_resize(win, 240, 120);
	evas_object_resize(progressbar, 120, 60);
	evas_object_move(progressbar, 60, 40);

	evas_object_show(win);
	evas_object_show(bg);
	evas_object_show(progressbar);

	elm_progressbar_value_set(progressbar, 0.0);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
