CFLAGS?=-Werror -g
CFLAGS:=$(CFLAGS) -std=c11 -Wall

TARGET:=filesystem.a

OBJS:=$(patsubst %.c,%.o,$(shell find src -name "*.c"))

TOOLS:=$(patsubst %.c,%,$(shell find tools -name "*.c"))

all: $(TARGET) $(TOOLS)

.PHONY: all clean check test

-include $(OBJS:.o=.d)

$(TARGET): $(OBJS)
	rm -f $@
	ar -cvq $@ $(OBJS)

%.o: %.c
	$(CC) -c $< -MD -o $@ $(CFLAGS)

%: %.c $(TARGET)
	$(CC) $< $(TARGET) -MD -o $@ -I src $(CFLAGS)

clean:
	rm -f $(TARGET) $(OBJS) $(OBJS:.o=.d) $(TOOLS)
	$(MAKE) -C test clean

check: $(TARGET)
	$(MAKE) -C test
	test/run

test: check
