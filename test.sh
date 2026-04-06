#!/bin/bash
set -e

# =============================================================================
# LiquidFiles Unix CLI - Integration Test Script
#
# Usage:
#   LF_USER=user@company.com LF_PASSWORD=password \
#     LF_SERVER=https://files.company.com \
#     LF_RECIPIENT=recipient@example.com \
#     LF_FILEDROP=https://files.company.com/filedrop/some_filedrop \
#     ./test.sh
#
# Optional env vars:
#   LF_FILEDROP    - Full filedrop URL (e.g. https://host/filedrop/name)
#   LF_SKIP_CERT   - Set to "no" to validate certificates (default: -k)
#   LF_BINARY      - Path to the liquidfiles binary (default: ./liquidfiles)
# =============================================================================

BINARY="${LF_BINARY:-./liquidfiles}"
SKIP_CERT="${LF_SKIP_CERT:--k}"
TMPDIR=$(mktemp -d)
PASS=0
FAIL=0
SKIP=0

cleanup() {
  rm -rf "$TMPDIR"
}
trap cleanup EXIT

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

red()    { printf "\033[31m%s\033[0m" "$*"; }
green()  { printf "\033[32m%s\033[0m" "$*"; }
yellow() { printf "\033[33m%s\033[0m" "$*"; }
bold()   { printf "\033[1m%s\033[0m" "$*"; }

pass() {
  PASS=$((PASS + 1))
  echo "  $(green PASS): $1"
}

fail() {
  FAIL=$((FAIL + 1))
  echo "  $(red FAIL): $1"
  if [ -n "$2" ]; then
    echo "        $2"
  fi
}

skip() {
  SKIP=$((SKIP + 1))
  echo "  $(yellow SKIP): $1 -- $2"
}

section() {
  echo ""
  bold "=== $1 ==="
  echo ""
}

check_required_vars() {
  local missing=0
  for var in LF_USER LF_PASSWORD LF_SERVER LF_RECIPIENT; do
    if [ -z "${!var}" ]; then
      echo "$(red ERROR): Required env var $var is not set."
      missing=1
    fi
  done
  if [ $missing -eq 1 ]; then
    echo ""
    echo "Usage:"
    echo "  LF_USER=user@example.com LF_PASSWORD=secret \\"
    echo "    LF_SERVER=https://liquidfiles.example.com \\"
    echo "    LF_RECIPIENT=recipient@example.com \\"
    echo "    ./test.sh"
    exit 1
  fi
}

create_test_file() {
  local name="$1"
  local size="${2:-1024}"
  dd if=/dev/urandom of="$TMPDIR/$name" bs="$size" count=1 2>/dev/null
  echo "$TMPDIR/$name"
}

# ---------------------------------------------------------------------------
# Pre-flight
# ---------------------------------------------------------------------------

check_required_vars

if [ ! -x "$BINARY" ]; then
  echo "$(red ERROR): Binary not found at $BINARY"
  echo "  Build it first or set BINARY=/path/to/liquidfiles"
  exit 1
fi

echo ""
bold "LiquidFiles Unix CLI - Integration Tests"
echo "Binary:    $BINARY"
echo "Server:    $LF_SERVER"
echo "Username:  $LF_USER"
echo "Recipient: $LF_RECIPIENT"
echo "Cert:      ${SKIP_CERT:-(validating)}"
echo ""

# ---------------------------------------------------------------------------
# 1. Help
# ---------------------------------------------------------------------------
section "1. Help"

if $BINARY help >/dev/null 2>&1; then
  pass "help"
else
  fail "help" "Command returned non-zero"
fi

if $BINARY help send >/dev/null 2>&1; then
  pass "help send"
else
  fail "help send"
fi

# ---------------------------------------------------------------------------
# 2. Get API Key
# ---------------------------------------------------------------------------
section "2. Get API Key"

API_KEY=""
output=$($BINARY get_api_key $SKIP_CERT \
  --server="$LF_SERVER" \
  --username="$LF_USER" \
  --password="$LF_PASSWORD" 2>&1) && rc=0 || rc=$?

if [ $rc -eq 0 ]; then
  # The API key is printed on the last non-empty line
  API_KEY=$(echo "$output" | grep -v "^Getting" | grep -v "^$" | tail -1)
  if [ -n "$API_KEY" ]; then
    pass "get_api_key (key: ${API_KEY:0:8}...)"
  else
    fail "get_api_key" "No API key in output: $output"
  fi
else
  fail "get_api_key" "$output"
  echo "$(red 'Cannot continue without API key. Aborting.')"
  exit 1
fi

# Common args for all subsequent commands
COMMON="$SKIP_CERT --server=$LF_SERVER --api_key=$API_KEY"

# ---------------------------------------------------------------------------
# 3. Attach (upload file)
# ---------------------------------------------------------------------------
section "3. Attach (upload file)"

TEST_FILE=$(create_test_file "testfile.bin" 4096)

output=$($BINARY attach $COMMON "$TEST_FILE" 2>&1) && rc=0 || rc=$?
if [ $rc -eq 0 ]; then
  ATTACHMENT_ID=$(echo "$output" | grep -oE 'ID: [A-Za-z0-9]+' | head -1 | sed 's/ID: //')
  if [ -n "$ATTACHMENT_ID" ]; then
    pass "attach (id: $ATTACHMENT_ID)"
  else
    pass "attach (no ID parsed, but command succeeded)"
  fi
else
  fail "attach" "$output"
fi

# ---------------------------------------------------------------------------
# 4. Attach Chunk (upload file in chunks)
# ---------------------------------------------------------------------------
section "4. Attach Chunk (upload in chunks)"

CHUNK_FILE=$(create_test_file "chunkfile.bin" 8192)
CHUNK_SIZE=4096
CHUNK_DIR="$TMPDIR/chunks"
mkdir -p "$CHUNK_DIR"

# Split into 2 chunks
split -b $CHUNK_SIZE "$CHUNK_FILE" "$CHUNK_DIR/chunk."
CHUNKS=($(ls "$CHUNK_DIR"/chunk.*))
NUM_CHUNKS=${#CHUNKS[@]}

chunk_ok=true
for i in $(seq 0 $((NUM_CHUNKS - 1))); do
  output=$($BINARY attach_chunk $COMMON \
    --chunk=$i --chunks=$NUM_CHUNKS \
    --filename="chunkfile.bin" \
    "${CHUNKS[$i]}" 2>&1) && rc=0 || rc=$?
  if [ $rc -ne 0 ]; then
    fail "attach_chunk (chunk $i/$NUM_CHUNKS)" "$output"
    chunk_ok=false
    break
  fi
done

if $chunk_ok; then
  CHUNK_ATTACHMENT_ID=$(echo "$output" | grep -oE 'ID: [A-Za-z0-9]+' | head -1 | sed 's/ID: //')
  pass "attach_chunk ($NUM_CHUNKS chunks, id: ${CHUNK_ATTACHMENT_ID:-unknown})"
fi

# ---------------------------------------------------------------------------
# 5. Send message (with file)
# ---------------------------------------------------------------------------
section "5. Send message"

SEND_FILE=$(create_test_file "sendfile.txt" 512)
echo "This is test content for the send command." > "$SEND_FILE"

output=$($BINARY send $COMMON \
  --to="$LF_RECIPIENT" \
  --subject="CLI Test - send" \
  --message="Automated test from liquidfiles unix cli" \
  "$SEND_FILE" 2>&1) && rc=0 || rc=$?

MESSAGE_ID=""
if [ $rc -eq 0 ]; then
  MESSAGE_ID=$(echo "$output" | grep "Message sent" | grep -oE 'ID: [A-Za-z0-9]+' | sed 's/ID: //')
  pass "send (message id: ${MESSAGE_ID:-unknown})"
else
  fail "send" "$output"
fi

# ---------------------------------------------------------------------------
# 6. Send message (with attachment ID)
# ---------------------------------------------------------------------------
section "6. Send message (with attachment ID)"

if [ -n "$ATTACHMENT_ID" ]; then
  output=$($BINARY send $COMMON \
    --to="$LF_RECIPIENT" \
    --subject="CLI Test - send_attachments" \
    --message="Automated test sending by attachment ID" \
    --file_type=attachment \
    "$ATTACHMENT_ID" 2>&1) && rc=0 || rc=$?

  if [ $rc -eq 0 ]; then
    pass "send with attachment ID"
  else
    fail "send with attachment ID" "$output"
  fi
else
  skip "send with attachment ID" "No attachment ID from earlier upload"
fi

# ---------------------------------------------------------------------------
# 7. Messages (list inbox)
# ---------------------------------------------------------------------------
section "7. Messages (list inbox)"

output=$($BINARY messages $COMMON --sent_in_the_last=24 2>&1) && rc=0 || rc=$?
if [ $rc -eq 0 ]; then
  pass "messages --sent_in_the_last=24"
else
  fail "messages --sent_in_the_last=24" "$output"
fi

# Also test CSV output
output=$($BINARY messages $COMMON --sent_in_the_last=24 --output_format=csv 2>&1) && rc=0 || rc=$?
if [ $rc -eq 0 ]; then
  pass "messages --output_format=csv"
else
  fail "messages --output_format=csv" "$output"
fi

# ---------------------------------------------------------------------------
# 8. Messages (view single message)
# ---------------------------------------------------------------------------
section "8. View single message"

if [ -n "$MESSAGE_ID" ]; then
  output=$($BINARY messages $COMMON --message_id="$MESSAGE_ID" 2>&1) && rc=0 || rc=$?
  if [ $rc -eq 0 ]; then
    pass "messages --message_id=$MESSAGE_ID"
  else
    fail "messages --message_id=$MESSAGE_ID" "$output"
  fi
else
  skip "messages --message_id" "No message ID from send test"
fi

# ---------------------------------------------------------------------------
# 9. Download (by message ID)
# ---------------------------------------------------------------------------
section "9. Download"

DOWNLOAD_DIR="$TMPDIR/downloads"
mkdir -p "$DOWNLOAD_DIR"

if [ -n "$MESSAGE_ID" ]; then
  output=$($BINARY download $COMMON \
    --message_id="$MESSAGE_ID" \
    --download_to="$DOWNLOAD_DIR" 2>&1) && rc=0 || rc=$?
  if [ $rc -eq 0 ]; then
    dl_count=$(ls -1 "$DOWNLOAD_DIR" 2>/dev/null | wc -l | tr -d ' ')
    pass "download by message_id ($dl_count files)"
  else
    fail "download by message_id" "$output"
  fi
else
  skip "download by message_id" "No message ID from send test"
fi

# ---------------------------------------------------------------------------
# 10. Filelink (create)
# ---------------------------------------------------------------------------
section "10. Filelink"

LINK_FILE=$(create_test_file "linkfile.bin" 2048)

output=$($BINARY filelink $COMMON "$LINK_FILE" 2>&1) && rc=0 || rc=$?
FILELINK_URL=""
if [ $rc -eq 0 ]; then
  FILELINK_URL=$(echo "$output" | grep -oE 'URL: https?://[^ ]+' | head -1 | sed 's/URL: //')
  pass "filelink create (url: ${FILELINK_URL:-unknown})"
else
  fail "filelink create" "$output"
fi

# ---------------------------------------------------------------------------
# 11. Filelinks (list)
# ---------------------------------------------------------------------------
section "11. Filelinks (list)"

output=$($BINARY filelinks $COMMON --limit=5 2>&1) && rc=0 || rc=$?
if [ $rc -eq 0 ]; then
  pass "filelinks --limit=5"
else
  fail "filelinks --limit=5" "$output"
fi

output=$($BINARY filelinks $COMMON --limit=5 --output_format=csv 2>&1) && rc=0 || rc=$?
if [ $rc -eq 0 ]; then
  pass "filelinks --output_format=csv"
else
  fail "filelinks --output_format=csv" "$output"
fi

# ---------------------------------------------------------------------------
# 12. Delete filelink
# ---------------------------------------------------------------------------
section "12. Delete filelink"

if [ -n "$FILELINK_URL" ]; then
  # Extract the filelink ID from the URL (last path component)
  FILELINK_ID=$(echo "$FILELINK_URL" | grep -oE '[A-Za-z0-9]+$')
  if [ -n "$FILELINK_ID" ]; then
    output=$($BINARY delete_filelink $COMMON \
      --filelink_id="$FILELINK_ID" 2>&1) && rc=0 || rc=$?
    if [ $rc -eq 0 ]; then
      pass "delete_filelink (id: $FILELINK_ID)"
    else
      fail "delete_filelink" "$output"
    fi
  else
    skip "delete_filelink" "Could not parse filelink ID from URL"
  fi
else
  skip "delete_filelink" "No filelink URL from create test"
fi

# ---------------------------------------------------------------------------
# 13. File request
# ---------------------------------------------------------------------------
section "13. File request"

output=$($BINARY file_request $COMMON \
  --to="$LF_RECIPIENT" \
  --subject="CLI Test - file_request" \
  --message="Automated test file request" 2>&1) && rc=0 || rc=$?

if [ $rc -eq 0 ]; then
  pass "file_request"
else
  fail "file_request" "$output"
fi

# ---------------------------------------------------------------------------
# 14. Delete attachments (by message ID)
# ---------------------------------------------------------------------------
section "14. Delete attachments"

if [ -n "$MESSAGE_ID" ]; then
  output=$($BINARY delete_attachments $COMMON \
    --message_id="$MESSAGE_ID" 2>&1) && rc=0 || rc=$?
  if [ $rc -eq 0 ]; then
    pass "delete_attachments --message_id=$MESSAGE_ID"
  else
    fail "delete_attachments by message_id" "$output"
  fi
else
  skip "delete_attachments by message_id" "No message ID from send test"
fi

# ---------------------------------------------------------------------------
# 15. Filedrop
# ---------------------------------------------------------------------------
section "15. Filedrop"

if [ -n "$LF_FILEDROP" ]; then
  FILEDROP_FILE=$(create_test_file "filedrop_test.zip" 512)

  output=$($BINARY filedrop $SKIP_CERT \
    --server="$LF_FILEDROP" \
    --from="$LF_USER" \
    --subject="CLI Test - filedrop" \
    --message="Automated filedrop test" \
    "$FILEDROP_FILE" 2>&1) && rc=0 || rc=$?

  if [ $rc -eq 0 ]; then
    pass "filedrop"
  else
    fail "filedrop" "$output"
  fi
else
  skip "filedrop" "LF_FILEDROP not set"
fi

# ---------------------------------------------------------------------------
# Summary
# ---------------------------------------------------------------------------
echo ""
bold "==========================================="
TOTAL=$((PASS + FAIL + SKIP))
echo "  Total: $TOTAL  $(green "Pass: $PASS")  $(red "Fail: $FAIL")  $(yellow "Skip: $SKIP")"
bold "==========================================="
echo ""

if [ $FAIL -gt 0 ]; then
  exit 1
fi
exit 0
