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
* cc -o ctxpopup ctxpopup.c request.c `pkg-config --cflags elementary` -Wall `pkg-config --libs elementary`
*/
#include <Elementary.h>
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"
#include "utils.h"

#define DESCRITPION "Sample ctxpopup description"
#define NAME "Sample ctxpopup"

Evas_Object *box = NULL;

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}

void set_horizontal(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_panes_horizontal_set(obj, EINA_TRUE);
}

void unset_horizontal(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_panes_horizontal_set(obj, EINA_FALSE);
}

void parent_changed(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_ctxpopup_hover_parent_set(obj, box);
}

void dismiss(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_ctxpopup_dismiss(obj);
}

void _response_cb (void *data, Evas_Object *obj, void *event_info) {
	send_response(SUCCESS);
}

void register_action_dismiss(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	evas_object_smart_callback_add(obj, "dismissed", _response_cb, NULL);
}

EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *bg = NULL;
	Evas_Object *ctxpopup = NULL;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "ctxpopup");
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

	ctxpopup = elm_ctxpopup_add(win);
	Evas_Object *ic = elm_icon_add(ctxpopup);
	elm_icon_standard_set(ic, "home");
	elm_image_resizable_set(ic, EINA_FALSE, EINA_FALSE);
	elm_ctxpopup_item_append(ctxpopup, NULL, ic, NULL, NULL);
	elm_ctxpopup_hover_parent_set(ctxpopup, win);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, ctxpopup);

	register_request_listener(EVENT_SET_FOCUS, set_focus, ctxpopup);
	register_request_listener(EVENT_UNSET_FOCUS, unset_focus, ctxpopup);
	register_request_listener(EVENT_SET_FOCUSABLE, set_focusable, ctxpopup);
	register_request_listener(EVENT_UNSET_FOCUSABLE, unset_focusable, ctxpopup);
	register_request_listener(EVENT_HIDE, hide, ctxpopup);
	register_request_listener(EVENT_SHOW, show, ctxpopup);
	register_request_listener(EVENT_PARENT_CHANGED, parent_changed, ctxpopup);
	register_request_listener(EVENT_SET_HORIZONTAL, set_horizontal, ctxpopup);
	register_request_listener(EVENT_UNSET_HORIZONTAL, unset_horizontal, ctxpopup);
	register_request_listener(EVENT_DISMISS, dismiss, ctxpopup);

	register_request_listener(ACTION_DISMISS, register_action_dismiss, ctxpopup);

	evas_object_resize(win, 240, 120);
	evas_object_resize(ctxpopup, 120, 60);
	evas_object_move(ctxpopup, 60, 40);
	evas_object_resize(box, 120, 60);
	evas_object_move(box, 60, 30);

	evas_object_show(win);
	evas_object_show(ctxpopup);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
