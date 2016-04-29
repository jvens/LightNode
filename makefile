CC=g++
LD=g++

CFLAGS=-std=c++14 -Wall

srcdir=source
objdir=obj
comdir=../common
VPATH=source



all: LightNode

LightNode: $(objdir)/LightNode.o  $(objdir)/LightStrip.o $(objdir)/SPI.o $(objdir)/Color.o
	$(LD) $(CFLAGS) $(objdir)/Color.o $(objdir)/SPI.o $(objdir)/LightStrip.o $(objdir)/LightNode.o -lboost_system -lboost_thread -lpthread -lbcm2835 -o LightNode

$(objdir)/LightNode.o: $(srcdir)/LightNode.cpp $(srcdir)/LightNode.hpp
	$(CC) $(CFLAGS) -c $(srcdir)/LightNode.cpp -o $(objdir)/LightNode.o

$(objdir)/LightStrip.o: $(srcdir)/LightStrip.cpp $(srcdir)/LightStrip.hpp
	$(CC) $(CFLAGS) -c $(srcdir)/LightStrip.cpp -o $(objdir)/LightStrip.o

$(objdir)/SPI.o: $(srcdir)/SPI.c $(srcdir)/SPI.h
	$(CC) $(CFLAGS) -c $(srcdir)/SPI.c -o $(objdir)/SPI.o

$(objdir)/Color.o: $(srcdir)/Color.cpp $(srcdir)/Color.hpp
	$(CC) $(CFLAGS) -c $(srcdir)/Color.cpp -o $(objdir)/Color.o

clean:
	rm $(objdir)/*.o
