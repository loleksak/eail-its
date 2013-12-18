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

#define DESCRITPION "Sample Flipselector description"
#define NAME "Sample Flipselector"

void select_flipselector (void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_flipselector_flip_next(obj);
}

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}

void _response_next_prev_cb(void *data, Evas_Object *obj, void *event_info) {
	if (data == event_info)
		send_response(SUCCESS);
}

void register_action_next(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	evas_object_smart_callback_add(obj, "selected", _response_next_prev_cb, elm_flipselector_last_item_get(obj));
}

void register_action_previous(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	elm_flipselector_flip_next(obj);
	evas_object_smart_callback_add(obj, "selected", _response_next_prev_cb, elm_flipselector_first_item_get(obj));
}


EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *bg = NULL;
	Evas_Object *flipselector = NULL;

	win = elm_win_add(NULL, "Flipselector sample", ELM_WIN_BASIC);
	elm_win_title_set(win, "Flipselector sample");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);
	evas_object_show(bg);

	flipselector = elm_flipselector_add(win);
	elm_flipselector_item_append(flipselector, "Item", NULL, NULL);
	elm_flipselector_item_append(flipselector, "Item2", NULL, NULL);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, flipselector);

	register_request_listener(EVENT_SET_FOCUS, set_focus, flipselector);
	register_request_listener(EVENT_UNSET_FOCUS, unset_focus, flipselector);
	register_request_listener(EVENT_SET_FOCUSABLE, set_focusable, flipselector);
	register_request_listener(EVENT_UNSET_FOCUSABLE, unset_focusable, flipselector);
	register_request_listener(EVENT_SELECT, select_flipselector, flipselector);

	register_request_listener(ACTION_NEXT, register_action_next, flipselector);
	register_request_listener(ACTION_PREVIOUS, register_action_previous, flipselector);

	register_request_listener(EVENT_HIDE, hide, flipselector);
	register_request_listener(EVENT_SHOW, show, flipselector);

	evas_object_resize(win, 240, 60);
	evas_object_resize(flipselector, 120, 30);
	evas_object_move(flipselector, 60, 15);

	evas_object_show(win);
	evas_object_show(flipselector);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
