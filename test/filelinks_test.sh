#! /bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $DIR/common.sh

ID1=`$DIR/../src/liquidfiles filelink --server=$SERVER -k --api_key=$KEY $DIR/send_test.sh`
status=$?
if [ $status -ne 0 ]; then
    echo "Couldn't create filelink"
    echo "Test FAILED."
    exit 1
fi
ID1=${ID1##* }
ID1=${ID1##*/}

ID2=`$DIR/../src/liquidfiles attach --server=$SERVER -k --api_key=$KEY $DIR/attach_test.sh`
status=$?
if [ $status -ne 0 ]; then
    echo "Couldn't upload file"
    echo "Test FAILED."
    exit 1
fi
ID2=${ID2##* }

ID2=`$DIR/../src/liquidfiles filelink --server=$SERVER -k -r --api_key=$KEY $ID2`
status=$?
if [ $status -ne 0 ]; then
    echo "Couldn't create filelink"
    echo "Test FAILED."
    exit 1
fi
ID2=${ID2##* }
ID2=${ID2##*/}

$DIR/../src/liquidfiles filelinks --server=$SERVER -k --api_key=$KEY
status=$?
if [ $status -ne 0 ]; then
    echo "Couldn't retrieve filelinks"
    echo "Test FAILED."
    exit 1
fi

$DIR/../src/liquidfiles filelinks --server=$SERVER -k --api_key=$KEY --output_format=csv
status=$?
if [ $status -ne 0 ]; then
    echo "Couldn't retrieve filelinks in csv format"
    echo "Test FAILED."
    exit 1
fi

$DIR/../src/liquidfiles delete_filelink --server=$SERVER -k --api_key=$KEY --filelink_id=$ID1
status=$?
if [ $status -ne 0 ]; then
    echo "Couldn't delete filelink"
    echo "Test FAILED."
    exit 1
fi

$DIR/../src/liquidfiles delete_filelink --server=$SERVER -k --api_key=$KEY --filelink_id=$ID2
status=$?
if [ $status -ne 0 ]; then
    echo "Couldn't delete filelink"
    echo "Test FAILED."
    exit 1
fi

echo "Test PASSED."
