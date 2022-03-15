//
// Created by Mert Biyikli on 07.03.22.
//

#ifndef BREAKINGCYCLES_GRAPH_IO_H
#define BREAKINGCYCLES_GRAPH_IO_H


#include "../include/graph.h"
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


/* This is for reading the first line of the file. Should be of the format
n m t , where n = |V|, m=|E|, t defining whether the graph has node or edge weights */
//void ReadHeader(std::istream& file, NodeID& numNodes, EdgeID& numEdges);

/* For reading the graph as defined here: https://pacechallenge.org/2022/tracks/ */
int ReadGraph(const std::string& file, Graph& G);

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

    G.InitTheGraph(numNodes, numEdges);
    int NodeCounter = 0;
    int EdgeCounter = 0;
    int AdjacentCounter = 0;
    NodeID node = 0;
    while(  std::getline(in, Line)) {
        //If the line starts with a '%' the line is a comment
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
            G.graph[node].push_back(target-1);
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

std::vector<bool> readSolution(const int num_nodes,
                               const std::string& filename) {
    std::vector<bool> fvs(num_nodes, false);

    std::string line;

    // open file for reading
    std::ifstream in(filename.c_str());
    if (!in) {
        std::cerr << "Error opening " << filename << std::endl;
        std::exit(-1);
    }

    while( std::getline(in, line) ) {

        std::stringstream ss(line);
        long u;
        ss >> u;

        if ( u > num_nodes ) {
            std::cerr << "Invalid node in solution ( Node = " << u << " )" << std::endl;
            std::exit(-1);
        }

        if ( !ss.eof() ) {
            std::cerr << "Line contains more than one node ( Line = '" << line << "'" << std::endl;
            std::exit(-1);
        }

        if ( fvs[u - 1] ) {
            std::cerr << "Node " << u << " is contained multiple times in solution" << std::endl;
            std::exit(-1);
        }

        fvs[--u] = true;

        if(in.eof()) {
            break;
        }
    }

    return fvs;
}

#endif //BREAKINGCYCLES_GRAPH_IO_H
