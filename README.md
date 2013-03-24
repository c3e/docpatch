#   DocPatch – patching documents that matter


##  Description


### Who we are

We are part of the Open Data movement and supports the [Open Definition](http://opendefinition.org/okd/). **Our way** is to ‘hack’ on documents and build Free Software around them – **our goal** is to bring open data to governments – **our idea** is to re-publish constitutions, law texts, and any other public documents in a way everyone and everything may use them.


### What DocPatch stands for

**DocPatch** is our platform to provide these documents to you. It relies on some simple tools from software development. The ‘heart’ of this platform is a version control system (vcs) to manage the documents with their complete history and additional meta information. The documents itself are written in a markup language which allows us a) to use any text editor and b) to convert the texts into any format we want.

This is our current tool stack:

*   Shell: [Bourne-again Shell](http://www.gnu.org/software/bash/)
*   vcs: [git](http://git-scm.com/)
*   converter: [pandoc](http://johnmacfarlane.net/pandoc/index.html)
*   markup language: pandoc's [enhanced version](http://johnmacfarlane.net/pandoc/README.html#pandocs-markdown) of [John Gruber's markdown](http://daringfireball.net/projects/markdown/)

These tools are simple to use, but powerful, widely spread, and open. (Even if they won't fit any more in the future, they could be replaced by other, even better tools. For distributing the repositories, the code, the generated documents, and everything else we use some more tools, e.g. web server, operating system,…, but they don't matter for our intended purpose.)

Please visit our [website](http://wiki.die-foobar.org/wiki/DocPatch) for more information about DocPatch and our goals.


### First Milestone

Our first target is the German constitution, called *Grundgesetz*. First published in March 23th 1949 it was changed round about 60 times. It's interesting to see why it was changed, when, by whom and under what circumstances. The good is: All information is public. The bad: The information doesn't match the Open Definition and many details are hard to find.

In short: [We are putting the complete "Grundgesetz" under version control and publish it.](http://wiki.die-foobar.org/wiki/DocPatch/Grundgesetz)

Clone the repository to play around with **DocPatch**:

    git clone https://github.com/bheisig/grundgesetz-dev.git


### Contribution

Everthing is under heavy development. Still in progress. Under construction. We need *your* help to make **DocPatch** a success!

We are looking for people who create some fancy tools, design a cool website, define more milestones, or simply versionize with us.


##  Installation

Installing DocPatch and its components requires the following dependencies:

*   [POSIX](http://en.wikipedia.org/wiki/POSIX) operating system
*   `bash` ([‘Bourne-again Shell’](http://www.gnu.org/software/bash/))
*   `git` ([distributed version control system](http://git-scm.com/))
*   `gpg` ([‘GnuPG’, OpenPGP encryption and signing tool](http://www.gnupg.org/))
*   `pandoc` ([markup conversion tool](http://johnmacfarlane.net/pandoc/))
*   `quilt` ([patch management tool](http://savannah.nongnu.org/projects/quilt))
*   `getopt` in the version from [util-linux](https://www.kernel.org/pub/linux/utils/util-linux/) or the [‘enhanced’ version](http://software.frodo.looijaard.name/getopt/download.php). (This is usually preinstalled on Linux and can be obtained as a port for other systems)

After you met all requirements installation is simply done by:

    ./configure
    make
    make install

If you do not want to install to `/usr/local`, you must edit the `Makefile` after the `make`, and adjust the variables `prefix` and `DESTDIR`. For installation to `/usr`, set:

    # DESTDIR = WHATEVER  # (commented out or empty)
    prefix = /usr


##  Usage

-   Change into the directory there your documents are located.
-   Play around with **DocPatch** ;-)

There are several man pages for more information about using **DocPatch**: `docpatch(1)`, `docpatch-build(1)`, `docpatch-create(1)`, `git(1)`, `pandoc(1)`, `quilt(1)`


##  Bugs

Please, report any bugs to [our issue tracker](https://github.com/bheisig/DocPatch/issues).


##  Copyright

Copyright (C) 2012 [Benjamin Heisig](http://benjamin.heisig.name/). License [GPLv3+: GNU GPL version 3 or later](http://gnu.org/licenses/gpl.html). This is free software: you are free to change and redistribute it. There is NO WARRANTY, to the extent permitted by law.


##  Authors

*   [Benjamin Heisig](http://benjamin.heisig.name/)
*   Bernhard Físseni


##  Thanks

We would like to thank the following people:

*   from [foobar e. V.](http://wiki.die-foobar.de/) and [Chaospott Essen](http://chaospott.de/)
*   from [tiggersWelt.net](http://tiggerswelt.net/) who provide some of the hosting services

And everyone who keeps sending us feedback and improving this project! Thank you :-)
