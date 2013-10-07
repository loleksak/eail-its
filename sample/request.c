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

#include "request.h"

static Eina_List *requests;
static Ecore_Fd_Handler* fd_handler = NULL;
static Eina_Bool _fd_handler_cb(void *data, Ecore_Fd_Handler *handler);
static unsigned int request_init_count = 0;
static int tmp_scroll_x;
static int tmp_scroll_y;

typedef struct _request request;
struct _request
{
	Request_Cb func;
	Eina_Stringshare* request_name;
	void* data;
};

static Eina_Bool _fd_handler_cb(void *data, Ecore_Fd_Handler *handler) {
	char message[1024];
	size_t nbytes;
	int fd;
	const Eina_List *l;
	request *r;

	fd = ecore_main_fd_handler_fd_get(handler);
	nbytes = read(fd, message, sizeof(message));
	message[nbytes] = '\0';

	EINA_LIST_FOREACH(requests, l, r) {
		if(!strcmp(r->request_name, message) && r->func)
			r->func(r->data);
	}

	return ECORE_CALLBACK_RENEW;
}

Eina_Bool request_init(void){
	char* path;
	FILE* file;
	int fd;

	if(request_init_count > 0)
		return EINA_TRUE;
	request_init_count++;

	path = getenv("EAIL_ITS_REQUEST_PIPE");
	if (!path)
		return EINA_FALSE;

	file = fopen(path, "r");
	if (!file)
		return EINA_FALSE;
	fd = fileno(file);
	fd_handler = ecore_main_fd_handler_add(fd,
							 ECORE_FD_READ,
							 _fd_handler_cb,
							 NULL, NULL, NULL);
	if (!fd_handler)
		return EINA_FALSE;
	return EINA_TRUE;
}

void request_shutdown(void){
	request *data;

	if(!request_init_count)
		return;

	ecore_main_fd_handler_del(fd_handler);
	EINA_LIST_FREE(requests, data){
		eina_stringshare_del(data->request_name);
		free(data);
	}
}

void register_request_listener(const char* request_name, Request_Cb func, void *data) {
	request *r;

	request_init();
	r = malloc(sizeof(request));
	r->func = func;
	r->request_name = eina_stringshare_add(request_name);
	r->data = data;
	requests = eina_list_append(requests, r);
}


void send_response(const char* message) {

	char* path;
	FILE* file;
	path = getenv("EAIL_ITS_RESPONSE_PIPE");
	if (!path)
		return;
	file = fopen(path, "w");
	if (!file)
		return;
	fprintf(file, "%s", message);
	fclose(file);
}



void _scroll_left_response_cb(void *data, Evas_Object *obj, void *event_info) {
	int x;

	elm_scroller_region_get(obj, &x, NULL, NULL, NULL);
	if( tmp_scroll_x > x)
		send_response(SUCCESS);
}

void _scroll_up_response_cb(void *data, Evas_Object *obj, void *event_info) {
	int y;

	elm_scroller_region_get(obj, NULL, &y, NULL, NULL);
	if( tmp_scroll_y > y)
		send_response(SUCCESS);
}

void _scroll_down_response_cb(void *data, Evas_Object *obj, void *event_info) {
	int y;
	elm_scroller_region_get(obj, NULL, &y, NULL, NULL);
	if( tmp_scroll_y < y)
		send_response(SUCCESS);
}

void _scroll_right_response_cb(void *data, Evas_Object *obj, void *event_info) {
	int x;

	elm_scroller_region_get(obj, &x, NULL, NULL, NULL);
	if( tmp_scroll_x < x)
		send_response(SUCCESS);;
}

void register_action_scroll_left(void *data){
	int y, w, h;
	Evas_Object *obj = (Evas_Object *)data;

	elm_scroller_region_get(obj, &tmp_scroll_x, &y, &w, &h);
	tmp_scroll_x = tmp_scroll_x + 10;
	elm_scroller_region_show(obj, tmp_scroll_x, y, w, h);
	evas_object_smart_callback_add(obj, "scroll,anim,stop", _scroll_left_response_cb, NULL);
}

void register_action_scroll_right(void *data){
	Evas_Object *obj = (Evas_Object *)data;

	elm_scroller_region_get(obj, &tmp_scroll_x, NULL, NULL, NULL);
	evas_object_smart_callback_add(obj, "scroll,anim,stop", _scroll_right_response_cb, NULL);
}

void register_action_scroll_up(void *data){
	int x, w, h;
	Evas_Object *obj = (Evas_Object *)data;

	elm_scroller_region_get(obj, &x, &tmp_scroll_y, &w, &h);
	tmp_scroll_y = tmp_scroll_y + 10;
	elm_scroller_region_show(obj, x, tmp_scroll_y, w, h);
	evas_object_smart_callback_add(obj, "scroll,anim,stop", _scroll_up_response_cb, NULL);
}

void register_action_scroll_down(void *data){
	Evas_Object *obj = (Evas_Object *)data;

	elm_scroller_region_get(obj, NULL, &tmp_scroll_y, NULL, NULL);
	evas_object_smart_callback_add(obj, "scroll,anim,stop", _scroll_down_response_cb, NULL);
}

void _response_cb (void *data, Evas_Object *obj, void *event_info) {
	send_response(SUCCESS);
}

void register_action_click(void *data){
	Evas_Object *obj = (Evas_Object *)data;
	evas_object_smart_callback_add(obj, "clicked", _response_cb, NULL);
}

