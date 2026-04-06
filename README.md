# LiquidFiles Unix Command Line Utility

## Introduction

A Unix/Linux command line utility for working with [LiquidFiles](https://www.liquidfiles.com/)
servers. Supports sending files, listing messages, downloading files, managing filelinks, file
requests, and filedrops.

This utility extends the functionality of your LiquidFiles server to command line use and scripting
from supported Unix and Linux systems.

It is based on the [LiquidFiles API v4.2](https://docs.liquidfiles.com/api/v4.2/) and uses
[libcurl](https://curl.se/) for HTTP communication and
[nlohmann/json](https://github.com/nlohmann/json) for JSON serialization.

## Requirements

- C++11 compiler (GCC, Clang, etc.)
- libcurl (with SSL support)
- CMake 2.8+ or autotools

## Quick Start

### Build with CMake

```
mkdir build && cd build
cmake ../
make
make install
```

### Build with autotools

```
./configure
make
make install
```

## Testing

An integration test script is included that exercises all commands against a running LiquidFiles
instance:

```
LF_USER=user@example.com LF_PASSWORD=secret \
  LF_SERVER=https://liquidfiles.example.com \
  LF_RECIPIENT=recipient@example.com \
  ./test.sh
```

Optional environment variables:

- `LF_FILEDROP` -- Full filedrop URL (e.g. `https://host/filedrop/name`)
- `LF_SKIP_CERT` -- Set to `no` to validate certificates (default: `-k`)
- `LF_BINARY` -- Path to the liquidfiles binary (default: `./liquidfiles`)

## Installation

LiquidFiles can be built with shared or static libraries. Both options provide the same
functionality. Static builds produce a single executable that can be copied to other identical
systems without installing dependencies.

These instructions use the following placeholders:

- `<xxx-VERSION>` -- downloaded version filename
- `$PATH_X` -- user-defined installation path

### Shared build

#### Installing dependencies

If you already have OpenSSL and curl installed in default system paths, you can skip ahead to
building the CLI.

Install OpenSSL:

1. Download the latest OpenSSL from the official site
2. `tar -xz <openssl-VERSION>.tar.gz`
3. `cd <openssl-VERSION>`
4. `./config shared`
5. `make`
6. `make install`

Install curl:

1. Download the latest curl from the official site
2. `tar -xz <curl-VERSION>.tar.gz`
3. `cd <curl-VERSION>`
4. `./configure`
5. `make`
6. `make install`

> If OpenSSL is installed in a non-system path, specify `--with-ssl=$PATH_SSL` when configuring
> curl.

#### Building the CLI

1. Download the [source code](https://github.com/liquidfiles/liquidfiles_unix)
2. `./configure`
3. `make`
4. `make install`

> If OpenSSL and curl are in non-system paths, use `--with-curl=$PATH_CURL` and/or
> `--with-ssl=$PATH_SSL`.

### Static build

Static builds bundle all dependencies into a single executable. You need to build OpenSSL and curl
as static libraries first. It is recommended to use a dedicated prefix path (e.g. `/home/Static`)
for both.

#### Installing dependencies

Install OpenSSL (both shared and static):

1. Download the latest OpenSSL from the official site
2. `tar -xz <openssl-VERSION>.tar.gz`
3. `cd <openssl-VERSION>`
4. `./config shared --prefix=$PATH_X`
5. `make && make install && make clean`
6. `./config no-shared --prefix=$PATH_X`
7. `make && make install`

> Some curl static builds require both shared and static OpenSSL libraries. If in doubt, build
> both.

Install curl (static):

1. Download the latest curl from the official site
2. `tar -xz <curl-VERSION>.tar.gz`
3. `cd <curl-VERSION>`
4. `./configure --with-static --prefix=$PATH_X --with-ssl=$PATH_X --disable-shared`
5. `make`
6. `make install`

#### Building the CLI

```
./configure --with-curl=$EXT_PATH --prefix=$LF_EXT_PATH --enable-static
make
make install
```

> If curl is in default system paths, you can omit `--with-curl`.

#### Alternative: CMake build

```
mkdir build && cd build
cmake ../
make
make install
```

## Usage

The CLI invokes one command per session:

    liquidfiles <command> <command_args>

Use `liquidfiles help <command>` for detailed help on any command.

### Supported commands

| Command | Description |
|---------|-------------|
| `attach` | Upload file(s) to server |
| `attach_chunk` | Upload a file chunk to server |
| `delete_attachments` | Delete attachments |
| `delete_filelink` | Delete a filelink |
| `download` | Download files |
| `file_request` | Send a file request |
| `filedrop` | Send file(s) via filedrop |
| `filelink` | Upload file and create filelink |
| `filelinks` | List available filelinks |
| `get_api_key` | Retrieve API key for a user |
| `messages` | List available messages |
| `send` | Send file(s) to a user |

### Common options

Most commands accept these options:

    --server=<url>        Server URL
    --api_key=<key>       API key for authentication
    -k                    Skip certificate validation
    -s                    Save credentials to cache
    --report_level=<lvl>  Output level: silent, normal, verbose

### attach

Upload file(s) and return their attachment IDs.

    liquidfiles attach [--server=<url>] [--api_key=<key>] [-k] [-s] [--report_level=<level>]
        <file> ...

### attach_chunk

Upload a single chunk of a file. Use this for large file uploads split into multiple parts.

    liquidfiles attach_chunk [--server=<url>] [--api_key=<key>] [-k] [-s] [--report_level=<level>]
        --chunk=<int> --chunks=<int> --filename=<string> <file>

Options:

    --chunk       Current chunk number (0-indexed)
    --chunks      Total number of chunks
    --filename    Target filename on server

### send

Send file(s) to a specified user.

    liquidfiles send [--server=<url>] [--api_key=<key>] [-k] [-s] [--report_level=<level>]
        --to=<email> [--subject=<string>] [--message=<string>] [--message_file=<path>]
        [--file_type=<type>] <file> ...

Options:

    --to              Recipient email address
    --subject         Email subject
    --message         Message body text
    --message_file    Read message body from file
    --file_type       Type of unnamed arguments:
                        file_names (default) - file paths to upload
                        directory - directories to upload
                        attachment - pre-uploaded attachment IDs

### messages

List messages or view a single message.

    liquidfiles messages [--server=<url>] [--api_key=<key>] [-k] [-s] [--report_level=<level>]
        [--output_format=<format>] [--message_id=<id>] [--sent_in_the_last=<HOURS>]
        [--sent_after=YYYYMMDD]

Options:

    --output_format      Output format: table (default), csv
    --message_id         Show a specific message
    --sent_in_the_last   Filter by hours (e.g. 24)
    --sent_after         Filter by date (YYYYMMDD)

### download

Download files by URL, message ID, or date filter.

    liquidfiles download [--server=<url>] [--api_key=<key>] [-k] [-s] [--report_level=<level>]
        [--download_to=<path>] [--message_id=<id>] [--sent_in_the_last=<HOURS>]
        [--sent_after=YYYYMMDD] [<url> ...]

Options:

    --download_to        Target directory for downloads
    --message_id         Download attachments from this message
    --sent_in_the_last   Download from messages in last N hours
    --sent_after         Download from messages after date

### file_request

Send a file request to a user.

    liquidfiles file_request [--server=<url>] [--api_key=<key>] [-k] [-s] [--report_level=<level>]
        --to=<email> [--subject=<string>] [--message=<string>]

### filedrop

Send file(s) via a filedrop.

    liquidfiles filedrop --server=<filedrop_url> [-k] [--report_level=<level>]
        --from=<email> [--subject=<string>] [--message=<string>] [-r] <file> ...

Options:

    --server    The filedrop URL: <ServerURL>/filedrop/<Name>
    --from      Sender email address
    -r          Treat unnamed args as attachment IDs instead of files

### filelink

Upload a file and create a filelink, or create a filelink from an existing attachment ID.

    liquidfiles filelink [--server=<url>] [--api_key=<key>] [-k] [-s] [--report_level=<level>]
        [--expires=<YYYY-MM-DD>] [-r] <file>

Options:

    --expires   Expiration date for the filelink
    -r          Treat unnamed arg as attachment ID instead of file path

### filelinks

List available filelinks.

    liquidfiles filelinks [--server=<url>] [--api_key=<key>] [-k] [-s] [--report_level=<level>]
        [--output_format=<format>] [--limit=<number>]

### delete_filelink

Delete a filelink.

    liquidfiles delete_filelink [--server=<url>] [--api_key=<key>] [-k] [-s]
        [--report_level=<level>] --filelink_id=<id>

### delete_attachments

Delete attachments by ID or delete all attachments from a message.

    liquidfiles delete_attachments [--server=<url>] [--api_key=<key>] [-k] [-s]
        [--report_level=<level>] [--message_id=<id>] [<id> ...]

### get_api_key

Retrieve the API key for a user account.

    liquidfiles get_api_key --server=<url> [-k] --username=<email> --password=<password>
        [-s] [--report_level=<level>]
