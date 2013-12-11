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
* gcc -o win win.c request.c `pkg-config --cflags --libs elementary`
*/

#include "utils.h"
#include <Elementary.h>
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"

#define DESCRITPION "Sample win description"
#define NAME "Sample Win"

Evas_Object *box = NULL;

void add_children( void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	Evas_Object *label = NULL;
	box = elm_box_add(obj);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(obj, box);
	evas_object_show(box);
	label = elm_label_add(obj);
	elm_object_text_set(label,"Sample label");
	evas_object_show(label);
	elm_box_pack_end(box, label);
}

void rm_children( void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_win_resize_object_del(obj,box);
	evas_object_del(box);
	evas_object_show(obj);
}

void set_maximize(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_win_maximized_set(obj, EINA_TRUE);
}

void unset_maximize(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_win_maximized_set(obj, EINA_FALSE);
}

void set_minimize(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_win_iconified_set(obj, EINA_TRUE);
}

void resize (void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	evas_object_resize(obj, 300, 150);
	evas_object_show(obj);
}

void move (void *data) {
	int x, y;
	Evas_Object* obj = (Evas_Object*) data;
	evas_object_geometry_get(obj, &x, &y, NULL, NULL);
	evas_object_move(obj, x+10, y+10);
	evas_object_show(obj);
}

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}

void _response_cb (void *data, Evas_Object *obj, void *event_info) {
	send_response(SUCCESS);
}

void register_action_maximize(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	evas_object_smart_callback_add(obj, "maximized", _response_cb, NULL);
}

void register_action_minimize(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	evas_object_smart_callback_add(obj, "iconified", _response_cb, NULL);
}

void register_action_restore(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	elm_win_maximized_set(obj, EINA_TRUE);
	evas_object_smart_callback_add(obj, "unmaximized", _response_cb, NULL);
}


EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *label = NULL;
	Evas_Object *bg = NULL;

	win = elm_win_add(NULL, "Win sample", ELM_WIN_BASIC);
	elm_win_title_set(win, "Win sample");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);
	evas_object_show(bg);


	evas_object_smart_callback_add(win, "focus,in", _register_cb, win);

	register_request_listener(EVENT_ADD_CHILDREN, add_children, win);
	register_request_listener(EVENT_RM_CHILDREN, rm_children, label);
	register_request_listener(EVENT_HIDE, hide, win);
	register_request_listener(EVENT_SHOW, show, win);
	register_request_listener(EVENT_SET_ENABLED, set_enabled, win);
	register_request_listener(EVENT_SET_DISABLED, set_disabled, win);
	register_request_listener(EVENT_SET_FOCUS, set_focus, win);
	register_request_listener(EVENT_UNSET_FOCUS, unset_focus, win);
	register_request_listener(EVENT_SET_FOCUSABLE, set_focusable, win);
	register_request_listener(EVENT_UNSET_FOCUSABLE, unset_focusable, win);
	register_request_listener(EVENT_SET_MAXIMIZE, set_maximize, win);
	register_request_listener(EVENT_SET_MINIMIZE, set_minimize, win);
	register_request_listener(EVENT_UNSET_MAXIMIZE, unset_maximize, win);
	register_request_listener(EVENT_RESIZE, resize, win);
	register_request_listener(EVENT_MOVE, move, win);

	register_request_listener(ACTION_MAXIMIZE, register_action_maximize, win);
	register_request_listener(ACTION_MINIMIZE, register_action_minimize, win);
	register_request_listener(ACTION_RESTORE, register_action_restore, win);

	evas_object_resize(win, 240, 100);

	evas_object_show(win);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
