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
  info "Determining revisions..."

  debug "Parsing argument for revisions..."
  if [ -z "$REVISION" ]; then
      debug "No specific revision given. Assuming first revision."
      CURRENT_REVISIONS=0
    elif [ "$REVISION" == "first" ]; then
      debug "It's the first revision."
      CURRENT_REVISIONS=0
    elif [ "$REVISION" == "last" ]; then
      debug "It's the last revision."
      CURRENT_REVISIONS=$(($REVISIONS - 1))
    elif [ "$REVISION" == "all" ]; then
      debug "All revisions selected."

      for (( rev=1; rev < "$REVISIONS"; rev++ )); do
          if [ "$rev" -lt 0 ]; then
              warning "Inproperly named revision found: $rev"
              log "$LOG_ERROR "Failed to determine revisions."
              return 1
            fi

          if [ -n "$CURRENT_REVISIONS" ]; then
              CURRENT_REVISIONS="$CURRENT_REVISIONS "
            fi

          CURRENT_REVISIONS="${CURRENT_REVISIONS}$rev"
          debug "Appended revision $rev to list."
        done
    elif [[ "$REVISION" == ${REVISION//[^0-9]/} ]]; then
      if [ "$REVISION" -lt 0 ]; then
          warning "Inproperly named revision found: $REVISION"
          log "$LOG_ERROR "Failed to determine revisions."
          return 1
        fi
      debug "Revision $REVISION is meant."
      CURRENT_REVISIONS="$REVISION"
    else
      debug "Looking for more than one revision..."
      IFS=","
      for rev in $REVISION; do
          if [ "$rev" -lt 0 ]; then
              warning "Argument for one or more revisions is invalid."
              log "$LOG_ERROR "Failed to determine revisions."
              return 1
            fi

          if [ -n "$CURRENT_REVISIONS" ]; then
              CURRENT_REVISIONS="$CURRENT_REVISIONS "
            fi
          CURRENT_REVISIONS="${CURRENT_REVISIONS}$rev"
          debug "Appended revision $rev to list."
        done
      unset IFS
    fi
  debug "Found these revisions: $CURRENT_REVISIONS"

  debug "Revisions determined."
  return 0
}


## Counts revisions.
function countRevisions {
  info "Counting revisions..."
  cd "$REPO_DIR"
  REVISIONS=`"$GIT" tag -l | "$WC" -l`
  if [ "$?" -gt 0 ]; then
      log "$LOG_ERROR "Cannot count revisions."
      return 1
    fi
  if [ "$REVISIONS" -eq 1 ]; then
      debug "There is 1 revision."
    else
      debug "There are $REVISIONS revisions."
    fi
  debug "Revisions counted."
  return 0
}


## Determines Repository
function determineRepository {
  info "Determining repository..."

  debug "Checking argument..."
  if [ -z "$REPOSITORY" ]; then
      debug "Argument not set. Use default repository under ${REPO_DIR}."

      debug "Checking for existing repository..."
      if [ ! -d "${REPO_DIR}/.git/" ]; then
          notice "There is no repository under ${REPO_DIR}."
          warning "Cannot use default repository."
          log "$LOG_ERROR "Failed to determine repository."
          return 1
        fi
      debug "Repository found under ${REPO_DIR}."
    else
      debug "Cloning repository ${REPOSITORY}..."

      debug "Checking for existing repository..."
      if [ -d "${REPO_DIR}/.git/" ]; then
          notice "Repository found under ${REPO_DIR}."
          warning "Cannot clone repository ${REPOSITORY}."
          log "$LOG_ERROR "Failed to determine repository."
          return 1
        fi
      debug "There is no repository under ${REPO_DIR}."

      debug "Creating empty directory..."
      "$RM" -rf "$REPO_DIR"
      "$MKDIR" -p "$REPO_DIR"
      debug "Empty directory under $REPO_DIR created."

      debug "Cloning repository from ${REPOSITORY}..."
      "$GIT" clone "$REPOSITORY" "$REPO_DIR"
      if [ "$?" -gt 0 ]; then
          warning "Cannot clone repository $REPOSITORY under ${REPO_DIR}."
          log "$LOG_ERROR "Failed to determine repository."
          return 1
        fi
      debug "Repository cloned under ${REPO_DIR}."
    fi

  debug "Repository determined."
  return 0
}


## Switches repository to a revision.
##   $1 Revision
function switchRevision {
  info "Switching revision..."

  debug "Changing into directory ${REPO_DIR}..."
  cd "$REPO_DIR"

  debug "Checking out revision ${1}..."
  "$GIT" checkout "$1"
  if [ "$?" -gt 0 ]; then
      warning "Cannot checkout revision ${1}."
      log "$LOG_ERROR "Failed to switch revision."
      return 1
    fi
  debug "Check out done."

  debug "Revision switched."
  return 0
}


## Creates output.
function createOutput {
  info "Creating output..."

  debug "Iterating through each destinated revision..."
  for current_revision in $CURRENT_REVISIONS; do
      debug "Handling revision ${current_revision}..."

      switchRevision "$current_revision"
      if [ "$?" -gt 0 ]; then
          warning "Cannot handle revision ${current_revision}."
          log "$LOG_ERROR "Failed to create output."
          return 1
        fi

      produceOutput "$current_revision"
      if [ "$?" -gt 0 ]; then
          warning "Cannot handle revision ${current_revision}."
          log "$LOG_ERROR "Failed to create output."
          return 1
        fi

      debug "Handling done."
    done
  debug "Iteration done."

  debug "Output created."
  return 0
}


## Checks system requirements.
##   TODO Look for FIXMEs and TODOs and DELETEMEs
##   TODO Look for syntax errors in sources, meta information, patches and templates
function preChecks {
  info "Checking system requirements..."

  debug "Checking location..."
  debug "Base name: $BASE_NAME"
  debug "Dir name: $DIR_NAME"

  debug "Checking directories..."

  debug "Checking references..."
  if [ ! -d "$REF_DIR" ]; then
      warning "References not found under ${REF_DIR}."
      log "$LOG_ERROR "System checks failed."
      return 1
    fi
  debug "References found in ${REF_DIR}."

  debug "Checking sources..."
  if [ ! -d "$SRC_DIR" ]; then
      warning "Sources not found under ${SRC_DIR}."
      log "$LOG_ERROR "System checks failed."
      return 1
    fi
  debug "Sources found in ${SRC_DIR}."

  debug "Checking meta information..."
  if [ ! -d "$META_DIR" ]; then
      warning "Meta information not found ${META_DIR}."
      log "$LOG_ERROR "System checks failed."
      return 1
    fi
  debug "Meta information found in ${META_DIR}."

  debug "Checking special information..."
  if [ ! -d "$ETC_DIR" ]; then
      warning "Special information not found under ${ETC_DIR}."
      log "$LOG_ERROR "System checks failed."
      return 1
    fi
  debug "Special information found in ${ETC_DIR}."

  debug "Checking patches..."
  if [ ! -d "$PATCH_DIR" ]; then
      warning "Patches not found under ${PATCH_DIR}."
      log "$LOG_ERROR "System checks failed."
      return 1
    fi
  debug "Patches found in ${PATCH_DIR}."

  debug "Checking templates..."
  if [ ! -d "$TPL_DIR" ]; then
      warning "Templates not found under ${TPL_DIR}."
      log "$LOG_ERROR "System checks failed."
      return 1
    fi
  debug "Templates found in ${TPL_DIR}."

  debug "Checking libraries..."
  if [ ! -d "$LIB_DIR" ]; then
      warning "Libraries not found under ${LIB_DIR}."
      log "$LOG_ERROR "System checks failed."
      return 1
    fi
  debug "Libraries found in ${LIB_DIR}."

  debug "Checking configuration..."
  if [ ! -d "$CONFIG_DIR" ]; then
      debug "Configuration not found under ${CONF_DIR}."
    else
      debug "Configuration found under ${CONF_DIR}."
    fi

  debug "Checking examples..."
  if [ ! -d "$EXAMPLE_DIR" ]; then
      warning "Examples not found under ${EXAMPLE_DIR}."
      log "$LOG_ERROR "System checks failed."
      return 1
    fi
  debug "Examples found in ${EXAMPLE_DIR}."

  debug "Checking temporary directory..."
  if [ ! -d "$TPL_DIR" ]; then
      debug "Temporary directory not found. Create it."
      "$MKDIR" -p "$TPL_DIR"
      if [ "$?" -gt 0 ]; then
          warning "Cannot create temporary directory under ${TPL_DIR}."
          log "$LOG_ERROR "System checks failed."
          return 1
        fi
      debug "Temporary directory created under ${TPL_DIR}."
    else
      debug "Temporary directory found in ${TPL_DIR}."
    fi

  debug "Directories checked."

  debug "System checked."
  return 0
}


## Initiates configuration files.
function initiateConfig {
  info "Initiate configuration files..."
  debug "Copy files..."
  "$CP" ${EXAMPLE_DIR}/etc/* "$CONFIG_DIR"
  if [ "$?" -gt 0 ]; then
      warning "Cannot copy configuration files."
      log "$LOG_ERROR "Failed to initiate configuration files."
      return 1
    fi
  debug "Configuration files initiated."
  return 0
}


## Initiate auto completion in bash
function initiateAutoCompletion {
  info "Initiate auto completion in bash..."
  debug "Copy file..."
  "$CP" ${EXAMPLE_DIR}/bash_completion.d/${PROJECT_NAME} /etc/bash_completion.d/
  if [ "$?" -gt 0 ]; then
      warning "Cannot copy auto completion file."
      log "$LOG_ERROR "Failed to initiate auto completion in bash."
      return 1
    fi
  debug "Auto completion in bash initiated."
  return 0
}


## Includes command specific file.
##   #1 Path to file
function includeCommand {
  info "Including command specific file..."
  local cmd_file="${LIB_DIR}/$1"
  if [ ! -r "$cmd_file" ]; then
      warning "File $cmd_file does not exist or is not readable."
      log "$LOG_ERROR "Cannot include command specific file."
      return 1
    fi
  debug "Include file..."
  source "$cmd_file"
  debug "File $cmd_file included."
}

## Executes command.
##   $1 Command
##   $2 (optional) Log level. Defaults to "debug".
function exe {
  debug "Executing command..."
# FIXME
#  debug "Command: $2"
#  case "$VERBOSITY" in
#      0) verbose_level="-q" ;;
#      1|2) ;;
#      3) verbose_level="-v" ;;
#      4) verbose_level="-D" ;;
#      *) warning "Strange verbose level. Ignore it." ;;
#    esac
}


## Logs events to standard output and log file.
##   $1 Log level
##   $2 Log message
function log {
  if [ "$1" & "$LOG_LEVEL" ]; then
      "$ECHO" "[$1] $2" >> "$LOG_FILE"
    fi


  if [ "$1" & "$VERBOSITY" ]; then
      prntLn "[$1] $2"
    fi

  return 0
}

function debug {
  return log "$LOG_DEBUG" $1
}

function info {
  return log "$LOG_INFO" $1
}

function notice {
  return log "$LOG_NOTICE" $1
}

function warning {
  return log "$LOG_WARNING" $1
}

function error {
  return log "$LOG_ERROR" $1
}

function fatal {
  return log "$LOG_FATAL" $1
}


## Calculate spent time
function calculateSpentTime {
  info "Calculate spent time..."
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
  debug "Spent time calculated."
  notice "Everything done after ${duration}. Exiting."
  return 0
}


## Clean up system
function cleanUp {
  info "Cleaning up system..."

  debug "Removing temporary files..."
  "$RM" -f ${TMP_DIR}/${PID}_*
  if [ $? -gt 0 ]; then
      warning "Cannot remove temporary files."
      log "$LOG_ERROR "Failed to clean up."
      return 1
    fi
  debug "Temporary files '${TMP_DIR}/${PID}_*' removed."

  debug "Checking left repository..."
  if [ -d "${REPO_DIR}/.git" ]; then
      debug "Left repository found in ${REPO_DIR}."
      debug "Checking out master branch..."
      "$GIT" checkout master
      if [ "$?" -gt 0 ]; then
          warning "Cannot check out master branch."
          log "$LOG_ERROR "Failed to clean up."
          return 1
        fi
      debug "Checkout was successful."
    else
      debug "No repository left."
    fi

  debug "System is cleaned up."
  return 0
}


## Clean finishing
function finishing {
  info "Finish operation..."
  cleanUp
  calculateSpentTime
  debug "Exit code: 0"
  exit 0
}


## Clean abortion
##   $1 Exit code
function abort {
  info "Abort script..."
  cleanUp
  calculateSpentTime
  debug "Exit code: $1"
  log "$LOG_FATAL "Operation failed."
  exit $1;
}


## Apply nice level
function applyNiceLevel {
  info "Apply nice level..."
  PID="$$"
  debug "Current process ID is ${PID}."
  "$RENICE" "$NICE_LEVEL" "$PID" > /dev/null
  if [ "$?" -gt 0 ]; then
      warning "Re-nice failed."
      log "$LOG_ERROR "Failed to apply nice level."
      return 1
    fi
  debug "New nice level is ${NICE_LEVEL}."
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
  info "Print global usage..."
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
  debug "Usage printed."
  return 0
}


## Print some information about this script
function printVersion {
  info "Print some information about this script..."
  prntLn "$PROJECT_NAME $PROJECT_VERSION"
  prntLn "Copyright (C) 2011 $PROJECT_AUTHOR"
  prntLn "This program comes with ABSOLUTELY NO WARRANTY."
  prntLn "This is free software, and you are welcome to redistribute it"
  prntLn "under certain conditions. Type '--license' for details."
  debug "Information printed."
  return 0
}


## Print license information
function printLicense {
  info "Print license information..."
  debug "Look for license text..."
  licenses[0]="/usr/share/common-licenses/GPL"
  licenses[1]="/usr/share/doc/licenses/gpl-3.0.txt"
  licenses[2]="/usr/share/doc/${PROJECT_NAME}/COPYING"
  for i in "${licenses[@]}"; do
      if [ -f "$i" ]; then
          "$CAT" "$i" 1>&2
          debug "Information printed."
          return 0
        fi
    done
  warning "Cannot find any fitting license text on this system."
  log "$LOG_ERROR "Failed to print license. But it's the GPL3+."
  return 1
}
