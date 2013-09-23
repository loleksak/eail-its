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
* gcc -o fileselectorbutton fileselectorbutton.c request.c `pkg-config --cflags --libs elementary`
*/

#include "utils.h"
#include <Elementary.h>
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"

#define DESCRITPION "Sample FileSelectorButton description"
#define NAME "Sample FileSelectorButton"

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}

void resize (void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	evas_object_resize(obj, 100, 100);
}

void _response_cb (void *data, Evas_Object *obj, void *event_info)  {
	send_response(SUCCESS);
}

void register_action_press(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	evas_object_smart_callback_add(obj, "pressed", _response_cb, NULL);
}

void register_action_release(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	evas_object_smart_callback_add(obj, "unpressed", _response_cb, NULL);
}


EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *fs_button = NULL;

	win = elm_win_add(NULL, "FileSelectorButton sample", ELM_WIN_BASIC);
	elm_win_title_set(win, "FileSelectorButton sample");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	fs_button = elm_fileselector_button_add(win);
	elm_object_text_set(fs_button, "FileSelectorButton!");

	evas_object_smart_callback_add(win, "focus,in", _register_cb, fs_button);

	register_request_listener(EVENT_SET_FOCUS, set_focus, fs_button);
	register_request_listener(EVENT_UNSET_FOCUS, unset_focus, fs_button);
	register_request_listener(EVENT_SET_FOCUSABLE, set_focusable, fs_button);
	register_request_listener(EVENT_UNSET_FOCUSABLE, unset_focusable, fs_button);
	register_request_listener(EVENT_RESIZE, resize, fs_button);
	register_request_listener(EVENT_SET_DISABLED, set_disabled, fs_button);
	register_request_listener(EVENT_SET_ENABLED, set_enabled, fs_button);
	register_request_listener(EVENT_HIDE, hide, fs_button);
	register_request_listener(EVENT_SHOW, show, fs_button);
	register_request_listener(EVENT_CLICK, click, fs_button);
	register_request_listener(EVENT_PRESS, press, fs_button);
	register_request_listener(EVENT_RELEASE, release, fs_button);

	register_request_listener(ACTION_CLICK, register_action_click, fs_button);
	register_request_listener(ACTION_PRESS, register_action_press, fs_button);
	register_request_listener(ACTION_RELEASE, register_action_release, fs_button);

	evas_object_resize(win, 240, 60);
	evas_object_resize(fs_button, 120, 30);
	evas_object_move(fs_button, 60, 15);

	evas_object_show(win);
	evas_object_show(fs_button);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
