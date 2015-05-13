CC = gcc
CFLAGS = -c -I./jansson 
LDFLAGS = -L./jansson -ljansson
SOURCES = local.c main.c server.c input.c message.c msg_sender.c msg_rcv.c hash.c storage.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = p2p

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	   $(CC) $(LDFLAGS) $(OBJECTS) -o $@
		 
.c.o:
	$(CC) $(CFLAGS) $< -o $@ 
		    
clean:
	rm *.o $(EXECUTABLE)
