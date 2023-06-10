clean:
	rm server send

send:
	g++ -march=native send_message.cpp -o send

server:
	g++ -march=native server.cpp -o server

test: send server

rebuild: clean build

compile:
	gcc -c message.c -o network.o

retest: clean test
