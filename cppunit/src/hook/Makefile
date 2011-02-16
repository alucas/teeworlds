OBJDIR= ../../objs/
BINDIR= ../../bin/
INCDIR = ../../../src/game/server/
LIBDIR = ../../lib/

CC= g++
#teeworlds' src directory 
SRCD=../../../src
#headers
CPPFLAGS= -I$(SRCD) -I$(SRCD)/game/ -I$(SRCD)/game/generated -I$(SRCD)/shared/

CFLAGS= -g -O0 
LDLIBS= -lm -lcppunit -lpthread 
OUTFILE= $(BINDIR)hook

#teeworlds' objects directory 
OBJD = ../../../objs
#objects from teeworlds' project
TWOBJS= $(OBJD)/base/system.o $(OBJD)/game/collision.o $(OBJD)/game/gamecore.o 

#unit test objects.
OBJS= $(OBJDIR)hook.o $(TWOBJS)


all: bam 

.PHONY: bam hook_test

bam:
	cd ../../..;../bam/bam release;cd cppunit/src/hook;make hook_test

hook_test:$(OUTFILE)

# for newbies of makefile :p
#$^ : all dependencies
#$< : first dependency from the list of dependencies
#$@ : name of the target
$(OUTFILE):$(OBJS)
	test -d $(BINDIR) || mkdir $(BINDIR)
	$(CC) $^ $(LDLIBS) -o $@

$(OBJDIR)hook.o: hook.cpp
	test -d $(OBJDIR) || mkdir $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm $(OUTFILE) $(SRCDIR)*~ $(OBJDIR)*.o *~ *.o