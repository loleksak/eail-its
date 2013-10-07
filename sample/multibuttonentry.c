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
* gcc -o multibuttonentry multibuttonentry.c request.c `pkg-config --cflags --libs elementary`
*/

#include <Elementary.h>
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"

#define DESCRITPION "Sample multibuttonentry description"
#define NAME "Sample multibuttonentry"

void select_all(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_entry_select_all(obj);
}

void select_none(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_entry_select_none(obj);
}

void text_attributes_changed(void* data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_entry_text_style_user_push(obj, "DEFAULT='font_size=25 color=#FF0000'");
}

void text_insert( void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_entry_entry_insert(obj, "Example text");
}

void text_delete( void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_entry_entry_set(obj, NULL);
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

void register_action_shrink(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	elm_multibuttonentry_expanded_set(obj, EINA_TRUE);
	evas_object_smart_callback_add(obj, "expand,state,changed", _response_cb, NULL);
}

void register_action_expand(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	elm_multibuttonentry_expanded_set(obj, EINA_FALSE);
	evas_object_smart_callback_add(obj, "expanded", _response_cb, NULL);
}


EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *multibuttonentry = NULL;
	Evas_Object *bg = NULL;
	Evas_Object *box = NULL;
	Evas_Object *entry = NULL;

	win = elm_win_add(NULL, "multibuttonentry sample", ELM_WIN_BASIC);
	elm_win_title_set(win, "multibuttonentry sample");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
	evas_object_resize(win, 120, 100);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);

	box = elm_box_add(win);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, box);
	evas_object_show(box);

	multibuttonentry = elm_multibuttonentry_add(win);
	evas_object_size_hint_align_set(multibuttonentry, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_move(multibuttonentry, 100, 80);
	elm_box_pack_end(box, multibuttonentry);
	evas_object_show(multibuttonentry);

	elm_multibuttonentry_item_append(multibuttonentry, "First item", NULL, NULL);
	elm_multibuttonentry_item_append(multibuttonentry, "Second item", NULL, NULL);
	elm_multibuttonentry_item_append(multibuttonentry, "Third item", NULL, NULL);

	entry = elm_multibuttonentry_entry_get(multibuttonentry);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, multibuttonentry);

	register_request_listener(EVENT_TEXT_INSERT, text_insert, entry);
	register_request_listener(EVENT_TEXT_DELETE, text_delete, entry);
	register_request_listener(EVENT_TEXT_SELECT, select_all, entry);
	register_request_listener(EVENT_TEXT_UNSELECT, select_none, entry);
	register_request_listener(EVENT_TEXT_ATTRIBUTES_CHANGED, text_attributes_changed, entry);

	register_request_listener(ACTION_SHRINK, register_action_shrink, multibuttonentry);
	register_request_listener(ACTION_EXPAND, register_action_expand, multibuttonentry);

	evas_object_show(win);
	evas_object_show(bg);
	evas_object_show(multibuttonentry);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
