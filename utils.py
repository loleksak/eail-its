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
Common utilities used by all tests.
"""

import subprocess as sub
import threading
import pyatspi
import os
import time
import pytest
import signal

__all__ = ['EventHandler', 'EailApp', 'find_accessible_application',
           'find_accessible_object_by_name', 'get_accessible_actions',
           'get_action_index']


class RunnableApp(object):

    def __init__(self, app_path):
        self._app_path = os.path.abspath(app_path)
        self._app_name = os.path.basename(app_path)

    def run_and_wait(self, timeout):
        raise NotImplementedError

    def is_running(self):
        raise NotImplementedError

    def get_name(self):
        return self._app_name

    def kill_and_wait(self, timeout):
        raise NotImplementedError

    def terminate_and_wait(self, timeout):
        raise NotImplementedError


class EailApp(RunnableApp):
    """
        Class responsible for running, terminating and checking status
        of EFL-applications
    """

    def __init__(self, app_path, request_pipe_path='./my_request_fifo',
                 response_pipe_path='./my_response_fifo'):
        super(EailApp, self).__init__(app_path)
        self._app_proc = None
        self._request_pipe_path = request_pipe_path
        self._response_pipe_path = response_pipe_path
        self._event = threading.Event()
        self._event.set()

    def run_and_wait(self, timeout):
        if self.is_running():
            return
        try:
            denv = os.environ.copy()
            denv["EAIL_ITS_REQUEST_PIPE"] = self._request_pipe_path
            denv["EAIL_ITS_RESPONSE_PIPE"] = self._response_pipe_path
            denv["ELM_MODULES"] = "eail>eail/api"

            if os.path.exists(self._request_pipe_path):
                os.unlink(self._request_pipe_path)

            if not os.path.exists(self._request_pipe_path):
                os.mkfifo(self._request_pipe_path)

            if os.path.exists(self._response_pipe_path):
                os.unlink(self._response_pipe_path)

            if not os.path.exists(self._response_pipe_path):
                os.mkfifo(self._response_pipe_path)

            my_fifo = open(self._request_pipe_path, 'w+')
            self._app_proc = sub.Popen(self._app_path, shell=False, env=denv)
            time.sleep(timeout)
            my_fifo.close()
        except Exception:
            self.terminate_and_wait(timeout)

    def perform_request(self, request_name):
        my_fifo = open(self._request_pipe_path, 'w+')
        my_fifo.write(request_name)
        my_fifo.close()

    def _stop_listen(self):
        self._event.set()

    def _start_listen(self):
        self._response = None
        f = os.open(self._response_pipe_path, os.O_RDONLY | os.O_NONBLOCK)
        while not self._event.is_set():
            try:
                line = os.read(f, 1024)
                if line:
                    self._response = line
                    break
            except OSError:
                continue
        os.close(f)

    def start_listen_on_events(self, timeout):
        self._event = threading.Event()
        self._event.clear()
        threading.Thread(target=self._start_listen).start()
        threading.Timer(timeout, self._stop_listen).start()

    def get_response(self):
        self._event.wait()
        if self._response == "OK":
            return True
        return False

    def is_running(self):
        if self._app_proc and not self._app_proc.poll():
            return True
        return False

    def interrupt_and_wait(self, timeout):
        if self.is_running():
            self._app_proc.send_signal(signal.SIGINT)
            time.sleep(timeout)

    def kill_and_wait(self, timeout):
        if self.is_running():
            self._app_proc.kill()
            time.sleep(timeout)

    def terminate_and_wait(self, timeout):
        if self.is_running():
            self._app_proc.terminate()
            time.sleep(timeout)


def find_accessible_application(app_name):
    for app in pyatspi.Registry.getDesktop(0):
        if app.name == app_name:
            return app
    return None


def find_accessible_object_by_name(atk_object, name):
    if not atk_object:
        return None
    pred = lambda x: x.name == name
    return pyatspi.findDescendant(atk_object, pred)


def get_accessible_actions(obj):

    action_interface = obj.queryAction()
    implemented_actions = [action_interface.getName(n)
                           for n in range(action_interface.nActions)]
    return implemented_actions


def get_action_index(obj, name):
    action_interface = obj.queryAction()
    for n in range(action_interface.nActions):
        if action_interface.getName(n) == name:
            return n
    return -1


class EventHandler():
    def __init__(self, obj):
        self._obj = obj
        self._occured_events = []
        self._event = threading.Event()
        self._event.set()

    def _registry_stop(self):
        pyatspi.Registry.deregisterEventListener(
            self._event_handler, *self._event_filter)
        pyatspi.Registry.stop()
        self._event.set()

    def _event_handler(self, event):
        if event.source == self._obj:
            self._occured_events.append(event)

    def listen_on_events(self, event_filter, timeout):
        self._occured_events = []
        self._event_filter = event_filter
        pyatspi.Registry.registerEventListener(
            self._event_handler, *self._event_filter)
        threading.Timer(timeout, self._registry_stop).start()
        self._event.clear()
        threading.Thread(target=pyatspi.Registry.start).start()

    def get_occured_events(self):
        self._event.wait()
        return self._occured_events
