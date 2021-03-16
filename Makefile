RELEASE_PATH=Release
OUTPUT=csv_maker.exe
OPT=-Wall -O2 $$(pkg-config --libs --cflags aclib) $$(pkg-config --libs --cflags s7lib)
SRC=src


all:
	clear
	cc ${SRC}/csv_maker.c ${SRC}/controler.c ${SRC}/controler_parser.c ${SRC}/csv_item.c ${SRC}/csv_io.c ${OPT} -o ${RELEASE_PATH}/${OUTPUT}

run:
	clear
	cc ${SRC}/csv_maker.c ${SRC}/controler.c ${SRC}/controler_parser.c ${SRC}/csv_item.c ${SRC}/csv_io.c ${OPT} -o ${RELEASE_PATH}/${OUTPUT}
	${RELEASE_PATH}/${OUTPUT}

clean:
	rm -vf *.o
	rm -vf *.a
	rm -vf ${RELEASE_PATH}/${OUTPUT}
