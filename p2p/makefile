CC=gcc
CFLAGS=-c -std=c99
LDFLAGS=
SOURCES=main.c server.c FILENO_MZFK.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=p2p

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	   $(CC) $(LDFLAGS) $(OBJECTS) -o $@
		 
.c.o:
	$(CC) $(CFLAGS) $< -o $@
		    
clean:
	rm *.o $(EXECUTABLE)
