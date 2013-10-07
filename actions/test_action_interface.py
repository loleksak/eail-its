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
Tests whether the actions declared in the ATK-EAIL mapping emit the appropriate signals.
"""

#!/usr/bin/python

import pyatspi
import pytest
import time

from utils import *


def test_interface_Action_doAction(app_runnable, response_timeout, obj, action):
    """
    Verifies whether the actions declared for this widget in the ATK-EAIL mapping emit the appropriate signals.

    The mapping is provided in parameters.ini.

    :param app_runnable: test application
    :param response_timeout: number of seconds between start and stop listening on efl application response
    :param obj: widget object in test application.
    :param action: tested action.
    """

    assert obj, "No application or no ATK object has been found"

    action_interface = obj.queryAction()
    app_runnable.start_listen_on_events(response_timeout)
    app_runnable.perform_request("ACTION_%s" % action.upper())
    action_interface.doAction(get_action_index(obj, action))
    assert app_runnable.get_response(), \
        "No response for %s widget %s action" % (obj.getRoleName(), action)
