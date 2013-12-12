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

#define DESCRITPION "Sample entry description"
#define NAME "Sample entry"

void set_single_line(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_entry_single_line_set(obj, EINA_TRUE);
}

void unset_single_line(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_entry_single_line_set(obj, EINA_FALSE);
}

void set_editable(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_entry_single_line_set(obj, EINA_TRUE);
}

void unset_editable(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_entry_single_line_set(obj, EINA_FALSE);
}

void select_all(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_entry_entry_set(obj, "Sample text");
	elm_entry_select_all(obj);
}

void select_none(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_entry_select_none(obj);
}

void text_insert( void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_entry_entry_insert(obj, "example text");
}

void text_delete( void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_entry_entry_set(obj, NULL);
}

void text_attributes_changed(void* data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_entry_entry_set(obj, "Sample text");
	elm_entry_text_style_user_push(obj, "DEFAULT='font_size=25 color=#FF0000'");
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
	Evas_Object *entry = NULL;
	Evas_Object *bg = NULL;
	Evas_Object *box = NULL;

	win = elm_win_add(NULL, "Entry sample", ELM_WIN_BASIC);
	elm_win_title_set(win, "Entry sample");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
	evas_object_resize(win, 200, 100);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);

	box = elm_box_add(win);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, box);
	evas_object_show(box);

	entry = elm_entry_add(win);
	elm_entry_entry_set(entry, "Initial text");
	evas_object_size_hint_align_set(entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_move(entry, 100, 80);
	elm_box_pack_end(box, entry);
	evas_object_show(entry);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, entry);

	register_request_listener(EVENT_SET_DISABLED, set_disabled, entry);
	register_request_listener(EVENT_SET_ENABLED, set_enabled, entry);
	register_request_listener(EVENT_SET_FOCUS, set_focus, entry);
	register_request_listener(EVENT_UNSET_FOCUS, unset_focus, entry);
	register_request_listener(EVENT_SET_FOCUSABLE, set_focusable, entry);
	register_request_listener(EVENT_UNSET_FOCUSABLE, unset_focusable, entry);
	register_request_listener(EVENT_SET_SINGLE_LINE, set_single_line, entry);
	register_request_listener(EVENT_UNSET_SINGLE_LINE, unset_single_line, entry);
	register_request_listener(EVENT_SET_EDITABLE, set_editable, entry);
	register_request_listener(EVENT_UNSET_EDITABLE, unset_editable, entry);
	register_request_listener(EVENT_TEXT_SELECT, select_all, entry);
	register_request_listener(EVENT_TEXT_UNSELECT, select_none, entry);
	register_request_listener(EVENT_TEXT_INSERT, text_insert, entry);
	register_request_listener(EVENT_TEXT_DELETE, text_delete, entry);
	register_request_listener(EVENT_SELECT_ALL, select_all, entry);
	register_request_listener(EVENT_SELECT_NONE, select_none, entry);
	register_request_listener(EVENT_TEXT_ATTRIBUTES_CHANGED, text_attributes_changed, entry);
	register_request_listener(EVENT_TEXT_CARET_MOVE, text_caret_move, entry);
	register_request_listener(EVENT_SHOW, show, entry);
	register_request_listener(EVENT_HIDE, hide, entry);

	evas_object_show(win);
	evas_object_show(bg);
	evas_object_show(entry);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
