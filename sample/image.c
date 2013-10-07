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

#define DESCRITPION "Sample Image description"
#define NAME "Sample Image"

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}

void resize(void *data) {
	Evas_Object *obj = (Evas_Object*) data;
	evas_object_resize(obj, 60, 120);
}

void set_animated(void *data) {
	Evas_Object *obj = (Evas_Object*) data;
	if (elm_image_animated_available_get(obj)) {
		elm_image_animated_set(obj, EINA_TRUE);
		elm_image_animated_play_set(obj, EINA_TRUE);
	}
}

void unset_animated(void *data) {
	Evas_Object *obj = (Evas_Object*) data;
	if (elm_image_animated_available_get(obj)) {
		elm_image_animated_set(obj, EINA_FALSE);
		elm_image_animated_play_set(obj, EINA_FALSE);
	}
}

EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *image = NULL;
	Evas_Object *bg = NULL;
	char *img_file = NULL;

	win = elm_win_add(NULL, "Image sample", ELM_WIN_BASIC);
	elm_win_title_set(win, "Image sample");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	bg = elm_bg_add(win);
	elm_bg_color_set(bg, 255,255 ,255);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);
	evas_object_show(bg);

	image = elm_image_add(win);
	img_file = get_file(argv[0], "lena.gif");
	elm_image_file_set(image, img_file, NULL);
	evas_object_resize(image, 120, 100);
	evas_object_show(image);

	request_init();

	evas_object_smart_callback_add(win, "focus,in", _register_cb, image);

	register_request_listener(EVENT_RESIZE, resize, image);
	register_request_listener(EVENT_SET_DISABLED, set_disabled, image);
	register_request_listener(EVENT_SET_ENABLED, set_enabled, image);
	register_request_listener(EVENT_HIDE, hide, image);
	register_request_listener(EVENT_SHOW, show, image);
	register_request_listener(EVENT_CLICK, click, image);
	register_request_listener(EVENT_SET_ANIMATED, set_animated, image);
	register_request_listener(EVENT_UNSET_ANIMATED, unset_animated, image);

	register_request_listener(ACTION_CLICK, register_action_click, image);

	evas_object_resize(win, 240, 200);

	evas_object_show(win);

	elm_run();
	request_shutdown();
	elm_shutdown();
	free(img_file);
	return 0;
}
ELM_MAIN()
