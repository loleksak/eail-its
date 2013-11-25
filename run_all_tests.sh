#!/bin/bash

result_file="results.xml"

export DISPLAY=:0.0

tests="
    registration/tests_registration.py
    mapping/tests_mapping.py
    events/events.py
    actions/test_action_interface.py
"

if [ $1 == "--with-coverage" ]; then
    export COVERAGE=1
fi

make

if test $? != 0; then
    echo "Building sample applications failed. Aborting."
    exit 1
fi

py.test --junitxml=$result_file --timeout=1.0 -v $tests
