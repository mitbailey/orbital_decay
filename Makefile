CC = gcc
RM = rm -vf

EDCFLAGS := -O2 -Wall -I include/ $(CFLAGS)
EDLDFLAGS := -lpthread -lm $(LDFLAGS)
TARGET := reentry
COBJ = src/decay.o

all: $(TARGET)

reentry: src/ui.o $(COBJ)
	$(CC) $(EDCFLAGS) $< $(COBJ) -o reentry.out $(EDLDFLAGS)

%.o: %.c
	$(CC) $(EDCFLAGS) -o $@ -c $<

.PHONY: clean

clean:
	$(RM) *.out
	$(RM) src/*.o
	$(RM) *.tmp