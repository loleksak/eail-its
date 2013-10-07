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
* gcc -o flip flip.c request.c `pkg-config --cflags --libs elementary`
*/

#include "utils.h"
#include <Elementary.h>
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"

#define DESCRITPION "Sample flip description"
#define NAME "Sample flip"

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}

void _response_cb (void *data, Evas_Object *obj, void *event_info) {
	send_response(SUCCESS);
}

void register_action_flip(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	evas_object_smart_callback_add(obj, "animate,done", _response_cb, NULL);
}


EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win, *bg, *rect, *flip;

	win = elm_win_add(NULL, "Flip sample", ELM_WIN_BASIC);
	elm_win_title_set(win, "Flip sample");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);
	evas_object_show(bg);

	rect = evas_object_rectangle_add(evas_object_evas_get(win));
	evas_object_resize(rect, 100, 100);
	evas_object_color_set(rect, 255, 0, 255, 255);
	evas_object_show(rect);

	flip = elm_flip_add(win);
	elm_object_part_content_set(flip, "front", rect);
	evas_object_resize(flip, 100, 100);
	evas_object_show(flip);

	elm_flip_interaction_set(flip, ELM_FLIP_INTERACTION_PAGE);
	elm_flip_interaction_direction_enabled_set(flip, ELM_FLIP_DIRECTION_LEFT, EINA_TRUE);
	elm_flip_interaction_direction_enabled_set(flip, ELM_FLIP_DIRECTION_RIGHT, EINA_TRUE);
	elm_flip_interaction_direction_hitsize_set(flip, ELM_FLIP_DIRECTION_LEFT, 1);
	elm_flip_interaction_direction_hitsize_set(flip, ELM_FLIP_DIRECTION_RIGHT, 1);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, flip);

	register_request_listener(EVENT_PRESS, press, flip);
	register_request_listener(EVENT_SHOW, show, flip);
	register_request_listener(EVENT_HIDE, hide, flip);

	register_request_listener(ACTION_FLIP, register_action_flip, flip);

	evas_object_resize(win, 200, 200);
	evas_object_show(win);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
