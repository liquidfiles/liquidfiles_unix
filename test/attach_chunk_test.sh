#! /bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $DIR/common.sh

split $DIR/large_file -l 4

$EXEC attach_chunk --server=$SERVER -k --api_key=$KEY --filename=large_file1 --chunk=1 --chunks=2 xaa
test_status "Couldn't upload first chunk"

ID=`$EXEC attach_chunk --server=$SERVER -k --api_key=$KEY --filename=large_file1 --chunk=2 --chunks=2 xab`
test_status "Couldn't upload second chunk"
ID=${ID##* }
rm xaa xab

MESSAGE=`$EXEC send --to=xustup@example.com --server=$SERVER -k -r --api_key=$KEY --message="Hello" --subject="Hello!" $ID`
test_status "Couldn't send message."
MESSAGE=${MESSAGE##* }
test_message $MESSAGE
if [ ! -f .tmp_test/large_file1 ]; then
    echo "Couldn't download file."
    fail
fi
diff .tmp_test/large_file1 $DIR/large_file
test_status "File split and merge doesn't work correctly."
rm -rf .tmp_test

count=`ls $DIR | wc -l`
ii=0
for file in `ls $DIR`;
do
    ii=$((ii + 1))
    ID=`$EXEC attach_chunk --server=$SERVER -k --api_key=$KEY --filename=large_file1 --chunk=$ii --chunks=$count $DIR/$file`
    test_status "Couldn't upload chunk $ii"
done
ID=${ID##* }
MESSAGE=`$EXEC send --to=xustup@example.com --server=$SERVER -k -r --api_key=$KEY --message="Hello" --subject="Hello!" $ID`
test_status "Couldn't send message."
MESSAGE=${MESSAGE##* }
test_message $MESSAGE
cat $DIR/* > .tmp_test/large_file2
diff .tmp_test/large_file1 .tmp_test/large_file2
test_status "Chunks attached incorrectly."
rm -rf .tmp_test

echo "Test PASSED."
