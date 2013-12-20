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

#define DESCRITPION "Sample bubble description"
#define NAME "Sample bubble"

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

void register_action_click(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	evas_object_smart_callback_add(obj, "clicked", _response_cb, NULL);
}

EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *bubble = NULL;
	Evas_Object *bg = NULL;
	Evas_Object *label = NULL;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "Sample bubble app");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
	evas_object_resize(win, 300, 200);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);
	evas_object_show(bg);

	label = elm_label_add(win);
	elm_object_text_set(label, "Content in bubble");
	evas_object_show(label);

	bubble = elm_bubble_add(win);
	elm_win_resize_object_add(win, bubble);
	elm_object_content_set(bubble, label);
	evas_object_show(bubble);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, bubble);

	register_request_listener(EVENT_HIDE, hide, bubble);
	register_request_listener(EVENT_SHOW, show, bubble);
	register_request_listener(ACTION_CLICK, register_action_click, bubble);

	evas_object_show(win);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
