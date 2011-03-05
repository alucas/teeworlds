
PREFIX= ../../

#INCDIR = ../../../cppunit/lib/include 
INCDIR = ../src/game/server/
LIBDIR = lib/

OBJDIR= objs/
BINDIR= bin/
SRCDIR= src/

CC= g++
CPPFLAGS= -I$(INCDIR) -I../src/engine/shared/ -I../src/ -Isrc/ -I../src/game/server/entities -L$(LIBDIR)
CFLAGS= -g -O0 
LDFLAGS=
LDLIBS= -lcppunit -lpthread


OUTFILE= $(BINDIR)unittest
TWOBJS= ../objs/game/generated/server_data.o ../objs/game/generated/nethash.o ../objs/game/generated/protocol.o $(patsubst %_d.o,,$(wildcard ../objs/base/*.o)) $(patsubst %_d.o,,$(wildcard ../objs/engine/external/*/*.o)) $(patsubst %_d.o,,$(wildcard ../objs/engine/shared/*.o)) $(patsubst %_d.o,,$(wildcard ../objs/game/*.o)) $(patsubst %_d.o,,$(wildcard ../objs/game/server/*.o))  $(patsubst %_d.o,,$(wildcard ../objs/game/server/*/*.o)) $(patsubst %_d.o,,$(wildcard ../objs/engine/server/*.o))


OBJS= $(OBJDIR)unittest.o $(OBJDIR)mockobjs.o $(TWOBJS) 

all: bam 

.PHONY: bam unit_test

unit_test:$(OUTFILE)

bam:
		cd ..;../bam/bam release;cd cppunit;make unit_test

#$^ : toutes les dépendances
#$@ : nom de la cible
$(OUTFILE): $(OBJS)
		test -d $(BINDIR) || mkdir $(BINDIR)
		$(CC) $^ $(LDLIBS) -o $@

$(OBJDIR)unittest.o: $(SRCDIR)unittest.cpp $(SRCDIR)mockobjs.h
		test -d $(OBJDIR) || mkdir $(OBJDIR)
		$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJDIR)mockobjs.o: $(SRCDIR)mockobjs.cpp $(SRCDIR)mockobjs.h
		test -d $(OBJDIR) || mkdir $(OBJDIR)
		$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
		rm $(OUTFILE) $(SRCDIR)*~ $(OBJDIR)*.o *~ *.o

