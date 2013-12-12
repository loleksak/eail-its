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
* cc -o index index.c request.c `pkg-config --cflags --libs elementary`
*/

#include "utils.h"
#include <Elementary.h>
#include "request.h"
#include <atk/atk.h>
#include <eail_factory.h>

#define DESCRITPION "Sample index description"
#define NAME "Sample index"

#define DESCRITPION_ITEM "Sample index item description"
#define NAME_ITEM "Sample index item"

void add_children (void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	elm_index_item_append(obj, "d", NULL, NULL);
	elm_index_level_go(obj, 0);
	elm_index_autohide_disabled_set(obj, EINA_TRUE);
}

void rm_children (void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	Elm_Object_Item *it;
	it = elm_index_selected_item_get(obj,0);
	elm_object_item_del(it);
	elm_index_level_go(obj, 0);
	elm_index_autohide_disabled_set(obj, EINA_TRUE);
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


EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *index = NULL;
	Evas_Object *box = NULL;
	Evas_Object *bg = NULL;
	Evas_Object *list = NULL;
	Elm_Object_Item *index_it;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "Hello, World!");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
	evas_object_resize(win, 200, 500);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);
	evas_object_show(bg);

	box = elm_box_add(win);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, box);
	evas_object_show(box);

	list = elm_list_add(win);
	evas_object_size_hint_weight_set(list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(list, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_show(list);
	elm_box_pack_end(box, list);

	index = elm_index_add(win);
	evas_object_size_hint_weight_set(index, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(index, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_win_resize_object_add(win, index);
	elm_index_autohide_disabled_set(index, EINA_TRUE);

	elm_index_item_append(index, "a", NULL, NULL);
	index_it = elm_index_item_append(index, "b", NULL, NULL);
	elm_index_item_append(index, "c", NULL, NULL);

	elm_index_item_selected_set(index_it, EINA_TRUE);

	int i=0;
	for (i = 0; i < 10; i++) {
		elm_list_item_append(list, "aaaaa", NULL, NULL, NULL, NULL);
	}
	for (i = 0; i < 10; i++) {
		elm_list_item_append(list, "bbbbb", NULL, NULL, NULL, NULL);
	}
	for (i = 0; i < 10; i++) {
		elm_list_item_append(list, "ccccc", NULL, NULL, NULL, NULL);
	}

	elm_index_level_go(index, 0);
	elm_index_indicator_disabled_set(index,EINA_FALSE);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, index);

	register_request_listener(EVENT_ADD_CHILDREN, add_children, index);
	register_request_listener(EVENT_RM_CHILDREN, rm_children, index);
	register_request_listener(EVENT_SHOW, show, index);
	register_request_listener(EVENT_HIDE, hide, index);

	evas_object_show(index);
	evas_object_show(win);
	evas_object_resize(list, 180, 480);
	evas_object_move(list, 20, 20);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
