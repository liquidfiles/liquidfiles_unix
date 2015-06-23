# LiquidFiles Unix Command Line Utility
## Introduction
liquidfiles_unix is UNIX command line utility, to work with Liquidfiles server, for sending files, listing messages, downloading files, etc.

LiquidFiles Unix command line utility extends the functionality of your LiquidFiles server to command line use and scripting from supported 
Unix and Linux servers.

Liquidfiles Unix is fully based on [Liquidfiles public API](http://support.liquidfiles.net/forums/21423392-API) and user [libcurl](http://curl.haxx.se/) to send and receive data from Liquidfiles server.

This document consists of the following sections:

1. Installation - describes the typical installation steps including dependent libraries.
2. Usage of tool - contains full description of commands, their arguments as well as usage examples.

## Installation
LiquidFiles can be installed statically or to use shared libraries where all system dependencies are met. LiquidFiles relies heavily on OpenSSL 
and Curl dependencies, so you will need to ensure all system dependencies are installed. It is recommended to use the latest supported versions 
of both OpenSSL and Curl.

Both installation options work in the same way and provide the same supported functionality, while the static executable can be copied to another 
identical machine and executed there, offering portability for larger or more controlled environments. Shared installations operate on the 
installed system only.

These instructions use the following keys for substituting user defined values:
* &lt;xxx-VERSION&gt;  (downloaded_version_filename)
* $PATH_X  (user_defined_path).

### Installing shared
Shared build will need to be performed on each system you wish to run Liquidfiles command line utility from. The following instructions assume 
you will use default installation paths.

#### Installing dependencies
If you already have OpenSSL or Curl libraries installed, you may skip some of the below steps.  If your system has OpenSSL or Curl installed in 
the default paths, you will not require the prefix option that allows user specified path for installation.

Install OpenSSL first:

1. Download latest OpenSSL version from official site.
2. Extract archive (e.g. tar –xz &lt;openssl-VERSION&gt;.tar.gz)
3. Go to directory (e.g. cd &lt;openssl-VERSION&gt;)
4. run Configure './config shared'
5. in the directory run 'make'
6. then run 'make install'

After installing OpenSSL, you need to install Curl:

1. Download latest curl version from official site.
2. Extract archive	(e.g. tar –xz &lt;curl-VERSION&gt;.tar.gz)
3. Go to directory	(e.g. cd &lt;curl-VERSION&gt;)
4. run Configure** './configure'
5. in the directory run 'make'
6. then run 'make install'

**Note - if your OpenSSL is installed in a non-system path, then you need to specify --with-ssl=$PATH_SSL  option.  
Refer to OpenSSL installation instructions and options for further information.**

#### Installing liquidfiles command line utility
1. Download LiquidFiles [Source Code] (github_link_here) and unpack the files, then enter the directory
2. Run Configure* './configure'
3. Make			make
4. Install		make install

**Note - if your OpenSSL and Curl is installed in non-system path, then you need to specify using the install options
  --with-curl=$PATH_CURL   and/or  --with-ssl=$PATH_SSL**

### Installing static
The benefit of building statically is that you do not necessarily have to install OpenSSL and Curl into default system 
path for use by other applications or when you have a different version installed on your build system.  The static 
installation creates a single, executable file that can be used on any other identical environment to the build system 
without the need to install the system dependencies at all.

Static build is similar to shared in that the compiling of OpenSSL and Curl that are heavily depended on, so you need 
to have dependent libraries to be installed statically and using the --enable-static configure option to build 
LiquidFiles statically.

This method also requires in some instances that a shared installation of OpenSSL is also present on the build system. 
It is highly recommended to use an external path (e.g. /home/Static) for BOTH OpenSSL and Curl static installations. 
Then specify this single path for building the LiquidFiles package into a directory of your preference. The instructions 
assume this is the case to provide a reliable installation option.

#### Installing dependencies
To install OpenSSL

1. Download latest OpenSSL version from official site.
2. Extract archive (e.g. tar –xz &lt;openssl-VERSION&gt;.tar.gz)
3. Go to directory (e.g. cd &lt;openssl-VERSION&gt;)
4. Configure shared libs (e.g. ./config shared --prefix=$PATH_X)
5. in the directory run 'make'
6. then run 'make install'
7. then run 'make clean'
8. Configure static libs	(./config no-shared --prefix=$PATH_X)
9. in the directory run 'make'
10. then run 'make install'

**NOTE - For some cases, Curl static build is also not passed with only static OpenSSL.  In these cases you need to build 
OpenSSL both statically and shared, in some instances you may be able to skip step 7-10 above, if in doubt do both.**

After having OpenSSL, you need to install curl

1. Download latest curl version from official site.
2. Extract archive (e.g. tar –xz &lt;curl-VERSION&gt;.tar.gz)
3. Go to directory (cd &lt;curl-VERSION&gt;)
4. Configure static files (e.g. ./configure --with-static --prefix=$PATH_X --with-ssl=$PATH_X --disable-shared)
5. in the directory run 'make'
6. then run 'make install'

**Note - Using these instructions, you will need the external path however if your OpenSSL is on system default paths, 
then you may not need to specify --with-ssl option, or if you have specified custom path you need to substitute.**

#### Installing liquidfiles
1. Download LiquidFiles Unix CLI Source
2. Configure static LiquidFiles** (e.g. ./configure --with-curl=$EXT_PATH --prefix=$LF_EXT_PATH --enable-static)
3. in the directory run 'make'
4. then run 'make install'

**Note - Using these instructions, you will need the external path however if your Curl is on system default paths, then 
you don’t need to specify --with-curl option.**

## Usage
Liquidfiles is command line utility. It invokes one command per session and exits. General usage is the following:

    liquidfiles <command> <command_args>

The list of supported commands is:
* attach              Uploads given files to server.
* attach_chunk        Uploads given chunk of file to server.
* delete_attachments  Deletes the given attachments.
* delete_filelink     Deletes the given filelink.
* download            Download given files.
* file_request        Sends the file request to specified user.
* filedrop            Sends the file(s) by filedrop.
* filelink            Uploads given file and creates filelink on it.
* filelinks           Lists the available filelinks.
* get_api_key         Retrieves api key for the specified user.
* messages            Lists the available messages.
* send                Sends the file(s) to specified user.

To get command's detailed description, options and usage 'help' command can be used:

    liquidfiles help <command>

Below subsections contain detailed descriptions of commands

### attach
Description:

    Uploads the given file(s) to the liquidfiles server and returns the id(s) of uploaded file(s).

Usage:

	liquidfiles attach [--server=<url>] [--api_key=<key>] [-k] [-s] [--report_level=<level>] <file> ...

Arguments:

	--server
	    The server URL. If not specified, tries to retrieve from saved credentials.

	--api_key
	    API key of liquidfiles, to login to system. If not specified, tries to retrieve from saved credentials.

	-k
	    If specified, do not validate server certificate. If not specified, tries to retrieve from saved credentials.

	-s
	    If specified, saves current credentials in cache. Credentials to save are - '-k', '--server' and '--api_key'.

	--report_level
	    Level of reporting.
	    Valid values: silent, normal, verbose.
	    Default value: "normal".

	<file> ...
	    File path(s) to upload.

### attach_chunk
Description:

	Uploads given chunk of file to server.

Usage:

	liquidfiles attach_chunk [--server=<url>] [--api_key=<key>] [-k] [-s] [--report_level=<level>] --chunk=<int> --chunks=<int> --filename=<string> <file>

Arguments:

	--server
	    The server URL. If not specified, tries to retrieve from saved credentials.

	--api_key
	    API key of liquidfiles, to login to system. If not specified, tries to retrieve from saved credentials.

	-k
	    If specified, do not validate server certificate. If not specified, tries to retrieve from saved credentials.

	-s
	    If specified, saves current credentials in cache. Credentials to save are - '-k', '--server' and '--api_key'.

	--report_level
	    Level of reporting.
	    Valid values: silent, normal, verbose.
	    Default value: "normal".

	--chunk
	    ID of current chunk.

	--chunks
	    Whole count of chunks.

	--filename
	    Name of target file.

	<file>
	    File chunk path to upload.

### delete_attachments
Description:

	Deletes the given attachments.

Usage:

	liquidfiles delete_attachments [--server=<url>] [--api_key=<key>] [-k] [-s] [--report_level=<level>] [--message_id=<id>] [<id> ...]

Arguments:

	--server
	    The server URL. If not specified, tries to retrieve from saved credentials.

	--api_key
	    API key of liquidfiles, to login to system. If not specified, tries to retrieve from saved credentials.

	-k
	    If specified, do not validate server certificate. If not specified, tries to retrieve from saved credentials.

	-s
	    If specified, saves current credentials in cache. Credentials to save are - '-k', '--server' and '--api_key'.

	--report_level
	    Level of reporting.
	    Valid values: silent, normal, verbose.
	    Default value: "normal".

	--message_id
	    Message id to delete attachments of it.

	<id> ...
	    Id(s) of attachments to delete.

### delete_filelink
Description:

	Deletes the given filelink.

Usage:

	liquidfiles delete_filelink [--server=<url>] [--api_key=<key>] [-k] [-s] [--report_level=<level>] --filelink_id=<id>

Arguments:

	--server
	    The server URL. If not specified, tries to retrieve from saved credentials.

	--api_key
	    API key of liquidfiles, to login to system. If not specified, tries to retrieve from saved credentials.

	-k
	    If specified, do not validate server certificate. If not specified, tries to retrieve from saved credentials.

	-s
	    If specified, saves current credentials in cache. Credentials to save are - '-k', '--server' and '--api_key'.

	--report_level
	    Level of reporting.
	    Valid values: silent, normal, verbose.
	    Default value: "normal".

	--filelink_id
	    ID of filelink to delete.

### download
Description:

	Downloads given files.
    This command gives 2 ways to download files from liquidfiles.
    First way to by specifying direct url to file(s) by unnamed arguments. In this case command downloads the specified files from the url.
    Second way is by specifying message(s) by '--message_id' argument or by '--sent_in_the_last' or '--sent_after'. In this case command retrieves the message(s) and downloads all the files attached to it.

Usage:

	liquidfiles download [--server=<url>] [--api_key=<key>] [-k] [-s] [--report_level=<level>] [--download_to=<path>] [--message_id=<id>] [--sent_in_the_last=<HOURS>] [--sent_after=YYYYMMDD] [<url> ...]

Arguments:

	--server
	    The server URL. If not specified, tries to retrieve from saved credentials.

	--api_key
	    API key of liquidfiles, to login to system. If not specified, tries to retrieve from saved credentials.

	-k
	    If specified, do not validate server certificate. If not specified, tries to retrieve from saved credentials.

	-s
	    If specified, saves current credentials in cache. Credentials to save are - '-k', '--server' and '--api_key'.

	--report_level
	    Level of reporting.
	    Valid values: silent, normal, verbose.
	    Default value: "normal".

	--download_to
	    Directory path to download files there.
	    Default value: "".

	--message_id
	    Message id to download attachments of it.

	--sent_in_the_last
	    Download files sent in the last specified hours.

	--sent_after
	    Download files sent after specified date.

	<url> ...
	    Url(s) of files to download.

### file_request
Description:

	Sends the file request to specified user.

Usage:

	liquidfiles file_request [--server=<url>] [--api_key=<key>] [-k] [-s] [--report_level=<level>] --to=<username> [--subject=<string>] [--message=<string>]

Arguments:

	--server
	    The server URL. If not specified, tries to retrieve from saved credentials.

	--api_key
	    API key of liquidfiles, to login to system. If not specified, tries to retrieve from saved credentials.

	-k
	    If specified, do not validate server certificate. If not specified, tries to retrieve from saved credentials.

	-s
	    If specified, saves current credentials in cache. Credentials to save are - '-k', '--server' and '--api_key'.

	--report_level
	    Level of reporting.
	    Valid values: silent, normal, verbose.
	    Default value: "normal".

	--to
	    User name or email, to send file request.

	--subject
	    Subject of composed email.
	    Default value: "".

	--message
	    Message text of composed email.
	    Default value: "".

	--message_file
	    The file in which contained message text of composed email.
	    Only one of --meassage and --message_file arguments must be given.

### filedrop
Description:

	Sends the file(s) by filedrop.

Usage:

	liquidfiles filedrop --server=<url> [-k] [--report_level=<level>] --from=<username> [--subject=<string>] [--message=<string>] [-r] <file> ...

Arguments:

	--server
	    The server URL.

	-k
	    If specified, do not validate server certificate.

	--report_level
	    Level of reporting.
	    Valid values: silent, normal, verbose.
	    Default value: "normal".

	--from
	    User who sends the files

	--subject
	    Subject of composed email.
	    Default value: "".

	--message
	    Message text of composed email.
	    Default value: "".

	-r
	    If specified, it means that unnamed arguments are attachment IDs, otherwise they are file paths.

	<file> ...
	    File path(s) or attachments IDs to send to user.

### filelink
Description:

	Creates filelink.
    This command creates filelink for the given files. It can upload given file and create link on it or get already uploaded file ID and create link on it.
    If '-r' option is specified, it means that given unnamed argument is ID of already uploaded file, so command just creates link on it.
    If '-r' is not specified, then given unnamed argument is file path, and command uploads that file and creates link on it.

Usage:

	liquidfiles filelink [--server=<url>] [--api_key=<key>] [-k] [-s] [--report_level=<level>] [--expires=<YYYY-MM-DD>] [-r] <file>

Arguments:

	--server
	    The server URL. If not specified, tries to retrieve from saved credentials.

	--api_key
	    API key of liquidfiles, to login to system. If not specified, tries to retrieve from saved credentials.

	-k
	    If specified, do not validate server certificate. If not specified, tries to retrieve from saved credentials.

	-s
	    If specified, saves current credentials in cache. Credentials to save are - '-k', '--server' and '--api_key'.

	--report_level
	    Level of reporting.
	    Valid values: silent, normal, verbose.
	    Default value: "normal".

	--expires
	    Expire date for the filelink.

	-r
	    If specified, it means that unnamed argument is attachment ID, otherwise it is file path.

	<file>
	    File path or attachment id to create filelink.

### filelinks
Description:

	Lists the available filelinks.
    The output of this command is list of filelinks. The format of output depends on '--output_format' argument.
    If '--output_format' is table, then table is printed. Each row of table represents one filelink.
    If '--output_format' is csv, then output is csv format. All the filelinks are separated by comma.

Usage:

	liquidfiles filelinks [--server=<url>] [--api_key=<key>] [-k] [-s] [--report_level=<level>] [--output_format=<format>] [--limit=<number>]

Arguments:

	--server
	    The server URL. If not specified, tries to retrieve from saved credentials.

	--api_key
	    API key of liquidfiles, to login to system. If not specified, tries to retrieve from saved credentials.

	-k
	    If specified, do not validate server certificate. If not specified, tries to retrieve from saved credentials.

	-s
	    If specified, saves current credentials in cache. Credentials to save are - '-k', '--server' and '--api_key'.

	--report_level
	    Level of reporting.
	    Valid values: silent, normal, verbose.
	    Default value: "normal".

	--output_format
	    Specifies output string format.
	    Valid values: table, csv.
	    Default value: "table".

	--limit
	    Limit of filelinks list.

### get_api_key
Description:

	Retrieves api key for the specified user.

Usage:

	liquidfiles get_api_key [-k] --server=<url> --username=<email> --password=<password> [-s] [--report_level=<level>]

Arguments:

	-k
	    If specified, do not validate server certificate.

	--server
	    The server URL.

	--username
	    Username.

	--password
	    Password.

	-s
	    If specified, saves current credentials in cache. Credentials to save are - '-k', '--server' and retrieved key.

	--report_level
	    Level of reporting.
	    Valid values: silent, normal, verbose.
	    Default value: "normal".

### messages
Description:
	Lists the available messages.
    The output of this command is list of messages. The format of output depends on '--output_format' argument.
    If '--output_format' is table, then table is printed. Each row of table represents one message.
    If '--output_format' is csv, then output is csv format. All the messages are separated by comma.

Usage:
	liquidfiles messages [--server=<url>] [--api_key=<key>] [-k] [-s] [--report_level=<level>] [--output_format=<format>] [--message_id=<id>] [--sent_in_the_last=<HOURS>] [--sent_after=YYYYMMDD]

Arguments:
	--server
	    The server URL. If not specified, tries to retrieve from saved credentials.

	--api_key
	    API key of liquidfiles, to login to system. If not specified, tries to retrieve from saved credentials.

	-k
	    If specified, do not validate server certificate. If not specified, tries to retrieve from saved credentials.

	-s
	    If specified, saves current credentials in cache. Credentials to save are - '-k', '--server' and '--api_key'.

	--report_level
	    Level of reporting.
	    Valid values: silent, normal, verbose.
	    Default value: "normal".

	--output_format
	    Specifies output string format.
	    Valid values: table, csv.
	    Default value: "table".

	--message_id
	    Message id to show.

	--sent_in_the_last
	    Show messages sent in the last specified hours.

	--sent_after
	    Show messages sent after specified date.

### send
Description:

	Sends the file(s) to specified user.
	This command can upload given files or files in given directories and send them by message or get already uploaded file IDs and send them.

Usage:

	liquidfiles send [--server=<url>] [--api_key=<key>] [-k] [-s] [--report_level=<level>] --to=<username> [--subject=<string>] [--message=<string>] [--message_file=<string>] [--file_type=<file_type>] <file> ...

Arguments:

	--server
	    The server URL. If not specified, tries to retrieve from saved credentials.

	--api_key
	    API key of liquidfiles, to login to system. If not specified, tries to retrieve from saved credentials.

	-k
	    If specified, do not validate server certificate. If not specified, tries to retrieve from saved credentials.

	-s
	    If specified, saves current credentials in cache. Credentials to save are - '-k', '--server' and '--api_key'.

	--report_level
	    Level of reporting.
	    Valid values: silent, normal, verbose.
	    Default value: "normal".

	--to
	    User name or email, to send file.

	--subject
	    Subject of composed email.
	    Default value: "".

	--message
	    Message text of composed email.
	    Default value: "".

	--message_file
	    The file in which contained message text of composed email.

	--file_type
	    It specifies the type of file must be send to user.
	    Valid values: file_names, directory, attachments.
	    Default value: "file_names".
        values:
	        file_names : given unnamed arguments are file paths, and command uploads that files and sends them.
	        directory :  given unnamed arguments are directory paths, and command uploads files in that directory and sends them.
	        attachements :   given unnamed arguments are IDs of already uploaded files, so command just sends them.

	<file> ...
	    File path(s),  attachments IDs or directories to send to user.

