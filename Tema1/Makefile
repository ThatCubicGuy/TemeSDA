# Compiler setup
CC:=gcc
CFLAGS:=-Iinclude
BIN:=bin
OUT:=tema1

# Files
SRCS:=$(wildcard *.c)
OBJS:=${SRCS:%.c=${BIN}/%.o}

build: ${BIN} ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${OUT}

run: build
	./${OUT}

${BIN}:
	mkdir ${BIN}

${OBJS}: ${BIN}/%.o: %.c
	${CC} ${CFLAGS} -c $^ -o $@

clean:
	rm -f bin/* ${OUT}* rezultat_valgrind.txt VlasinAlexCatalin_314CB.*

pack:
	zip -Du VlasinAlexCatalin_314CB.zip *.{c,h} Makefile README*

cc:
	${CC} --version

.PHONY: build clean cc