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

#include "utils.h"

/* generic function definitions
 * show, hide, set_enabled, set_disabled,
 * set_focus, unset_focus, set_focusable, unset_focusable,
 * click, press, release, _mouse_down, _mouse_up
 */

char* get_file(const char *argv0, const char *file_name) {
	char *last = strrchr(argv0, '/');
	char *file = (char*)malloc(
		(last-argv0+strlen(file_name)+1) * sizeof(char)
		);
	eina_strlcpy(file, argv0, last-argv0+2);
	eina_strlcat(file, file_name, strlen(file_name)+strlen(file)+1);
	return file;
}

void show(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	evas_object_show(obj);
}

void hide(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	evas_object_hide(obj);
}

void set_disabled(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_object_disabled_set(obj, EINA_TRUE);
}

void set_enabled(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_object_disabled_set(obj,EINA_FALSE);
}

void set_focus(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_object_focus_set(obj, EINA_TRUE);
}

void unset_focus(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_object_focus_set(obj, EINA_FALSE);
}

void set_focusable(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_object_tree_focus_allow_set(obj, EINA_TRUE);
}

void unset_focusable(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	elm_object_tree_focus_allow_set(obj, EINA_FALSE);
}

void release(void *data) {
	Evas_Object *obj = data;
	evas_event_feed_mouse_up(evas_object_evas_get(obj), 1,
			EVAS_BUTTON_NONE, 0, NULL);
}

Eina_Bool _mouse_up(void *data) {
	release(data);
	return EINA_FALSE;
}

void _mouse_down(Evas_Object *obj) {
	int x, y, w, h;
	Evas *e;
	e = evas_object_evas_get(obj);
	evas_object_geometry_get(obj, &x, &y, &w, &h);
	evas_event_feed_mouse_move(e, x + w * 0.25, y + h * 0.5, 0, NULL);
	evas_event_feed_mouse_down(e, 1, EVAS_BUTTON_NONE, 0, NULL);
	evas_event_feed_mouse_move(e, x + w * 0.75, y + h * 0.5, 0, NULL);
}

void click(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	_mouse_down(obj);
	ecore_timer_add(0.3, _mouse_up, obj);
}

void press(void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	_mouse_down(obj);
}


