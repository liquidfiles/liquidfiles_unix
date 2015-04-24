#! /bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $DIR/common.sh

$EXEC get_api_key --server=$SERVER -k --username=xustup@gmail.com --password=TestPassword_1 -s
KEY=${KEY##* }

if [ ! -f ~/.liquidfiles/credentials ]; then
    echo "Credentials didn't saved."
    echo "Test FAILED."
    exit 99
fi

$EXEC messages

test_status "Credentials not working."

rm ~/.liquidfiles/credentials

echo "Test PASSED."
