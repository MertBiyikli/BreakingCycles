//
// Created by Mert Biyikli on 21.04.22.
//

#ifndef BREAKINGCYCLES_GRAPH_IO_H
#define BREAKINGCYCLES_GRAPH_IO_H

#include "../include/graph.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>

/*
int ReadGraph(Graph& G)
{
    string Line;
    std::getline(cin,Line);
    //skip comments
    while( Line[0] == '%' ) {
        std::getline(cin, Line);
    }

    int numNodes, numEdges, t;
    std::stringstream ss(Line);
    ss >> numNodes;
    ss >> numEdges;
    ss >> t;
    G.Init(numNodes);
    //cout << G.GetNumVertices() << " " << G.GetNumEdges() << endl;
    int NodeCounter = 0;
    int EdgeCounter = 0;
    int AdjacentCounter = 0;
    NodeID node = 0;
    std::string command;
    for(int i = 0; i<numNodes; i++) {
        //If the line starts with a '%' the line is a comment
        std::getline(cin, Line);
        if (Line[0] == '%') {
            continue;
        }
        AdjacentCounter++;
        std::stringstream ss(Line);
        NodeID target;
        NodeCounter++;
        while (ss >> target) {
            //check for self-loops
            if (target - 1 == node) {
                std::cerr
                        << "The graph file contains self-loops. This is not supported. Please remove them from the file."
                        << std::endl;
                return -1;
            }
            EdgeCounter++;
            //edge_counter++;
            //EdgeID e = G.new_edge(node, target-1);
            //G.graph[node].push_back(target-1);
            G.AddEdge(node, target-1);
            EdgeCounter++;
            //G.setEdgeWeight(e, edge_weight);
        }
        NodeCounter++;
        node++;
    }
    if(NodeCounter!=numNodes)
    {
        return -1;
    }
    if(EdgeCounter!= numEdges)
    {
        return -1;
    }*/
    //return 0;
//}*/

/*
int ReadGraph(const std::string& file, Graph& G) {
    std::string Line;
    std::ifstream in(file.c_str());
    if (!in) {
        std::cerr << "Error opening " << file << std::endl;
        std::exit(-1);
    }

    int t; // actually dont need it, but it has to be read from the file
    std::getline(in,Line);
    //skip comments
    while( Line[0] == '%' ) {
        std::getline(in, Line);
    }

    int numNodes, numEdges;
    std::stringstream ss(Line);
    ss >> numNodes;
    ss >> numEdges;
    ss >> t;

    G.Init(numNodes);
    for(int it = 0; it<numNodes; it++)
    {
        G.AddVertex(it);
    }
    int NodeCounter = 0;
    int EdgeCounter = 0;
    int AdjacentCounter = 0;
    int node = 0;
    while(  getline(in, Line)) {
        //If the line starts with a '%' the line is a comment
        if (Line[0] == '%') {
            continue;
        }
        AdjacentCounter++;
        std::stringstream ss(Line);
        int target;
        NodeCounter++;
        while (ss >> target) {
            //check for self-loops
            if (target - 1 == node) {
                std::cerr
                        << "The graph file contains self-loops. This is not supported. Please remove them from the file."
                        << std::endl;
                return -1;
            }
            EdgeCounter++;
            //edge_counter++;
            //EdgeID e = G.new_edge(node, target-1);
            G.AddEdge(node, target-1);
            //G.setEdgeWeight(e, edge_weight);
        }
        node++;
    }
    if(NodeCounter!=numNodes)
    {
        std::cerr << "The number of nodes dont match the number of nodes read" << std::endl;
        return -1;
    }
    if(EdgeCounter!= numEdges)
    {
        std::cerr << "The number of edges dont match the number of edges read" << std::endl;
        return -1;
    }
    return 0;
}
*/



int ReadGraph(Graph& G)
{
    string Line;
    std::getline(cin,Line);
    //skip comments
    while( Line[0] == '%' ) {
        std::getline(cin, Line);
    }

    int numNodes, numEdges, t;
    std::stringstream ss(Line);
    ss >> numNodes;
    ss >> numEdges;
    ss >> t;
    G.Init(numNodes);
    for(int it = 0; it<numNodes; it++)
    {
        G.AddVertex(it);
    }
    int NodeCounter = 0;
    int EdgeCounter = 0;
    int AdjacentCounter = 0;
    NodeID node = 0;
    std::string command;
    for(int i = 0; i<numNodes; ++i) {
        //If the line starts with a '%' the line is a comment
        std::getline(cin, Line);
        if (Line[0] == '%') {
            i--;
            continue;
        }
        AdjacentCounter++;
        std::stringstream ss(Line);
        NodeID target;
        NodeCounter++;
        while (ss >> target) {
            //check for self-loops
            if (target - 1 == node) {
                std::cerr
                        << "The graph file contains self-loops. This is not supported. Please remove them from the file."
                        << std::endl;
                return -1;
            }
            EdgeCounter++;
            G.AddEdge(node, target-1);
            EdgeCounter++;
        }
        NodeCounter++;
        node++;
    }
    return 0;
}
#endif //BREAKINGCYCLES_GRAPH_IO_H
