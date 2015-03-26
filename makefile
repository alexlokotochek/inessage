CC=gcc
CFLAGS=-c
LDFLAGS=
SOURCES=main.c server.c FILENO_MZFK.c message.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=p2p

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	   $(CC) $(LDFLAGS) $(OBJECTS) -o $@
		 
.c.o:
	$(CC) $(CFLAGS) $< -o $@ 
		    
clean:
	rm *.o $(EXECUTABLE)