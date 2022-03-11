#include <iostream>
#include "MFVS/include/graph.h"
#include "MFVS/algorithms/SCC.h"
#include "MFVS/io/graph_io.h"
#include "MFVS/heuristic/Greedy.h"
#include "MFVS/exact/BranchAndBoundOne.h"

int main() {
    int ctr = 0;
    Graph G;
    int error = ReadGraph("e_001", G);
    if (error != 0) {
        std::cerr << "Problems accured!" << std::endl;
        return 1;
    }
    Greedy Gr(G);
    auto FVS_one = Gr.GreedyOne();
    cout << "first Greedy: " << isAcyclic(G, FVS_one.first) << " |FVS| = " << FVS_one.second << endl;
    auto FVS_two = Gr.GreedyTwo();
    cout << "first Greedy: " << isAcyclic(G, FVS_two.first) << " |FVS| = " << FVS_two.second << endl;


    SCC S(G);
    S.GetSCC();
    for (NodeID v = 0; v < S.components.size(); v++)
    {
        cout << v << ": " << S.components[v] << endl;
    }

    //cout << S.GetSCC_Number() << endl;

    ctr = 0;
    vector<int> tmp(G.GetNumVertices(), 0);
    for(auto i = 0; i<S.components.size(); i++){
        tmp[S.components[i]]++;
    }

//    G.print();


    bool ThereAreIndeed = true;
    for(NodeID v = 447; v<G.GetNumVertices(); v++)
    {
        for(NodeID w = 447; w<G.GetNumVertices(); w++)
        {
            if(w==v)
                continue;
            if(G.DFS(v, w) && G.DFS(w, v))
                cout << "These are in the SCC: " << v << " " << w << endl;
        }
    }


    for(int i=0; i<tmp.size(); i++)
    {
        cout << i << ": " << tmp[i] << endl;
    }


/*
    for(auto i : G.neighbors(12))
        cout << i << " ";
    cout << endl;
*/

    /*
    for(int i = 101; i<200; i+=2){
        Graph G_;
        int error = ReadGraph("e_" + to_string(i), G_);
        if(error!=0)
        {
            std::cerr << "Problems accured!" << std::endl;
            return 1;
        }

        cout << "graph " << i << endl;
        Greedy Gr(G_);
        auto FVS_one = Gr.GreedyOne();
        cout << "first Greedy: " << isAcyclic(G_, FVS_one.first) << " |FVS| = " << FVS_one.second << endl;
        auto FVS_two = Gr.GreedyTwo();
        cout << "first Greedy: " << isAcyclic(G_, FVS_two.first) << " |FVS| = " << FVS_two.second <<  endl;
    }*/
    cout << "Works" << endl;
    return 0;
}
