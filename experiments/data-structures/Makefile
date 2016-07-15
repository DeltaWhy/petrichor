CFLAGS?=-Werror -g
CFLAGS:=$(CFLAGS) -std=c11 -Wall

TARGET:=data-structures.a

OBJS:=$(patsubst %.c,%.o,$(shell find src -name "*.c"))

all: $(TARGET)

.PHONY: all clean check test

-include $(OBJS:.o=.d)

$(TARGET): $(OBJS)
	ar -cvq $@ $(OBJS)

%.o: %.c
	$(CC) -c $< -MD -o $@ $(CFLAGS)

clean:
	rm -f $(TARGET) $(OBJS) $(OBJS:.o=.d)
	$(MAKE) -C test clean

check: $(TARGET)
	$(MAKE) -C test
	test/run

test: check
