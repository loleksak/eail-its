# Copyright (c) 2013 Samsung Electronics Co., Ltd.

# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library; see the file COPYING.LIB.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.

"""
Configuration file for events.py
"""

import pytest
import os
import sys
import ConfigParser


DATA_DIR = './'
CONFIG_FILE = 'parameters.ini'

path = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, path + '/../')

from utils import *

EVENTS = {
    'bounds-change': ['object:bounds-changed', '', 'resize'],
    'children-add': ['object:children-changed:add', '', 'add_children'],
    'children-remove': ['object:children-changed:remove', 'add_children',
                        'rm_children'],
    'children-remove-all': ['object:children-changed:remove', 'add_children',
                            'rm_all_children'],
    'armed-press': ['object:state-changed:armed', '', 'press'],
    'armed-release': ['object:state-changed:armed', 'press', 'release'],
    'press': ['object:state-changed:pressed', '', 'press'],
    'horizontal-set': ['object:state-changed:horizontal', 'set_vertical',
                       'set_horizontal'],
    'horizontal-unset': ['object:state-changed:horizontal', 'set_horizontal',
                         'set_vertical'],
    'vertical-set': ['object:state-changed:vertical', 'set_horizontal',
                     'set_vertical'],
    'vertical-unset': ['object:state-changed:vertical',  'set_vertical',
                       'set_horizontal'],
    'animated-set': ['object:state-changed:animated', 'unset_animated',
                     'set_animated'],
    'animated-unset': ['object:state-changed:animated',  'set_animated',
                       'unset_animated'],
    'select-all': ['object:state-changed:selected', '', 'select_all'],
    'select-none': ['object:state-changed:selected', 'select_all',
                    'select_none'],
    'value-set': ['object:visible-data-changed', '', 'set_value'],
    'dismiss': ['object:state-changed:visible', '', 'dismiss'],
    'editable-set': ['object:state-changed:editable', 'unset_editable',
                     'set_editable'],
    'editable-unset': ['object:state-changed:editable',  'set_editable',
                       'unset_editable'],
    'single-line-set': ['object:state-changed:single-line',
                        'unset_single_line', 'set_single_line'],
    'single-line-unset': ['object:state-changed:single-line',
                          'set_single_line', 'unset_single_line'],
    'multi-line-set': ['object:state-changed:single-line', 'set_single_line',
                       'unset_single_line'],
    'multi-line-unset': ['object:state-changed:single-line',
                         'unset_single_line', 'set_single_line'],
    'checked-set': ['object:state-changed:checked', 'set_unchecked',
                    'set_checked'],
    'checked-unset': ['object:state-changed:checked', 'set_checked',
                      'set_unchecked'],
    'animated-set': ['object:state-changed:animated', 'unset_animated',
                     'set_animated'],
    'animated-unset': ['object:state-changed:animated', 'set_animated',
                       'unset_animated'],
    'expand': ['object:state-changed:expanded', '', 'expand'],
    'shrink': ['object:state-changed:expanded', 'expand', 'shrink'],
    'select': ['object:selection-changed', '', 'select'],
    'unselect': ['object:selection-changed', 'select', 'unselect'],
    'multiselect': ['object:selection-changed', '', 'multiselect'],
    'multiunselect': ['object:selection-changed', 'multiselect', 'unselect'],
    'focus-click': ['object:focus-event', '', 'click'],
    'parent-change': ['object:property-changed:accessible-parent', 'unparent',
                      'parent_change'],
    'text-insert': ['object:text-changed-insert', '', 'text_insert'],
    'text-delete': ['object:text-changed-delete', 'text_insert',
                    'text_delete'],
    'text-caret-move': ['object:text-caret-moved', '',
                    'text_caret_move'],
    'attributes-changed': ['text-attributes-changed', '',
                           'text_attributes_changed'],
    'maximize': ['window:maximize', '', 'set_maximize'],
    'lower': ['window:lower', 'set_maximize', 'unset_maximize'],
    'minimize': ['window:minimize', '', 'set_minimize'],
    'focusable-set': ['object:state-changed:focusable', 'unset_focusable',
                      'set_focusable'],
    'focusable-unset': ['object:state-changed:focusable', 'set_focusable',
                        'unset_focusable'],
    'focused-set': ['object:state-changed:focused', 'unset_focus',
                    'set_focus'],
    'focused-unset': ['object:state-changed:focused', 'set_focus',
                      'unset_focus'],
}


EVENTS_ALL = {
    'enabled-disable': ['object:state-changed:enabled', '', 'set_disabled'],
    'enabled-enable': ['object:state-changed:enabled', 'set_disabled',
                       'set_enabled'],
    'showing-show': ['object:state-changed:showing', 'hide', 'show'],
    'visible-hide': ['object:state-changed:visible', '', 'hide'],
    'visible-show': ['object:state-changed:visible', 'hide', 'show']
}


def pytest_addoption(parser):
    parser.addoption('--listener_timeout', action='store',
                     default=2, help='number of seconds'
                     'between start and stop listening on events')


@pytest.fixture()
def app(request, timeout):
    app_path = request.param.strip()
    if app_path is None:
        return None
    app = EailApp(app_path)
    app.run_and_wait(timeout)

    def finalize():
        if os.getenv("COVERAGE"):
            app.interrupt_and_wait(timeout)
        else:
            app.kill_and_wait(timeout)
    request.addfinalizer(finalize)
    return app


@pytest.fixture()
def obj(request):
    app_name = request.param[0]
    obj_name = request.param[1]

    app_accessible_obj = find_accessible_application(app_name)
    if app_accessible_obj:
        return find_accessible_object_by_name(app_accessible_obj, obj_name)

    return None


@pytest.fixture
def event(request):
    if request.param:
        return request.param.strip()
    return None


@pytest.fixture
def request1(request):
    if request.param:
        return request.param.strip()
    return None


@pytest.fixture
def request2(request):
    if request.param:
        return request.param.strip()
    return None


@pytest.fixture
def timeout(request):
    return float(request.config.option.timeout)


@pytest.fixture
def listener_timeout(request):
    return float(request.config.option.listener_timeout)


def pytest_generate_tests(metafunc):
    """
    Generates test cases for the event broadcast verification test category.
    """
    global CONFIG_PATH

    parser = ConfigParser.ConfigParser()
    config_path = os.path.join(DATA_DIR, CONFIG_FILE)
    parser.read(config_path)
    arg_values = []
    ids = []

    for section in parser.sections():
        for name, event in EVENTS_ALL.iteritems():
            values = (
                parser.get(section, 'App_Path').strip(),
                (
                    parser.get(section, 'App_Path').strip().split('/')[-1],
                    parser.get(section, 'Name').strip()
                ),
                event[0],
                event[1],
                event[2]
            )
            arg_values.append(values)
            ids.append(section + "[" + event[0] + "[" + event[2] + "]]")

        if parser.has_option(section, 'Events'):
            events = parser.get(section, 'Events').strip()
        else:
            continue
        for event in events.split(','):
            e = event.strip()
            values = (
                parser.get(section, 'App_Path').strip(),
                (
                    parser.get(section, 'App_Path').strip().split('/')[-1],
                    parser.get(section, 'Name').strip()
                ),
                EVENTS[e][0],
                EVENTS[e][1],
                EVENTS[e][2]
            )
            arg_values.append(values)
            ids.append(
                section + "[" + EVENTS[e][0] + "[" + EVENTS[e][2] + "]]"
            )
    metafunc.parametrize(("app", "obj", "event", "request1", "request2"),
                         arg_values, indirect=True, ids=ids)
