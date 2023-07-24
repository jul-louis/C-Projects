CC	= gcc -Wall -O
CLI	= client.o tands.o
SER	= server.o tands.o

all: client server getmanpdf

client.o: client.c
	$(CC) -c client.c -o client.o

server.o: server.c
	$(CC) -c server.c -o server.o

tands.o: tands.c
	$(CC) -c tands.c -o tands.o

client: $(CLI)
	$(CC) -o client $(CLI)

server: $(SER)
	$(CC) -o server $(SER)

getmanpdf:
	bash -c "groff -Tpdf -mandoc client.man > client.pdf"
	bash -c "groff -Tpdf -mandoc server.man > server.pdf"

clean:
	$(RM) client server *.o