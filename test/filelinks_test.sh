#! /bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source $DIR/common.sh

ID1=`$EXEC filelink --server=$SERVER -k --api_key=$KEY $DIR/send_test.sh`
test_status "Couldn't create filelink"
ID1=${ID1##* }
ID1=${ID1##*/}

ID2=`$EXEC attach --server=$SERVER -k --api_key=$KEY $DIR/attach_test.sh`
test_status "Couldn't upload file"
ID2=${ID2##* }

ID2=`$EXEC filelink --server=$SERVER -k -r --api_key=$KEY $ID2`
test_status "Couldn't create filelink"
ID2=${ID2##* }
ID2=${ID2##*/}

$EXEC filelinks --server=$SERVER -k --api_key=$KEY
test_status "Couldn't retrieve filelinks"

$EXEC filelinks --server=$SERVER -k --api_key=$KEY --output_format=csv
test_status "Couldn't retrieve filelinks in csv format"

$EXEC delete_filelink --server=$SERVER -k --api_key=$KEY --filelink_id=$ID1
test_status "Couldn't delete filelink"

$EXEC delete_filelink --server=$SERVER -k --api_key=$KEY --filelink_id=$ID2
test_status "Couldn't delete filelink"

echo "Test PASSED."
