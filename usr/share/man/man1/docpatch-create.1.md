% DOCPATCH-CREATE(1) DocPatch User Manuals | Version 0.1
%
% August 22, 2012


# NAME

docpatch-create - produce pretty output from source


# SYNOPSIS

docpatch [*output*] create [*options*]


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

-e, \--embed, \--embedded
:   Create an embedded version.

-f, \--format, \--output *FORMAT*
:   Select output format *FORMAT*. Supported formats are "pdf", "native", "json", "html", "html+lhs", "s5", "slidy", "docbook", "opendocument", "latex", "latex+lhs", "context", "texinfo", "man", "markdown", "markdown+lhs", "plain", "rst", "rst+lhs", "mediawiki", "textile", "rtf", "org", "odt", "epub". Defaults to "pdf".

-r, \--rev, \--revision *REV*
:   Generate output from revision *REV*. "first", "last", "all" and a comma separated list of revisions are allowed. Defaults to "0".

-R, \--repo, \--repository *REPO*
:   Select repository *REPO* which will be cloned. Defaults to existing repository under "repo/".

-S, \--simple, \--not-smart
:   Do not create a smart version.

-t, \--toc
:   Add table of contents (TOC).

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

`docpatch`(1), `docpatch-build`(1)
