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
* gcc -o thumbnail thumbnail.c request.c `pkg-config --cflags --libs elementary`
*/
#include <Elementary.h>
#include "utils.h"
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"

#define DESCRITPION "Sample Thumbnail description"
#define NAME "Sample Thumbnail"

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}

void resize(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	evas_object_resize(obj,100,100);
}

EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *bg = NULL;
	Evas_Object *thumb = NULL;
	char *img_file = NULL;

	elm_need_ethumb();

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);
	evas_object_show(bg);

	thumb = elm_thumb_add(win);
	elm_thumb_editable_set(thumb, EINA_FALSE);
	img_file = get_file(argv[0], "lena2.jpg");
	elm_thumb_file_set(thumb, img_file, NULL);
	elm_thumb_reload(thumb);
	evas_object_show(thumb);

	evas_object_size_hint_weight_set(thumb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, thumb);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, thumb);

	register_request_listener(EVENT_RESIZE, resize, thumb);
	register_request_listener(EVENT_CLICK, click, thumb);

	register_request_listener(ACTION_CLICK, register_action_click, thumb);

	evas_object_resize(win, 160, 160);
	evas_object_show(win);

	elm_run();
	request_shutdown();
	elm_shutdown();
	free(img_file);
	return 0;
}
ELM_MAIN()
