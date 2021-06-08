CC = gcc
RM = rm -vf

EDCFLAGS := -O2 -Wall -I include/ $(CFLAGS)
EDLDFLAGS := -lpthread -lm $(LDFLAGS)
TARGET := decay
COBJ = src/decay.o

all: $(TARGET)

decay: src/ui.o $(COBJ)
	$(CC) $(EDCFLAGS) $< $(COBJ) -o decay.out $(EDLDFLAGS)

%.o: %.c
	$(CC) $(EDCFLAGS) -o $@ -c $<

.PHONY: clean

clean:
	$(RM) *.out
	$(RM) src/*.o
	$(RM) *.tmp