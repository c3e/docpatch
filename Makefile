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

SHELL = /bin/sh

INSTALL = $(shell which install)
INSTALL_PROGRAM = $(INSTALL) -m 755
INSTALL_DATA = $(INSTALL) -m 644
PANDOC = $(shell which pandoc)
GIT = $(shell which git)
GPG = $(shell which gpg)
MKDIR = mkdir -m 755

project = $(shell cat usr/share/docpatch/config.inc | sed -n 's/^PROJECT_NAME="\(.*\)"$$/\1/p')
man1pages = $(project)
metainfos = CHANGELOG README
languages = de
version = $(shell cat usr/share/docpatch/config.inc | sed -n 's/^PROJECT_VERSION="\(.*\)"$$/\1/p')

prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(exec_prefix)/bin
datarootdir = $(prefix)/share
datadir = $(datarootdir)
docdir = $(datarootdir)/doc/$(project)
htmldir = $(docdir)/html
pdfdir = $(docdir)/pdf
mandir = $(datarootdir)/man
man1dir = $(mandir)/man1
localedir = $(datadir)/locale
patch_docpatch = ./patch_docpatch


## Build

all : man readme changelog meta

man : $(man1pages)

html :
	$(PANDOC) --from markdown --to html -o $(project).html --toc --standalone --smart README.md

pdf :
	$(PANDOC) --from markdown --toc --smart -o $(project).pdf README.md

meta : $(metainfos)
	cp LICENSE usr/share/doc/$(project)/LICENSE
	cp README usr/share/doc/$(project)/README
	cp CHANGELOG usr/share/doc/$(project)/CHANGELOG

readme :
	$(PANDOC) --from markdown --to plain --standalone README.md > README

changelog :
	$(PANDOC) --from markdown --to plain --standalone CHANGELOG.md > CHANGELOG

% : usr/share/man/man1/%.1.md
	$(PANDOC) -s --from markdown --to man $< | gzip -c > usr/share/man/man1/$@.1.gz

% : %.md
	$(PANDOC) --from markdown --to plain $< > usr/share/doc/$(project)/$@

pot :
	bash --dump-po-strings usr/bin/$(project) > usr/share/locale/$(project).pot
	bash --dump-po-strings usr/share/$(project)/build >> usr/share/locale/$(project).pot
	bash --dump-po-strings usr/share/$(project)/config.inc >> usr/share/locale/$(project).pot
	bash --dump-po-strings usr/share/$(project)/create >> usr/share/locale/$(project).pot
	bash --dump-po-strings usr/share/$(project)/$(project).inc >> usr/share/locale/$(project).pot
	bash --dump-po-strings usr/share/$(project)/help >> usr/share/locale/$(project).pot
	awk '/^msgid/&&!seen[$0]++;!/^msgid/' usr/share/locale/$(project).pot > usr/share/locale/$(project).pot.tmp
	mv usr/share/locale/$(project).pot.tmp usr/share/locale/$(project).pot


## Install/Uninstall

install : normal-install $(languages) post-install

normal-install :
	$(NORMAL_INSTALL)
	$(MKDIR) -p $(DESTDIR)$(bindir)
	$(patch_docpatch) usr/bin/docpatch "$(DESTDIR)" "$(prefix)"
	$(INSTALL_PROGRAM) usr/bin/* $(DESTDIR)$(bindir)
	$(MKDIR) -p $(DESTDIR)$(datadir)/$(project)
	$(INSTALL_DATA) usr/share/$(project)/* $(DESTDIR)$(datadir)/$(project)
	$(MKDIR) -p $(DESTDIR)$(docdir)
	$(INSTALL_DATA) \
	    usr/share/doc/$(project)/LICENSE \
	    usr/share/doc/$(project)/CHANGELOG \
	    usr/share/doc/$(project)/README \
	    $(DESTDIR)$(docdir)
	$(MKDIR) -p $(DESTDIR)$(docdir)/examples
	$(MKDIR) -p $(DESTDIR)$(docdir)/examples/etc
	$(INSTALL_DATA) usr/share/doc/$(project)/examples/etc/* $(DESTDIR)$(docdir)/examples/etc
	$(MKDIR) -p $(DESTDIR)/etc/bash_completion.d
	$(INSTALL_DATA) usr/share/doc/$(project)/examples/bash_completion.d/$(project) $(DESTDIR)/etc/bash_completion.d/

post-install :
	$(POST_INSTALL)
	$(MKDIR) -p $(DESTDIR)$(man1dir)
	$(INSTALL_DATA) usr/share/man/man1/*.1.gz $(DESTDIR)$(man1dir)

% : usr/share/locale/%
	$(MKDIR) -p $(DESTDIR)$(localedir)/$@
	$(INSTALL_DATA) usr/share/locale/$@/$(project).po $(DESTDIR)$(localedir)/$@/

install-html :
	$(INSTALL_DATA) *.html $(DESTDIR)$(htmldir)

install-pdf :
	$(INSTALL_DATA) *.pdf $(DESTDIR)$(pdfdir)

uninstall :
	$(PRE_UNINSTALL)
	$(NORMAL_UNINSTALL)
	rm $(DESTDIR)$(bindir)/$(project)
	rm -r $(DESTDIR)$(datadir)/$(project)/
	rm -r $(DESTDIR)$(docdir)
	rm $(DESTDIR)$(man1dir)/$(project).1.gz
	rm $(DESTDIR)$(localedir)/de/$(project).po
	rm $(DESTDIR)/etc/bash_completion.d/$(project)


## Release

dist :
	$(MKDIR) $(project)-$(version)
	chmod 777 -R $(project)-$(version)/
	find docs usr -type d -exec $(MKDIR) $(project)-$(version)/{} \; \
	  -exec chmod 777 $(project)-$(version)/{} \;
	find docs usr -type f -exec cp {} $(project)-$(version)/{} \; \
	  -exec chmod 755 $(project)-$(version)/{} \;
	$(INSTALL_PROGRAM) -m 755 configure LICENSE Makefile CHANGELOG README $(project)-$(version)
	tar czf $(project)-$(version).tar.gz $(project)-$(version)


## Clean up

clean : distclean
	rm -rf $(project)-$(version)/
	rm -f $(project)-$(version).tar.gz
	rm -f README
	rm -f CHANGELOG

distclean :
	find usr/share/man -name '*.gz' -delete
	rm -rf usr/share/info/
	rm -f $(project).html
	rm -f $(project).pdf
	rm -f usr/share/doc/$(project)/LICENSE
	rm -f usr/share/doc/$(project)/CHANGELOG
	rm -f usr/share/doc/$(project)/README

mostlyclean : clean

maintainer-clean : clean
