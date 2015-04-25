#! /bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $DIR/common.sh

if [ -f ~/.liquidfiles/credentials ]; then
    mv ~/.liquidfiles/credentials ~/.liquidfiles/.credentials
fi

$EXEC messages > /dev/null
status=$?
if [ $status -eq 0 ]; then
    if [ -f ~/.liquidfiles/.credentials ]; then
        mv ~/.liquidfiles/.credentials ~/.liquidfiles/credentials
    fi
    echo "Error: Credentials not working"
    echo "Test FAILED."
    exit 99
fi

$EXEC get_api_key --server=$SERVER -k --username=xustup@gmail.com --password=TestPassword_1 -s
KEY=${KEY##* }

if [ ! -f ~/.liquidfiles/credentials ]; then
    if [ -f ~/.liquidfiles/.credentials ]; then
        mv ~/.liquidfiles/.credentials ~/.liquidfiles/credentials
    fi
    echo "Credentials didn't saved."
    echo "Test FAILED."
    exit 99
fi

$EXEC messages

test_status "Credentials not working."

rm ~/.liquidfiles/credentials
if [ -f ~/.liquidfiles/.credentials ]; then
    mv ~/.liquidfiles/.credentials ~/.liquidfiles/credentials
fi

echo "Test PASSED."
