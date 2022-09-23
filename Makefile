CC = gcc
COBJS = src/decay.o src/ui.o
EDCFLAGS = -I ./ -I ./include/ -Wall -pthread $(CFLAGS)
EDLDFLAGS = -lpthread -lm $(LDFLAGS)
TARGET = decay.out

RM = rm -vf

all: $(COBJS)
	$(CC) $(EDCFLAGS) $(COBJS) -o $(TARGET) $(EDLDFLAGS)

%.o: %.c
	$(CC) $(EDCFLAGS) -o $@ -c $<

.PHONY: clean

clean:
	$(RM) *.out
	$(RM) src/*.o
	$(RM) *.tmp

dataless:
	$(RM) data.csv