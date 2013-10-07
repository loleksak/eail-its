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
* cc -o list list.c request.c `pkg-config --cflags --libs elementary`
*/

#include "utils.h"
#include <Elementary.h>
#include <atk/atk.h>
#include <eail_factory.h>
#include "eail_list.h"
#include "request.h"

#define DESCRITPION "Sample list description"
#define NAME "Sample list"

#define DESCRITPION_ITEM "Sample list item description"
#define NAME_ITEM "Sample list item"

Evas_Object *list = NULL;
static int tmp_scroll_x;
static int tmp_scroll_y;


void set_horizontal(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_list_horizontal_set(obj,EINA_TRUE);
}

void unset_horizontal(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_list_horizontal_set(obj,EINA_FALSE);
}

void add_children( void *data) {
	Evas_Object *list = (Evas_Object*) data;
	elm_list_item_append(list, "ELEM 4", NULL, NULL, NULL, NULL);
	elm_list_go(list);
}

void rm_children( void *data) {
	Evas_Object *list = (Evas_Object*) data;
	Elm_Object_Item *item = elm_list_first_item_get(list);
	elm_object_item_del(item);
	elm_list_go(list);
}

void rm_all_children( void *data) {
	Evas_Object* list = (Evas_Object*) data;
	elm_list_clear(list);
	elm_list_go(list);
}

void select_list(void *data) {
	Elm_Object_Item *obj = (Elm_Object_Item *)data;
	elm_list_item_selected_set(obj, EINA_TRUE);
}

void unselect_list(void *data) {
	Elm_Object_Item *obj = (Elm_Object_Item *)data;
	elm_list_item_selected_set(obj, EINA_FALSE);
}

void multiselect_list(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	elm_list_multi_select_set(obj, EINA_TRUE);
	Elm_Object_Item * first_item = elm_list_first_item_get(obj);
	elm_list_item_selected_set(first_item, EINA_TRUE);
	elm_list_item_selected_set(elm_list_item_next(first_item), EINA_TRUE);
}

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj, *obj_item;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
	obj_item = atk_object_ref_accessible_child(obj,0);
	atk_object_set_description(obj_item, DESCRITPION_ITEM);
	atk_object_set_name (obj_item, NAME_ITEM);
}

Eina_Bool _scroll_left_response_cb(void *data) {
	int x;

	elm_scroller_region_get(list, &x, NULL, NULL, NULL);
	if( tmp_scroll_x > x)
		send_response(SUCCESS);
	return EINA_FALSE;
}

Eina_Bool _scroll_up_response_cb(void *data) {
	int y;

	elm_scroller_region_get(list, NULL, &y, NULL, NULL);
	if( tmp_scroll_y > y)
		send_response(SUCCESS);
	return EINA_FALSE;
}

Eina_Bool _scroll_down_response_cb(void *data) {
	int y;

	elm_scroller_region_get(list, NULL, &y, NULL, NULL);
	if( tmp_scroll_y < y)
		send_response(SUCCESS);
	return EINA_FALSE;
}

Eina_Bool _scroll_right_response_cb(void *data) {
	int x;

	elm_scroller_region_get(list, &x, NULL, NULL, NULL);
	if( tmp_scroll_x < x)
		send_response(SUCCESS);
	return EINA_FALSE;
}

void register_action_scroll_left(void *data){
	Evas_Object *obj = (Evas_Object *)data;
	int y, w, h;

	elm_scroller_region_get(obj, &tmp_scroll_x, &y, &w, &h);
	tmp_scroll_x = tmp_scroll_x + 10;
	elm_scroller_region_show(obj, tmp_scroll_x, y, w, h);
	ecore_timer_add(1.0, _scroll_left_response_cb, NULL);
}

void register_action_scroll_right(void *data){
	Evas_Object *obj = (Evas_Object *)data;

	elm_scroller_region_get(obj, &tmp_scroll_x, NULL, NULL, NULL);
	ecore_timer_add(1.0, _scroll_right_response_cb, NULL);
}

void register_action_scroll_up(void *data){
	int x, w, h;
	Evas_Object *obj = (Evas_Object *)data;

	elm_scroller_region_get(obj, &x, &tmp_scroll_y, &w, &h);
	tmp_scroll_y = tmp_scroll_y + 10;
	elm_scroller_region_show(obj, x, tmp_scroll_y, w, h);
	ecore_timer_add(1.0, _scroll_up_response_cb, NULL);
}

void register_action_scroll_down(void *data){
	int x, w, h;
	Evas_Object *obj = (Evas_Object *)data;

	elm_scroller_region_get(obj, &x, &tmp_scroll_y, &w, &h);
	ecore_timer_add(1.0, _scroll_down_response_cb, NULL);
}

void _on_select_child_cb (void *data, Evas_Object *obj, void *event_info) {
	if(event_info == data)
		send_response(SUCCESS);
}

void select_child_0(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	Elm_Object_Item *item = elm_list_first_item_get(obj);
	evas_object_smart_callback_add(obj, "selected", _on_select_child_cb, item);
}

EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *bg = NULL;

	Elm_Object_Item *list_item = NULL;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "list");
	elm_win_autodel_set(win, EINA_TRUE);
	evas_object_resize(win, 60, 40);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);
	evas_object_show(bg);

	list = elm_list_add(win);
	evas_object_size_hint_weight_set(list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, list);
	evas_object_show(list);

	list_item = elm_list_item_append(list, "ELEM 1", NULL, NULL, NULL, NULL);

	elm_list_item_append(list, "ELEM 2", NULL, NULL, NULL, NULL);
	elm_list_item_append(list, "ELEM 3", NULL, NULL, NULL, NULL);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, list);

	register_request_listener(EVENT_SELECT, select_list, list_item);
	register_request_listener(EVENT_UNSELECT, unselect_list, list_item);
	register_request_listener(EVENT_MULTISELECT, multiselect_list, list);
	register_request_listener(EVENT_ADD_CHILDREN, add_children,list);
	register_request_listener(EVENT_RM_CHILDREN, rm_children,list);
	register_request_listener(EVENT_RM_ALL_CHILDREN, rm_all_children,list);
	register_request_listener(EVENT_SET_DISABLED, set_disabled, list);
	register_request_listener(EVENT_SET_ENABLED, set_enabled, list);
	register_request_listener(EVENT_SET_HORIZONTAL, set_horizontal, list);
	register_request_listener(EVENT_UNSET_HORIZONTAL, unset_horizontal, list);
	register_request_listener(EVENT_HIDE, hide, list);
	register_request_listener(EVENT_SHOW, show, list);

	register_request_listener(ACTION_CLICK, select_child_0, list);
	register_request_listener(ACTION_SCROLL_LEFT, register_action_scroll_left, list);
	register_request_listener(ACTION_SCROLL_RIGHT, register_action_scroll_right, list);
	register_request_listener(ACTION_SCROLL_UP, register_action_scroll_up, list);
	register_request_listener(ACTION_SCROLL_DOWN, register_action_scroll_down, list);

	evas_object_show(win);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
