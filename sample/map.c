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
* cc -o map map.c request.c `pkg-config --cflags --libs elementary`
*/
#include <Elementary.h>
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"

#define DESCRITPION "Sample map description"
#define NAME "Sample map"

Evas_Object *win = NULL;
static int zoom;

void add_children(void *data) {
	Evas_Object *map = (Evas_Object*) data;
	Evas_Object *icon = NULL;
	Elm_Map_Overlay *item =  NULL;
	icon = elm_icon_add(win);
	elm_object_text_set(icon, "Icon2");
	elm_icon_standard_set(icon, "home");
	item = elm_map_overlay_add(map,-53.276369,-9.968851);
	elm_map_overlay_icon_set(item, icon);
}

void rm_children( void *data) {
	Elm_Map_Overlay *item = (Elm_Map_Overlay*) data;
	elm_map_overlay_del(item);
}

void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}

void _zoom_in_response_cb (void *data, Evas_Object *obj, void *event_info) {
	if( zoom < elm_map_zoom_get(obj))
		send_response(SUCCESS);
}

void _zoom_out_response_cb (void *data, Evas_Object *obj, void *event_info) {
	if( zoom > elm_map_zoom_get(obj))
		send_response(SUCCESS);
}

void register_action_zoom_in(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	zoom = elm_map_zoom_get(obj);
	evas_object_smart_callback_add(obj, "zoom,stop", _zoom_in_response_cb, NULL);
}

void register_action_zoom_out(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	zoom = elm_map_zoom_get(obj);
	evas_object_smart_callback_add(obj, "zoom,stop", _zoom_out_response_cb, NULL);
}

void _response_cb (void *data, Evas_Object *obj, void *event_info) {
	send_response(SUCCESS);
}

void register_action_press(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	evas_object_smart_callback_add(obj, "press", _response_cb, NULL);
}

EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *map = NULL;
	Evas_Object *bg = NULL;
	Evas_Object *icon = NULL;
	Elm_Map_Overlay *item =  NULL;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "Hello, My World Map!");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);
	evas_object_show(bg);

	map = elm_map_add(win);
	evas_object_size_hint_weight_set(map, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, map);
	evas_object_show(map);

	icon = elm_icon_add(win);
	elm_object_text_set(icon, "Icon1");
	elm_icon_standard_set(icon, "home");

	item = elm_map_overlay_add(map,21.009865, 52.224855);
	elm_map_overlay_icon_set(item, icon);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, map);

	register_request_listener(EVENT_ADD_CHILDREN, add_children,map);
	register_request_listener(EVENT_RM_CHILDREN, rm_children,item);

	register_request_listener(ACTION_ZOOM_IN, register_action_zoom_in, map);
	register_request_listener(ACTION_ZOOM_OUT, register_action_zoom_out, map);
	register_request_listener(ACTION_SCROLL_LEFT, register_action_scroll_left, map);
	register_request_listener(ACTION_SCROLL_RIGHT, register_action_scroll_right, map);
	register_request_listener(ACTION_SCROLL_UP, register_action_scroll_up, map);
	register_request_listener(ACTION_SCROLL_DOWN, register_action_scroll_down, map);
	register_request_listener(ACTION_CLICK, register_action_click, map);
	register_request_listener(ACTION_PRESS, register_action_press, map);

	elm_map_zoom_set(map, 2);

	evas_object_resize(win, 800, 800);
	evas_object_show(win);

	elm_run();
	elm_shutdown();
	request_shutdown();
	return 0;
}
ELM_MAIN()
