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

#define DESCRITPION "Sample popup description"
#define NAME "Sample popup"

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
	Evas_Object *popup = NULL;

	win = elm_win_add(NULL, "Popup sample", ELM_WIN_BASIC);
	elm_win_title_set(win, "Popup sample app");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	popup = elm_popup_add(win);
	elm_object_text_set(popup, "Popup");

	evas_object_smart_callback_add(win, "focus,in", _register_cb, popup);

	register_request_listener(EVENT_RESIZE, resize, popup);
	register_request_listener(EVENT_SET_ENABLED, set_enabled, popup);
	register_request_listener(EVENT_SET_DISABLED, set_disabled, popup);
	register_request_listener(EVENT_HIDE, hide, popup);
	register_request_listener(EVENT_SHOW, show, popup);
	register_request_listener(EVENT_SET_FOCUS, set_focus, popup);
	register_request_listener(EVENT_UNSET_FOCUS, unset_focus, popup);
	register_request_listener(EVENT_SET_FOCUSABLE, set_focusable, popup);
	register_request_listener(EVENT_UNSET_FOCUSABLE, unset_focusable, popup);

	evas_object_resize(win, 240, 60);
	evas_object_resize(popup, 200, 50);
	evas_object_move(popup, 60, 15);

	evas_object_show(win);
	evas_object_show(popup);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
