INC=-I/opt/cunit/include -I/usr/local/engine/engine -I/usr/local/engine/nginx-1.19.3/src/core -I/usr/local/engine/nginx-1.19.3/src/http -I/usr/local/engine/nginx-1.19.3/objs -I/usr/local/engine/nginx-1.19.3/src/os/unix -I/usr/local/engine/nginx-1.19.3/src/event -I/usr/local/engine/nginx-1.19.3/src/http/modules -I/usr/include/python3.6m

LIB=-L/opt/cunit/lib
LIB2=`python3.6-config --ldflags`

main: cunit.o utils.o engine_pin.o
	gcc -o main cunit.o utils.o engine_pin.o ${LIB} -lcunit

cunit.o: cunit.c
	gcc -c $(INC) cunit.c

utils.o: ../../engine/utils.c
	gcc -c $(INC) ../../engine/utils.c

engine_pin.o: ../../engine/engine_pin.c
	gcc -D UNITTEST -c $(INC) ../../engine/engine_pin.c
