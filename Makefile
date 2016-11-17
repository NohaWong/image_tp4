#######################################
# Makefile PBM                        #
#                                     #
# E.B.                                #
#######################################


PROG = gradient

all : $(PROG)

# Variables for file compilation
CC        =  gcc
CFLAGS    =  -g -Wall
CPPFLAGS  =  -DDEBUG
LDFLAGS   =  -g -lm

#pxmtopxm : pxmtopxm.o Util.o
gradient : gradient.o Util.o
	$(CC) $(CFLAGS) $^ -o gradient $(LDFLAGS)

stretching : stretching.o Util.o

median : median.o Util.o

filtering_5*5 : filtering_5*5.c Util.o

filtering : filtering.o Util.o

pgmtopgm : pgmtopgm.o Util.o

decompose : decompose.o Util.o

ppmtopgm : ppmtopgm.o Util.o

clean :
	@rm -f *.o

cleanall : clean
	@rm -f $(PROG)

