//
// Created by Mert Biyikli on 07.03.22.
//

#include <iostream>
#include "MFVS/include/graph.h"
#include "MFVS/algorithms/SCC.h"
#include "MFVS/io/graph_io.h"
#include "MFVS/heuristic/Greedy.h"
#include "MFVS/exact/BranchAndBoundOne.h"

int main(){
    int ctr = 0;
    Graph G;
    G.InitTheGraph(5, 6);
    int error = 1;//ReadGraph("e_001", G);
    if(error!=0)
    {
        std::cerr << "Problems accured!" << std::endl;
        return 1;
    }
    return 0;
}