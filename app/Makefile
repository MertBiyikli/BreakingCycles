CPP_FLAGS='-std=c++17'

output:  BreakingCycleHeuristic.o graph.o graph_io.o MakeMinimal.o
	g++ BreakingCycleHeuristic.cpp -o heurisitc

BreakingCycleHeuristic.o: BreakingCycleHeuristic.cpp
	g++ -c BreakingCycleHeuristic.cpp

graph.o: ../NewMFVS/include/graph.cpp ../NewMFVS/include/graph.h
	g++ -c ../NewMFVS/include/graph.cpp

graph_io.o: ../NewMFVS/io/graph_io.cpp ../NewMFVS/io/graph_io.h
	g++ -c ../NewMFVS/io/graph_io.cpp

MakeMinimal.o: ../NewMFVS/heuristic/MakeMinimalSCCSorting.cpp ../NewMFVS/heuristic/MakeMinimalSCCSorting.h
	g++ -c ../NewMFVS/heuristic/MakeMinimalSCCSorting.cpp

clean:
	rm *.o output