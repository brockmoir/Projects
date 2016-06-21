CC = gcc
CFLAGSo = -O -g
CFLAGSf = -lm -o

newt: newt.c
	$(CC) $(CFLAGSo) newt.c $(CFLAGSf) newt
	./newt

spec: spec.c
	$(CC) $(CFLAGSo) spec.c $(CFLAGSf) spec
	./spec

schw: schw.c
	$(CC) $(CFLAGSo) schw.c $(CFLAGSf) schw
	./schw
all: 
	$(CC) $(CFLAGSo) schw.c $(CFLAGSf) schw
	$(CC) $(CFLAGSo) spec.c $(CFLAGSf) spec
	$(CC) $(CFLAGSo) newt.c $(CFLAGSf) newt
	./newt
	./spec
	./schw

