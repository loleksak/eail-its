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
Tests whether an application registers and deregisters correctly in the AT-SPI registry.
"""


#!/usr/bin/python

import pyatspi
import pytest
import os
from utils import *


def test_app_register(app_runnable, timeout):
    """
    Verifies whether the application registers correctly in AT-SPI.

    :param app_runnable: test application
    :param timeout: time needed to register app in AT-SPI registry
    """

    app_runnable.run_and_wait(timeout)
    assert app_runnable.is_running(), 'Unable to run test application'

    app_found = False
    for app in pyatspi.Registry.getDesktop(0):
        if app.name.strip() == app_runnable.get_name():
            app_found = True
            break

    app_runnable.kill_and_wait(timeout)

    assert app_found, 'Application failed to register in AT-SPI'


def test_app_unregister_by_kill(app_runnable, timeout):
    """
    Verifies whether the application unregisters correctly from the AT-SPI registry using the SIGKILL signal.

    :param app_runnable: test application
    :param timeout: time needed to register app in AT-SPI registry
    """

    app_runnable.run_and_wait(timeout)
    assert app_runnable.is_running(), 'Unable to run test application'

    app_runnable.kill_and_wait(timeout)

    app_found = False
    for app in pyatspi.Registry.getDesktop(0):
        if app.name.strip() == app_runnable.get_name():
            app_found = True
            break

    assert not app_found, 'Application failed to unregister in AT-SPI'


def test_app_unregister_by_terminate(app_runnable, timeout):
    """
    Verifies whether the application unregisters correctly from the AT-SPI registry using the SIGTERM signal.

    :param app_runnable: test application
    :param timeout: time needed to register app in AT-SPI registry
    """

    app_runnable.run_and_wait(timeout)
    assert app_runnable.is_running(), 'Unable to run test application'

    app_runnable.terminate_and_wait(timeout)

    app_found = False
    for app in pyatspi.Registry.getDesktop(0):
        if app.name.strip() == app_runnable.get_name():
            app_found = True
            break

    assert not app_found, 'Application failed to unregister in AT-SPI'
