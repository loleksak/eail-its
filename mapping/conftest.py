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
Configuration file for tests_mapping.py
"""


import pytest
import os
import sys
import ConfigParser


DATA_DIR = './'
CONFIG_FILE = 'parameters.ini'

INTERFACES_NAMES = [
    'Collection', 'Accessible', 'Component', 'Action', 'EditableText',
    'Document', 'HyperText', 'Image', 'Selection', 'StreamableContent',
    'Table', 'Text', 'Value', 'Window'
]

path = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, path + '/../')

from utils import *


@pytest.fixture
def app_obj(request):
    app_path = request.getfuncargvalue('app_path')
    app = EailApp(app_path)
    timeout = float(request.config.option.timeout)
    app.run_and_wait(timeout)

    def close():
        app.kill_and_wait(timeout)

    request.addfinalizer(close)
    return find_accessible_application(os.path.basename(app_path))


@pytest.fixture
def obj(request):
    app_path = request.param[0]
    atk_obj_name = request.param[1]
    app = EailApp(app_path)
    timeout = float(request.config.option.timeout)
    app.run_and_wait(timeout)

    def close():
        app.kill_and_wait(timeout)

    request.addfinalizer(close)
    app_obj = find_accessible_application(os.path.basename(app_path))
    return find_accessible_object_by_name(app_obj, atk_obj_name)


@pytest.fixture
def role(request):
    return request.param


@pytest.fixture
def interfaces(request):
    infcs = [x.strip() for x in request.param.split(',')]
    for i in infcs:
        if i not in INTERFACES_NAMES:
            raise ValueError('Invalid interface name: %s' % i)
    return infcs


@pytest.fixture
def actions(request):
    actions = [x.strip() for x in request.param.split(',')]
    actions = filter(None, actions)
    return actions


def pytest_generate_tests(metafunc):
    """
    Generates test cases for the mapping verification test category.
    """
    global CONFIG_PATH
    parser = ConfigParser.ConfigParser()
    config_path = os.path.join(DATA_DIR, CONFIG_FILE)
    parser.read(config_path)
    arg_values = []
    ids = []
    if metafunc.function.__name__ == "test_widget_role":
        for section in parser.sections():
            role = parser.get(section, 'Role').strip()
            values = (
                (
                    parser.get(section, 'App_Path').strip(),
                    parser.get(section, 'Name').strip()
                ),
                role
            )
            ids.append(section + str([role]))
            arg_values.append(values)
        metafunc.parametrize(("obj", "role"), arg_values, indirect=True,
                             ids=ids)

    if metafunc.function.__name__ == "test_widget_interfaces":
        for section in parser.sections():
            if not parser.has_option(section, 'Interfaces'):
                continue
            infc = parser.get(section, 'Interfaces').strip()
            values = (
                (
                    parser.get(section, 'App_Path').strip(),
                    parser.get(section, 'Name').strip()
                ),
                infc
            )
            infc = infc.split(',')
            ids.append(section + str(infc))
            arg_values.append(values)
        metafunc.parametrize(("obj", "interfaces"), arg_values, indirect=True,
                             ids=ids)
    if metafunc.function.__name__ == "test_widget_actions":
        for section in parser.sections():
            if not parser.has_option(section, 'Actions'):
                continue
            actions = parser.get(section, 'Actions').strip()
            if not actions:
                continue
            values = (
                (
                    parser.get(section, 'App_Path').strip(),
                    parser.get(section, 'Name').strip()
                ),
                actions
            )
            ids.append(section + str(actions.split(',')))
            arg_values.append(values)
        metafunc.parametrize(("obj", "actions"), arg_values, indirect=True,
                             ids=ids)
