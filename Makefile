streams : main.o message.o lock.o server.o log.o game.o  memory.o semaphore.o
	gcc out/main.o out/message.o out/lock.o out/server.o out/log.o out/game.o out/memory.o out/semaphore.o -o out/streams

main.o : main.c message.c log.c lock.c server.c game.c
	gcc -c main.c -o out/main.o

message.o : message.c log.c
	gcc -c message.c -o out/message.o

lock.o : lock.c log.c
	gcc -c lock.c -o out/lock.o

server.o : server.c
	gcc -c server.c -o out/server.o

log.o : log.c
	gcc -c log.c -o out/log.o

game.o : game.c
	gcc -c game.c -o out/game.o

memory.o : memory.c log.c
	gcc -c memory.c -o out/memory.o

semaphore.o : semaphore.c log.c
	gcc -c semaphore.c -o out/semaphore.o
