#! /bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $DIR/common.sh

SERVER=https://pink.liquidfiles.com/filedrop/cli_test

$EXEC filedrop --from=xustup@example.com --server=$SERVER -k --message="Hello" --subject="Hello!" $DIR/aaa.jpg
test_status "Couldn't send filedrop message."
$EXEC filedrop --from=xustup@example.com --server=$SERVER -k --message="Hello" --subject="Hello!" -r ArfLsCGXxpy9jINQEtj2vX
test_status "Couldn't send filedrop message by attachments."
echo "Test PASSED."
