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
* cc -o scroller scroller.c request.c `pkg-config --cflags elementary` -Wall `pkg-config --libs elementary`
*/

#include <Elementary.h>
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"
#include "utils.h"

#define DESCRITPION "Sample scroller description"
#define NAME "Sample scroller"

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
	Evas_Object *scroller = NULL;
	Evas_Object *box = NULL;
	Evas_Object *bg = NULL;
	Evas_Object *hlab = NULL;
	Evas_Object *vlab = NULL;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "scroller");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);
	evas_object_show(bg);

	box = elm_box_add(win);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, box);
	elm_box_padding_set(box, 10, 10);
	evas_object_show(box);

	scroller = elm_scroller_add(win);
	elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_ON, ELM_SCROLLER_POLICY_ON);
	evas_object_size_hint_weight_set(scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, scroller);
	evas_object_show(scroller);

	hlab = elm_label_add(win);
	elm_object_text_set(hlab, "This is very long content of the scroller - horizontal label");
	elm_box_pack_start(box, hlab);
	evas_object_show(hlab);

	vlab = elm_label_add(win);
	elm_object_text_set(vlab, "VERTICAL");
	elm_label_wrap_width_set(vlab, 10);
	elm_label_line_wrap_set(vlab, ELM_WRAP_MIXED);
	elm_box_pack_end(box, vlab);
	evas_object_show(vlab);

	elm_object_content_set(scroller, box);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, scroller);

	register_request_listener(EVENT_SET_FOCUSABLE, set_focusable, scroller);
	register_request_listener(EVENT_UNSET_FOCUSABLE, unset_focusable, scroller);
	register_request_listener(EVENT_SET_FOCUS, set_focus, scroller);
	register_request_listener(EVENT_UNSET_FOCUS, unset_focus, scroller);

	register_request_listener(ACTION_SCROLL_LEFT, register_action_scroll_left, scroller);
	register_request_listener(ACTION_SCROLL_RIGHT, register_action_scroll_right, scroller);
	register_request_listener(ACTION_SCROLL_UP, register_action_scroll_up, scroller);
	register_request_listener(ACTION_SCROLL_DOWN, register_action_scroll_down, scroller);

	register_request_listener(EVENT_HIDE, hide, scroller);
	register_request_listener(EVENT_SHOW, show, scroller);

	evas_object_resize(win, 240, 60);
	evas_object_show(box);
	evas_object_show(win);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
