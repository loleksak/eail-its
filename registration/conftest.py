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
Configuration file for tests_registration.py
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


@pytest.fixture(params=['button'])
def app_runnable(request):
    return EailApp(os.path.join(DATA_DIR, 'sample', request.param))


@pytest.fixture
def timeout(request):
    return float(request.config.option.timeout)
