all: client server main_file

ex.h: ex.x
	rpcgen ex.x

tools.o: tools.c
	gcc -c tools.c -o tools.o

ex_svc.c ex_clnt.c exClient.c: ex.h

ex_xdr.o: ex_xdr.c
	gcc -o ex_xdr.o -c ex_xdr.c

main_file: main_file.c tools.o
	gcc -o main_file tools.o main_file.c

client: exClient.o ex_clnt.o tools.o ex_xdr.o
	gcc -o client -pthread exClient.o ex_clnt.o tools.o ex_xdr.o -lnsl

server: exServer.o ex_svc.o tools.o ex_xdr.o
	gcc -o server exServer.o ex_svc.o tools.o ex_xdr.o -lnsl

.PHONY: clean

clean:
	-rm *.o
	-rm client*
	-rm server*
	-rm main_file
	-rm *.serv
	-rm ex.h
	-rm exClnt.c
	-rm ex_svc.c
	-rm ex_svc.c
	-rm ex_clnt.c
