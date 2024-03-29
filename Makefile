run: bin/player1 bin/player2 bin/server | pipes/playerToServer0 pipes/serverToPlayer0 pipes/playerToServer1 pipes/serverToPlayer1 pipes/playerToServer2 pipes/serverToPlayer2 pipes/playerToServer3 pipes/serverToPlayer3 pipes/playerToServer4 pipes/serverToPlayer4 pipes/playerToServer5 pipes/serverToPlayer5 pipes/playerToServer6 pipes/serverToPlayer6 pipes/playerToServer7 pipes/serverToPlayer7 pipes/playerToServer8 pipes/serverToPlayer8 pipes/playerToServer9 pipes/serverToPlayer9
	bin/server pipes/playerToServer0 pipes/serverToPlayer0 pipes/playerToServer1 pipes/serverToPlayer1 pipes/playerToServer2 pipes/serverToPlayer2 pipes/playerToServer3 pipes/serverToPlayer3 pipes/playerToServer4 pipes/serverToPlayer4 pipes/playerToServer5 pipes/serverToPlayer5 pipes/playerToServer6 pipes/serverToPlayer6 pipes/playerToServer7 pipes/serverToPlayer7 pipes/playerToServer8 pipes/serverToPlayer8 pipes/playerToServer9 pipes/serverToPlayer9 \
	RED_TEAM_NAME BLUE_TEAM_NAME & \
	bin/player1 pipes/playerToServer0 pipes/serverToPlayer0 & \
	bin/player1 pipes/playerToServer1 pipes/serverToPlayer1 & \
	bin/player1 pipes/playerToServer2 pipes/serverToPlayer2 & \
	bin/player1 pipes/playerToServer3 pipes/serverToPlayer3 & \
	bin/player1 pipes/playerToServer4 pipes/serverToPlayer4 & \
	bin/player2 pipes/playerToServer5 pipes/serverToPlayer5 & \
	bin/player2 pipes/playerToServer6 pipes/serverToPlayer6 & \
	bin/player2 pipes/playerToServer7 pipes/serverToPlayer7 & \
	bin/player2 pipes/playerToServer8 pipes/serverToPlayer8 & \
	bin/player2 pipes/playerToServer9 pipes/serverToPlayer9 & wait

#in order to test your players, modify either player1 (for red team) or player2 (for blue team).


#pipes!
pipes:
	mkdir pipes

pipes/serverToPlayer0: | pipes
	mkfifo pipes/serverToPlayer0
pipes/playerToServer0: | pipes
	mkfifo pipes/playerToServer0
	
pipes/serverToPlayer1: | pipes
	mkfifo pipes/serverToPlayer1
pipes/playerToServer1: | pipes
	mkfifo pipes/playerToServer1

pipes/serverToPlayer2: | pipes
	mkfifo pipes/serverToPlayer2
pipes/playerToServer2: | pipes
	mkfifo pipes/playerToServer2
	
pipes/serverToPlayer3: | pipes
	mkfifo pipes/serverToPlayer3
pipes/playerToServer3: | pipes
	mkfifo pipes/playerToServer3
	
pipes/serverToPlayer4: | pipes
	mkfifo pipes/serverToPlayer4
pipes/playerToServer4: | pipes
	mkfifo pipes/playerToServer4
	
pipes/serverToPlayer5: | pipes
	mkfifo pipes/serverToPlayer5
pipes/playerToServer5: | pipes
	mkfifo pipes/playerToServer5
	
pipes/serverToPlayer6: | pipes
	mkfifo pipes/serverToPlayer6
pipes/playerToServer6: | pipes
	mkfifo pipes/playerToServer6
	
pipes/serverToPlayer7: | pipes
	mkfifo pipes/serverToPlayer7
pipes/playerToServer7: | pipes
	mkfifo pipes/playerToServer7
	
pipes/serverToPlayer8: | pipes
	mkfifo pipes/serverToPlayer8
pipes/playerToServer8: | pipes
	mkfifo pipes/playerToServer8
	
pipes/serverToPlayer9: | pipes
	mkfifo pipes/serverToPlayer9
pipes/playerToServer9: | pipes
	mkfifo pipes/playerToServer9


bin: 
	mkdir bin

bin/viewer.o: src/viewer.cpp src/headers/viewer.h src/headers/game.h | bin
	g++ -c src/viewer.cpp -o bin/viewer.o

bin/game.o: src/game.cpp src/headers/game.h | bin
	g++ -c src/game.cpp -o bin/game.o

bin/protocol.o: src/protocol.cpp src/headers/protocol.h src/headers/game.h | bin
	g++ -c src/protocol.cpp -o bin/protocol.o

bin/server.o: src/server.cpp src/headers/viewer.h src/headers/protocol.h src/headers/game.h | bin
	g++ -c src/server.cpp -o bin/server.o

bin/player1.o: players/player1.cpp src/headers/protocol.h src/headers/game.h | bin
	g++ -c players/player1.cpp -o bin/player1.o

bin/player2.o: players/player2.cpp src/headers/protocol.h src/headers/game.h | bin
	g++ -c players/player2.cpp -o bin/player2.o

bin/server: bin/server.o bin/viewer.o bin/game.o bin/protocol.o | bin
	g++ bin/server.o bin/viewer.o bin/game.o bin/protocol.o -lglut -lGL -pthread -o bin/server

bin/player1: bin/player1.o bin/game.o bin/protocol.o | bin
	g++ bin/player1.o bin/game.o bin/protocol.o -o bin/player1

bin/player2: bin/player2.o bin/game.o bin/protocol.o | bin
	g++ bin/player2.o bin/game.o bin/protocol.o -o bin/player2

clean:
	rm -f bin -R

kill:
	killall server & wait
	killall player & wait

install:
	sudo apt install mesa-utils
	sudo apt install freeglut3-dev

openfiles: 
	geany src/server.cpp & geany src/game.cpp & geany src/protocol.cpp & geany src/viewer.cpp & geany src/headers/game.h & geany src/headers/protocol.h & geany src/headers/viewer.h & geany players/player.cpp
	

