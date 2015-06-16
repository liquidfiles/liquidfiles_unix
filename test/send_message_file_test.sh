#! /bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $DIR/common.sh

MESSAGE=`$EXEC send --to=xustup@example.com --server=$SERVER -k --api_key=$KEY --message_file="large_file" --subject="Hello!" $DIR/send_test.sh`
test_status "Couldn't send message."
MESSAGE=${MESSAGE##* }

test_message $MESSAGE

if [ ! -f .tmp_test/send_test.sh ]; then
    echo "Couldn't download file."
    fail
fi
rm -rf .tmp_test
echo "Test PASSED."
