CC := clang
CFLAGS := -Wall -Wextra -std=c99 -m32
LD := lld
LDFLAGS := -fuse-ld=$(LD)

SOURCES_PARSER = src/main.c src/parse.c src/launcher.c

build_parser:
	$(CC) $(CFLAGS) $(SOURCES_PARSER)


