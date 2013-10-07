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
* gcc -o genlist genlist.c request.c `pkg-config --cflags --libs elementary`
*/

#define _GNU_SOURCE

#include "utils.h"
#include <Elementary.h>
#include <atk/atk.h>
#include <eail_factory.h>
#include <stdio.h>
#include "request.h"

#define DESCRITPION "Sample genlist description"
#define NAME "Sample genlist"

#define DESCRITPION_ITEM "Sample genlist item description"
#define NAME_ITEM "Sample genlist item"

char* item_label_get(void *data, Evas_Object *obj, const char *part) {
	char* buf;
	asprintf(&buf, "Item # %i", (int)(long)data);
	return buf;
}

void select_genlist(void *data) {
	Elm_Object_Item *obj = (Elm_Object_Item *)data;
	elm_genlist_item_selected_set(obj, EINA_TRUE);
}

void unselect_genlist(void *data) {
	Elm_Object_Item *obj = (Elm_Object_Item *)data;
	elm_genlist_item_selected_set(obj, EINA_FALSE);
}

void multiselect_unselect_genlist(void *data) {
	Elm_Object_Item *obj = (Elm_Object_Item *)data;
	elm_genlist_item_selected_set(obj, EINA_FALSE);
	elm_genlist_item_selected_set(elm_genlist_item_next_get(obj), EINA_FALSE);
}

void multiselect_genlist(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	Elm_Object_Item * first_item = elm_genlist_first_item_get(obj);
	elm_genlist_multi_select_set(obj, EINA_TRUE);
	elm_genlist_item_selected_set(first_item, EINA_TRUE);
	elm_genlist_item_selected_set(elm_genlist_item_next_get(first_item), EINA_TRUE);
}

void add_children(void *data) {
	Evas_Object *genlist = (Evas_Object *) data;
	Elm_Genlist_Item_Class *gic = NULL;
	gic = elm_genlist_item_class_new();
	gic->item_style = "default";
	gic->func.text_get = item_label_get;
	gic->func.content_get = NULL;
	gic->func.state_get = NULL;
	gic->func.del = NULL;
	elm_genlist_item_append(genlist, gic, (void *)(long)5,
				NULL, ELM_GENLIST_ITEM_GROUP, NULL, NULL);
	elm_genlist_item_class_free(gic);
}

void rm_children(void *data) {
	Evas_Object *genlist = (Evas_Object *) data;
	Elm_Object_Item *item = elm_genlist_first_item_get(genlist);
	elm_object_item_del(item);
}

char* item_label_get(void *data, Evas_Object *obj, const char *part);

void _register_cb(void *data, Evas_Object *win, void *event_info)
 {
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

void _press_cb (void *data, Evas_Object *obj, void *event_info) {
	if(data == event_info)
		pressed = EINA_TRUE;
}

void _release_cb (void *data, Evas_Object *obj, void *event_info) {
	if(pressed == EINA_TRUE && data == event_info)
		send_response(SUCCESS);
}

void register_action_click(void *data) {
	Elm_Object_Item *it = (Elm_Object_Item *)data;
	Evas_Object *obj = elm_object_item_widget_get(it);
	evas_object_smart_callback_add(obj, "pressed", _press_cb, data);
	evas_object_smart_callback_add(obj, "released", _release_cb, data);
}

void _response_cb (void *data, Evas_Object *obj, void *event_info) {
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


EAPI_MAIN int
elm_main(int argc, char **argv) {

	Evas_Object *win = NULL;
	Evas_Object *genlist = NULL;
	Evas_Object *box = NULL;
	Evas_Object *bg = NULL;
	Elm_Genlist_Item_Class *genitem = NULL;
	Elm_Object_Item * testitem;

	win = elm_win_add(NULL, "Genlist sample", ELM_WIN_BASIC);
	elm_win_title_set(win, "Genlist sample");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
	evas_object_resize(win, 100, 50);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);
	evas_object_show(bg);

	box = elm_box_add(win);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_win_resize_object_add(win, box);
	evas_object_show(box);

	genlist = elm_genlist_add(win);
	evas_object_size_hint_weight_set(genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_box_pack_end(box, genlist);
	evas_object_show(genlist);

	genitem = elm_genlist_item_class_new();
	genitem->item_style = "default";
	genitem->func.text_get = item_label_get;
	genitem->func.content_get = NULL;
	genitem->func.state_get = NULL;
	genitem->func.del = NULL;

	testitem = elm_genlist_item_append(genlist, genitem, (void *)(long)1,
						NULL, ELM_GENLIST_ITEM_GROUP, NULL, NULL);
	elm_genlist_item_append(genlist, genitem, (void *)(long)2,
				NULL, ELM_GENLIST_ITEM_GROUP, NULL, NULL);
	elm_genlist_item_append(genlist, genitem, (void *)(long)3,
				NULL, ELM_GENLIST_ITEM_GROUP, NULL, NULL);
	elm_genlist_item_append(genlist, genitem, (void *)(long)4,
				NULL, ELM_GENLIST_ITEM_GROUP, NULL, NULL);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, genlist);

	register_request_listener(EVENT_HIDE, hide, genlist);
	register_request_listener(EVENT_SHOW, show, genlist);
	register_request_listener(EVENT_SET_ENABLED, set_enabled, genlist);
	register_request_listener(EVENT_SET_DISABLED, set_disabled, genlist);
	register_request_listener(EVENT_ADD_CHILDREN, add_children, genlist);
	register_request_listener(EVENT_RM_CHILDREN, rm_children, genlist);
	register_request_listener(EVENT_SELECT, select_genlist, testitem);
	register_request_listener(EVENT_UNSELECT, unselect_genlist, testitem);
	register_request_listener(EVENT_MULTISELECT, multiselect_genlist, genlist);
	register_request_listener(EVENT_MULTISELECT_UNSELECT, multiselect_unselect_genlist, testitem);

	register_request_listener(ACTION_SCROLL_LEFT, register_action_scroll_left, genlist);
	register_request_listener(ACTION_SCROLL_RIGHT, register_action_scroll_right, genlist);
	register_request_listener(ACTION_SCROLL_UP, register_action_scroll_up, genlist);
	register_request_listener(ACTION_SCROLL_DOWN, register_action_scroll_down, genlist);
	register_request_listener(ACTION_CLICK, register_action_click, testitem);
	register_request_listener(ACTION_PRESS, register_action_press, testitem);
	register_request_listener(ACTION_RELEASE, register_action_release, testitem);

	evas_object_show(win);

	elm_genlist_item_class_free(genitem);
	elm_run();
	elm_genlist_item_class_free(genitem);

	request_shutdown();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
