#! /bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

tests="
    attach_chunk_test
    attach_test
    credential_test
    file_request_test
    filelinks_test
    send_test
    sending_many_files
    "

count=0
failed=0
for t in $tests;
do
    echo "Running $t"
    $DIR/$t.sh > /dev/null
    count=$((count + 1))
    status=$?
    if [ $status -ne 0 ]; then
        failed=$((failed + 1))
        echo "FAILED"
    else
        echo "PASSED"
    fi
done

echo "$failed of $count tests failed"
if [ $failed -ne 0 ]; then
    echo "FINAL RESULT: FAILED"
else
    echo "FINAL RESULT: PASSED"
fi
exit $failed
