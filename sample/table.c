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
* cc -o table table.c `pkg-config --cflags elementary` -Wall `pkg-config --libs elementary`
*/
#include <Elementary.h>
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"
#include "utils.h"

#define DESCRITPION "Sample table description"
#define NAME "Sample table"

Evas_Object *win = NULL;

void add_children (void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	Evas_Object * button = NULL;
	button = elm_button_add(win);
	evas_object_size_hint_min_set(button, 100, 100);
	evas_object_resize(button, 100, 100);
	elm_table_pack(obj, button, 0, 1, 1, 1);
	evas_object_show(button);
}

void rm_children (void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_table_clear(obj,EINA_TRUE);
}

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}


EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *bg = NULL;
	Evas_Object *table = NULL;

	Evas_Object * button1 = NULL;
	Evas_Object * button2 = NULL;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "table");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);

	table = elm_table_add(win);
	evas_object_size_hint_weight_set(table, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, table);

	button1 = elm_button_add(win);
	button2 = elm_button_add(win);

	evas_object_resize(win, 240, 120);

	elm_table_homogeneous_set(table, EINA_TRUE);

	evas_object_size_hint_min_set(button1, 50, 100);
	evas_object_size_hint_min_set(button2, 100, 100);
	evas_object_resize(button1, 100, 100);
	evas_object_resize(button2, 100, 100);

	elm_table_pack(table, button1, 0, 0, 1, 1);
	elm_table_pack(table, button2, 1, 1, 1, 1);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, table);

	register_request_listener(EVENT_ADD_CHILDREN, add_children, table);
	register_request_listener(EVENT_RM_CHILDREN, rm_children, table);
	register_request_listener(EVENT_HIDE, hide, table);
	register_request_listener(EVENT_SHOW, show, table);

	evas_object_show(win);
	evas_object_show(bg);
	evas_object_show(table);

	evas_object_show(button1);
	evas_object_show(button2);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
