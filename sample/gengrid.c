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
* gcc -o gengrid gengrid.c request.c `pkg-config --cflags --libs elementary`
*/

#include "utils.h"
#include <Elementary.h>
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"

#define DESCRITPION "Sample gengrid description"
#define NAME "Sample gengrid"

#define DESCRITPION_ITEM "Sample gengrid item description"
#define NAME_ITEM "Sample gengrid item"


void gengrid_select(void *data) {
	Elm_Object_Item *obj = (Elm_Object_Item *) data;
	elm_gengrid_item_selected_set(obj, EINA_TRUE);
}

void gengrid_unselect(void *data) {
	Elm_Object_Item *obj = (Elm_Object_Item *) data;
	elm_gengrid_item_selected_set(obj, EINA_FALSE);
}

void gengrid_multiselect(void *data) {
	Evas_Object *obj = (Evas_Object *) data;
	elm_gengrid_multi_select_set(obj, EINA_TRUE);
	elm_gengrid_item_selected_set(elm_gengrid_first_item_get(obj), EINA_TRUE);
	elm_gengrid_item_selected_set(elm_gengrid_last_item_get(obj), EINA_TRUE);
}

static char *
_grid_text_get(void *data, Evas_Object *obj, const char  *part) {
	return strdup("item");
}

static Evas_Object *
_grid_content_get(void *data, Evas_Object *obj, const char  *part) {
	if (!strcmp(part, "elm.swallow.icon")) {
		Evas_Object *icon = NULL;
		icon= elm_icon_add(obj);
		elm_icon_standard_set(icon, "home");
		evas_object_size_hint_aspect_set(icon, EVAS_ASPECT_CONTROL_VERTICAL, 1,1);
		evas_object_show(icon);
		return icon;
	}
	return NULL;
}

static void add_children(void *data) {
	Evas_Object *gengrid = (Evas_Object *) data;
	Elm_Gengrid_Item_Class gic;
	gic.item_style = "default";
	gic.func.text_get = _grid_text_get;
	gic.func.content_get =  _grid_content_get;
	gic.func.del = NULL;
	elm_gengrid_item_append(gengrid, &gic, NULL, NULL, NULL);
}

static void rm_children(void *data) {
	Evas_Object *gengrid = (Evas_Object *) data;
	Elm_Object_Item *gitem = elm_gengrid_first_item_get(gengrid);
	elm_object_item_del(gitem);
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

static Eina_Bool pressed = EINA_FALSE;
void _press_cb (void *data, Evas_Object *obj, void *event_info)  {
	if(data == event_info)
		pressed = EINA_TRUE;
}

void _release_cb (void *data, Evas_Object *obj, void *event_info)  {
	if(pressed == EINA_TRUE && data == event_info)
		send_response(SUCCESS);
}

void register_action_click(void *data) {
	Elm_Object_Item *it = (Elm_Object_Item *)data;
	Evas_Object *obj = elm_object_item_widget_get(it);
	evas_object_smart_callback_add(obj, "pressed", _press_cb, data);
	evas_object_smart_callback_add(obj, "released", _release_cb, data);
}

void _response_cb (void *data, Evas_Object *obj, void *event_info)  {
	if(event_info == data)
		send_response(SUCCESS);
}

void register_action_press(void *data) {
	Elm_Object_Item *it = (Elm_Object_Item *)data;
	Evas_Object *obj = elm_object_item_widget_get(it);
	evas_object_smart_callback_add(obj, "pressed", _response_cb, data);
}

void register_action_release(void *data) {
	Elm_Object_Item *it = (Elm_Object_Item *)data;
	Evas_Object *obj = elm_object_item_widget_get(it);
	AtkObject *atk_item;
	atk_item = atk_object_ref_accessible_child(eail_factory_get_accessible(obj),0);
	atk_action_do_action(ATK_ACTION(atk_item), 1);
	evas_object_smart_callback_add(obj, "released", _response_cb, data);
}

EAPI_MAIN int elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *gengrid = NULL;
	Evas_Object *bg = NULL;

	Elm_Object_Item *gengrid_item;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "Gengrid sample");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);
	evas_object_show(bg);

	gengrid = elm_gengrid_add(win);
	evas_object_resize(gengrid, 200, 100);
	elm_gengrid_item_size_set(gengrid, 50, 50);
	evas_object_show(gengrid);
	evas_object_size_hint_weight_set(gengrid, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	Elm_Gengrid_Item_Class gic;
	gic.item_style = "default";
	gic.func.text_get = _grid_text_get;
	gic.func.content_get = _grid_content_get;
	gic.func.del = NULL;

	gengrid_item = elm_gengrid_item_append(gengrid, &gic, NULL, NULL, NULL);
	elm_gengrid_item_append(gengrid, &gic, NULL, NULL, NULL);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, gengrid);

	register_request_listener(EVENT_SET_DISABLED, set_disabled, gengrid);
	register_request_listener(EVENT_SET_ENABLED, set_enabled, gengrid);
	register_request_listener(EVENT_SET_FOCUS, set_focus, gengrid);
	register_request_listener(EVENT_UNSET_FOCUS, unset_focus, gengrid);
	register_request_listener(EVENT_SET_FOCUSABLE, set_focusable, gengrid);
	register_request_listener(EVENT_UNSET_FOCUSABLE, unset_focusable, gengrid);
	register_request_listener(EVENT_SELECT, gengrid_select, gengrid_item);
	register_request_listener(EVENT_UNSELECT, gengrid_unselect, gengrid_item);
	register_request_listener(EVENT_MULTISELECT, gengrid_multiselect, gengrid);
	register_request_listener(EVENT_ADD_CHILDREN, add_children, gengrid);
	register_request_listener(EVENT_RM_CHILDREN, rm_children, gengrid);

	register_request_listener(ACTION_CLICK, register_action_click, gengrid_item);
	register_request_listener(ACTION_PRESS, register_action_press, gengrid_item);
	register_request_listener(ACTION_RELEASE, register_action_release, gengrid_item);

	evas_object_resize(win, 200, 100);
	evas_object_show(win);

	elm_run();
	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
