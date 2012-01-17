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
  log "info" "Determining revisions..."

  log "debug" "Parsing argument for revisions..."
  if [ "$REVISION" -eq "first" ]; then
      log "debug" "It's the first revision."
      CURRENT_REVISIONS=0
    else if [ "$REVISION" -eq "last" ]; then
      log "debug" "It's the last revision."
      CURRENT_REVISIONS=$(("$REVISIONS" - 1))
    else if [ "$REVISION" -eq "all" ]; then
      log "debug" "All revisions selected."
      countRevisions
      if [ "$?" -gt 0 ]; then
          log "error" "Failed to determine revisions."
          return 1
        fi
      for (( rev=1; rev < "$REVISIONS"; rev++ )); do
          if [ "$rev" -lt 0 ]; then
              log "warning" "Inproperly named revision found: $rev"
              log "error" "Failed to determine revisions."
              return 1
            fi
          CURRENT_REVISIONS="$rev "
          log "debug" "Appended revision $rev to list."
        done
    else if [ "$REVISION" -ge 0 ]; then
      log "debug" "Revision $REVISION is meant."
      CURRENT_REVISIONS="$REVISION"
    else
      log "debug" "Looking for more than one revision..."
      IFS=$','
      for rev in "$REVISION"; do
          if [ "$rev" -lt 0 ]; then
              log "warning" "Argument for one or more revisions is invalid."
              log "error" "Failed to determine revisions."
              return 1
            fi
          CURRENT_REVISIONS="$rev "
          log "debug" "Appended revision $rev to list."
        done
      unset IFS
    fi
  log "debug" "Found these revisions: $CURRENT_REVISIONS"

  log "debug" "Revisions determined."
  return 0
}


## Counts revisions.
function countRevisions {
  log "info" "Counting revisions..."
  cd "$REPO_DIR"
  REVISIONS=`"$GIT" tag -l | "$WC" -l`
  if [ "$?" -gt 0 ]; then
      log "error" "Cannot count revisions."
      return 1
    fi
  if [ "$REVISIONS" -eq 1 ]; then
      log "debug" "There is 1 revision."
    else
      log "debug" "There are $REVISIONS revisions."
    fi
  log "debug" "Revisions counted."
  return 0
}


## Determines Repository
function determineRepository {
  log "info" "Determining repository..."

  log "debug" "Checking argument..."
  if [ -z "$REPOSITORY" ]; then
      log "debug" "Argument not set. Use default repository under ${REPO_DIR}."

      log "debug" "Checking for existing repository..."
      if [ !-d "${REPO_DIR}/.git" ]; then
          log "notice" "There is no repository under ${REPO_DIR}."
          log "warning" "Cannot use default repository."
          log "error" "Failed to determine repository."
          return 1
        fi
      log "debug" "Repository found under ${REPO_DIR}."
    else
      log "debug" "Cloning repository ${REPOSITORY}..."

      log "debug" "Checking for existing repository..."
      if [ -d "${REPO_DIR}/.git" ]; then
          log "notice" "Repository found under ${REPO_DIR}."
          log "warning" "Cannot clone repository ${REPOSITORY}."
          log "error" "Failed to determine repository."
          return 1
        fi
      log "debug" "There is no repository under ${REPO_DIR}."

      log "debug" "Creating empty directory..."
      "$RM" -rf "$REPO_DIR"
      "$MKDIR" -p "$REPO_DIR"
      log "debug" "Empty directory under $REPO_DIR created."

      log "debug" "Cloning repository from ${REPOSITORY}..."
      "$GIT" clone "$REPOSITORY" "$REPO_DIR"
      if [ "$?" -gt 0 ]; then
          log "warning" "Cannot clone repository $REPOSITORY under ${REPO_DIR}."
          log "error" "Failed to determine repository."
          return 1
        fi
      log "debug" "Repository cloned under ${REPO_DIR}."
    fi

  log "debug" "Repository determined."
  return 0
}


## Switches repository to a revision.
##   $1 Revision
function switchRevision {
  log "info" "Switching revision..."

  log "debug" "Changing into directory ${REPO_DIR}..."
  cd "$REPO_DIR"

  log "debug" "Checking out revision ${1}..."
  "$GIT" checkout "$1"
  if [ "$?" -gt 0 ]; then
      log "warning" "Cannot checkout revision ${1}."
      log "error" "Failed to switch revision."
      return 1
    fi
  log "debug" "Check out done."

  log "debug" "Revision switched."
  return 0
}


## Creates output.
function createOutput {
  log "info" "Creating output..."

  log "debug" "Iterating through each destinated revision..."
  for rev in "$CURRENT_REVISIONS"; do
      log "debug" "Handling revision ${rev}..."

      switchRevision "$rev"
      if [ "$?" -gt 0 ]; then
          log "warning" "Cannot handle revision ${rev}."
          log "error" "Failed to create output."
          return 1
        fi

      produceOutput "$rev"
      if [ "$?" -gt 0 ]; then
          log "warning" "Cannot handle revision ${rev}."
          log "error" "Failed to create output."
          return 1
        fi

      log "debug" "Handling done."
    done
  log "debug" "Iteration done."

  log "debug" "Output created."
  return 0
}


## Checks system requirements.
##   TODO Look for FIXMEs and TODOs and DELETEMEs
##   TODO Look for syntax errors in sources, meta information, patches and templates
function preChecks {
  log "info" "Checking system requirements..."

  log "debug" "Checking directories..."

  log "debug" "Checking references..."
  if [ !-d "$REF_DIR" ]; then
      log "warning" "References not found."
      log "error" "System checks failed."
      return 1
    fi
  log "debug" "References found in ${REF_DIR}."

  log "debug" "Checking sources..."
  if [ !-d "$SRC_DIR" ]; then
      log "warning" "Sources not found."
      log "error" "System checks failed."
      return 1
    fi
  log "debug" "Sources found in ${SRC_DIR}."

  log "debug" "Checking meta information..."
  if [ !-d "$META_DIR" ]; then
      log "warning" "Meta information not found."
      log "error" "System checks failed."
      return 1
    fi
  log "debug" "References found in ${META_DIR}."

  log "debug" "Checking patches..."
  if [ !-d "$PATCH_DIR" ]; then
      log "warning" "Patches not found."
      log "error" "System checks failed."
      return 1
    fi
  log "debug" "Patches found in ${PATCH_DIR}."

  log "debug" "Checking templates..."
  if [ !-d "$TPL_DIR" ]; then
      log "warning" "Templates not found."
      log "error" "System checks failed."
      return 1
    fi
  log "debug" "Templates found in ${TPL_DIR}."

  log "debug" "Checking libraries..."
  if [ !-d "$LIB_DIR" ]; then
      log "warning" "Libraries not found."
      log "error" "System checks failed."
      return 1
    fi
  log "debug" "Libraries found in ${LIB_DIR}."

  log "debug" "Checking configuration..."
  if [ !-d "$CONFIG_DIR" ]; then
      log "warning" "Configuration not found."
      log "error" "System checks failed."
      return 1
    fi
  log "debug" "Configuration found in ${CONF_DIR}."

  log "debug" "Checking examples..."
  if [ !-d "$EXAMPLE_DIR" ]; then
      log "warning" "Examples not found."
      log "error" "System checks failed."
      return 1
    fi
  log "debug" "Examples found in ${EXAMPLE_DIR}."

  log "debug" "Checking temporary directory..."
  if [ !-d "$TPL_DIR" ]; then
      log "debug" "Temporary directory not found. Create it."
      "$MKDIR" -p "$TPL_DIR"
      if [ "$?" -gt 0 ]; then
          log "warning" "Cannot create temporary directory under ${TPL_DIR}."
          log "error" "System checks failed."
          return 1
        fi
      log "debug" "Temporary directory created under ${TPL_DIR}."
    else
      log "debug" "Temporary directory found in ${TPL_DIR}."
    fi

  log "debug" "Directories checked."

  log "debug" "System checked."
  return 0
}


## Initiates configuration files.
function initiateConfig {
  log "info" "Initiate configuration files..."
  log "debug" "Copy files..."
  "$CP" ${EXAMPLE_DIR}etc/* "$CONFIG_DIR"
  if [ "$?" -gt 0 ]; then
      log "warning" "Cannot copy configuration files."
      log "error" "Failed to initiate configuration files."
      return 1
    fi
  log "debug" "Configuration files initiated."
  return 0
}


## Initiate auto completion in bash
function initiateAutoCompletion {
  log "info" "Initiate auto completion in bash..."
  log "debug" "Copy file..."
  "$CP" ${EXAMPLE_DIR}bash_completion.d/${PROJECT_NAME} /etc/bash_completion.d/
  if [ "$?" -gt 0 ]; then
      log "warning" "Cannot copy auto completion file."
      log "error" "Failed to initiate auto completion in bash."
      return 1
    fi
  log "debug" "Auto completion in bash initiated."
  return 0
}


## Include command specific file
##   #1 Path to file
function includeCommand {
  log "info" "Include command specific file..."
  local cmd_file="${LIB_DIR}$1"
  if [ ! -r "$cmd_file" ]; then
      log "warning" "File $cmd_file does not exist or is not readable."
      log "error" "Cannot include command specific file."
      return 1
    fi
  log "debug" "Include file..."
  source "$cmd_file"
  log "debug" "File $cmd_file included."
}


## Log events to standard output and log file
##   $1 Log level
##   $2 Log message
function log {
  local now=`"$DATE" +%F_%H:%M`
  if [ \( "$1" = "fatal" -a "$VERBOSITY" -ge 1 \) -o \
       \( "$1" = "error" -a "$VERBOSITY" -ge 1 \) -o \
       \( "$1" = "warning" -a "$VERBOSITY" -ge 2 \) -o \
       \( "$1" = "notice" -a "$VERBOSITY" -ge 2 \) -o \
       \( "$1" = "info" -a "$VERBOSITY" -ge 3 \) -o \
       \( "$1" = "debug" -a "$VERBOSITY" -ge 4 \) ]; then
        prntLn "[$1] $2"
    fi
  if [ \( "$1" = "fatal" -a "$LOGLEVEL" -ge 1 \) -o \
       \( "$1" = "error" -a "$LOGLEVEL" -ge 1 \) -o \
       \( "$1" = "warning" -a "$LOGLEVEL" -ge 2 \) -o \
       \( "$1" = "notice" -a "$LOGLEVEL" -ge 2 \) -o \
       \( "$1" = "info" -a "$LOGLEVEL" -ge 3 \) -o \
       \( "$1" = "debug" -a "$LOGLEVEL" -ge 4 \) ]; then
      "$ECHO" "[$1] $2" >> "$LOG_FILE"
    fi
  # Count log events
  if [ "$1" == "fatal" ]; then
      LOG_FATAL=$((LOG_FATAL + 1))
    fi
  if [ "$1" == "error" ]; then
      LOG_ERROR=$((LOG_ERROR + 1))
    fi
  if [ "$1" == "warning" ]; then
      LOG_WARNING=$((LOG_WARNING + 1))
    fi
  if [ "$1" == "notice" ]; then
      LOG_NOTICE=$((LOG_NOTICE + 1))
    fi
  if [ "$1" == "info" ]; then
      LOG_INFO=$((LOG_INFO + 1))
    fi
  if [ "$1" == "debug" ]; then
      LOG_DEBUG=$((LOG_DEBUG + 1))
    fi
  return 0
}


## Calculate spent time
function calculateSpentTime {
  log "info" "Calculate spent time..."
  local now=`"$DATE" +%s`
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
  log "debug" "Spent time calculated."
  log "notice" "Everything done after ${duration}. Exiting."
  return 0
}


## Clean up system
function cleanUp {
  log "info" "Cleaning up system..."

  log "debug" "Remove temporary files..."
  "$RM" -f ${TMP_DIR}${PID}_*
  if [ $? -gt 0 ]; then
      log "warning" "Cannot remove temporary files."
      log "error" "Failed to clean up."
      return 1
    fi
  log "debug" "Temporary files '${TMP_DIR}${PID}_*' removed."

  log "debug" "Checking left repository..."
  if [ -d "{$REPO_DIR}/.git" ]; then
      log "debug" "Left repository found in ${REPO_DIR}."
      log "debug" "Checking out master branch..."
      "$GIT" checkout master
      if [ "$?" -gt 0 ]; then
          log "warning" "Cannot check out master branch."
          log "error" "Failed to clean up."
          return 1
        fi
      log "debug" "Checkout was successful."
    fi

  log "debug" "System is cleaned up."
  return 0
}


## Clean finishing
function finishing {
  log "info" "Finish operation..."
  cleanUp
  calculateSpentTime
  log "debug" "Exit code: 0"
  exit 0
}


## Clean abortion
##   $1 Exit code
function abort {
  log "info" "Abort script..."
  cleanUp
  calculateSpentTime
  log "debug" "Exit code: $1"
  log "fatal" "Operation failed."
  exit $1;
}


## Apply nice level
function applyNiceLevel {
  log "info" "Apply nice level..."
  PID="$$"
  log "debug" "Current process ID is ${PID}."
  "$RENICE" "$NICE_LEVEL" "$PID" > /dev/null
  if [ "$?" -gt 0 ]; then
      log "warning" "Re-nice failed."
      log "error" "Failed to apply nice level."
      return 1
    fi
  log "debug" "New nice level is ${NICE_LEVEL}."
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
  log "info" "Print global usage..."
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
      prntLn "    pdf\t\t${COMMAND_PDF}"
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
  log "debug" "Usage printed."
  return 0
}


## Print some information about this script
function printVersion {
  log "info" "Print some information about this script..."
  prntLn "$PROJECT_NAME $PROJECT_VERSION"
  prntLn "Copyright (C) 2011 $PROJECT_AUTHOR"
  prntLn "This program comes with ABSOLUTELY NO WARRANTY."
  prntLn "This is free software, and you are welcome to redistribute it"
  prntLn "under certain conditions. Type '--license' for details."
  log "debug" "Information printed."
  return 0
}


## Print license information
function printLicense {
  log "info" "Print license information..."
  log "debug" "Look for license text..."
  licenses[0]="/usr/share/common-licenses/GPL"
  licenses[1]="/usr/share/doc/licenses/gpl-3.0.txt"
  licenses[2]="/usr/share/doc/${PROJECT_NAME}/COPYING"
  for i in "${licenses[@]}"; do
      if [ -f "$i" ]; then
          "$CAT" "$i" 1>&2
          log "debug" "Information printed."
          return 0
        fi
    done
  log "warning" "Cannot find any fitting license text on this system."
  log "error" "Failed to print license. But it's the GPL3+."
  return 1
}
