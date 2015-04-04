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
* <xxx-VERSION>  (downloaded_version_filename)
* $PATH_X  (user_defined_path).

### Installing shared
Shared build will need to be performed on each system you wish to run Liquidfiles command line utility from. The following instructions assume 
you will use default installation paths.

#### Installing dependencies
If you already have OpenSSL or Curl libraries installed, you may skip some of the below steps.  If your system has OpenSSL or Curl installed in 
the default paths, you will not require the prefix option that allows user specified path for installation.

Install OpenSSL first:

1. Download latest OpenSSL version from official site.
2. Extract archive (e.g. tar –xz <openssl-VERSION>.tar.gz)
3. Go to directory (e.g. cd <openssl-VERSION>)
4. run Configure './config shared'
5. in the directory run 'make'
6. then run 'make install'

After installing OpenSSL, you need to install Curl:

1. Download latest curl version from official site.
2. Extract archive	(e.g. tar –xz <curl-VERSION>.tar.gz)
3. Go to directory	(e.g. cd <curl-VERSION>)
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
  --with-curl=$PATH_CURL   and/or  --with-ssl=$PATH_SSL **

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
2. Extract archive (e.g. tar –xz <openssl-VERSION>.tar.gz)
3. Go to directory (e.g. cd <openssl-VERSION>)
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
2. Extract archive (e.g. tar –xz <curl-VERSION>.tar.gz)
3. Go to directory (cd <curl-VERSION>)
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
* <code> liquidfiles &lt;command&gt; &lt;command_args&gt; </code>

The list of supported commands is:
* attach              Uploads given files to server.
* delete_filelink     Deletes the given filelink.
* download            Download given files.
* file_request        Sends the file request to specified user.
* filelink            Uploads given file and creates filelink on it.
* filelinks           Lists the available filelinks.
* get_api_key         Retrieves api key for the specified user.
* messages            Lists the available messages.
* send                Sends the file(s) to specified user.

To get command's detailed description, options and usage 'help' command can be used:
* <code> liquidfiles help &lt;command&gt; </code>

