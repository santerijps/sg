NAME=sg
CC=gcc
WARN=-Wall -Wextra
LINK=src/main.c src/modules/md4c/src/md4c.c src/modules/md4c/src/md4c-html.c src/modules/md4c/src/entity.c

dev: bin src/main.c
	$(CC) $(WARN) -o bin/$(NAME).dev.exe $(LINK)

prod: bin src/main.c
	$(CC) $(WARN) -O3 -o bin/$(NAME) $(LINK)

bin:
	mkdir bin

test:
	bin/$(NAME).dev.exe --enable-hljs --hljs-style nord --css https://cdn.jsdelivr.net/npm/water.css@2/out/light.css examples/index.md