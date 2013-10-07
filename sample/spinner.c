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
* cc -o spinner spinner.c `pkg-config --cflags elementary` -Wall `pkg-config --libs elementary`
*/
#include <Elementary.h>
#include "utils.h"
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.c"

#define DESCRITPION "Sample spinner description"
#define NAME "Sample spinner"

void set_editable(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_spinner_editable_set(obj, EINA_TRUE);
}

void unset_editable(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_spinner_editable_set(obj, EINA_FALSE);
}

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}

void set_value(void *data) {
	Evas_Object *obj = (Evas_Object*) data;
	elm_spinner_value_set(obj, 8.0);
	evas_object_smart_callback_call(obj, "changed", NULL);
}


EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *spinner = NULL;
	Evas_Object *bg = NULL;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "spinner");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	bg = elm_bg_add(win);
	elm_win_resize_object_add(win, bg);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	spinner = elm_spinner_add(win);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, spinner);

	register_request_listener(EVENT_HIDE, hide, spinner);
	register_request_listener(EVENT_SHOW, show, spinner);
	register_request_listener(EVENT_SET_ENABLED, set_enabled, spinner);
	register_request_listener(EVENT_SET_DISABLED, set_disabled, spinner);
	register_request_listener(EVENT_SET_FOCUS, set_focus, spinner);
	register_request_listener(EVENT_UNSET_FOCUS, unset_focus, spinner);
	register_request_listener(EVENT_SET_FOCUSABLE, set_focusable, spinner);
	register_request_listener(EVENT_UNSET_FOCUSABLE, unset_focusable, spinner);
	register_request_listener(EVENT_SET_EDITABLE, set_editable, spinner);
	register_request_listener(EVENT_UNSET_EDITABLE, unset_editable, spinner);
	register_request_listener(EVENT_SET_VALUE, set_value, spinner);

	evas_object_resize(win, 240, 120);
	evas_object_resize(spinner, 120, 60);
	evas_object_move(spinner, 60, 40);

	evas_object_show(win);
	evas_object_show(bg);
	evas_object_show(spinner);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
