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
* gcc -o toolbar toolbar.c request.c `pkg-config --cflags --libs elementary`
*/
#include <Elementary.h>
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"
#include "utils.h"

#define DESCRITPION "Sample toolbar description"
#define NAME "Sample toolbar"

#define DESCRITPION_ITEM "Sample toolbar item description"
#define NAME_ITEM "Sample toolbar item"

void add_children(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_toolbar_item_append(obj, NULL, "Item 3", NULL, NULL);
}

void rm_children(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	Elm_Object_Item *item = NULL;
	item=elm_toolbar_last_item_get(obj);
	elm_object_item_del(item);
}

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj, *obj_item;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
	obj_item = atk_object_ref_accessible_child(obj,0);
	atk_object_set_description(obj_item, DESCRITPION_ITEM);
	atk_object_set_name (obj_item, NAME_ITEM);
}


void _response_cb (void *data, Evas_Object *obj, void *event_info)  {
	if((event_info == elm_toolbar_first_item_get((Evas_Object *)obj)))
		send_response(SUCCESS);
}

void register_action_click(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	evas_object_smart_callback_add(obj, "clicked", _response_cb, NULL);
}


EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *toolbar = NULL;
	Evas_Object *box = NULL;
	Evas_Object *bg = NULL;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "Sample toolbar item application!");
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

	toolbar = elm_toolbar_add(win);
	elm_toolbar_shrink_mode_set(toolbar, ELM_TOOLBAR_SHRINK_SCROLL);
	evas_object_size_hint_weight_set(toolbar, 0.0, 0.0);
	evas_object_size_hint_align_set(toolbar, EVAS_HINT_FILL, 0.0);
	evas_object_show(toolbar);

	elm_toolbar_item_append(toolbar, NULL, "Item 1", NULL, NULL);
	elm_toolbar_item_append(toolbar, NULL, "Item 2", NULL, NULL);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, toolbar);

	register_request_listener(EVENT_SET_FOCUS, set_focus, toolbar);
	register_request_listener(EVENT_UNSET_FOCUS, unset_focus, toolbar);
	register_request_listener(EVENT_SET_FOCUSABLE, set_focusable, toolbar);
	register_request_listener(EVENT_UNSET_FOCUSABLE, unset_focusable, toolbar);
	register_request_listener(EVENT_ADD_CHILDREN, add_children, toolbar);
	register_request_listener(EVENT_RM_CHILDREN, rm_children, toolbar);
	register_request_listener(EVENT_HIDE, hide, toolbar);
	register_request_listener(EVENT_SHOW, show, toolbar);

	register_request_listener(ACTION_CLICK, register_action_click, toolbar);

	evas_object_resize(win, 240, 60);
	evas_object_show(box);
	elm_box_pack_end(box, toolbar);
	evas_object_show(win);
	evas_object_show(toolbar);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
