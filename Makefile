bin: 
	mkdir bin
src:
	mkdir src
pipes:
	mkdir pipes
openfiles: 
	geany src/server.cpp & geany src/game.cpp & geany src/protocol.cpp & geany src/viewer.cpp & geany src/headers/game.h & geany src/headers/protocol.h & geany src/headers/viewer.h
	

