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
BIN_TIME = memfit_time

CC       = gcc
CFLAGS   = -D_GNU_SOURCE -Wall -g
CPP      = g++
CPPFLAGS = -D_GNU_SOURCE -Wall -g
LNFLAGS  = -Wall

CFILES := $(wildcard *.c)
OBJS1 := $(CFILES:.c=.o)

CPPFILES := $(wildcard *.C)
OBJS2 := $(CPPFILES:.C=.o)

COMMON_OBJS := $(filter-out memfit.o memfit_time.o,$(OBJS2)) $(OBJS1)
HEADERFILES := $(wildcard *.H)

%.o: %.c $(HEADERFILES)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $<

%.o: %.C $(HEADERFILES)
	@echo "Compiling $<..."
	@$(CPP) $(CPPFLAGS) -c $<

all: $(BIN) $(BIN_TIME)

$(BIN): memfit.o $(COMMON_OBJS)
	@echo "Linking $@..."
	@$(CPP) $(LNFLAGS) -o $@ memfit.o $(COMMON_OBJS)

$(BIN_TIME): memfit_time.o $(COMMON_OBJS)
	@echo "Linking $@..."
	@$(CPP) $(LNFLAGS) -o $@ memfit_time.o $(COMMON_OBJS)

clean:
	@echo "Cleaning files..."
	#@-rm *.o *~ $(BIN) $(BIN_TIME) $(BIN2) $(BIN3)
	@-rm *.o *~ $(BIN) $(BIN_TIME)

#
# Bedeutung Makefile Makros:
#
# $< the name of the related file that caused the action.
# $* the prefix shared by target and dependent files.
# $@ is the name of the file to be made.
# $? is the names of the changed dependents.