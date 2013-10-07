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
* gcc -o actionslider actionslider.c request.c `pkg-config --cflags --libs elementary`
*/

#include <Elementary.h>
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"
#include "utils.h"

#define DESCRITPION "Sample actionslider description"
#define NAME "Sample actionslider"

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}

void set_value(void *data) {
	Evas_Object *obj = (Evas_Object*) data;
	elm_actionslider_enabled_pos_set(obj,ELM_ACTIONSLIDER_RIGHT);
	evas_object_smart_callback_call(obj, "pos_changed", NULL);
}


EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *actionslider = NULL;

	win = elm_win_add(NULL, "Actionslider sample", ELM_WIN_BASIC);
	elm_win_title_set(win, "Actionslider sample");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	actionslider = elm_actionslider_add(win);
	elm_object_text_set(actionslider, "Slide!");

	evas_object_resize(win, 240, 60);
	evas_object_resize(actionslider, 120, 30);
	evas_object_move(actionslider, 60, 15);

	evas_object_show(win);
	evas_object_show(actionslider);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, actionslider);

	register_request_listener(EVENT_SHOW, show, actionslider);
	register_request_listener(EVENT_HIDE, hide, actionslider);
	register_request_listener(EVENT_SET_DISABLED, set_disabled, actionslider);
	register_request_listener(EVENT_SET_ENABLED, set_enabled, actionslider);
	register_request_listener(EVENT_PRESS, press, actionslider);
	register_request_listener(EVENT_SET_VALUE, set_value, actionslider);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
