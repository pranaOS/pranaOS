#!/bin/sh

echo
echo "RUNNING TESTS"

run-tests --show-progress=false
fail_count=$?

echo "Failed: $fail_count" > ./test-results.log

if test $DO_SHUTDOWN_AFTER_TESTS {
    shutdown -n
}

exit $fail_count
