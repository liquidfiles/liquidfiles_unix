#! /bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $DIR/common.sh

MESSAGE=`$EXEC file_request --to=xustup@example.com --server=$SERVER -k --api_key=$KEY --message="Hello" --subject="Hello!"`
test_status "Couldn't send file request \n$MESSAGE"
echo "Test PASSED."

MESSAGE=`$EXEC file_request --to=xustup@example.com --server=$SERVER -k --api_key=$KEY --message_file="$DIR/large_file" --subject="Hello!"`
test_status "Couldn't send file request \n$MESSAGE"

echo "Test PASSED."
