/* 
 * File:   EdgeWeightedDigraph.h
 * Author: Olivier Cuisenaire
 *
 * Created on 05. novembre 2014, 13:26
 */

#ifndef ASD2_EdgeWeightedDigraph_h
#define ASD2_EdgeWeightedDigraph_h

#include "EdgeWeightedGraphCommon.h"

// Classe definissant les arcs ponderes (orientes)
// derive de la class EdgeCommon

template< typename T> // Type du poids, par exemple int ou double
					  // T doit etre comparable, et être un parametre
					  // valide pour std::numeric_limits
struct WeightedDirectedEdge  : public EdgeCommon<T> {
	 // defini la class mere comme BASE.
	typedef EdgeCommon<T> BASE;
	
public:
	// Constructeur par defaut. Appelle BASE() par defaut
	WeightedDirectedEdge() { }
	
	// Constructeur spécifiant les deux sommets et le poids
	WeightedDirectedEdge(int v, int w, T _weight) : BASE(v,w,_weight) { }
	
	// Sommet de départ de l'arc
	int From() const { return this->v1; }

	// Sommet d'arrivée de l'arc
	int To() const { return this->v2; }
	
	// Affiche "v1->v2 (weight)"
	friend std::ostream& operator << (std::ostream& s, const WeightedDirectedEdge<T>& e) {
		return s << e.From() << "->" << e.To() << " (" << e.Weight() << ")";
	}
};



// Classe definissant un graphe pondere non oriente.
// Elle herite de EdgeWeightedGraphCommon en
// specifiant des aretes de type Edge<T>

template< typename T> // Type du poids, par exemple int ou double
					  // T doit etre comparable, et être un parametre
					  // valide pour std::numeric_limits
class EdgeWeightedDiGraph : public EdgeWeightedGraphCommon< WeightedDirectedEdge<T> > {
	// defini la class mere comme BASE.
	typedef EdgeWeightedGraphCommon< WeightedDirectedEdge<T> > BASE;
	
public:
	// Type des arcs
	typedef typename BASE::Edge Edge;
   
	// Type de donnée pour les poids
	typedef typename BASE::WeightType WeightType;
	
	// Constructeur a partir d'un nom de fichier
	EdgeWeightedDiGraph(const std::string& filename) {
		this->ReadFromFile(filename);
	}
	
	// Constructeur a partie d'un stream
	EdgeWeightedDiGraph(std::istream& s) {
		this->ReadFromStream(s);
	}
	
	// Constructeur specifiant le nombre de sommet
	// Il faudra appeler addEdge pour ajouter les arcs
	EdgeWeightedDiGraph(int N) : BASE(N) {
	}
	
	// Ajoute un arcs de poids weight de v q w
	// On ajoute cette arete à la liste d'adjacence de v
	virtual void addEdge(int v, int w, WeightType weight) {
		this->edgeAdjacencyLists.at(v).push_back(Edge(v,w,weight));
	}
	
	// Parcours de tous les sommets adjacents au sommet v
	// la fonction f doit prendre un seul argument de type int
	template<typename Func>
	void forEachAdjacentVertex(int v, Func f) const  {
		for(const Edge& e : this->adjacentEdges(v))
			f(e.To());
	}

	// Parcours de tous les arcs du graphe.
	// la fonction f doit prendre un seul argument de type
	// EdgeWeightedDiGraph::Edge
	template<typename Func>
	void forEachEdge(Func f) const  {
		for(int v=0;v<BASE::V();++v)
			for(const Edge& e : this->adjacentEdges(v))
				f(e);
	}
};
	
#endif
