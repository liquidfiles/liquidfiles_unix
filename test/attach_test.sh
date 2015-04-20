#! /bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $DIR/common.sh

ID1=`$DIR/../src/liquidfiles attach --server=$SERVER -k --api_key=$KEY $DIR/send_test.sh`
status=$?
if [ $status -ne 0 ]; then
    echo "Couldn't upload file"
    echo "Test FAILED."
    exit 1
fi
ID1=${ID1##* }

ID2=`$DIR/../src/liquidfiles attach --server=$SERVER -k --api_key=$KEY $DIR/attach_test.sh`
status=$?
if [ $status -ne 0 ]; then
    echo "Couldn't upload file"
    echo "Test FAILED."
    exit 1
fi
ID2=${ID2##* }

MESSAGE=`$DIR/../src/liquidfiles send --to=xustup@example.com --server=$SERVER -k -r --api_key=$KEY --message="Hello" --subject="Hello!" $ID1 $ID2`
status=$?
if [ $status -ne 0 ]; then
    echo "Couldn't send message."
    echo "Test FAILED."
    exit 1
fi
MESSAGE=${MESSAGE##* }
$DIR/../src/liquidfiles messages --server=$SERVER -k --api_key=$KEY --message_id=$MESSAGE
status=$?
if [ $status -ne 0 ]; then
    echo "Couldn't retrieve message."
    echo "Test FAILED."
    exit 1
fi
$DIR/../src/liquidfiles messages --server=$SERVER -k --api_key=$KEY --message_id=$MESSAGE --output_format=csv
status=$?
if [ $status -ne 0 ]; then
    echo "Couldn't retrieve message in csv format."
    echo "Test FAILED."
    exit 1
fi
echo ""
mkdir .tmp_test
$DIR/../src/liquidfiles download --server=$SERVER -k --api_key=$KEY --message_id=$MESSAGE --download_to=.tmp_test
status=$?
if [ $status -ne 0 ]; then
    echo "Couldn't download file."
    echo "Test FAILED."
    exit 1
fi
if [ ! -f .tmp_test/send_test.sh ]; then
    echo "Couldn't download file."
    echo "Test FAILED."
    exit 1
fi
if [ ! -f .tmp_test/attach_test.sh ]; then
    echo "Couldn't download file."
    echo "Test FAILED."
    exit 1
fi
rm -rf .tmp_test
echo "Test PASSED."
