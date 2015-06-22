#! /bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $DIR/common.sh

MESSAGE=`$EXEC send --to=xustup@example.com --server=$SERVER -k --api_key=$KEY --file_type="directory"  --message_file="$DIR/large_file" --subject="Hello!" $DIR`
test_status "Couldn't send message files.\n$MESSAGE"
MESSAGE=${MESSAGE##* }

test_message $MESSAGE

if [ ! -f .tmp_test/send_test.sh ]; then
    echo "Couldn't download file."
    fail
fi
rm -rf .tmp_test
echo "Test PASSED."

