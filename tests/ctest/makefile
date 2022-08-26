INC=-I/opt/cunit/include -I/var/engine/engine -I/var/nginx/nginx-1.19.3/src/core -I/var/nginx/nginx-1.19.3/src/http -I/var/nginx/nginx-1.19.3/objs -I/var/nginx/nginx-1.19.3/src/os/unix -I/var/nginx/nginx-1.19.3/src/event -I/var/nginx/nginx-1.19.3/src/http/modules

LIB=-L/opt/cunit/lib `python3.6-config --ldflags`

main: cunit.o utils.o engine_pin.o
	gcc -o main cunit.o utils.o engine_pin.o ${LIB} -lcunit

cunit.o: cunit.c
	gcc -c $(INC) cunit.c

utils.o: ../../engine/utils.c
	gcc -c $(INC) ../../engine/utils.c

engine_pin.o: ../../engine/engine_pin.c
	gcc -c $(INC) ../../engine/engine_pin.c
