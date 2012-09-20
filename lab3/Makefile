CC=gcc
PROGOPTS=
DBGFLAG=-ggdb 
OPTFLAG=
CFLAGS=-std=c99 $(DBGFLAG) -Wall $(OPTFLAG) $(PROGOPTS)
LDLIBS=-lreadline
LDFLAGS=$(DBGFLAG) $(OPTFLAG)

SOURCES=signals.c

OBJECTS=$(SOURCES:.c=.o)

TARGET=signals


.PHONY : clean 

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(LDFLAGS) $(OBJECTS) $(LDLIBS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

.depend: depend 

test: $(TARGET)
	./$(TARGET) | circo -Tpng | display


depend: $(SOURCES)
	ctags -R
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^>>./.depend;
include .depend

clean:
	rm $(OBJECTS)
