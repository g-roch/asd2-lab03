//
//  main.cpp
//  Labo3
//
//  Created by Olivier Cuisenaire on 18.11.14.
//
//

#include <cstdlib>
#include <iostream>
#include <ctime>

#include "TrainNetwork.h"
#include "MinimumSpanningTree.h"
#include "ShortestPath.h"

#include "EdgeWeightedGraph.h"
#include "EdgeWeightedDiGraph.h"

#include "TrainGraphWrapper.h"

using namespace std;

template<typename Path>
ostream & printVia(ostream & os, Path path, TrainNetwork const & tn, int v) {
		os << "  via " << tn.cities[path.front().From()].name;
		for(auto const & i : path) {
			os << " → " << tn.cities[i.To()].name;
		}
		os << endl;
		return os;
}

// Calcule et affiche le plus reseau a renover couvrant toutes les villes le moins cher.
// Le prix pour renover 1 km de chemin de fer est de :
// - 15 CHF par km pour les lignes ayant 4 voies
// - 10 CHF par km pour les lignes ayant 3 voies
// - 6 CHF par km pour les lignes ayant 2 voies
// - 3 CHF par km pour les lignes ayant 1 voie
void ReseauLeMoinsCher(TrainNetwork &tn) {
		std::vector<int> cost = {0, 3, 6, 10, 15};
    TrainGraphWrapperCostTrack tgw(tn, cost);
		tgw.forEachEdge([] (auto i) { cout << i.Either() << " ←→ " << i.Other(i.Either()) << endl; });
		auto mst = MinimumSpanningTree<TrainGraphWrapperCostTrack>::EagerPrim(tgw);
    //auto mst = MinimumSpanningTree<TrainGraphWrapperCostTrack>::Kruskal(tgw);
		for(auto const & i : mst) {
			cout << tn.cities[tn.lines[i.Either()].cities.first].name << " (" << tn.lines[i.Either()].cities.first << ") → ";
			cout << tn.cities[tn.lines[i.Either()].cities.second].name << " (" << tn.lines[i.Either()].cities.second << ") : ";
			cout << i.Weight() << " MF" << endl;
		}
    /* A IMPLEMENTER */
}

// Calcule et affiche le plus court chemin de la ville depart a la ville arrivee
// en passant par le reseau ferroviaire tn. Le critere a optimiser est la distance.
void PlusCourtChemin(const string& depart, const string& arrivee, TrainNetwork& tn) {
    TrainGraphWrapperDistance tgw(tn);
    DijkstraSP<TrainGraphWrapperDistance> referenceSP(tgw,tn.cityIdx[depart]);
		cout << "  longueur = " << referenceSP.DistanceTo(tn.cityIdx[arrivee]) << " km" << endl;
		printVia(cout, referenceSP.PathTo(tn.cityIdx[arrivee]), tn, tn.cityIdx[arrivee]);
    /* A IMPLEMENTER */
}

// Calcule et affiche le plus court chemin de la ville depart a la ville arrivee
// en passant par le reseau ferroviaire tn ayant une ville en travaux et donc
// inaccessible. Vous pouvez mettre un cout infini aux arcs ayant comme depart ou
// comme arrivee cette ville en travaux. Le critere a optimiser est la distance.
void PlusCourtCheminAvecTravaux(const string& depart, const string& arrivee, const string& gareEnTravaux, TrainNetwork& tn) {
	TrainNetwork tnTravaux(tn);
	int idGareEnTravaux = tnTravaux.cityIdx.at(gareEnTravaux);

	for(int lineid : tnTravaux.cities[idGareEnTravaux].lines) {
		tnTravaux.lines[lineid].length = std::numeric_limits<int>::max();
	}
	tnTravaux.cities[idGareEnTravaux].lines.resize(0);

	PlusCourtChemin(depart, arrivee, tnTravaux);
    /* A IMPLEMENTER */
}

// Calcule et affiche le plus rapide chemin de la ville depart a la ville arrivee via la ville "via"
// en passant par le reseau ferroviaire tn. Le critere a optimiser est le temps de parcours
void PlusRapideChemin(const string& depart, const string& arrivee, const string& via, TrainNetwork& tn) {
  TrainGraphWrapperDuration tgw(tn);
	DijkstraSP<TrainGraphWrapperDuration> part1(tgw,tn.cityIdx[depart]);
	DijkstraSP<TrainGraphWrapperDuration> part2(tgw,tn.cityIdx[via]);
	auto tot = part1.DistanceTo(tn.cityIdx[via]) + part2.DistanceTo(tn.cityIdx[arrivee]);
	cout << "  temp = " << tot << " minutes" << endl;
	auto path = part1.PathTo(tn.cityIdx[via]);
	auto path2 = part2.PathTo(tn.cityIdx[arrivee]);
	path.insert(path.end(), path2.begin(), path2.end());
	printVia(cout, path, tn, tn.cityIdx[arrivee]);

    /* A IMPLEMENTER */
}


// compare les algorithmes Dijkstra et BellmanFord pour calculer les plus courts chemins au
// sommet 0 dans le graphe defini par filename.
void testShortestPath(string filename)
{
    cout << "Testing " << filename << endl;

    bool ok = true;

    typedef EdgeWeightedDiGraph<double> Graph;
    Graph ewd(filename);

    clock_t startTime = clock();

    BellmanFordSP<Graph> referenceSP(ewd,0);

    cout << "Bellman-Ford: " << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;
    startTime = clock();

    DijkstraSP<Graph> testSP(ewd,0);


    cout << "Dijkstra:     " << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;

    for (int v=0; v<ewd.V(); ++v) {
        if (referenceSP.DistanceTo(v) != testSP.DistanceTo(v) ) {
            cout << "Oops: vertex" << v << " has " << referenceSP.DistanceTo(v) << " != " <<  testSP.DistanceTo(v) << endl;
            ok = false;
            break;
        }
    }

    if(ok) cout << " ... test succeeded " << endl << endl;
}



int main() {

    // Permet de tester votre implémentation de Dijkstra
    //testShortestPath("tinyEWD.txt");
    //testShortestPath("mediumEWD.txt");
    //testShortestPath("1000EWD.txt");
    //testShortestPath("10000EWD.txt");

    TrainNetwork tn("reseau.txt");

    cout << "1. Quelles lignes doivent etre renovees ? Quel sera le cout de la renovation de ces lignes ?" << endl;

    ReseauLeMoinsCher(tn);

    cout << "2. Chemin le plus court entre Geneve et Coire" << endl;

    PlusCourtChemin("Geneve", "Coire", tn);

    cout << "3. Chemin le plus court entre Geneve et Coire, avec la gare de Sion en travaux" << endl;

    PlusCourtCheminAvecTravaux("Geneve", "Coire", "Sion", tn);

    cout << "4. Chemin le plus rapide entre Geneve et Coire en passant par Brigue" << endl;

    PlusRapideChemin("Geneve", "Coire", "Brigue", tn);

    cout << "5. Chemin le plus rapide entre Lausanne et Zurich en passant par Bale" << endl;

    PlusRapideChemin("Lausanne", "Zurich", "Bale", tn);

    return EXIT_SUCCESS;
}

