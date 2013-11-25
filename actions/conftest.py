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
Configuration file for test_action_interfaces.py
"""

import pytest
import os
import sys
import ConfigParser


DATA_DIR = './'
CONFIG_FILE = 'parameters.ini'


path = os.path.dirname(os.path.abspath(__file__))
print path
sys.path.insert(0, path + '/../')

from utils import *


def pytest_addoption(parser):
    parser.addoption('--response_timeout', action='store',
                     default=2, help='number of seconds between start '
                     'and stop listening on response from efl app')


@pytest.fixture
def app_runnable(request):
    return EailApp(os.path.join(DATA_DIR, 'sample', ''))


@pytest.fixture
def response_timeout(request):
    return float(request.config.option.response_timeout)


@pytest.fixture
def action(request):
    if request.param:
        return request.param.strip()
    return None


@pytest.fixture
def obj(request):
    app_path = request.param[0]
    obj_name = request.param[1]

    app = EailApp(app_path)
    timeout = float(request.config.option.timeout)
    app.run_and_wait(timeout)

    def close():
        if os.getenv("COVERAGE"):
            app.interrupt_and_wait(timeout)
        else:
            app.kill_and_wait(timeout)

    request.addfinalizer(close)
    app_obj = find_accessible_application(os.path.basename(app_path))

    return find_accessible_object_by_name(app_obj, obj_name)


def pytest_generate_tests(metafunc):
    """
    Generates test cases for the action verification test category.
    """
    global CONFIG_PATH

    parser = ConfigParser.ConfigParser()
    config_path = os.path.join(DATA_DIR, CONFIG_FILE)
    parser.read(config_path)
    arg_values = []
    ids = []

    if metafunc.function.__name__ == 'test_interface_Action_doAction':
        for section in parser.sections():
            if parser.has_option(section, 'Actions'):
                actions = parser.get(section, 'Actions').strip()
            else:
                continue
            for a in actions.split(','):

                values = (
                    (parser.get(section, 'App_Path').strip(),
                     parser.get(section, 'Name').strip()),
                    a
                )
                ids.append(section + str([a]))
                arg_values.append(values)
        metafunc.parametrize(("obj", "action"), arg_values, indirect=True,
                             ids=ids)
