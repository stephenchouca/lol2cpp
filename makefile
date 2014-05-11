CC=g++
LFLAGS=-Wall -Werror
CFLAGS=$(LFLAGS) -c -std=c++11 

OBJS=lol2cpp.o \
	 codegen.o \
	 program_visitor.o \
	 parser.o \
	 statement.o \
	 expression.o \
	 tokenizer.o \
	 tokenlist.o \
	 token.o

all: $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) -o lol2cpp

-include $(OBJS:.o=.d)

%.o: %.cpp
	$(CC) $(CFLAGS) $*.cpp -o $*.o
	$(CC) -MM $(CFLAGS) $*.cpp > $*.d

run: all
	./lol2cpp test/99beers.lols
	g++ -std=c++11 -O2 a.cpp
	a.exe

clean:
	rm -rf *.d *.o *.exe lol2cpp a.cpp
