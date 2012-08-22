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


SHELL = $(which sh)

INSTALL = $(which install)
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL) -m 644
FIND = $(which find)
GZIP = $(which gzip)
PANDOC = $(which pandoc)
BASH = $(which bash)
GPG = $(which gpg)
TOUCH = $(which touch)

project = $(shell cat usr/share/docpatch/config.inc | sed -n 's/^PROJECT_NAME="\(.*\)"$$/\1/p')
man1pages = $(project) $(project)-build $(project)-create
metainfos = NEWS README TODO
languages = de
version = $(shell cat usr/share/docpatch/config.inc | sed -n 's/^PROJECT_VERSION="\(.*\)"$$/\1/p')

prefix = /usr
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
datarootdir = $(prefix)/share
datadir = $(datarootdir)
docdir = $(datarootdir)/doc/$(project)
infodir = $(datarootdir)/info
htmldir = $(docdir)/html
pdfdir = $(docdir)/pdf
mandir = $(datarootdir)/man
man1dir = $(mandir)/man1
localedir = $(datadir)/locale


## Build

all : man info readme meta
	@echo "$(project) built. Ready to install. Continue with '(sudo) make install'"

man : $(man1pages)
	@echo "Man pages built."

info :
	@echo "Building info documentation..."
	@mkdir -p usr/share/info
	@$(PANDOC) --from markdown --to texinfo --toc --standalone --smart usr/share/man/man1/$(project).1.md | $(GZIP) -c > usr/share/info/$(project).info.gz
	@$(PANDOC) --from markdown --to texinfo --toc --standalone --smart usr/share/man/man1/$(project)-build.1.md | $(GZIP) -c > usr/share/info/$(project)-build.info.gz
	@$(PANDOC) --from markdown --to texinfo --toc --standalone --smart usr/share/man/man1/$(project)-create.1.md | $(GZIP) -c > usr/share/info/$(project)-create.info.gz

html :
	@echo "Building HTML documentation..."
	@$(PANDOC) --from markdown --to html -o $(project).html --toc --standalone --smart README.md

pdf :
	@echo "Building PDF documentation..."
	@$(PANDOC) --from markdown --toc --smart -o $(project).pdf README.md

dvi :
	@echo "Failed to build DVI documentation. It's not supported."
	@exit 1

ps :
	@echo "Failed to build Postscript documentation. It's not supported."
	@exit 1

meta : $(metainfos)
	@echo "Copying meta information COPYING..."
	@cp COPYING usr/share/doc/$(project)/COPYING
	@cp README usr/share/doc/$(project)/README
	@echo "Meta information built."

readme :
	@echo "Building README..."
	@$(PANDOC) --from markdown --to plain --standalone README.md > README

% : usr/share/man/man1/%.1.md
	@echo "Building man1 page $@..."
	@$(PANDOC) -s --from markdown --to man $< | $(GZIP) -c > usr/share/man/man1/$@.1.gz

% : %.md
	@echo "Building meta information $@..."
	@$(PANDOC) --from markdown --to plain $< > usr/share/doc/$(project)/$@

pot :
	@echo "Building pot file..."
	@bash --dump-po-strings usr/bin/$(project) > usr/share/locale/$(project).pot
	@bash --dump-po-strings usr/share/$(project)/build >> usr/share/locale/$(project).pot
	@bash --dump-po-strings usr/share/$(project)/config.inc >> usr/share/locale/$(project).pot
	@bash --dump-po-strings usr/share/$(project)/create >> usr/share/locale/$(project).pot
	@bash --dump-po-strings usr/share/$(project)/$(project).inc >> usr/share/locale/$(project).pot
	@bash --dump-po-strings usr/share/$(project)/help >> usr/share/locale/$(project).pot
	@awk '/^msgid/&&!seen[$0]++;!/^msgid/' usr/share/locale/$(project).pot > usr/share/locale/$(project).pot.tmp
	@mv usr/share/locale/$(project).pot.tmp usr/share/locale/$(project).pot
	@echo "pot file built."

manifest :
	@echo "Creating manifest..."
	@$(FIND) * -type f > MANIFEST

signature : checksums
	@echo "Creating signature..."
	@gpg --clearsign SIGNATURE
	@mv SIGNATURE.asc SIGNATURE

checksums :
	@echo "Creating checksums..."
	@rm -f SIGNATURE
	@bash -c 'while read line; do sha1sum $line >> SIGNATURE 2> /dev/null ; done < <(cat MANIFEST)'


## Install/Uninstall

install : normal-install $(languages) post-install
	@echo "$(project) installed."

normal-install :
	$(NORMAL_INSTALL)
	@echo "Installing $(project)..."
	@mkdir -p $(DESTDIR)$(bindir)
	@$(INSTALL) usr/bin/* $(DESTDIR)$(bindir)
	@mkdir -p $(DESTDIR)$(datadir)/$(project)
	@$(INSTALL) usr/share/$(project)/* $(DESTDIR)$(datadir)/$(project)
	@mkdir -p $(DESTDIR)$(docdir)
	@$(INSTALL_DATA) \
	    usr/share/doc/$(project)/COPYING \
	    usr/share/doc/$(project)/NEWS \
	    usr/share/doc/$(project)/README \
	    usr/share/doc/$(project)/TODO $(DESTDIR)$(docdir)
	@mkdir -p $(DESTDIR)$(docdir)/examples
	@mkdir -p $(DESTDIR)$(docdir)/examples/etc
	@$(INSTALL_DATA) usr/share/doc/$(project)/examples/etc/* $(DESTDIR)$(docdir)/examples/etc
	@$(INSTALL_DATA) usr/share/doc/$(project)/examples/bash_completion.d/* $(DESTDIR)/etc/bash_completion.d

post-install :
	$(POST_INSTALL)
	@echo "Installing info documentation..."
	@mkdir -p $(DESTDIR)$(infodir)
	@$(INSTALL_DATA) usr/share/info/*.info.gz $(DESTDIR)$(infodir)
	install-info --dir-file="$(DESTDIR)$(infodir)/dir" $(DESTDIR)$(infodir)/$(project).info.gz
	@mkdir -p $(DESTDIR)$(man1dir)
	@$(INSTALL_DATA) usr/share/man/man1/*.1.gz $(DESTDIR)$(man1dir)

% : usr/share/locale/%
	@echo "Copying po files for language $@..."
	@mkdir -p $(DESTDIR)$(localedir)/$@
	@$(INSTALL_DATA) usr/share/locale/$@/$(project).po $(DESTDIR)$(localedir)/$@/

install-html :
	@echo "Installing HTML documentation..."
	@$(INSTALL_DATA) *.html $(DESTDIR)$(htmldir)

install-pdf :
	@echo "Installing PDF documentation..."
	@$(INSTALL_DATA) *.pdf $(DESTDIR)$(pdfdir)

uninstall :
	$(PRE_UNINSTALL)
	@echo "Uninstalling info documentation..."
	@rm $(DESTDIR)$(infodir)/$(project).info.gz
	$(NORMAL_UNINSTALL)
	@echo "Uninstalling $(project)..."
	@rm $(DESTDIR)$(bindir)/$(project)
	@rm -r $(DESTDIR)$(datadir)/$(project)/
	@rm -r $(DESTDIR)$(docdir)
	@rm $(DESTDIR)$(man1dir)/$(project).1.gz
	@rm $(DESTDIR)$(localedir)/de/$(project).po
	@rm $(DESTDIR)/etc/bash_completion.d/$(project)


## Release

dist : changelog credits
	@echo "Creating directory $(project)-$(version)/..."
	@mkdir $(project)-$(version)
	@echo "Copying files to $(project)-$(version)/..."
	@chmod 777 -R $(project)-$(version)/
	@find docs usr -type d -exec mkdir $(project)-$(version)/{} \; \
	  -exec chmod 777 $(project)-$(version)/{} \;
	@find docs usr -type f -exec cp {} $(project)-$(version)/{} \; \
	  -exec chmod 755 $(project)-$(version)/{} \;
	@$(INSTALL_PROGRAM) -m 755 ChangeLog configure CREDITS COPYING Makefile MANIFEST NEWS.md README SIGNATURE TODO.md $(project)-$(version)
	@echo "Creating tarball $(project)-$(version).tar.gz..."
	@tar czf $(project)-$(version).tar.gz $(project)-$(version)
	@echo "Release made."

changelog :
	@echo "Creating changelog from git log..."
	@if [ -d ".git" ]; then \
	    git log --date-order --date=short | \
	    sed -e '/^commit.*$$/d' | \
	    awk '/^Author/ {sub(/\\$$/,""); getline t; print $$0 t; next}; 1' | \
	    sed -e 's/^Author: //g' | \
	    sed -e 's/>Date:   \([0-9]*-[0-9]*-[0-9]*\)/>\t\1/g' | \
	    sed -e 's/^\(.*\) \(\)\t\(.*\)/\3    \1    \2/g' > ChangeLog ; \
	  else \
	    echo "No git repository present." ; \
	    exit 1 ; \
	  fi

credits :
	@echo "Extracting authors from git log..."
	@if [ -d ".git" ]; then \
	    git shortlog --numbered --no-merges --email --summary > CREDITS \
	  else \
	    echo "No git repository present." ; \
	    exit 1 ; \
	  fi


## Clean up

clean : distclean
	@echo "Removing distribution..."
	@rm -rf $(project)-$(version)/
	@rm -f $(project)-$(version).tar.gz
	@rm -f ChangeLog CREDITS

distclean :
	@echo "Removing man pages..."
	@$(FIND) usr/share/man -name '*.gz' -delete
	@echo "Removing documentation..."
	@rm -rf usr/share/info/
	@rm -f $(project).html
	@rm -f $(project).pdf
	@echo "Removing meta information about $(project)..."
	@rm -f usr/share/doc/$(project)/COPYING
	@rm -f usr/share/doc/$(project)/NEWS
	@rm -f usr/share/doc/$(project)/README
	@rm -f usr/share/doc/$(project)/TODO

mostlyclean : clean

maintainer-clean : clean
