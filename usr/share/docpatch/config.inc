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
if [ -z "${GIT+1}" ]; then
    GIT=$(which git)
fi
if [ -z "${GPG+1}" ]; then
    GPG=$(which gpg)
fi
if [ -z "${MARKDOWN2PDF+1}" ]; then
    MARKDOWN2PDF=$(which markdown2pdf)
fi
if [ -z "${PANDOC+1}" ]; then
    PANDOC=$(which pandoc)
fi
if [ -z "${QUILT+1}" ]; then
    QUILT=$(which quilt)
fi

## Helping tools:
fi
if [ -z "${AWK+1}" ]; then
    AWK=$(which awk)
fi
if [ -z "${CAT+1}" ]; then
    CAT=$(which cat)
fi
if [ -z "${CP+1}" ]; then
    CP=$(which cp)
fi
if [ -z "${ECHO+1}" ]; then
    ECHO=$(which echo)
fi
if [ -z "${GETOPT+1}" ]; then
    GETOPT=$(which getopt)
fi
if [ -z "${GREP+1}" ]; then
    GREP=$(which grep)
fi
if [ -z "${GZIP+1}" ]; then
    GZIP=$(which gzip)
fi
if [ -z "${HEAD+1}" ]; then
    HEAD=$(which head)
fi
if [ -z "${MAN+1}" ]; then
    MAN=$(which man)
fi
if [ -z "${MKDIR+1}" ]; then
    MKDIR=$(which mkdir)
fi
if [ -z "${PWD+1}" ]; then
    PWD=$(which pwd)
fi
if [ -z "${RENICE+1}" ]; then
    RENICE=$(which renice)
fi
if [ -z "${RM+1}" ]; then
    RM=$(which rm)
fi
if [ -z "${WC+1}" ]; then
    WC=$(which wc)
fi


##
## Paths
##

## References:
if [ -z "${REF_DIR+1}" ]; then
    REF_DIR="${BASE_DIR}/ref"
fi

## Source:
if [ -z "${SRC_DIR+1}" ]; then
    SRC_DIR="${BASE_DIR}/src"
fi

## Meta information:
if [ -z "${META_DIR+1}" ]; then
    META_DIR="${BASE_DIR}/meta"
fi

## Meta information:
if [ -z "${ETC_DIR+1}" ]; then
    ETC_DIR="${BASE_DIR}/etc"
fi

## Patches:
if [ -z "${PATCH_DIR+1}" ]; then
    PATCH_DIR="${BASE_DIR}/src/patches"
fi

## Templates:
if [ -z "${TPL_DIR+1}" ]; then
    TPL_DIR="${BASE_DIR}/tpl";
fi

## Outputs:
if [ -z "${OUTPUT_DIR+1}" ]; then
    OUTPUT_DIR="${BASE_DIR}/out"
fi

## Repository:
if [ -z "${REPO_DIR+1}" ]; then
    REPO_DIR="${BASE_DIR}/repo"
fi

## Path to shared libraries:
if [ -z "${LIB_DIR+1}" ]; then
    LIB_DIR="/usr/share/${PROJECT_NAME}"
fi

## Path to configuration files:
if [ -z "${CONFIG_DIR+1}" ]; then
    CONFIG_DIR="/etc/${PROJECT_NAME}"
fi

## Path to temporary files:
if [ -z "${TMP_DIR+1}" ]; then
    TMP_DIR="/tmp"
fi

## Path to examples (configuration and template files)
if [ -z "${EXAMPLE_DIR+1}" ]; then
    EXAMPLE_DIR="/usr/share/doc/${PROJECT_NAME}/examples"
fi

## Path to file with information about output formats:
if [ -z "${OUTPUT_FORMAT_FILE+1}" ]; then
    OUTPUT_FORMAT_FILE="${LIB_DIR}/output_formats.txt"
fi


##
## Logging and output
##

## Log events:
LOG_NONE=0
LOG_FATAL=1
LOG_ERROR=2
LOG_WARNING=4
LOG_NOTICE=8
LOG_INFO=16
LOG_DEBUG=32
LOG_ALL=63

## Main log file
if [ -z "${LOG_FILE+1}" ]; then
    LOG_FILE="${BASE_DIR}/${PROJECT_NAME}.log"
fi

## Log level:
if [ -z "${LOG_LEVEL+1}" ]; then
    LOG_LEVEL=$LOG_NONE
fi

## Verbose level:
if [ -z "${VERBOSITY+1}" ]; then
    VERBOSITY=$(($LOG_FATAL | $LOG_ERROR))
fi


##
## Meta information
## (based on Dublin Core Metadata Element Set, Version 1.1,
## <http://dublincore.org/documents/dces/>)
##

if [ -z "${CONTRIBUTOR+1}" ]; then
    CONTRIBUTOR=""
fi
if [ -z "${COVERAGE+1}" ]; then
    COVERAGE=""
fi
if [ -z "${CREATOR+1}" ]; then
    CREATOR=""
fi
if [ -z "${DATE+1}" ]; then
    DATE=""
fi
if [ -z "${DESCRIPTION+1}" ]; then
    DESCRIPTION=""
fi
if [ -z "${FORMAT+1}" ]; then
    FORMAT=""
fi
if [ -z "${IDENTIFIER+1}" ]; then
    IDENTIFIER=""
fi
if [ -z "${LANGUAGE+1}" ]; then
    LANGUAGE=""
fi
if [ -z "${PUBLISHER+1}" ]; then
    PUBLISHER=""
fi
if [ -z "${RELATION+1}" ]; then
    RELATION=""
fi
if [ -z "${RIGHTS+1}" ]; then
    RIGHTS=""
fi
if [ -z "${SOURCE+1}" ]; then
    SOURCE=""
fi
if [ -z "${SUBJECT+1}" ]; then
    SUBJECT=""
fi
if [ -z "${TITLE+1}" ]; then
    TITLE=""
fi
if [ -z "${TYPE+1}" ]; then
    TYPE=""
fi


##
## Meta files
##

## Docpatch configuration file:
if [ -z "${DOCPATCH_CONF_SOURCE+1}" ]; then
    DOCPATCH_CONF_SOURCE="${ETC_DIR}/docpatch.conf"
fi
if [ -z "${DOCPATCH_CONF_TARGET+1}" ]; then
    DOCPATCH_CONF_TARGET="${REPO_DIR}/.docpatch"
fi

## git configuration file:
if [ -z "${GIT_CONF_SOURCE+1}" ]; then
    GIT_CONF_SOURCE="${ETC_DIR}/git.conf"
fi
if [ -z "${GIT_CONF_TARGET+1}" ]; then
    GIT_CONF_TARGET="${REPO_DIR}/.git/config"
fi

## .gitignore:
if [ -z "${GIT_IGNORE_SOURCE+1}" ]; then
    GIT_IGNORE_SOURCE="${ETC_DIR}/gitignore"
fi
if [ -z "${GIT_IGNORE_SOURCE+1}" ]; then
    GIT_IGNORE_SOURCE="${REPO_DIR}/.gitignore"
fi

## README:
if [ -z "${README_SOURCE+1}" ]; then
    README_SOURCE="${ETC_DIR}/README.md"
fi
if [ -z "${README_TARGET+1}" ]; then
    README_TARGET="${REPO_DIR}/README"
fi


##
## Default runtime settings
##

## Input format:
if [ -z "${INPUT_FORMAT+1}" ]; then
    INPUT_FORMAT="markdown"
fi

## File extension of input format:
if [ -z "${INPUT_FORMAT_EXT+1}" ]; then
    INPUT_FORMAT_EXT=".md"
fi

## Output format:
if [ -z "${OUTPUT_FORMAT+1}" ]; then
    OUTPUT_FORMAT="pdf"
fi

## Revision taken from given arguments:
if [ -z "${ARG_REVISION+1}" ]; then
    ARG_REVISION=""
fi

## Current handled revision:
if [ -z "${REVISION+1}" ]; then
    REVISION=""
fi

## Amount of revisions:
if [ -z "${REVISIONS+1}" ]; then
    REVISIONS=0
fi

## All revisions that will be handled:
if [ -z "${LIST_OF_REVISIONS+1}" ]; then
    LIST_OF_REVISIONS=""
fi

## Repository:
if [ -z "${REPOSITORY+1}" ]; then
    REPOSITORY=""
fi

## Add OpenPGP signature taken from git configuration file:
if [ -z "${SIGN+1}" ]; then
    SIGN=0
fi

## Prints table of contents (TOC) to output:
if [ -z "${TOC+1}" ]; then
    TOC=0
fi

## Embed output format (otherwise create standalone version if supported):
if [ -z "${EMBED+1}" ]; then
    EMBED=0
fi

## Use simple mode to create output (otherwise create smart version if
## supported)
if [ -z "${SIMPLE+1}" ]; then
    SIMPLE=0
fi

## Own nice level:
if [ -z "${NICE_LEVEL+1}" ]; then
    NICE_LEVEL=0
fi

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
