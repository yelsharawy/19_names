
all: interface worker

interface: interface.o common.o
	gcc -o interface interface.o common.o

worker: worker.o common.o
	gcc -o worker worker.o common.o

interface.o: interface.c common.h
	gcc -c interface.c

worker.o: worker.c common.h
	gcc -c worker.c

common.o: common.c common.h
	gcc -c common.c

clean:
	-rm ./*.o ./interface ./worker ./ptc_pipe ./ctp_pipe
