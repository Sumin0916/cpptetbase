# Set compiler to use
CC=g++
CFLAGS=-g -I. -fpermissive -Wno-deprecated
LDFLAGS=
DEBUG=0

#all:: Lab1.exe Lab2.exe Lab3.exe
all:: Lab3.exe Lab4.exe Lab5.exe

Lab3.exe: Lab3.o
	$(CC) -o $@ $^ -std=c++11 $(CFLAGS) $(LDFLAGS)

Lab4.exe: Lab4.o
	$(CC) -o $@ $^ -std=c++11 $(CFLAGS) $(LDFLAGS)

Lab5.exe: Lab5.o
	$(CC) -o $@ $^ -std=c++11 $(CFLAGS) $(LDFLAGS)

%.o: %.c $(DEPS_CHAT)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS_TET)
	$(CC) -c -o $@ $< $(CFLAGS)

clean: 
	rm -f *.exe 
	rm -f *.o *~ *.stackdump
