CC=clang
CFLAGS=-Wall -g -luriparser -lpthread -lcurl -lcrypto -largp
LD=clang++

INCLUDES=-Iincludes/ -Iincludes/util/
SOURCES=$(wildcard src/*.c) $(wildcard src/util/*.c)

MAIN_FILE=src/main.c
SOURCES:=$(filter-out $(MAIN_FILE), $(SOURCES))

TESTS=$(wildcard test/*.c)

all: $(SOURCES) $(MAIN_FILE)
	@mkdir -p bin
	$(CC) $(CFLAGS) $(INCLUDES) $(MAIN_FILE) $(SOURCES) -o bin/flood

tests: $(SOURCES) test/bencode_test.c
	@mkdir -p bin
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) -shared -o bin/lib.so
	$(CC) $(CFLAGS) $(INCLUDES) test/bencode_test.c bin/lib.so -o bin/bencode_test
	$(CC) $(CFLAGS) $(INCLUDES) test/udp_protocol_test.c bin/lib.so -o bin/udp_protocol_test
	$(CC) $(CFLAGS) $(INCLUDES) test/hashtable_test.c bin/lib.so -o bin/hashtable_test

clean:
	@rm bin/flood
