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
* cc -o photo photo.c request.c `pkg-config --cflags elementary` -Wall `pkg-config --libs elementary`
*/
#include <Elementary.h>
#include "utils.h"
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"

#define DESCRITPION "Sample photo description"
#define NAME "Sample photo"

Evas_Object *box = NULL;

void resize(void *data) {
	Evas_Object *obj = (Evas_Object*) data;
	elm_box_unpack(box, obj);
	evas_object_resize(obj, 60, 120);
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
	Evas_Object *win = NULL;
	Evas_Object *bg = NULL;
	Evas_Object *photo = NULL;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "photo");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);

	box = elm_box_add(win);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, box);

	photo = elm_photo_add(win);
	elm_box_pack_end(box, photo);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, photo);

	register_request_listener(EVENT_RESIZE, resize, photo);
	register_request_listener(EVENT_SET_DISABLED, set_disabled, photo);
	register_request_listener(EVENT_SET_ENABLED, set_enabled, photo);
	register_request_listener(EVENT_HIDE, hide, photo);
	register_request_listener(EVENT_SHOW, show, photo);
	register_request_listener(EVENT_CLICK, click, photo);

	register_request_listener(ACTION_CLICK, register_action_click, photo);

	evas_object_resize(win, 240, 120);
	evas_object_resize(photo, 120, 60);
	evas_object_move(photo, 60, 40);
	elm_photo_size_set(photo, 120);

	evas_object_show(win);
	evas_object_show(bg);
	evas_object_show(box);
	evas_object_show(photo);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
