% DOCPATCH-EPUB(1) Create EPUB version
% Benjamin Heisig
% January 16, 2012


# NAME

docpatch-epub - create EPUB version


# SYNOPSIS

docpatch [*output*] epub [*options*]


# DESCRIPTION

FIXME


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

-r, \--rev, \--revision *REV*
:   Generate output from revision *REV*. 'first', 'last', 'all' and a comma separated list of revisions are allowed. Defaults to `0`.

-R, \--repo, \--repository *REPO*
:   Select repository *REPO* which will be cloned. Defaults to existing repository under `repo/`.

-h, \--help
:   Show this help and exit.

\--license
:   Show license information and exit.

\--version
:   Show information about this script and exit.


# ENVIRONMENT

FIXME


# DIAGNOSTICS

FIXME


# EXIT CODES

* `0`: Operation was successful.
* `> 0`: Something went wrong.


# BUGS

Please, report any bugs to `<[https://forge.benjaminheisig.de/docpatch](https://forge.benjaminheisig.de/docpatch "Project website")>`.


# Copyright

Copyright (C) 2012 Benjamin Heisig `<[http://benjamin.heisig.name/](http://benjamin.heisig.name/ "Website of Benjamin Heisig")>`. License GPLv3+: GNU GPL version 3 or later `<[http://gnu.org/licenses/gpl.html](http://gnu.org/licenses/gpl.html "GNU GPL version 3 or later")>`. This is free software: you are free to change and redistribute it. There is NO WARRANTY, to the extent permitted by law.


# AUTHOR

Benjamin Heisig `<[http://benjamin.heisig.name/](http://benjamin.heisig.name/ "Website of Benjamin Heisig")>`


# SEE ALSO

`docpatch`(1), `docpatch-build`(1), `docpatch-pdf`(1)
