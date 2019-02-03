



source:=$(wildcard src/*)
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
	$(MAKE) -C latex clean

docs latex: doxygen.cfg $(source) README.md
	rm -fr docs latex
	doxygen doxygen.cfg


################################################################################
#                                    CLEAN
################################################################################


super_clean:
	rm -fr docs latex $(pdfman)
