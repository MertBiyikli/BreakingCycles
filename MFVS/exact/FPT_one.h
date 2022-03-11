//
// Created by Mert Biyikli on 09.03.22.
//

#ifndef BREAKINGCYCLES_FPT_ONE_H
#define BREAKINGCYCLES_FPT_ONE_H

#include "../include/graph.h"

class FPT_one{
public:
    Graph G;
    FPT_one(const Graph& G);

};

FPT_one::FPT_one(const Graph &G_)
{
    this->G = G_;
}





#endif //BREAKINGCYCLES_FPT_ONE_H
