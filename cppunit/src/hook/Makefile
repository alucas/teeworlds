OBJDIR= ../objs/
BINDIR= ../bin/
INCDIR = ../../src/game/server/
LIBDIR = ../lib/

CC= g++
# peut être à rajouter : -I../../src/base/
CPPFLAGS= -I ../../src -I../../src/game/  
#-I$(INCDIR) -I../../src/ -I ../src/ -I ../../src/game/ -I ../../src/base/ -L$(LIBDIR)

CFLAGS= -g -O0 
LDLIBS= -lm -lcppunit -lpthread 
OUTFILE= $(BINDIR)hook

#objects from teeworlds' project
TWOBJS= ../../objs/base/system.o ../../objs/game/collision.o ../../objs/game/gamecore.o  
#unit test objects.
OBJS= $(OBJDIR)hook.o $(TWOBJS)


all: bam 

.PHONY: bam hook_test

bam:
	cd ../..;../bam/bam release;cd cppunit/hook;make hook_test

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