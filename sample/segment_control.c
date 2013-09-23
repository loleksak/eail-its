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
* cc -o segment_control segment_control.c request.c `pkg-config --cflags elementary` -Wall `pkg-config --libs elementary`
*/

#include <Elementary.h>
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"
#include "utils.h"

#define DESCRITPION "Sample segment_control description"
#define NAME "Sample segment_control"

void select_segment_control (void *data) {
	Elm_Object_Item *item = (Elm_Object_Item*) data;
	elm_segment_control_item_selected_set(item, EINA_TRUE);
}

void unselect_segment_control (void *data) {
	Elm_Object_Item *item = (Elm_Object_Item*) data;
	elm_segment_control_item_selected_set(item, EINA_FALSE);
}

void add_children (void *data) {
	Evas_Object *obj = (Evas_Object *) data;
	elm_segment_control_item_add (obj, NULL, "D");
}

void rm_children (void *data) {
	Evas_Object *obj = (Evas_Object *) data;
	elm_segment_control_item_del_at(obj,1);
}

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}


EAPI_MAIN int
elm_main(int argc, char **argv)
 {
	Evas_Object *win = NULL;
	Evas_Object *bg = NULL;
	Evas_Object *box = NULL;
	Evas_Object *segment_control = NULL;

	Elm_Object_Item *item = NULL;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "segment_control");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);
	evas_object_show(bg);

	box = elm_box_add(win);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, box);
	evas_object_show(box);

	segment_control = elm_segment_control_add(win);
	evas_object_show(segment_control);
	elm_box_pack_end(box, segment_control);

	elm_segment_control_item_add (segment_control, NULL, "A");
	item = elm_segment_control_item_add (segment_control, NULL, "B");
	elm_segment_control_item_add (segment_control, NULL, "C");

	evas_object_smart_callback_add(win, "focus,in", _register_cb, segment_control);

	register_request_listener(EVENT_SELECT, select_segment_control, item);
	register_request_listener(EVENT_UNSELECT, unselect_segment_control, item);
	register_request_listener(EVENT_ADD_CHILDREN, add_children, segment_control);
	register_request_listener(EVENT_RM_CHILDREN, rm_children, segment_control);
	register_request_listener(EVENT_HIDE, hide, segment_control);
	register_request_listener(EVENT_SHOW, show, segment_control);
	register_request_listener(EVENT_SET_ENABLED, set_enabled, segment_control);
	register_request_listener(EVENT_SET_DISABLED, set_disabled, segment_control);
	register_request_listener(EVENT_SET_FOCUS, set_focus, segment_control);
	register_request_listener(EVENT_UNSET_FOCUS, unset_focus, segment_control);
	register_request_listener(EVENT_SET_FOCUSABLE, set_focusable, segment_control);
	register_request_listener(EVENT_UNSET_FOCUSABLE, unset_focusable, segment_control);

	evas_object_resize(win, 240, 120);
	evas_object_resize(segment_control, 220, 60);
	evas_object_move(segment_control, 60, 40);

	evas_object_show(win);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()

