CFLAGS = -I.
CFLAGS += -Werror=return-type

LDLIBS = -lpthread

HEADERS = tap.h
TARGETS = port
PORT_OBJS = port.o tap.o

all: ${TARGETS}

port: ${PORT_OBJS} $(HEADERS)

*.o: $(HEADERS)

clean:
	rm -f ${PORT_OBJS} ${TARGETS}