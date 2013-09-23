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
* gcc -o hoversel hoversel.c request.c `pkg-config --cflags --libs elementary`
*/

#include <Elementary.h>
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"
#include "utils.h"

#define DESCRITPION "Sample hoversel description"
#define NAME "Sample Hoversel"

void set_horizontal(void *data) {
	Evas_Object *obj = (Evas_Object*) data;
	elm_hoversel_horizontal_set(obj, EINA_TRUE);
}

void unset_horizontal(void *data) {
	Evas_Object *obj = (Evas_Object*) data;
	elm_hoversel_horizontal_set(obj, EINA_FALSE);
}

void add_children(void *data) {
	Evas_Object *obj = (Evas_Object*) data;
	elm_hoversel_item_add(obj, "c", NULL, ELM_ICON_NONE, NULL, NULL);
}

void rm_all_children(void *data)
 {
	Evas_Object *obj = (Evas_Object*) data;
	elm_hoversel_clear(obj);
}

void expand(void *data) {
	Evas_Object *obj = (Evas_Object*) data;
	elm_hoversel_hover_begin(obj);
}

void shrink(void *data) {
	Evas_Object *obj = (Evas_Object*) data;
	elm_hoversel_hover_end(obj);
}

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

Eina_Bool _expand_cb (void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	if(EINA_TRUE == elm_hoversel_expanded_get (obj))
		send_response(SUCCESS);
	return EINA_FALSE;
}

void register_action_shrink(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	elm_hoversel_hover_begin(obj);
	evas_object_smart_callback_add(obj, "dismissed", _response_cb, NULL);
}

void register_action_expand(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	elm_hoversel_hover_end(obj);
	ecore_timer_add(1.0, _expand_cb, obj);
}

EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *hoversel = NULL;

	win = elm_win_add(NULL, "Hoversel sample", ELM_WIN_BASIC);
	elm_win_title_set(win, "Hoversel sample");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	hoversel = elm_hoversel_add(win);
	elm_object_text_set(hoversel, "Hoversel");

	elm_hoversel_item_add(hoversel, "a", NULL, ELM_ICON_NONE, NULL, NULL);
	elm_hoversel_item_add(hoversel, "b", NULL, ELM_ICON_NONE, NULL, NULL);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, hoversel);

	register_request_listener(EVENT_SET_FOCUS, set_focus, hoversel);
	register_request_listener(EVENT_UNSET_FOCUS, unset_focus, hoversel);
	register_request_listener(EVENT_SET_FOCUSABLE, set_focusable, hoversel);
	register_request_listener(EVENT_UNSET_FOCUSABLE, unset_focusable, hoversel);
	register_request_listener(EVENT_ADD_CHILDREN, add_children, hoversel);
	register_request_listener(EVENT_RM_ALL_CHILDREN, rm_all_children, hoversel);
	register_request_listener(EVENT_SET_DISABLED, set_disabled, hoversel);
	register_request_listener(EVENT_SET_ENABLED, set_enabled, hoversel);
	register_request_listener(EVENT_SET_HORIZONTAL, set_horizontal, hoversel);
	register_request_listener(EVENT_UNSET_HORIZONTAL, unset_horizontal, hoversel);
	register_request_listener(EVENT_HIDE, hide, hoversel);
	register_request_listener(EVENT_SHOW, show, hoversel);
	register_request_listener(EVENT_EXPAND, expand, hoversel);
	register_request_listener(EVENT_SHRINK, shrink, hoversel);

	register_request_listener(ACTION_SHRINK, register_action_shrink, hoversel);
	register_request_listener(ACTION_EXPAND, register_action_expand, hoversel);

	evas_object_resize(win, 240, 240);
	evas_object_resize(hoversel, 120, 45);
	evas_object_move(hoversel, 60, 40);

	evas_object_show(win);
	evas_object_show(hoversel);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
