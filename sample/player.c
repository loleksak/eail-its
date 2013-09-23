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
* gcc -o player player.c request.c `pkg-config --cflags --libs elementary`
*/
#include <Elementary.h>
#include "utils.h"
#include <atk/atk.h>
#include <eail_factory.h>
#include "request.h"

#define DESCRITPION "Sample player description"
#define NAME "Sample player"
#define MY_VIDEO "Helicopter.mp4"

void resize (void *data) {
	Evas_Object* obj = (Evas_Object*) data;
	evas_object_resize(obj, 100, 100);
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

void register_action_forward(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	evas_object_smart_callback_add(obj, "forward,clicked", _response_cb, NULL);
}

void register_action_next(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	evas_object_smart_callback_add(obj, "next,clicked", _response_cb, NULL);
}

void register_action_pause(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	evas_object_smart_callback_add(obj, "pause,clicked", _response_cb, NULL);
}

void register_action_play(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	evas_object_smart_callback_add(obj, "play,clicked", _response_cb, NULL);
}

void register_action_prev(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	evas_object_smart_callback_add(obj, "prev,clicked", _response_cb, NULL);
}

void register_action_rewind(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	evas_object_smart_callback_add(obj, "rewind,clicked", _response_cb, NULL);
}

void register_action_stop(void *data) {
	Evas_Object *obj = (Evas_Object *)data;
	evas_object_smart_callback_add(obj, "stop,clicked", _response_cb, NULL);
}


EAPI_MAIN int
elm_main(int argc, char **argv) {
	Evas_Object *win = NULL;
	Evas_Object *video = NULL;
	Evas_Object *player = NULL;
	char *video_file = NULL;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "Player app!");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	player = elm_player_add(win);

	video = elm_video_add(win);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, player);

	register_request_listener(EVENT_SET_FOCUSABLE, set_focusable, player);
	register_request_listener(EVENT_UNSET_FOCUSABLE, unset_focusable, player);
	register_request_listener(EVENT_SET_FOCUS, set_focus, player);
	register_request_listener(EVENT_UNSET_FOCUS, unset_focus, player);
	register_request_listener(EVENT_RESIZE, resize, player);
	register_request_listener(EVENT_SET_DISABLED, set_disabled, player);
	register_request_listener(EVENT_SET_ENABLED, set_enabled, player);
	register_request_listener(EVENT_HIDE, hide, player);
	register_request_listener(EVENT_SHOW, show, player);

	register_request_listener(ACTION_FORWARD, register_action_forward, player);
	register_request_listener(ACTION_NEXT, register_action_next, player);
	register_request_listener(ACTION_PAUSE, register_action_pause, player);
	register_request_listener(ACTION_PLAY, register_action_play, player);
	register_request_listener(ACTION_PREV, register_action_prev, player);
	register_request_listener(ACTION_REWIND, register_action_rewind, player);
	register_request_listener(ACTION_STOP, register_action_stop, player);

	elm_video_stop(video);

	evas_object_resize(win, 480, 320);
	elm_win_resize_object_add(win, video);
	evas_object_resize(player, 480, 10);
	evas_object_move(player, 0, 300);
	elm_object_part_content_set(player, "video", video);

	evas_object_show(win);
	evas_object_show(video);
	evas_object_show(player);

	video_file = get_file(argv[0], MY_VIDEO);
	elm_video_file_set(video, video_file);
	elm_video_play(video);

	elm_run();
	request_shutdown();
	elm_shutdown();
	free(video_file);
	return 0;
}
ELM_MAIN()
