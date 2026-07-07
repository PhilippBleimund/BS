#===========================================================================
#
#Makefile --
#
#
#Wolfram Schenck <wolfram.schenck@fh-bielefeld.de>
#     Copyright (C) 2015
#     Engineering Informatics
#     Department of Engineering Sciences and Mathematics
#     University of Applied Sciences Bielefeld
#
#1.2 / 13. Oct 15 (ws)
#
#
#===========================================================================

BIN     = memfit

CC       = gcc
CFLAGS   = -D_GNU_SOURCE -Wall -g
CPP      = g++
CPPFLAGS = -D_GNU_SOURCE -Wall -g
LNFLAGS  = -Wall

CFILES := $(wildcard *.c)
OBJS1 := $(CFILES:.c=.o)

CPPFILES := $(wildcard *.C)
OBJS2 := $(CPPFILES:.C=.o)

HEADERFILES := $(wildcard *.H)

%.o: %.c $(HEADERFILES)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $<

%.o: %.C $(HEADERFILES)
	@echo "Compiling $<..."
	@$(CPP) $(CPPFLAGS) -c $<

all: $(BIN)

$(BIN): $(OBJS2) $(OBJS1)
	@echo "Linking $@..."
	@$(CPP) $(LNFLAGS) -o $@ $(OBJS2) $(OBJS1)

clean:
	@echo "Cleaning files..."
	#@-rm *.o *~ $(BIN) $(BIN2) $(BIN3)
	@-rm *.o *~ $(BIN)

#
# Bedeutung Makefile Makros:
#
# $< the name of the related file that caused the action.
# $* the prefix shared by target and dependent files.
# $@ is the name of the file to be made.
# $? is the names of the changed dependents.
