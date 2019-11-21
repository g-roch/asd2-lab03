/**
 * @file main.cpp
 * Labo3
 * @author Gabriel Roch
 * @author Gwendoline Dossegger
 * @author Jean-Luc Blanc
 * 
 * Created by Olivier Cuisenaire on 18.11.14.
 */

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

/**
 * @brief Affichage des lignes de trains
 * @param os, flux de sortie
 * @param path, path chemin de station(id)
 * @param tn, réseau de trains et de lignes complet
 * @return 
 */
template<typename Path>
ostream & printVia(ostream & os, Path path, TrainNetwork const & tn) {
		os << "  via " << tn.cities[path.front().From()].name;
		for(auto const & i : path) {
			os << " -> " << tn.cities[i.To()].name;
		}
		os << endl << endl;
		return os;
}


/**
 * @brief Calcule et affiche le plus reseau a renover couvrant toutes les villes le moins cher.
             Le prix pour renover 1 km de chemin de fer est de :
            - 15 CHF par km pour les lignes ayant 4 voies
            - 10 CHF par km pour les lignes ayant 3 voies
            - 6 CHF par km pour les lignes ayant 2 voies
            - 3 CHF par km pour les lignes ayant 1 voie
 * @param tn, réseau de trains et de lignes
 */
void ReseauLeMoinsCher(TrainNetwork &tn) {
	std::vector<int> cost = {0, 3, 6, 10, 15};
	TrainGraphWrapper tgw(tn, [&cost] (TrainNetwork::Line l)-> int { return cost.at(l.nbTracks)*l.length; });
	//auto mst = MinimumSpanningTree<TrainGraphWrapper>::EagerPrim(tgw);
	auto mst = MinimumSpanningTree<TrainGraphWrapper>::Kruskal(tgw);
	unsigned int weightTotal = 0;
	for(auto const & i : mst) {
		weightTotal += i.Weight();
		cout << tn.cities[i.Either()].name  << " - "
		     << tn.cities[i.Other(i.Either())].name << " : "
		     << i.Weight() << " MF" << endl;
	}
	cout << endl << "Coût Total : " << weightTotal << " MF" << endl << endl;
}


/**
 * @brief Calcule et affiche le plus court chemin de la ville depart a la ville arrivee
 *          en passant par le reseau ferroviaire tn. Le critere a optimiser est la distance.
 * @param depart, Ville de départ
 * @param arrivee, Ville d'arrivée
 * @param tn, réseau de trains et de lignes complet
 */
void PlusCourtChemin(const string& depart, const string& arrivee, TrainNetwork& tn) {
	TrainDiGraphWrapper tgw(tn, [] (TrainNetwork::Line l)-> int { return l.length; });
	DijkstraSP<TrainDiGraphWrapper> referenceSP(tgw,tn.cityIdx[depart]);
	cout << "  longueur = " << referenceSP.DistanceTo(tn.cityIdx[arrivee]) << " km" << endl;
	printVia(cout, referenceSP.PathTo(tn.cityIdx[arrivee]), tn);
}


/**
 * @brief Calcule et affiche le plus court chemin de la ville depart a la ville arrivee
 *        en passant par le reseau ferroviaire tn ayant une ville en travaux et donc
 *        inaccessible. Vous pouvez mettre un cout infini aux arcs ayant comme depart ou
 *        comme arrivee cette ville en travaux. Le critere a optimiser est la distance.
 * @param depart, Ville de départ
 * @param arrivee, Ville d'arrivée
 * @param gareEnTravaux, gare où les travaux ont lieu
 * @param tn, réseau de trains et de lignes complet
 */
void PlusCourtCheminAvecTravaux(const string& depart, const string& arrivee, const string& gareEnTravaux, TrainNetwork& tn) {
	int idGareEnTravaux = tn.cityIdx.at(gareEnTravaux);

	TrainDiGraphWrapper tgw(tn, [idGareEnTravaux] (TrainNetwork::Line l)-> int {
			if(l.cities.first == idGareEnTravaux or l.cities.second == idGareEnTravaux)
				return std::numeric_limits<int>::max();
			else
				return l.length; 
			});
	DijkstraSP<TrainDiGraphWrapper> referenceSP(tgw,tn.cityIdx[depart]);
	cout << "  longueur = " << referenceSP.DistanceTo(tn.cityIdx[arrivee]) << " km" << endl;
	printVia(cout, referenceSP.PathTo(tn.cityIdx[arrivee]), tn);

}


/**
 * @brief Calcule et affiche le plus rapide chemin de la ville depart a la ville arrivee via la ville "via"
 *        en passant par le reseau ferroviaire tn. Le critere a optimiser est le temps de parcours
 * @param depart, Ville de départ
 * @param arrivee, Ville d'arrivée
 * @param via, direction
 * @param tn, réseau de trains et de lignes complet
 */
void PlusRapideChemin(const string& depart, const string& arrivee, const string& via, TrainNetwork& tn) {
	TrainDiGraphWrapper tgw(tn, [] (TrainNetwork::Line l)-> int { return l.duration; });
	DijkstraSP<TrainDiGraphWrapper> part1(tgw,tn.cityIdx[depart]);
	DijkstraSP<TrainDiGraphWrapper> part2(tgw,tn.cityIdx[via]);
	auto tot = part1.DistanceTo(tn.cityIdx[via]) + part2.DistanceTo(tn.cityIdx[arrivee]);
	cout << "  temps = " << tot << " minutes" << endl;
	auto path = part1.PathTo(tn.cityIdx[via]);
	auto path2 = part2.PathTo(tn.cityIdx[arrivee]);
	path.insert(path.end(), path2.begin(), path2.end());
	printVia(cout, path, tn);
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



/**
 * @brief Fonction principale permettant d'effectué les tests
 */
int main() {

    // Permet de tester votre implémentation de Dijkstra
    testShortestPath("tinyEWD.txt");
    testShortestPath("mediumEWD.txt");
    testShortestPath("1000EWD.txt");
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

