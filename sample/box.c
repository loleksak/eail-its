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
* cc -o box box.c request.c `pkg-config --cflags --libs elementary`
*/

#include "utils.h"
#include <Elementary.h>
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"

#define DESCRITPION "Sample box description"
#define NAME "Sample box"

Evas_Object *win = NULL;
Evas_Object *box = NULL;

void add_children( void *data) {
	Evas_Object *box = (Evas_Object*) data;
	Evas_Object *label = NULL;
	label = elm_label_add(win);
	elm_object_text_set(label, "label 3");
	elm_box_pack_end(box,label);
	evas_object_show(label);
}

void rm_children( void *data) {
	Evas_Object *obj = (Evas_Object*) data;
	evas_object_del(obj);
}

void set_horizontal(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_box_horizontal_set(obj, EINA_TRUE);
}

void set_vertical(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_box_horizontal_set(obj, EINA_FALSE);
}

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}

void unparent(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_table_unpack(obj, box);
}

void parent_changed(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_table_pack(obj, box , 1, 1, 1, 1);
}


EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *label = NULL;
	Evas_Object *label2 = NULL;
	Evas_Object *button = NULL;
	Evas_Object *table, *table1 = NULL;
	Evas_Object *bg, *bg1, *bg2 = NULL;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "Main Window");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);

	table = elm_table_add(win);
	evas_object_size_hint_weight_set(table, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, table);
	evas_object_show(table);

	box = elm_box_add(table);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_resize(box, 80, 40);
	evas_object_show(box);
	elm_table_pack(table, box, 1, 1, 1, 1);

	table1 = elm_table_add(win);
	evas_object_size_hint_weight_set(table1, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_resize(table1, 150, 120);
	evas_object_show(table1);

	bg1 = elm_bg_add(table);
	elm_bg_color_set(bg1, 151, 2, 221);
	evas_object_resize(bg1, 50, 60);
	evas_object_move(bg1, 100, 100);
	evas_object_show(bg1);

	bg2 = elm_bg_add(table1);
	elm_bg_color_set(bg2, 120, 12, 80);
	evas_object_resize(bg2, 50, 60);
	evas_object_move(bg2, 20, 10);
	evas_object_show(bg2);

	evas_object_show(bg);

	label = elm_label_add(win);
	elm_object_text_set(label, "label 1");
	elm_box_pack_end(box,label);
	evas_object_show(label);

	label2 = elm_label_add(win);
	elm_object_text_set(label2, "label 2");
	elm_box_pack_end(box,label2);
	evas_object_show(label2);

	button = elm_button_add(win);
	elm_object_text_set(button, "Button");
	elm_box_pack_end(box, button);
	evas_object_show(button);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, box);

	register_request_listener(EVENT_HIDE, hide, box);
	register_request_listener(EVENT_SHOW, show, box);
	register_request_listener(EVENT_SET_HORIZONTAL, set_horizontal, box);
	register_request_listener(EVENT_SET_VERTICAL, set_vertical, box);
	register_request_listener(EVENT_ADD_CHILDREN, add_children,box);
	register_request_listener(EVENT_RM_CHILDREN, rm_children,label2);
	register_request_listener(EVENT_UNPARENT, unparent, table);
	register_request_listener(EVENT_PARENT_CHANGED, parent_changed, table1);

	evas_object_resize(win, 300, 100);
	evas_object_show(win);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
