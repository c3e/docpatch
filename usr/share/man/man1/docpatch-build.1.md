% DOCPATCH-BUILD(1) DocPatch User Manuals | Version 0.1
%
% August 22, 2012


# NAME

docpatch-build - build repository


# SYNOPSIS

docpatch [*output*] build [*options*]


#   DESCRIPTION

Please visit our website for more information about DocPatch and our goals: <https://wiki.die-foobar.org/wiki/DocPatch>


# OUTPUT

-q
:   Be quiet (for scripting).

-v
:   Be verbose.

-V
:   Be verboser.

-D
:   Be verbosest (for debugging).


# OPTIONS

-s, \--sign
:   Add a OpenPGP signature to commits and tags.

-h, \--help
:   Show this help and exit.

\--license
:   Show license information and exit.

\--version
:   Show information about this script and exit.


# ENVIRONMENT

Most of the configuration settings maybe set in environment.


# DIAGNOSTICS


## EXIT CODES

* `0`: Operation was successful.
* `> 0`: Something went wrong.


# SEE ALSO

`docpatch`(1), `docpatch-create`(1),
