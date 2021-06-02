CC = gcc
RM = rm -vf

EDCFLAGS := -O2 -Wall -I include/ $(CFLAGS)
EDLDFLAGS := -lpthread -lm $(LDFLAGS)
TARGET := test
COBJ = src/decay.o

all: $(TARGET)

test: src/test.o $(COBJ)
	$(CC) $(EDCFLAGS) $< $(COBJ) -o test.out $(EDLDFLAGS)

%.o: %.c
	$(CC) $(EDCFLAGS) -o $@ -c $<

.PHONY: clean

clean:
	$(RM) *.out
	$(RM) src/*.o
	$(RM) *.tmp