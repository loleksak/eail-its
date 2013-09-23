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

#define DESCRITPION "Sample diskselector description"
#define NAME "Sample diskselector"

void resize(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	evas_object_resize(obj, 180, 60);
}

void add_children(void *data) {
	Evas_Object *obj = (Evas_Object *) data;
	elm_diskselector_item_append(obj, "ELEM 3", NULL, NULL, NULL);
}

void rm_children(void *data) {
	Evas_Object *obj = (Evas_Object *) data;
	Elm_Object_Item *item = elm_diskselector_first_item_get(obj);
	elm_object_item_del(item);
}

void select_diskselector(void *data) {
	Evas_Object *obj = (Evas_Object*) data;
	Elm_Object_Item *item = elm_diskselector_last_item_get(obj);
	elm_diskselector_item_selected_set(item, EINA_TRUE);
}

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}

void _response_next_prev_cb(void *data, Evas_Object *obj, void *event_info) {
    if(data == event_info)
        send_response(SUCCESS);
}

void register_action_next(void *data) {
    Evas_Object *obj = (Evas_Object *)data;
    evas_object_smart_callback_add(obj, "selected", _response_next_prev_cb, elm_diskselector_last_item_get(obj));
}

void register_action_previous(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	Elm_Object_Item *item = elm_diskselector_last_item_get(obj);
	elm_diskselector_item_selected_set(item, EINA_TRUE);
	evas_object_smart_callback_add(obj, "selected", _response_next_prev_cb, elm_diskselector_item_prev_get(item));
}

EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *diskselector = NULL;

	win = elm_win_add(NULL, "Diskselector sample", ELM_WIN_BASIC);
	elm_win_title_set(win, "Diskselector sample");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	diskselector = elm_diskselector_add(win);
	elm_object_text_set(diskselector, "Diskselector!");

	elm_diskselector_item_append(diskselector, "ELEM 1", NULL, NULL, NULL);
	elm_diskselector_item_append(diskselector, "ELEM 2", NULL, NULL, NULL);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, diskselector);

	register_request_listener(EVENT_SET_FOCUS, set_focus, diskselector);
	register_request_listener(EVENT_UNSET_FOCUS, unset_focus, diskselector);
	register_request_listener(EVENT_SET_FOCUSABLE, set_focusable, diskselector);
	register_request_listener(EVENT_UNSET_FOCUSABLE, unset_focusable, diskselector);
	register_request_listener(EVENT_RESIZE, resize, diskselector);
	register_request_listener(EVENT_SET_DISABLED, set_disabled, diskselector);
	register_request_listener(EVENT_SET_ENABLED, set_enabled, diskselector);
	register_request_listener(EVENT_HIDE, hide, diskselector);
	register_request_listener(EVENT_SHOW, show, diskselector);
	register_request_listener(EVENT_ADD_CHILDREN, add_children, diskselector);
	register_request_listener(EVENT_RM_CHILDREN, rm_children, diskselector);
	register_request_listener(EVENT_SELECT, select_diskselector,
		diskselector);

	register_request_listener(ACTION_SCROLL_LEFT, register_action_previous, diskselector);
	register_request_listener(ACTION_SCROLL_RIGHT, register_action_next, diskselector);
	register_request_listener(ACTION_NEXT, register_action_next, diskselector);
	register_request_listener(ACTION_PREVIOUS, register_action_previous, diskselector);

	evas_object_resize(win, 240, 60);
	evas_object_resize(diskselector, 120, 30);
	evas_object_move(diskselector, 60, 15);

	evas_object_show(win);
	evas_object_show(diskselector);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
