#! /bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $DIR/common.sh

MESSAGE=`$EXEC send --to=xustup@example.com --server=$SERVER -k --api_key=$KEY --message="Hello" --subject="Hello!" $DIR/*`
test_status "Couldn't send message."
MESSAGE=${MESSAGE##* }

test_message $MESSAGE

diff .tmp_test $DIR -r

test_status "Files sent incorrectly."

rm -rf .tmp_test
echo "Test PASSED."
