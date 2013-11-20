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

char* get_file(const char *argv0, const char *file_name);
void show(void *data);
void hide(void *data);
void set_disabled(void *data);
void set_enabled(void *data);
void set_focus(void *data);
void unset_focus(void *data);
void set_focusable(void *data);
void unset_focusable(void *data);
void release(void *data);
Eina_Bool _mouse_up(void *data);
void _mouse_down(Evas_Object *obj);
void click(void *data);
void press(void *data);
void text_caret_move( void *data);
