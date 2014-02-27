CC=g++
CFLAGS=-c -std=c++11

OBJS=lol2cpp.o tokenizer.o tokenlist.o token.o

all: $(OBJS)
	$(CC) $(OBJS) -o lol2cpp

-include $(OBJS:.o=.d)

%.o: %.cpp
	$(CC) $(CFLAGS) $*.cpp -o $*.o
	$(CC) -MM $(CFLAGS) $*.cpp > $*.d

run: all
	./lol2cpp

clean:
	rm -rf *.o lol2cpp
