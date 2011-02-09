PREFIX= ../../

#INCDIR = ../../../cppunit/lib/include 
INCDIR = ../src/game/server/
LIBDIR = lib/

OBJDIR= objs/
BINDIR= bin/
SRCDIR= src/

CC= g++
CPPFLAGS= -I$(INCDIR) -I../src/ -Isrc/ -L$(LIBDIR)
CFLAGS= -g -O0 
LDFLAGS=
LDLIBS= -lcppunit -lpthread

OUTFILE= $(BINDIR)TestPlayer
TWOBJS= ../objs/game/generated/server_data.o ../objs/game/generated/nethash.o ../objs/game/generated/protocol.o $(wildcard ../objs/base/*.o) $(wildcard ../objs/engine/external/*/*.o) $(wildcard ../objs/engine/shared/*.o) $(wildcard ../objs/game/*.o) $(wildcard ../objs/game/server/*.o)  $(wildcard ../objs/game/server/*/*.o) 

OBJS= $(OBJDIR)TestPlayer.o $(OBJDIR)mockobjs.o $(TWOBJS) 



all: $(OUTFILE)

$(OUTFILE): $(OBJS)
		test -d $(BINDIR) || mkdir $(BINDIR)
		$(CC) $^ $(LDLIBS) -o $@


$(OBJDIR)TestPlayer.o: $(SRCDIR)TestPlayer.cpp $(SRCDIR)mockobjs.h
		test -d $(OBJDIR) || mkdir $(OBJDIR)
		$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJDIR)mockobjs.o: $(SRCDIR)mockobjs.cpp $(SRCDIR)mockobjs.h
		test -d $(OBJDIR) || mkdir $(OBJDIR)
		$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
		rm $(OUTFILE) $(SRCDIR)*~ $(OBJDIR)*.o *~ *.o

