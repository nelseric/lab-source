CC=gcc
PROGOPTS=
DBGFLAG=-ggdb 
OPTFLAG=
CFLAGS=-std=c99 $(DBGFLAG) -Wall $(OPTFLAG) $(PROGOPTS)
LDLIBS=-lreadline -lpthread -lm
LDFLAGS=$(DBGFLAG) $(OPTFLAG)

SOURCES=server.c

OBJECTS=$(SOURCES:.c=.o)

TARGET=server


.PHONY : clean 

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(LDFLAGS) $(OBJECTS) $(LDLIBS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

.depend: depend 

depend: $(SOURCES)
	ctags -R
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^>>./.depend;
include .depend

clean:
	rm $(OBJECTS)
