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

#define DESCRITPION "Sample Hover description"
#define NAME "Sample Hover"

Evas_Object *box = NULL;

void dismiss(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_hover_dismiss(obj);
}

void target_set(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_hover_target_set(obj, box);
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
	Evas_Object *hover = NULL;
	Evas_Object *bg = NULL;

	win = elm_win_add(NULL, "Hover sample", ELM_WIN_BASIC);
	elm_win_title_set(win, "Hover sample");
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

	hover = elm_hover_add(win);
	elm_hover_parent_set(hover, win);
	elm_object_text_set(hover, "Hover");

	request_init();

	evas_object_smart_callback_add(win, "focus,in", _register_cb, hover);

	register_request_listener(EVENT_HIDE, hide, hover);
	register_request_listener(EVENT_SHOW, show, hover);
	register_request_listener(EVENT_DISMISS, dismiss, hover);
	register_request_listener(EVENT_PARENT_CHANGED, target_set, hover);

	register_request_listener(ACTION_CLICK, register_action_click, hover);

	evas_object_resize(win, 240, 120);
	evas_object_resize(hover, 120, 60);
	evas_object_move(hover, 60, 40);
	evas_object_resize(box, 120, 60);
	evas_object_move(box, 60, 30);

	evas_object_show(win);
	evas_object_show(hover);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
