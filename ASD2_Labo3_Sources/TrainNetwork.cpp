/*
 * File:   TrainNetwork.cpp
 * Author: Olivier Cuisenaire
 * Modified: 
 *
 * Created on 18. novembre 2014, 10:48
 */

#include "TrainNetwork.h"

int TrainNetwork::str2int(const std::string& s) {
    std::istringstream iss(s);
    int i;
    iss >> i;
    return i;
}

TrainNetwork::TrainNetwork(const std::string& filename)
{
    std::ifstream s(filename);
    
    int N;
    s >> N ;
    
    cities.resize(N);
    for(int i=0;i<N;++i) {
        std::string name;
        std::getline(s,name);
        
        if (!name.empty() && name[name.size() - 1] == '\r')
            name.erase(name.size() - 1);
        if(name.size()) {
            cities[i].name = name;
            cityIdx[name] = i;
        } else --i;
    }
    
    while (s.good()) {
        std::string line;
        std::getline(s,line);

        auto elements = split(line,';');
        
        int s1 = cityIdx[elements[0]];
        int s2 = cityIdx[elements[1]];

        int rIdx = int(lines.size());
        cities[s1].lines.push_back(rIdx);
        cities[s2].lines.push_back(rIdx);
        lines.push_back(Line(s1,s2,
                             str2int(elements[2]),
                             str2int(elements[3]),
                             str2int(elements[4])
                             ));
    }
    s.close();
}
