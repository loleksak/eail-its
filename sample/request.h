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
#define SUCCESS "OK"

#define ACTION_CLICK "ACTION_CLICK"
#define ACTION_PRESS "ACTION_PRESS"
#define ACTION_RELEASE "ACTION_RELEASE"

#define ACTION_ZOOM_IN "ACTION_ZOOM_IN"
#define ACTION_ZOOM_OUT "ACTION_ZOOM_OUT"

#define ACTION_SCROLL_LEFT "ACTION_SCROLL_LEFT"
#define ACTION_SCROLL_RIGHT "ACTION_SCROLL_RIGHT"
#define ACTION_SCROLL_UP "ACTION_SCROLL_UP"
#define ACTION_SCROLL_DOWN "ACTION_SCROLL_DOWN"

#define ACTION_DISMISS "ACTION_DISMISS"

#define ACTION_MINIMIZE "ACTION_MINIMIZE"
#define ACTION_MAXIMIZE "ACTION_MAXIMIZE"
#define ACTION_RESTORE "ACTION_RESTORE"

#define ACTION_NEXT "ACTION_NEXT"
#define ACTION_PREVIOUS "ACTION_PREVIOUS"

#define ACTION_PAUSE "ACTION_PAUSE"
#define ACTION_PLAY "ACTION_PLAY"
#define ACTION_PREV "ACTION_PREV"
#define ACTION_FORWARD "ACTION_FORWARD"
#define ACTION_REWIND "ACTION_REWIND"
#define ACTION_STOP "ACTION_STOP"

#define ACTION_EXPAND "ACTION_EXPAND"
#define ACTION_SHRINK "ACTION_SHRINK"

#define ACTION_START "ACTION_START"
#define ACTION_FLIP "ACTION_FLIP"
#define ACTION_TOGGLE "ACTION_TOGGLE"

#define EVENT_SHOW "EVENT_SHOW"
#define EVENT_HIDE "EVENT_HIDE"
#define EVENT_SET_DISABLED "EVENT_SET_DISABLED"
#define EVENT_SET_ENABLED "EVENT_SET_ENABLED"
#define EVENT_SET_FOCUS "EVENT_SET_FOCUS"
#define EVENT_UNSET_FOCUS "EVENT_UNSET_FOCUS"
#define EVENT_SET_FOCUSABLE "EVENT_SET_FOCUSABLE"
#define EVENT_UNSET_FOCUSABLE "EVENT_UNSET_FOCUSABLE"
#define EVENT_CLICK "EVENT_CLICK"
#define EVENT_PRESS "EVENT_PRESS"
#define EVENT_RELEASE "EVENT_RELEASE"
#define EVENT_RESIZE "EVENT_RESIZE"
#define EVENT_ADD_CHILDREN "EVENT_ADD_CHILDREN"
#define EVENT_RM_CHILDREN "EVENT_RM_CHILDREN"
#define EVENT_RM_ALL_CHILDREN "EVENT_RM_ALL_CHILDREN"
#define EVENT_PARENT_CHANGED "EVENT_PARENT_CHANGED"
#define EVENT_UNPARENT "EVENT_UNPARENT"
#define EVENT_SET_HORIZONTAL "EVENT_SET_HORIZONTAL"
#define EVENT_SET_VERTICAL "EVENT_SET_VERTICAL"
#define EVENT_UNSET_HORIZONTAL "EVENT_UNSET_HORIZONTAL"
#define EVENT_SET_VALUE "EVENT_SET_VALUE"
#define EVENT_SET_CHECKED "EVENT_SET_CHECKED"
#define EVENT_SET_UNCHECKED "EVENT_SET_UNCHECKED"
#define EVENT_SET_ANIMATED "EVENT_SET_ANIMATED"
#define EVENT_UNSET_ANIMATED "EVENT_UNSET_ANIMATED"
#define EVENT_SET_EDITABLE "EVENT_SET_EDITABLE"
#define EVENT_UNSET_EDITABLE "EVENT_UNSET_EDITABLE"
#define EVENT_DISMISS "EVENT_DISMISS"
#define EVENT_EXPAND "EVENT_EXPAND"
#define EVENT_SHRINK "EVENT_SHRINK"
#define EVENT_SET_MAXIMIZE "EVENT_SET_MAXIMIZE"
#define EVENT_SET_MINIMIZE "EVENT_SET_MINIMIZE"
#define EVENT_UNSET_MAXIMIZE "EVENT_UNSET_MAXIMIZE"
#define EVENT_MOVE "EVENT_MOVE"

#define EVENT_SELECT "EVENT_SELECT"
#define EVENT_UNSELECT "EVENT_UNSELECT"
#define EVENT_MULTISELECT "EVENT_MULTISELECT"
#define EVENT_MULTISELECT_UNSELECT "EVENT_MULTISELECT_UNSELECT"
#define EVENT_SELECT_ALL "EVENT_SELECT_ALL"
#define EVENT_SELECT_NONE "EVENT_SELECT_NONE"

#define EVENT_SET_SINGLE_LINE "EVENT_SET_SINGLE_LINE"
#define EVENT_UNSET_SINGLE_LINE "EVENT_UNSET_SINGLE_LINE"
#define EVENT_TEXT_SELECT "EVENT_TEXT_SELECT"
#define EVENT_TEXT_UNSELECT "EVENT_TEXT_UNSELECT"
#define EVENT_TEXT_INSERT "EVENT_TEXT_INSERT"
#define EVENT_TEXT_DELETE "EVENT_TEXT_DELETE"
#define EVENT_TEXT_ATTRIBUTES_CHANGED "EVENT_TEXT_ATTRIBUTES_CHANGED"
#define EVENT_TEXT_CARET_MOVE "EVENT_TEXT_CARET_MOVE"

typedef void(* Request_Cb )(void *data);
Eina_Bool request_init(void);
void request_shutdown(void);
void register_request_listener(const char* request_name, Request_Cb func, void *data);
void send_response(const char* message);

void register_action_scroll_left(void *data);
void register_action_scroll_right(void *data);
void register_action_scroll_up(void *data);
void register_action_scroll_down(void *data);
void register_action_click(void *data);
