/*
 * File:   TrainNetwork.h
 * Author: Olivier Cuisenaire
 *
 * Created on 18. novembre 2014, 10:48
 */

#ifndef __ASD2__TrainNetwork__
#define __ASD2__TrainNetwork__


#include <stdio.h>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <fstream>
#include <sstream>

#include "Util.h"

// Classe lisant et donnant accès au reseau ferroviaire des CFF

class TrainNetwork {
public:
    
    // Classe City stocke le nom de la ville et l'indice des lignes qui
    // en partent dans le tableau lines
    struct City {
    public:
        std::string name;
        std::vector<int> lines;
    };
    
    // Classe Line, stocke les indices dans cities des deux villes que la ligne relie,
    // la longueur en kilometres de la ligne, la duree du trajet en minutes ainsi que
    // le nombre de voies de la ligne.
    struct Line {
    public:
        Line(int s1, int s2, int length, int duration, int nbTracks) :
        cities(std::make_pair(s1,s2)), length(length), duration(duration), nbTracks(nbTracks) { }
        
        std::pair<int,int> cities;
        int length;
        int duration;
        int nbTracks;
    };
    
    // vecteur des villes du reseau
    std::vector<City> cities;
    
    // vecteur des lignes du reseau
    std::vector<Line> lines;
    
    // permet de trouver l'indice d'une ville dans cities par son nom.
    std::map<std::string,int> cityIdx;
    
    int str2int(const std::string& s);
    
    // affiche le reseau
    friend std::ostream& operator << (std::ostream& os, const TrainNetwork& sn) {
        for( auto s : sn.lines )
            os << sn.cities[s.cities.first].name << " - "
            << sn.cities[s.cities.second].name
            << " : \t" << s.length << " kilomètres"
            << " , " << s.duration << " minutes"
            << " , " << s.nbTracks << " voies"
            << std::endl;
        return os;
    }
    
    // Constructeur avec nom de fichier.
    TrainNetwork(const std::string& filename);
};


#endif /* defined(__ASD2__TrainNetwork__) */
