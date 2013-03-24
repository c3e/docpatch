%   DOCPATCH(1) DocPatch User Manuals | Version 0.1
%
%   August 22, 2012


#   NAME

DocPatch - patching documents that matter


#   SYNOPSIS

**docpatch** [ *debugging options* ] [ *command* ] [ *options* ]


#   DESCRIPTION

DocPatch is our platform to to re-publish constitutions, law texts, and any other public documents in a way everyone and everything may use them.

#   NOTE

This manpage is very terse. Please visit our website for more information about DocPatch and our goals: <https://wiki.die-foobar.org/wiki/DocPatch>


#   GLOBAL OPTIONS

-h, \--help
:   Show this help and exit.

\--license
:   Show license information and exit.

\--version
:   Show information about this script and exit.


##  Debugging options

-q
:   Be quiet (for scripting).

-v
:   Be verbose.

-V
:   Be verboser.

-D
:   Be verbosest (for debugging).


<!--- create -->

# COMMANDS

build
:   Build repository.

create
:   Produce pretty output from source.

## docpatch create

docpatch [ *debugging options* ] create [ *options* ]


Options:

-e, \--embed, \--embedded
:   Create an embedded version.

-f, \--format, \--output *FORMAT*
:   Select output format *FORMAT*. Supported formats are those supported by `pandoc(1)`: "pdf", "native", "json", "html", "html+lhs", "s5", "slidy", "docbook", "opendocument", "latex", "latex+lhs", "context", "texinfo", "man", "markdown", "markdown+lhs", "plain", "rst", "rst+lhs", "mediawiki", "textile", "rtf", "org", "odt", "epub". Defaults to "pdf".

-r, \--rev, \--revision *REV*
:   Generate output from revision *REV*. "first", "last", "all" and a comma separated list of revisions are allowed. Defaults to "0".

-R, \--repo, \--repository *REPO*
:   Select repository *REPO* which will be cloned. Defaults to existing repository under "repo/". The directory must not exist when `docpatch` is run.

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

<!--- create -->

<!--- build -->


## docpatch build

docpatch [ *debugging options* ] build [ *options* ]


Options:

-s, \--sign
:   Add a OpenPGP signature to commits and tags.

-h, \--help
:   Show this help and exit.

\--license
:   Show license information and exit.

\--version
:   Show information about this script and exit.


<!--- build end -->

#   FILES

`/etc/docpatch/docpatch.conf`
:   System wide configuration

`~/.docpatch.conf`
:   User specific configuration


#   DIAGNOSTICS

Exit Codes are used as usual:

`0`
: Operation was successful.

`> 0`
: Something went wrong.


#   SEE ALSO

`git`(1), `markdown2pdf`(1), `pandoc(1)`, `quilt`(1)
