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
Tests whether the events declared in the ATK-EAIL mapping emit the appropriate signals.
"""


from utils import *

def test_events_broadcast(app, obj, event, request1, request2,
                          listener_timeout):
    """
    Verifies whether the events declared for this widget in the ATK-EAIL mapping emit the appropriate signals.

    The mapping is provided in parameters.ini.

    :param app: test application.
    :param obj: widget object in test application.
    :param event: tested event.
    :param request1: function call needed to set the widget's initial state in which request2 can trigger the event.
    :param request2: function call that triggers the event.
    :param listener_timeout: time the event handler waits for an event signal.
    """

    assert obj and app and app.is_running(), "Preconditions failed"

    handler = EventHandler(obj)

    if request1:
        app.perform_request("EVENT_%s" % request1.upper())
    handler.listen_on_events([event], listener_timeout)
    app.perform_request("EVENT_%s" % request2.upper())

    events = handler.get_occured_events()
    assert len(events) and events[0].type == event,\
        "Event %s has not occurred" % event
