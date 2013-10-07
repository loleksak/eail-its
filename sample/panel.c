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

#define DESCRITPION "Sample panel description"
#define NAME "Sample panel"

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}

static Eina_Bool hidden = EINA_FALSE;
Eina_Bool _toggle_response_cb (void *data) {
	if(elm_panel_hidden_get((Evas_Object *)data) != hidden)
		send_response(SUCCESS);
	return EINA_FALSE;
}

void register_action_toggle(void *data) {
	hidden = elm_panel_hidden_get((Evas_Object *)data);
	ecore_timer_add(1, _toggle_response_cb, data);
}

EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *panel = NULL;

	win = elm_win_add(NULL, "Panel sample", ELM_WIN_BASIC);
	elm_win_title_set(win, "Panel sample");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	panel = elm_panel_add(win);
	evas_object_show(panel);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, panel);

	register_request_listener(EVENT_SET_ENABLED, set_enabled, panel);
	register_request_listener(EVENT_SET_DISABLED, set_disabled, panel);
	register_request_listener(EVENT_HIDE, hide, panel);
	register_request_listener(EVENT_SHOW, show, panel);

	register_request_listener(ACTION_TOGGLE, register_action_toggle, panel);

	evas_object_resize(win, 240, 150);
	evas_object_resize(panel, 120, 50);

	evas_object_show(win);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
