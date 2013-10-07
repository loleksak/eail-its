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
#include <atk/atk.h>
#include <eail_factory.h>
#include <Evas_GL.h>

#define DESCRITPION "Sample glview description"
#define NAME "Sample glview"

typedef struct _GLData GLData;
struct _GLData {
	Evas_GL_API *glapi;
	GLuint       program;
	GLuint       vtx_shader;
	GLuint       fgmt_shader;
	GLuint       vbo;
	int          initialized : 1;
};

static float color = 1.0;

static void _init_gl(Evas_Object *object) {
	GLData *gld = evas_object_data_get(object, "gld");
	Evas_GL_API *gl_evas_api = gld->glapi;
	GLfloat vVertices[] =  {  0.0f,  0.5f, 0.0f,-0.5f, -0.5f, 0.0f,0.5f, -0.5f, 0.0f };
	gl_evas_api->glGenBuffers(1, &gld->vbo);
	gl_evas_api->glBindBuffer(GL_ARRAY_BUFFER, gld->vbo);
	gl_evas_api->glBufferData(GL_ARRAY_BUFFER, 3 * 3 * 4, vVertices, GL_STATIC_DRAW);
}

static void _del_gl(Evas_Object *object) {
	GLData *gld = evas_object_data_get(object, "gld");
	if (!gld) {
		printf("Unable to get GLData. n");
		return;
	}
	Evas_GL_API *gl_evas_api = gld->glapi;
	gl_evas_api->glDeleteShader(gld->vtx_shader);
	gl_evas_api->glDeleteShader(gld->fgmt_shader);
	gl_evas_api->glDeleteProgram(gld->program);
	gl_evas_api->glDeleteBuffers(1, &gld->vbo);
	evas_object_data_del((Evas_Object*)object, "..gld");
	free(gld);
}
static void _draw_gl(Evas_Object *object) {
	Evas_GL_API *gl_evas_api = elm_glview_gl_api_get(object);
	GLData *gld = evas_object_data_get(object, "gld");
	if (!gld) return;
	int width, hight;
	elm_glview_size_get(object, &width, &hight);
	gl_evas_api->glViewport(0, 0, width, hight);
	gl_evas_api->glClearColor(color,0.8,0.3,1);
	gl_evas_api->glClear(GL_COLOR_BUFFER_BIT);
	gl_evas_api->glEnable(GL_BLEND);
	gl_evas_api->glUseProgram(gld->program);
	gl_evas_api->glBindBuffer(GL_ARRAY_BUFFER, gld->vbo);
	gl_evas_api->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,0, 0);
	gl_evas_api->glEnableVertexAttribArray(0);
	gl_evas_api->glDrawArrays(GL_TRIANGLES, 0, 3);
	gl_evas_api->glFinish();
	color -= 0.1;
	if (color < 0.0) color = 1.0;
}

static Eina_Bool _anim(void *data) {
	elm_glview_changed_set(data);
	return EINA_TRUE;
}

static void _del(void *data, Evas *evas, Evas_Object *object, void *event_info) {
	Ecore_Animator *ani_mator = evas_object_data_get(object, "ani");
	ecore_animator_del(ani_mator);
}
void _register_cb(void *data, Evas_Object *win, void *event_info) {
	Evas_Object *eo = (Evas_Object *)data;
	AtkObject *obj;
	obj = eail_factory_get_accessible(eo);
	atk_object_set_description(obj, DESCRITPION);
	atk_object_set_name (obj, NAME);
}


EAPI_MAIN int
elm_main(int argc, char **argv) {
	elm_config_preferred_engine_set("opengl_x11");
	Evas_Object *win = NULL;
	Evas_Object *glview = NULL;
	Evas_Object *box = NULL;
	Evas_Object *bg = NULL;
	Ecore_Animator *ani = NULL;
	GLData *gld = NULL;
	if (!(gld = calloc(1, sizeof(GLData)))) return 1;

	win = elm_win_add(NULL, "Main Window", ELM_WIN_BASIC);
	elm_win_title_set(win, "Hello, World!");
	elm_win_autodel_set(win, EINA_TRUE);
	elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);

	bg = elm_bg_add(win);
	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, bg);
	evas_object_show(bg);

	box = elm_box_add(win);
	evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, box);
	evas_object_show(box);

	glview = elm_glview_add(win);
	gld->glapi = elm_glview_gl_api_get(glview);
	evas_object_size_hint_align_set(glview, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(glview, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_glview_mode_set(glview, ELM_GLVIEW_ALPHA | ELM_GLVIEW_DEPTH);
	elm_glview_resize_policy_set(glview, ELM_GLVIEW_RESIZE_POLICY_RECREATE);
	elm_glview_render_policy_set(glview, ELM_GLVIEW_RENDER_POLICY_ON_DEMAND);
	elm_glview_init_func_set(glview, _init_gl);
	elm_glview_del_func_set(glview, _del_gl);
	elm_glview_render_func_set(glview, _draw_gl);
	elm_box_pack_end(box, glview);
	evas_object_show(glview);
	elm_object_focus_set(glview, EINA_TRUE);
	ani = ecore_animator_add(_anim, glview);
	evas_object_data_set(glview, "ani", ani);
	evas_object_data_set(glview, "gld", gld);
	evas_object_event_callback_add(glview, EVAS_CALLBACK_DEL, _del, glview);

	evas_object_smart_callback_add(win, "focus,in", _register_cb, glview);

	evas_object_resize(win, 240, 60);
	evas_object_show(box);
	evas_object_show(win);

	elm_run();
	elm_shutdown();
	return 0;
}
ELM_MAIN()
