#! /bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $DIR/common.sh

ID1=`$EXEC attach --server=$SERVER -k --api_key=$KEY $DIR/send_test.sh`
test_status "Couldn't upload file"
ID1=${ID1##* }

ID2=`$EXEC attach --server=$SERVER -k --api_key=$KEY $DIR/attach_test.sh`
test_status "Couldn't upload file"
ID2=${ID2##* }

MESSAGE=`$EXEC send --to=xustup@example.com --server=$SERVER -k -r --api_key=$KEY --message="Hello" --subject="Hello!" $ID1 $ID2`
test_status "Couldn't send message."
MESSAGE=${MESSAGE##* }

test_message $MESSAGE

if [ ! -f .tmp_test/send_test.sh ]; then
    echo "Couldn't download file."
    fail
fi
if [ ! -f .tmp_test/attach_test.sh ]; then
    echo "Couldn't download file."
    fail
fi
rm -rf .tmp_test
echo "Test PASSED."
