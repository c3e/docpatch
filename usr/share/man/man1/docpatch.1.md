%   DOCPATCH(1) DocPatch User Manuals | Version 0.1
%
%   August 22, 2012


#   NAME

DocPatch - patching documents that matter


#   SYNOPSIS

docpatch [*output*] [*command*] [*options*]


#   DESCRIPTION

Please visit our website for more information about DocPatch and our goals: <https://wiki.die-foobar.org/wiki/DocPatch>


#   OUTPUT

-q
:   Be quiet (for scripting).

-v
:   Be verbose.

-V
:   Be verboser.

-D
:   Be verbosest (for debugging).


#   COMMANDS

build
:   Build repository.

create
:   Produce pretty output from source.


#   OPTIONS

-h, \--help
:   Show this help and exit.

\--license
:   Show license information and exit.

\--version
:   Show information about this script and exit.


#   FILES

`/etc/docpatch/docpatch.conf`
:   System wide configuration

`~/.docpatch.conf`
:   User specific configuration


#   ENVIRONMENT

Most of the configuration settings maybe set in environment.


#   DIAGNOSTICS


##  EXIT CODES

*   `0`: Operation was successful.
*   `> 0`: Something went wrong.


#   SEE ALSO

`docpatch-build`(1), `docpatch-create`(1), `git`(1), `markdown2pdf`(1), `pandoc(1)`, `quilt`(1)
