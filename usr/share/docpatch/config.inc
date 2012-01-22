#!/bin/bash


## DocPatch -- patching documents that matter
## Copyright (C) 2012 Benjamin Heisig <http://benjamin.heisig.name/>
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.


##
## Default configuration
##


##
## About this tool
##


## Name
PROJECT_NAME="docpatch"
## Short description
PROJECT_SHORT_DESC="patching documents that matter"
## Author
PROJECT_AUTHOR="Benjamin Heisig <http://benjamin.heisig.name/>"
## Project website
PROJECT_WEBSITE="https://forge.benjaminheisig.de/${PROJECT_NAME}"
## Version
PROJECT_VERSION="0.1"


##
## Executables
##

## Main tools:
GIT=$(which git)
GPG=$(which gpg)
MARKDOWN2PDF=$(which markdown2pdf)
PANDOC=$(which pandoc)
QUILT=$(which quilt)

## Helping tools:
CAT=$(which cat)
CP=$(which cp)
ECHO=$(which echo)
GETOPT=$(which getopt)
HEAD=$(which head)
MAN=$(which man)
MKDIR=$(which mkdir)
PWD=$(which pwd)
RENICE=$(which renice)
RM=$(which rm)
WC=$(which wc)


##
## Paths
##

## References:
REF_DIR="${BASE_DIR}/ref"

## Source:
SRC_DIR="${BASE_DIR}/src"

## Meta information:
META_DIR="${BASE_DIR}/meta"

## Meta information:
ETC_DIR="${BASE_DIR}/etc"

## Patches:
PATCH_DIR="${BASE_DIR}/src/patches"

## Templates:
TPL_DIR="${BASE_DIR}/tpl";

## Outputs:
OUTPUT_DIR="${BASE_DIR}/out"

## Repository:
REPO_DIR="${BASE_DIR}/repo"

## Path to shared libraries:
LIB_DIR="/usr/share/${PROJECT_NAME}"

## Path to configuration files:
CONFIG_DIR="/etc/${PROJECT_NAME}"

## Path to temporary files:
TMP_DIR="/tmp/${PROJECT_NAME}"

## Path to examples (configuration and template files)
EXAMPLE_DIR="/usr/share/doc/${PROJECT_NAME}/examples"


##
## Logging and output
##

## Main log file
LOG_FILE="${BASE_DIR}/${PROJECT_NAME}.log"

## Log events:
LOG_NONE=0
LOG_FATAL=1
LOG_ERROR=2
LOG_WARNING=4
LOG_NOTICE=8
LOG_INFO=16
LOG_DEBUG=32
LOG_ALL=63

## Log level:
LOG_LEVEL=$(($LOG_FATAL | $LOG_ERROR))

## Verbose level:
VERBOSITY=$(($LOG_FATAL | $LOG_ERROR))

##
## Meta information
## (based on Dublin Core Metadata Element Set, Version 1.1,
## <http://dublincore.org/documents/dces/>)
##

CONTRIBUTOR=""
COVERAGE=""
CREATOR=""
DATE=""
DESCRIPTION=""
FORMAT=""
IDENTIFIER=""
LANGUAGE=""
PUBLISHER=""
RELATION=""
RIGHTS=""
SOURCE=""
SUBJECT=""
TITLE=""
TYPE=""


##
## Meta files
##

## Docpatch configuration file:
DOCPATCH_CONF_SOURCE="${ETC_DIR}/docpatch.conf"
DOCPATCH_CONF_TARGET="${REPO_DIR}/.docpatch"

## git configuration file:
GIT_CONF_SOURCE="${ETC_DIR}/git.conf"
GIT_CONF_TARGET="${REPO_DIR}/.git/config"

## .gitignore:
GIT_IGNORE_SOURCE="${ETC_DIR}/gitignore"
GIT_IGNORE_SOURCE="${REPO_DIR}/.gitignore"

## README:
README_SOURCE="${ETC_DIR}/README.md"
README_TARGET="${REPO_DIR}/README"


##
## Runtime settings
##

## Revision:
REVISION=""

## Amount of revisions:
REVISIONS=0

## All revisions that will be handled:
CURRENT_REVISIONS=""

## Repository:
REPOSITORY=""

## Add OpenPGP signature taken from git configuration file:
SIGN=0

## Own nice level:
NICE_LEVEL=0

## Process identifier (PID):
PID=0


##
## Output
##

## Print usage:
##   0 disabled [default]
##   1 enabled
PRINT_USAGE=0
## Print some information about this script:
##   0 disabled [default]
##   1 enabled
PRINT_VERSION=0
## Print license:
##   0 disabled [default]
##   1 enabled
PRINT_LICENSE=0
