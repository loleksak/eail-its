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
* cc -o menu menu.c `pkg-config --cflags elementary` -Wall `pkg-config --libs elementary`
*/

#include "utils.h"
#include <Elementary.h>
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"

#define DESCRITPION "Sample menu description"
#define NAME "Sample menu"

#define DESCRITPION_ITEM "Sample menu item description"
#define NAME_ITEM "Sample menu item"

Evas_Object *box2 = NULL;

void add_children(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_menu_item_add (obj, NULL, NULL, "THE NEW ITEM", NULL, NULL);
}

void rm_children(void *data) {
	Elm_Object_Item * obj = (Elm_Object_Item *) data;
	elm_object_item_del(obj);
}

void parent_changed(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_menu_parent_set(obj, box2);
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

void _on_select_child_cb (void *data, Evas_Object *win, void *event_info) {
	send_response(SUCCESS);
}

EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *bg = NULL;
	Evas_Object *box = NULL;
	Evas_Object *menu = NULL;

	Elm_Object_Item * item;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "Menu App");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
	evas_object_show(win);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);
	evas_object_show(bg);

	box = elm_box_add(win);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, box);
	evas_object_show(box);

	box2 = elm_box_add(win);
	evas_object_size_hint_weight_set(box2, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, box2);

	menu = elm_menu_add(win);
	elm_box_pack_end(box, menu);
	evas_object_show(menu);

	item = elm_menu_item_add (menu, NULL, "home", "THE ATK ITEM", _on_select_child_cb, NULL);
	elm_menu_item_add (menu, NULL, NULL, "THE ITEM", NULL, NULL);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, menu);

	register_request_listener(EVENT_PARENT_CHANGED, parent_changed, menu);
	register_request_listener(EVENT_ADD_CHILDREN, add_children, menu);
	register_request_listener(EVENT_RM_CHILDREN, rm_children, item);
	register_request_listener(EVENT_SET_ENABLED, set_enabled, menu);
	register_request_listener(EVENT_SET_DISABLED, set_disabled, menu);
	register_request_listener(EVENT_HIDE, hide, menu);
	register_request_listener(EVENT_SHOW, show, menu);

	evas_object_resize(win, 240, 120);
	evas_object_resize(menu, 120, 60);
	evas_object_move(menu, 60, 40);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
