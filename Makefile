#
all : strgcomp
	echo done
#bgl 
#-save-temps
bgl :
	mpiCC -o bgl  bgl.cpp -lboost_system -lboost_mpi 

strgcomp : strongcomponents.cpp
	mpiCC -g -o strgcomp  strongcomponents.cpp -lboost_system  -lboost_graph