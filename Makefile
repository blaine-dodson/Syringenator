



source:=$(wildcard src/*)
source+=$(wildcard *.md)

pdfman:=refman.pdf

.PHONEY: all documentation super_clean

all: documentation


################################################################################
#                                DOCUMENTATION
################################################################################

documentation: docs latex $(pdfman)

$(pdfman): latex
	$(MAKE) -C latex all
	cp latex/refman.pdf ./


docs latex: doxygen.cfg $(source)
	rm -fr docs latex $(pdfman)
	doxygen doxygen.cfg


################################################################################
#                                    CLEAN
################################################################################


super_clean:
	rm -fr docs latex $(pdfman)
