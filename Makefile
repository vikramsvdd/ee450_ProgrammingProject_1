# -*- Makefile -*-

#target: dependencies
	#action

all: servermain client

servermain: servermain.cpp
	g++ -o servermain servermain.cpp 


client: client.cpp
	g++ -o client client.cpp 


clean:
	rm*.o error_detection

