#
all : strgcomp componentsreadbin readalldata
	echo done
#bgl 
#-save-temps
bgl :
	mpiCC -o bgl  bgl.cpp -lboost_system -lboost_mpi 

strgcomp : strongcomponents.cpp
	mpiCC -g -o strgcomp  strongcomponents.cpp -lboost_system  -lboost_graph

componentsreadbin : componentsreadbin.c
	gcc -o componentsreadbin componentsreadbin.c
# use this to create a pack file perl process_components.pl

readalldata : readalldata.cpp
	g++ -g -o readalldata readalldata.cpp

waysreadbin : waysreadbin.c
	g++ -o waysreadbin waysreadbin.c
