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

#include <Elementary.h>
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"
#include "utils.h"

#define DESCRITPION "Sample panes description"
#define NAME "Sample panes"

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}

void set_horizontal(void *data) {
    Evas_Object* obj = (Evas_Object*) data;
    elm_panes_horizontal_set(obj, EINA_TRUE);
}

void unset_horizontal(void *data) {
    Evas_Object* obj = (Evas_Object*) data;
    elm_panes_horizontal_set(obj, EINA_FALSE);
}

void set_value(void *data) {
	Evas_Object *obj = (Evas_Object*) data;
	elm_panes_content_left_size_set (obj, 0.2);
	evas_object_smart_callback_call(obj, "changed", NULL);
}



EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *panes = NULL;
	Evas_Object *bg = NULL;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "Sample panes application!");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);

	panes = elm_panes_add(win);
	evas_object_size_hint_weight_set(panes, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(panes, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_win_resize_object_add(win, panes);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, panes);

	register_request_listener(EVENT_SET_ENABLED, set_enabled, panes);
	register_request_listener(EVENT_SET_DISABLED, set_disabled, panes);
	register_request_listener(EVENT_HIDE, hide, panes);
	register_request_listener(EVENT_SHOW, show, panes);
	register_request_listener(EVENT_SET_HORIZONTAL, set_horizontal, panes);
	register_request_listener(EVENT_UNSET_HORIZONTAL, unset_horizontal, panes);
	register_request_listener(EVENT_SET_VALUE, set_value, panes);

	evas_object_resize(win, 240, 60);
	evas_object_show(panes);
	evas_object_show(win);
	evas_object_show(bg);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
