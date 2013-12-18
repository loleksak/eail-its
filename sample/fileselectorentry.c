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

#include "utils.h"
#include "request.h"

#define DESCRITPION "Sample FileSelectorEntry description"
#define NAME "Sample FileSelectorEntry"

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}

void resize(void *data) {
	Evas_Object *obj = (Evas_Object*) data;
	evas_object_resize(obj, 200, 40);
}


EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *fs_entry = NULL;

	win = elm_win_add(NULL, "FileSelectorEntry sample", ELM_WIN_BASIC);
	elm_win_title_set(win, "FileSelectorEntry sample");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	fs_entry = elm_fileselector_entry_add(win);
	elm_object_text_set(fs_entry, "Select a file");

	request_init();

	evas_object_smart_callback_add(win, "focus,in", _register_cb, fs_entry);

	register_request_listener(EVENT_RESIZE, resize, fs_entry);
	register_request_listener(EVENT_SET_DISABLED, set_disabled, fs_entry);
	register_request_listener(EVENT_SET_ENABLED, set_enabled, fs_entry);
	register_request_listener(EVENT_SET_FOCUS, set_focus, fs_entry);
	register_request_listener(EVENT_UNSET_FOCUS, unset_focus, fs_entry);
	register_request_listener(EVENT_SET_FOCUSABLE, set_focusable, fs_entry);
	register_request_listener(EVENT_UNSET_FOCUSABLE, unset_focusable, fs_entry);
	register_request_listener(EVENT_HIDE, hide, fs_entry);
	register_request_listener(EVENT_SHOW, show, fs_entry);

	register_request_listener(ACTION_CLICK, register_action_click, fs_entry);

	evas_object_resize(win, 240, 80);
	evas_object_resize(fs_entry, 150, 30);
	evas_object_move(fs_entry, 60, 30);

	evas_object_show(win);
	evas_object_show(fs_entry);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
