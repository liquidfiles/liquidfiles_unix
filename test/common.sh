#! /bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

SERVER=https://pink.liquidfiles.com

EXEC=$DIR/../build/liquidfiles

KEY=`$EXEC get_api_key --server=$SERVER -k --username=xustup@gmail.com --password=TestPassword_1`
KEY=${KEY##* }

function fail {
    echo "Test FAILED"
    exit $?
}

function test_status {
    status=$?
    if [ $status -ne 0 ]; then
        echo -e "Error: $1"
        fail
    fi
}

function test_message {
    $EXEC messages --server=$SERVER -k --api_key=$KEY --message_id=$1
    test_status "Couldn't retrieve message."
    $EXEC messages --server=$SERVER -k --api_key=$KEY --message_id=$1 --output_format=csv
    test_status "Couldn't retrieve message in csv format."
    echo ""
    mkdir .tmp_test
    $EXEC download --server=$SERVER -k --api_key=$KEY --message_id=$1 --download_to=.tmp_test
    test_status "Couldn't download file."
}
