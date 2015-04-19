#! /bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

SERVER=https://pink.liquidfiles.net

$DIR/../src/liquidfiles get_api_key --server=$SERVER -k --username=xustup@gmail.com --password=TestPassword_1 -s
KEY=${KEY##* }

if [ ! -f ~/.liquidfiles/credentials ]; then
    echo "Credentials didn't saved."
    echo "Test FAILED."
    exit 1
fi

$DIR/../src/liquidfiles messages 

status=$?
if [ $status -ne 0 ]; then
    echo "Credentials not working."
    echo "Test FAILED."
    exit 1
fi

rm ~/.liquidfiles/credentials

echo "Test PASSED."
