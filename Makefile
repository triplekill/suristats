CC=gcc
CFLAGS=-W -Wall -Wextra
LDFLAGS=-l sqlite3

TARGET=suristats

SOURCES  := $(wildcard *.c)
INCLUDES := $(wildcard *.h)
OBJECTS  := $(SOURCES:%.c=%.o)

#all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJECTS): %.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)

suristats.o: suristats.h ssDb.h ssParse.h
ssDb.o: ssDb.h ssListe.h
ssListe.o: ssListe.h
ssParse.O: ssDb.h ssParse.h

clean:
	rm -rf $(OBJECTS) $(TARGET)
