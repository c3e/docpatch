#!/bin/bash


## DocPatch -- patching documents that matter
## Copyright (C) 2012 Benjamin Heisig <https://benjamin.heisig.name/>
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
## Logic
##


## Checks system requirements.
function preChecks {
  loginfo $"Checking system requirements..."

  logdebug $"Checking location..."
  logdebug $"Base name is '${BASE_NAME}'."
  logdebug $"Dir name is '${DIR_NAME}'."

  logdebug $"Checking directories..."

  logdebug $"Checking references..."
  if [ ! -d "$REF_DIR" ]; then
      logwarning $"References not found under '${REF_DIR}'."
      logerror $"System checks failed."
      return 1
    fi
  logdebug $"References found in '${REF_DIR}'."

  logdebug $"Checking sources..."
  if [ ! -d "$SRC_DIR" ]; then
      logwarning $"Sources not found under '${SRC_DIR}'."
      logerror $"System checks failed."
      return 1
    fi
  logdebug $"Sources found in '${SRC_DIR}'."

  logdebug $"Checking meta information..."
  if [ ! -d "$META_DIR" ]; then
      logwarning $"Meta information not found '${META_DIR}'."
      logerror $"System checks failed."
      return 1
    fi
  logdebug $"Meta information found in '${META_DIR}'."

  logdebug $"Checking special information..."
  if [ ! -d "$ETC_DIR" ]; then
      logwarning $"Special information not found under '${ETC_DIR}'."
      logerror $"System checks failed."
      return 1
    fi
  logdebug $"Special information found in '${ETC_DIR}'."

  logdebug $"Checking patches..."
  if [ ! -d "$PATCH_DIR" ]; then
      logwarning $"Patches not found under '${PATCH_DIR}'."
      logerror $"System checks failed."
      return 1
    fi
  logdebug $"Patches found in '${PATCH_DIR}'."

  logdebug $"Checking templates..."
  if [ ! -d "$TPL_DIR" ]; then
      logwarning $"Templates not found under '${TPL_DIR}'."
      logerror $"System checks failed."
      return 1
    fi
  logdebug $"Templates found in '${TPL_DIR}'."

  logdebug $"Checking libraries..."
  if [ ! -d "$LIB_DIR" ]; then
      logwarning $"Libraries not found under '${LIB_DIR}'."
      logerror $"System checks failed."
      return 1
    fi
  logdebug $"Libraries found in '${LIB_DIR}'."

  logdebug $"Checking configuration..."
  if [ ! -d "$CONFIG_DIR" ]; then
      logdebug $"Configuration not found under '${CONF_DIR}'."
    else
      logdebug $"Configuration found under '${CONF_DIR}'."
    fi

  logdebug $"Checking examples..."
  if [ ! -d "$EXAMPLE_DIR" ]; then
      logwarning $"Examples not found under '${EXAMPLE_DIR}'."
      logerror $"System checks failed."
      return 1
    fi
  logdebug $"Examples found in '${EXAMPLE_DIR}'."

  logdebug $"Checking temporary directory..."
  if [ ! -d "$TPL_DIR" ]; then
      logdebug $"Temporary directory not found. Create it."
      exe "$MKDIR -p $TPL_DIR"
      if [ "$?" -gt 0 ]; then
          logwarning $"Cannot create temporary directory under '${TPL_DIR}'."
          logerror $"System checks failed."
          return 1
        fi
      logdebug $"Temporary directory created under '${TPL_DIR}'."
    else
      logdebug $"Temporary directory found under '${TPL_DIR}'."
    fi

  logdebug $"Directories checked."

  logdebug $"System checked."
  return 0
}


## Includes command specific file.
##   #1 Path to file
function includeCommand {
  loginfo $"Including command specific file..."
  local cmd_file="${LIB_DIR}/$1"
  if [ ! -r "$cmd_file" ]; then
      logwarning $"File '${cmd_file}' does not exist or is not readable."
      logerror $"Cannot include command specific file."
      return 1
    fi
  source "$cmd_file"
  logdebug $"File '${cmd_file}' included."
}

## Executes command.
##   $1 Command
function exe {
  logdebug $"Executing command..."

  logdebug $"Execute '${1}'"

  let "relevant = (($LOG_DEBUG & $VERBOSITY))"
  if [ "$relevant" -gt 0 ]; then
      eval $1
      local status="$?"
    else
      logdebug $"Suppress output."
      eval $1 &> /dev/null
      local status="$?"
    fi

  return $status
}


## Logs events to standard output and log file.
##   $1 Log level
##   $2 Log message
function log {
  local level=""

  case "$1" in
      "$LOG_DEBUG") level="debug";;
      "$LOG_INFO") level="info";;
      "$LOG_NOTICE") level="notice";;
      "$LOG_WARNING") level="warning";;
      "$LOG_ERROR") level="error";;
      "$LOG_FATAL") level="fatal";;
      *) logwarning $"Unknown log event triggered.";;
    esac

  let "relevant = (($1 & $LOG_LEVEL))"
  if [ "$relevant" -gt 0 ]; then
      "$ECHO" "[$level] $2" >> "$LOG_FILE"
    fi

  let "relevant = (($1 & $VERBOSITY))"
  if [ "$relevant" -gt 0 ]; then
      prntLn "[$level] $2"
    fi
}

function logdebug {
  log "$LOG_DEBUG" "$1"
}

function loginfo {
  log "$LOG_INFO" "$1"
}

function lognotice {
  log "$LOG_NOTICE" "$1"
}

function logwarning {
  log "$LOG_WARNING" "$1"
}

function logerror {
  log "$LOG_ERROR" "$1"
}

function logfatal {
  log "$LOG_FATAL" "$1"
}


## Calculate spent time
function calculateSpentTime {
  loginfo $"Calculate spent time..."
  local now=`date +%s`
  local sec=`expr $now - $START`
  local duration=""
  local div=0
  if [ "$sec" -ge 3600 ]; then
      div=`expr "$sec" \/ 3600`
      sec=`expr "$sec" - "$div" \* 3600`
      if [ "$div" = 1 ]; then
          duration=$"$div hour"
        elif [ "$div" -gt 1 ]; then
          duration=$"$div hours"
        fi
    fi
  if [ "$sec" -ge 60 ]; then
      if [ -n "$duration" ]; then
          duration=$"$duration and "
        fi
      div=`expr "$sec" \/ 60`
      sec=`expr "$sec" - "$div" \* 60`
      if [ "$div" = 1 ]; then
          duration=$"${duration}${div} minute"
        elif [ "$div" -gt 1 ]; then
          duration=$"${duration}${div} minutes"
        fi
    fi
  if [ "$sec" -ge 1 ]; then
      if [ -n "$duration" ]; then
          duration=$"$duration and "
        fi
      duration=$"${duration}${sec} second"
      if [ "$sec" -gt 1 ]; then
          duration=$"${duration}s"
        fi
    fi
  if [ -z "$duration" ]; then
      duration=$"0 seconds"
    fi
  logdebug $"Spent time calculated."
  lognotice $"Everything done after ${duration}. Exiting."
  return 0
}


## Clean up system
function cleanUp {
  loginfo $"Cleaning up system..."

  logdebug $"Removing temporary files..."
  exe "$RM -f ${TMP_DIR}/${PID}_*"
  if [ $? -gt 0 ]; then
      logwarning $"Cannot remove temporary files."
      logerror $"Failed to clean up."
      return 1
    fi
  logdebug $"Temporary files '${TMP_DIR}/${PID}_*' removed."

  logdebug $"Checking left repository..."
  if [ -d "${REPO_DIR}/.git" ]; then
      logdebug $"Left repository found under '${REPO_DIR}'."
      logdebug $"Checking out master branch..."
      exe "$GIT checkout master"
      if [ "$?" -gt 0 ]; then
          logwarning $"Cannot check out master branch."
          logerror $"Failed to clean up."
          return 1
        fi
      logdebug $"Checkout was successful."
    else
      logdebug $"No repository left under '${REPO_DIR}'."
    fi

  logdebug $"System is cleaned up."
  return 0
}


## Clean finishing
function finishing {
  loginfo $"Finish operation..."
  cleanUp
  calculateSpentTime
  logdebug $"Exit code: 0"
  exit 0
}


## Clean abortion
##   $1 Exit code
function abort {
  loginfo $"Abort script..."
  cleanUp
  calculateSpentTime
  logdebug $"Exit code: $1"
  logfatal $"Operation failed."
  exit $1;
}


## Apply nice level
function applyNiceLevel {
  loginfo $"Applying nice level..."

  PID="$$"
  logdebug $"Current process ID is '${PID}'."

  exe "$RENICE $NICE_LEVEL $PID"
  if [ "$?" -gt 0 ]; then
      logwarning $"Re-nice to '${NICE_LEVEL}' failed."
      logerror $"Failed to apply nice level."
      return 1
    fi

  logdebug $"New nice level is '${NICE_LEVEL}'."
  return 0
}


## Print line to standard output
##   $1 string
function prntLn {
  "$ECHO" -e "$1" 1>&2
  return 0
}


## Print line without trailing new line to standard output
##   $1 string
function prnt {
  "$ECHO" -e -n "$1" 1>&2
  return 0
}


## Print global usage
function printUsage {
  loginfo $"Printing global usage..."

  local cmd_placeholder="[command]"
  if [ -n "$COMMAND" -a "$COMMAND" != "help" ]; then
      cmd_placeholder="$COMMAND"
      prntLn "$COMMAND_DESC"
    else
      prntLn "$PROJECT_SHORT_DESC"
    fi
  prntLn $"Usage: '$BASE_NAME [output] $cmd_placeholder [options]'"
  prntLn ""
  prntLn $"Output:"
  prntLn $"    -q\t\t\tBe quiet (for scripting)."
  prntLn $"    -v\t\t\tBe verbose."
  prntLn $"    -V\t\t\tBe verboser."
  prntLn $"    -D\t\t\tBe verbosest (for debugging)."
  prntLn ""
  if [ -z "$COMMAND" ]; then
      prntLn $"The most commonly used $PROJECT_NAME options are:"
      prntLn "    build\t\t${COMMAND_BUILD}"
      prntLn "    create\t\t${COMMAND_CREATE}"
    else
      prntLn $"Options:"
      printCommandOptions
    fi
  prntLn ""
  prntLn $"Information:"
  prntLn $"    -h, --help\t\tShow this help and exit."
  prntLn $"    --license\t\tShow license information and exit."
  prntLn $"    --version\t\tShow information about this script and exit."
  prntLn ""
  if [ -n "$COMMAND" -a "$COMMAND" != "help" ]; then
      prntLn $"See '$BASE_NAME help ${COMMAND}' for more information on this specific command."
    else
      prntLn $"See '$BASE_NAME help [command]' for more information on a specific command."
    fi

  logdebug $"Usage printed."
  return 0
}


## Print some information about this script
function printVersion {
  loginfo $"Printing some information about this script..."

  prntLn "$PROJECT_NAME $PROJECT_VERSION"
  prntLn "Copyright (C) 2012 $PROJECT_AUTHOR"
  prntLn "This program comes with ABSOLUTELY NO WARRANTY."
  prntLn "This is free software, and you are welcome to redistribute it"
  prntLn "under certain conditions. Type '--license' for details."

  logdebug $"Information printed."
  return 0
}


## Print license information
function printLicense {
  loginfo $"Printing license information..."

  logdebug $"Look for license text..."

  licenses[0]="/usr/share/common-licenses/GPL"
  licenses[1]="/usr/share/doc/licenses/gpl-3.0.txt"
  licenses[2]="/usr/share/doc/${PROJECT_NAME}/COPYING"

  for i in "${licenses[@]}"; do
      if [ -r "$i" ]; then
          logdebug "License text found under '${i}'."
          "$CAT" "$i" 1>&2
          logdebug $"License information printed."
          return 0
        fi
    done

  logwarning $"Cannot find any fitting license text on this system."
  logerror $"Failed to print license. But it's the GPL3+."
  return 1
}
