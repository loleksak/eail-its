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

#!/usr/bin/python

"""
Tests whether the widget's role is consistent with the role
declared in the ATK-EAIL mapping.
"""

import pyatspi
import pytest

from utils import *


def test_widget_role(obj, role):
    """
    Verifies whether the widget's role is consistent with the role
    declared in the ATK-EAIL mapping.

    The mapping is provided in parameters.ini.

    :param obj: widget object in test application.
    :param role: role expected in ATK-EAIL mapping.
    """

    assert obj, "No application or no ATK object has been found"

    impl_role = pyatspi.Role._enum_lookup[obj.getRole()]
    assert impl_role == role, "Invalid Atk role, expected '%s' different then"\
                              "current '%s'" % (role, impl_role)


def test_widget_interfaces(obj, interfaces):
    """
    Verifies whether the interfaces implemented by the widget
    are consistent with those declared in the ATK-EAIL mapping.

    The mapping is provided in parameters.ini.

    :param obj: widget object in test application.
    :param interfaces: list of interfaces expected in ATK-EAIL mapping.
    """

    assert obj, "No application or no ATK object has been found"

    implemented_interfaces = pyatspi.listInterfaces(obj)
    assert sorted(implemented_interfaces) == sorted(interfaces),\
        "Implemented interfaces different then expected: [%s] != [%s]" % \
        (str(implemented_interfaces), str(interfaces))


def test_widget_actions(obj, actions):
    """
    Verifies whether the actions implemented by the widget
    are consistent with those declared in the ATK-EAIL mapping.

    The mapping is provided in parameters.ini.

    :param obj: widget object in test application.
    :param actions: list of actions expected in ATK-EAIL mapping.
    """

    assert obj, "No application or no ATK object has been found"

    if actions and actions != []:
        actions_implemented = get_accessible_actions(obj)
        assert actions_implemented, "No AtkAction actions."
        assert sorted(actions) == sorted(actions_implemented),\
            "Implemented actions different then expected [%s] != [%s]" % \
            (str(actions_implemented), str(actions))
