CFLAGS?=-Werror -g
CFLAGS:=$(CFLAGS) -std=c11 -Wall -Wno-unused -I../src
LDFLAGS?=
LIBS:=-lcunit

TARGET:=../data-structures.a

OBJS:=$(patsubst %.c,%.o,$(shell find . -name "*.c"))

run: $(OBJS) $(TARGET)
	$(CC) -o $@ $(LDFLAGS) $(OBJS) $(TARGET) $(LIBS)

-include $(OBJS:.o=.d)

.PHONY: clean

%.o: %.c
	$(CC) -c $< -MD -o $@ $(CFLAGS)

clean:
	rm -f $(OBJS) $(OBJS:.o=.d) run
