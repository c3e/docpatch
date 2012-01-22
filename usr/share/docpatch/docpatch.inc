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
## Logic
##


## Determines which revisions are meant.
function determineRevisions {
  loginfo "Determining revisions..."

  logdebug "Parsing argument for revisions..."
  if [ -z "$REVISION" ]; then
      logdebug "No specific revision given. Assuming first revision."
      CURRENT_REVISIONS=0
    elif [ "$REVISION" == "first" ]; then
      logdebug "It's the first revision."
      CURRENT_REVISIONS=0
    elif [ "$REVISION" == "last" ]; then
      logdebug "It's the last revision."
      CURRENT_REVISIONS=$(($REVISIONS - 1))
    elif [ "$REVISION" == "all" ]; then
      logdebug "All revisions selected."

      for (( rev=1; rev < "$REVISIONS"; rev++ )); do
          if [ "$rev" -lt 0 ]; then
              logwarning"Inproperly named revision found: $rev"
              logerror "Failed to determine revisions."
              return 1
            fi

          if [ -n "$CURRENT_REVISIONS" ]; then
              CURRENT_REVISIONS="$CURRENT_REVISIONS "
            fi

          CURRENT_REVISIONS="${CURRENT_REVISIONS}$rev"
          logdebug "Appended revision $rev to list."
        done
    elif [[ "$REVISION" == ${REVISION//[^0-9]/} ]]; then
      if [ "$REVISION" -lt 0 ]; then
          logwarning"Inproperly named revision found: $REVISION"
          logerror "Failed to determine revisions."
          return 1
        fi
      logdebug "Revision $REVISION is meant."
      CURRENT_REVISIONS="$REVISION"
    else
      logdebug "Looking for more than one revision..."
      IFS=","
      for rev in $REVISION; do
          if [ "$rev" -lt 0 ]; then
              logwarning"Argument for one or more revisions is invalid."
              logerror "Failed to determine revisions."
              return 1
            fi

          if [ -n "$CURRENT_REVISIONS" ]; then
              CURRENT_REVISIONS="$CURRENT_REVISIONS "
            fi
          CURRENT_REVISIONS="${CURRENT_REVISIONS}$rev"
          logdebug "Appended revision $rev to list."
        done
      unset IFS
    fi
  logdebug "Found these revisions: $CURRENT_REVISIONS"

  logdebug "Revisions determined."
  return 0
}


## Counts revisions.
function countRevisions {
  loginfo "Counting revisions..."
  cd "$REPO_DIR"
  REVISIONS=`"$GIT" tag -l | "$WC" -l`
  if [ "$?" -gt 0 ]; then
      logerror "Cannot count revisions."
      return 1
    fi
  if [ "$REVISIONS" -eq 1 ]; then
      logdebug "There is 1 revision."
    else
      logdebug "There are $REVISIONS revisions."
    fi
  logdebug "Revisions counted."
  return 0
}


## Determines Repository
function determineRepository {
  loginfo "Determining repository..."

  logdebug "Checking argument..."
  if [ -z "$REPOSITORY" ]; then
      logdebug "Argument not set. Use default repository under ${REPO_DIR}."

      logdebug "Checking for existing repository..."
      if [ ! -d "${REPO_DIR}/.git/" ]; then
          lognotice "There is no repository under ${REPO_DIR}."
          logwarning"Cannot use default repository."
          logerror "Failed to determine repository."
          return 1
        fi
      logdebug "Repository found under ${REPO_DIR}."
    else
      logdebug "Cloning repository ${REPOSITORY}..."

      logdebug "Checking for existing repository..."
      if [ -d "${REPO_DIR}/.git/" ]; then
          lognotice "Repository found under ${REPO_DIR}."
          logwarning"Cannot clone repository ${REPOSITORY}."
          logerror "Failed to determine repository."
          return 1
        fi
      logdebug "There is no repository under ${REPO_DIR}."

      logdebug "Creating empty directory..."
      "$RM" -rf "$REPO_DIR"
      "$MKDIR" -p "$REPO_DIR"
      logdebug "Empty directory under $REPO_DIR created."

      logdebug "Cloning repository from ${REPOSITORY}..."
      "$GIT" clone "$REPOSITORY" "$REPO_DIR"
      if [ "$?" -gt 0 ]; then
          logwarning"Cannot clone repository $REPOSITORY under ${REPO_DIR}."
          logerror "Failed to determine repository."
          return 1
        fi
      logdebug "Repository cloned under ${REPO_DIR}."
    fi

  logdebug "Repository determined."
  return 0
}


## Switches repository to a revision.
##   $1 Revision
function switchRevision {
  loginfo "Switching revision..."

  logdebug "Changing into directory ${REPO_DIR}..."
  cd "$REPO_DIR"

  logdebug "Checking out revision ${1}..."
  "$GIT" checkout "$1"
  if [ "$?" -gt 0 ]; then
      logwarning"Cannot checkout revision ${1}."
      logerror "Failed to switch revision."
      return 1
    fi
  logdebug "Check out done."

  logdebug "Revision switched."
  return 0
}


## Creates output.
function createOutput {
  loginfo "Creating output..."

  logdebug "Iterating through each destinated revision..."
  for current_revision in $CURRENT_REVISIONS; do
      logdebug "Handling revision ${current_revision}..."

      switchRevision "$current_revision"
      if [ "$?" -gt 0 ]; then
          logwarning"Cannot handle revision ${current_revision}."
          logerror "Failed to create output."
          return 1
        fi

      produceOutput "$current_revision"
      if [ "$?" -gt 0 ]; then
          logwarning"Cannot handle revision ${current_revision}."
          logerror "Failed to create output."
          return 1
        fi

      logdebug "Handling done."
    done
  logdebug "Iteration done."

  logdebug "Output created."
  return 0
}


## Checks system requirements.
##   TODO Look for FIXMEs and TODOs and DELETEMEs
##   TODO Look for syntax errors in sources, meta information, patches and templates
function preChecks {
  loginfo "Checking system requirements..."

  logdebug "Checking location..."
  logdebug "Base name: $BASE_NAME"
  logdebug "Dir name: $DIR_NAME"

  logdebug "Checking directories..."

  logdebug "Checking references..."
  if [ ! -d "$REF_DIR" ]; then
      logwarning"References not found under ${REF_DIR}."
      logerror "System checks failed."
      return 1
    fi
  logdebug "References found in ${REF_DIR}."

  logdebug "Checking sources..."
  if [ ! -d "$SRC_DIR" ]; then
      logwarning"Sources not found under ${SRC_DIR}."
      logerror "System checks failed."
      return 1
    fi
  logdebug "Sources found in ${SRC_DIR}."

  logdebug "Checking meta information..."
  if [ ! -d "$META_DIR" ]; then
      logwarning"Meta information not found ${META_DIR}."
      logerror "System checks failed."
      return 1
    fi
  logdebug "Meta information found in ${META_DIR}."

  logdebug "Checking special information..."
  if [ ! -d "$ETC_DIR" ]; then
      logwarning"Special information not found under ${ETC_DIR}."
      logerror "System checks failed."
      return 1
    fi
  logdebug "Special information found in ${ETC_DIR}."

  logdebug "Checking patches..."
  if [ ! -d "$PATCH_DIR" ]; then
      logwarning"Patches not found under ${PATCH_DIR}."
      logerror "System checks failed."
      return 1
    fi
  logdebug "Patches found in ${PATCH_DIR}."

  logdebug "Checking templates..."
  if [ ! -d "$TPL_DIR" ]; then
      logwarning"Templates not found under ${TPL_DIR}."
      logerror "System checks failed."
      return 1
    fi
  logdebug "Templates found in ${TPL_DIR}."

  logdebug "Checking libraries..."
  if [ ! -d "$LIB_DIR" ]; then
      logwarning"Libraries not found under ${LIB_DIR}."
      logerror "System checks failed."
      return 1
    fi
  logdebug "Libraries found in ${LIB_DIR}."

  logdebug "Checking configuration..."
  if [ ! -d "$CONFIG_DIR" ]; then
      logdebug "Configuration not found under ${CONF_DIR}."
    else
      logdebug "Configuration found under ${CONF_DIR}."
    fi

  logdebug "Checking examples..."
  if [ ! -d "$EXAMPLE_DIR" ]; then
      logwarning"Examples not found under ${EXAMPLE_DIR}."
      logerror "System checks failed."
      return 1
    fi
  logdebug "Examples found in ${EXAMPLE_DIR}."

  logdebug "Checking temporary directory..."
  if [ ! -d "$TPL_DIR" ]; then
      logdebug "Temporary directory not found. Create it."
      "$MKDIR" -p "$TPL_DIR"
      if [ "$?" -gt 0 ]; then
          logwarning"Cannot create temporary directory under ${TPL_DIR}."
          logerror "System checks failed."
          return 1
        fi
      logdebug "Temporary directory created under ${TPL_DIR}."
    else
      logdebug "Temporary directory found in ${TPL_DIR}."
    fi

  logdebug "Directories checked."

  logdebug "System checked."
  return 0
}


## Initiates configuration files.
function initiateConfig {
  loginfo "Initiate configuration files..."
  logdebug "Copy files..."
  "$CP" ${EXAMPLE_DIR}/etc/* "$CONFIG_DIR"
  if [ "$?" -gt 0 ]; then
      logwarning"Cannot copy configuration files."
      logerror "Failed to initiate configuration files."
      return 1
    fi
  logdebug "Configuration files initiated."
  return 0
}


## Initiate auto completion in bash
function initiateAutoCompletion {
  loginfo "Initiate auto completion in bash..."
  logdebug "Copy file..."
  "$CP" ${EXAMPLE_DIR}/bash_completion.d/${PROJECT_NAME} /etc/bash_completion.d/
  if [ "$?" -gt 0 ]; then
      logwarning"Cannot copy auto completion file."
      logerror "Failed to initiate auto completion in bash."
      return 1
    fi
  logdebug "Auto completion in bash initiated."
  return 0
}


## Includes command specific file.
##   #1 Path to file
function includeCommand {
  loginfo "Including command specific file..."
  local cmd_file="${LIB_DIR}/$1"
  if [ ! -r "$cmd_file" ]; then
      logwarning"File $cmd_file does not exist or is not readable."
      logerror "Cannot include command specific file."
      return 1
    fi
  logdebug "Include file..."
  source "$cmd_file"
  logdebug "File $cmd_file included."
}

## Executes command.
##   $1 Command
##   $2 (optional) Log level. Defaults to "debug".
function exe {
  logdebug "Executing command..."
# FIXME
#  logdebug "Command: $2"
#  case "$VERBOSITY" in
#      0) verbose_level="-q" ;;
#      1|2) ;;
#      3) verbose_level="-v" ;;
#      4) verbose_level="-D" ;;
#      *) logwarning"Strange verbose level. Ignore it." ;;
#    esac
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
      *) logwarning "Unknown log event triggered.";;
    esac

  (($1 & $LOG_LEVEL))
  if [ "$?" -gt 0 ]; then
      "$ECHO" "[$level] $2" >> "$LOG_FILE"
    fi

  (($1 & $VERBOSITY))
  if [ "$?" -gt 0 ]; then
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
  loginfo "Calculate spent time..."
  local now=`date +%s`
  local sec=`expr $now - $START`
  local duration=""
  local div=0
  if [ "$sec" -ge 3600 ]; then
      div=`expr "$sec" \/ 3600`
      sec=`expr "$sec" - "$div" \* 3600`
      if [ "$div" = 1 ]; then
          duration="$div hour"
        elif [ "$div" -gt 1 ]; then
          duration="$div hours"
        fi
    fi
  if [ "$sec" -ge 60 ]; then
      if [ -n "$duration" ]; then
          duration="$duration and "
        fi
      div=`expr "$sec" \/ 60`
      sec=`expr "$sec" - "$div" \* 60`
      if [ "$div" = 1 ]; then
          duration="${duration}${div} minute"
        elif [ "$div" -gt 1 ]; then
          duration="${duration}${div} minutes"
        fi
    fi
  if [ "$sec" -ge 1 ]; then
      if [ -n "$duration" ]; then
          duration="$duration and "
        fi
      duration="${duration}${sec} second"
      if [ "$sec" -gt 1 ]; then
          duration="${duration}s"
        fi
    fi
  if [ -z "$duration" ]; then
      duration="0 seconds"
    fi
  logdebug "Spent time calculated."
  lognotice "Everything done after ${duration}. Exiting."
  return 0
}


## Clean up system
function cleanUp {
  loginfo "Cleaning up system..."

  logdebug "Removing temporary files..."
  "$RM" -f ${TMP_DIR}/${PID}_*
  if [ $? -gt 0 ]; then
      logwarning"Cannot remove temporary files."
      logerror "Failed to clean up."
      return 1
    fi
  logdebug "Temporary files '${TMP_DIR}/${PID}_*' removed."

  logdebug "Checking left repository..."
  if [ -d "${REPO_DIR}/.git" ]; then
      logdebug "Left repository found in ${REPO_DIR}."
      logdebug "Checking out master branch..."
      "$GIT" checkout master
      if [ "$?" -gt 0 ]; then
          logwarning"Cannot check out master branch."
          logerror "Failed to clean up."
          return 1
        fi
      logdebug "Checkout was successful."
    else
      logdebug "No repository left."
    fi

  logdebug "System is cleaned up."
  return 0
}


## Clean finishing
function finishing {
  loginfo "Finish operation..."
  cleanUp
  calculateSpentTime
  logdebug "Exit code: 0"
  exit 0
}


## Clean abortion
##   $1 Exit code
function abort {
  loginfo "Abort script..."
  cleanUp
  calculateSpentTime
  logdebug "Exit code: $1"
  logfatal "Operation failed."
  exit $1;
}


## Apply nice level
function applyNiceLevel {
  loginfo "Apply nice level..."
  PID="$$"
  logdebug "Current process ID is ${PID}."
  "$RENICE" "$NICE_LEVEL" "$PID" > /dev/null
  if [ "$?" -gt 0 ]; then
      logwarning"Re-nice failed."
      logerror "Failed to apply nice level."
      return 1
    fi
  logdebug "New nice level is ${NICE_LEVEL}."
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
  loginfo "Print global usage..."
  local cmd_placeholder="[command]"
  if [ -n "$COMMAND" -a "$COMMAND" != "help" ]; then
      cmd_placeholder="$COMMAND"
      prntLn "$COMMAND_DESC"
    fi
  prntLn "Usage: '$BASE_NAME [output] $cmd_placeholder [options]'"
  prntLn ""
  prntLn "Output:"
  prntLn "    -q\t\t\tBe quiet (for scripting)."
  prntLn "    -v\t\t\tBe verbose."
  prntLn "    -V\t\t\tBe verboser."
  prntLn "    -D\t\t\tBe verbosest (for debugging)."
  prntLn ""
  if [ -z "$COMMAND" ]; then
      prntLn "The most commonly used $PROJECT_NAME options are:"
      prntLn "    build\t\t${COMMAND_BUILD}"
      prntLn "    epub\t\t${COMMAND_EPUB}"
      prntLn "    pdf\t\t\t${COMMAND_PDF}"
    else
      prntLn "Options:"
      printCommandOptions
    fi
  prntLn ""
  prntLn "Information:"
  prntLn "    -h, --help\t\tShow this help and exit."
  prntLn "    --license\t\tShow license information and exit."
  prntLn "    --version\t\tShow information about this script and exit."
  prntLn ""
  if [ -n "$COMMAND" -a "$COMMAND" != "help" ]; then
      prntLn "See '$BASE_NAME help ${COMMAND}' for more information on this specific command."
    else
      prntLn "See '$BASE_NAME help [command]' for more information on a specific command."
    fi
  logdebug "Usage printed."
  return 0
}


## Print some information about this script
function printVersion {
  loginfo "Print some information about this script..."
  prntLn "$PROJECT_NAME $PROJECT_VERSION"
  prntLn "Copyright (C) 2011 $PROJECT_AUTHOR"
  prntLn "This program comes with ABSOLUTELY NO WARRANTY."
  prntLn "This is free software, and you are welcome to redistribute it"
  prntLn "under certain conditions. Type '--license' for details."
  logdebug "Information printed."
  return 0
}


## Print license information
function printLicense {
  loginfo "Print license information..."
  logdebug "Look for license text..."
  licenses[0]="/usr/share/common-licenses/GPL"
  licenses[1]="/usr/share/doc/licenses/gpl-3.0.txt"
  licenses[2]="/usr/share/doc/${PROJECT_NAME}/COPYING"
  for i in "${licenses[@]}"; do
      if [ -f "$i" ]; then
          "$CAT" "$i" 1>&2
          logdebug "Information printed."
          return 0
        fi
    done
  logwarning"Cannot find any fitting license text on this system."
  logerror "Failed to print license. But it's the GPL3+."
  return 1
}
