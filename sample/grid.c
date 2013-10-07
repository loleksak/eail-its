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
* cc -o grid grid.c `pkg-config --cflags elementary` -Wall `pkg-config --libs elementary`
*/

#include "utils.h"
#include <Elementary.h>
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"

#define DESCRITPION "Sample grid description"
#define NAME "Sample grid"

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}


EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *bg = NULL;
	Evas_Object *grid = NULL;

	Evas_Object *label1 = NULL;
	Evas_Object *label2 = NULL;
	Evas_Object *label3 = NULL;
	Evas_Object *label4 = NULL;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "grid");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);

	grid = elm_grid_add(win);
	evas_object_size_hint_weight_set(grid, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, grid);

	label1 = elm_label_add(win);
	label2 = elm_label_add(win);
	label3 = elm_label_add(win);
	label4 = elm_label_add(win);

	elm_object_text_set(label1, "Label1");
	elm_object_text_set(label2, "Label2");
	elm_object_text_set(label3, "Label3");
	elm_object_text_set(label4, "Label4");

	elm_grid_pack (grid, label1, 0, 0, 20, 20);
	elm_grid_pack (grid, label2, 20, 0, 40, 20);
	elm_grid_pack (grid, label3, 0, 20, 20, 40);
	elm_grid_pack (grid, label4, 20, 20, 40, 40);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, grid);

	register_request_listener(EVENT_SHOW, show, grid);
	register_request_listener(EVENT_HIDE, hide, grid);
	register_request_listener(EVENT_SET_ENABLED, set_enabled, grid);
	register_request_listener(EVENT_SET_DISABLED, set_disabled, grid);

	evas_object_resize(win, 240, 120);

	evas_object_show(win);
	evas_object_show(bg);
	evas_object_show(grid);
	evas_object_show(label1);
	evas_object_show(label2);
	evas_object_show(label3);
	evas_object_show(label4);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
