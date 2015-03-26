CC=gcc
CFLAGS=-c -I./jansson 
LDFLAGS=-L./jansson -ljansson
SOURCES=main.c server.c input.c message.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=p2p

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	   $(CC) $(LDFLAGS) $(OBJECTS) -o $@
		 
.c.o:
	$(CC) $(CFLAGS) $< -o $@ 
		    
clean:
	rm *.o $(EXECUTABLE)
