#
all : strgcomp componentsreadbin readalldata conncomp waysreadbin hierachy metisexport metisexport2
	echo done
#bgl 
#
bgl :
	mpiCC -o bgl  bgl.cpp -lboost_system -lboost_mpi 

strgcomp : strongcomponents.cpp
	mpiCC -g -o strgcomp  strongcomponents.cpp -lboost_system  -lboost_graph
#-save-temps
metisexport : metisexport.cpp
	g++ -g -o metisexport  metisexport.cpp -lboost_system  -lboost_graph

metisexport2 : metisexport2.cpp
	g++ -g -o metisexport2  metisexport2.cpp -lboost_system  -lboost_graph

conncomp : conncomponents.cpp
	mpiCC -g -o conncomp  conncomponents.cpp -lboost_system  -lboost_graph

componentsreadbin : componentsreadbin.c
	gcc -o componentsreadbin componentsreadbin.c
# use this to create a pack file perl process_components.pl

readalldata : readalldata.cpp FOSMBin.hpp
	g++ -g -o readalldata readalldata.cpp

waysreadbin : waysreadbin.c 
	g++ -o waysreadbin waysreadbin.c

dumplatlon : dumplatlon.cpp FOSMBin.hpp
	g++ -o dumplatlon dumplatlon.cpp

hierachy : hierarchybuilder.cpp FOSMBin.hpp
	g++ -o hierachy hierarchybuilder.cpp

