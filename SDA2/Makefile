# File organisation

# TODO: header dependencies

## Parent source folder. Sources may be placed here directly to be compiled solo
## or in a subfolder to be compiled together with every other file in that folder
SRC:=src
## Binaries folder. Compiled and linked executables are created here.
BIN:=.bin
## Object files folder. Compiled but unlinked executables are created here.
OBJ:=.obj
## Library folder. Universal dependencies may be placed here,
## with the same compilation logic as source files.
LIB:=sys
## Folder for library object files.
LIBOBJ:=${OBJ}/${LIB}
## Header files folder.
HDR:=include

FOLDERS:=${SRC} ${BIN} ${OBJ} ${LIBOBJ} ${HDR}
OBJECTS:=$(sort $(patsubst ${SRC}/%.c,${OBJ}/%.o,$(wildcard ${SRC}/*.c) $(wildcard ${SRC}/*/*.c)))
OBJFOLDERS:=$(patsubst ${SRC}/%/,${OBJ}/%,$(wildcard ${SRC}/*/))
LIBOBJECTS:=$(patsubst ${LIB}/%.c,${LIBOBJ}/%.o,$(wildcard ${LIB}/*.c) $(wildcard ${LIB}/*/*.c))
LIBFOLDERS:=$(patsubst ${LIB}/%/,${LIBOBJ}/%,$(wildcard ${LIB}/*/))
EXES:=${OBJFOLDERS:${OBJ}/%=${BIN}/%.exe} $(patsubst ${SRC}/%.c,${BIN}/%.exe,$(wildcard ${SRC}/*.c))

# Compiler setup
CC:=gcc
CWARNS:=all extra no-microsoft-anon-tag no-dangling-else #required because of one of my macros :moai:
CFLAGS:=${CWARNS:%=-W%} -I${HDR} -I${LIB} -include${HDR}/Keywords.h -include${LIB}/System/String.h -std=c17 -fms-extensions

all: ${EXES}

run: all
	${EXES:%=echo; TARGET=%; $${TARGET} || echo -e "\n$${TARGET}: exit $$?";}

# Basically enable Make to take filenames as arguments
$(basename $(notdir ${EXES})): %: ${BIN}/%.exe
	@$<

.SECONDEXPANSION:
${EXES}: $$(filter $$(basename $$(patsubst ${BIN}/%,${OBJ}/%,$$@))%,${OBJFOLDERS} ${OBJECTS}) ${LIBOBJECTS} | ${BIN}
	${CC} ${CFLAGS} $(filter %.o,$^) -o $@

${OBJECTS}: ${OBJ}/%.o: ${SRC}/%.c | ${OBJ}
	${CC} ${CFLAGS} -c $< -o $@

.SECONDEXPANSION:
${LIBOBJECTS}: ${LIBOBJ}/%.o: ${LIB}/%.c $(wildcard ${HDR}/*.h) $$(wildcard $$(subst ${LIBOBJ},${LIB},$$(dir $$@))/*.h) | ${LIBFOLDERS} ${LIBOBJ}
	${CC} ${CFLAGS} -c $< -o $@

${OBJFOLDERS}: | ${OBJ}
	mkdir $@

${LIBFOLDERS}: | ${OBJ} ${LIBOBJ}
	mkdir $@

${FOLDERS}:
	mkdir $@

clean:
	rm -f ${OBJECTS} ${EXES}

cleanlib:
	rm -f ${LIBOBJECTS}

reset:
	rm -rf ${BIN} ${OBJ}

debug:
	@echo -e "\033[35mUniversal dependencies:\033[35m${LIBOBJECTS:%=\n    %}\033[0m"
	@echo -e "\033[36mObject files:\033[33m${OBJECTS:%=\n    %}\033[0m"
	@echo -e "$(foreach ex,${EXES},$(patsubst %,\n\033[32m%,$(filter ${ex:${BIN}/%.exe=${OBJ}/%}%,${OBJECTS}))\033[33m\n  └──> \033[31m${ex}\033[0m)"

cc:
	${CC} --version

.PHONY: all run clean reset debug cc
