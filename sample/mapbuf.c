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

#define DESCRITPION "Sample mapbuf description"
#define NAME "Sample mapbuf"

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
	Evas_Object *mapbuf = NULL;
	Evas_Object *box = NULL;
	Evas_Object *bg = NULL;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "Hello, World!");
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

	mapbuf = elm_mapbuf_add(win);
	elm_box_pack_end(box, mapbuf);
	evas_object_show(mapbuf);
	Evas_Object *tb = elm_table_add(win);
	evas_object_size_hint_weight_set(tb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_fill_set(tb, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_show(tb);
	unsigned int i, j;
	for (i = 0; i < 8; i++)	{
		for (j = 0; j < 8; j++)	{
			Evas_Object *ic = elm_icon_add(win);
			elm_icon_standard_set(ic, "clock");
			elm_image_resizable_set(ic, EINA_FALSE, EINA_FALSE);
			evas_object_show(ic);
			elm_table_pack(tb, ic, i, j, 1, 1);
		}
	}
	elm_object_content_set(mapbuf, tb);
	elm_mapbuf_enabled_set(mapbuf, EINA_TRUE);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, mapbuf);

	evas_object_resize(win, 240, 60);
	evas_object_show(box);
	evas_object_show(win);

	elm_run();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
