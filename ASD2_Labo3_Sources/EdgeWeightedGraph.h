/* 
 * File:   EdgeWeightedGraph.h
 * Author: Olivier Cuisenaire
 *
 * Created on 27. octobre 2014, 16:24
 */

#ifndef __ASD2__EdgeWeightedGraph__
#define __ASD2__EdgeWeightedGraph__

#include "EdgeWeightedGraphCommon.h"

// Classe definissant les aretes ponderes (non orientees)
// derive de la class EdgeCommon

template< typename T> // Type du poids, par exemple int ou double
					  // T doit etre comparable, et être un parametre
					  // valide pour std::numeric_limits
struct WeightedEdge : public EdgeCommon<T> {
	// defini la class mere comme BASE.
	typedef EdgeCommon<T> BASE;
	
public:
	// Constructeur par defaut. Appelle BASE() par defaut
	WeightedEdge() { }
	
	// Constructeur spécifiant les deux sommets et le poids
	WeightedEdge(int v, int w, T _weight) : BASE(v,w,_weight) { }
	
	// Renvoie un des deux sommets
	int Either() const { return this->v1; }
	
	// Renvoie l'autre sommet
	int Other(int v) const { return (v==this->v1) ? this->v2 : this->v1; }

	// Affiche "v1-v2 (weight)"
	friend std::ostream& operator << (std::ostream& s, const WeightedEdge<T>& e) {
		return s << e.Either() << "-" << e.Other(e.Either()) << " (" << e.Weight() << ")";
	}
};

// Classe definissant un graphe pondere non oriente.
// Elle herite de EdgeWeightedGraphCommon en
// specifiant des aretes de type Edge<T>

template< typename T> // Type du poids, par exemple int ou double
					  // T doit etre comparable, et être un parametre
					  // valide pour std::numeric_limits
class EdgeWeightedGraph : public EdgeWeightedGraphCommon<WeightedEdge<T> > {
	// defini la class mere comme BASE.
	typedef EdgeWeightedGraphCommon< WeightedEdge<T> > BASE;
	
public:
	// Type des arêtes.
	typedef typename BASE::Edge Edge;
	
	 // Type de donnée pour les poids
	typedef typename BASE::WeightType WeightType;

	// Constructeur a partir d'un nom de fichier
	EdgeWeightedGraph(const std::string& filename) {
		this->ReadFromFile(filename);
	}
	
	// Constructeur a partie d'un stream
	EdgeWeightedGraph(std::istream& s) {
		this->ReadFromStream(s);
	}
	
	// Constructeur specifiant le nombre de sommet
	// Il faudra appeler addEdge pour ajouter les
	// aretes
	EdgeWeightedGraph(int N) : BASE(N) {
	}
	
	// Ajoute une arete de poids weight entre v et w
	// On ajoute cette arete aux listes d'adjacence des
	// deux sommets
	virtual void addEdge(int v, int w, WeightType weight) {
		this->edgeAdjacencyLists.at(v).push_back(Edge(v,w,weight));
		if(v != w) this->edgeAdjacencyLists.at(w).push_back(Edge(v,w,weight));
	}
	
	// Parcours de tous les sommets adjacents au sommet v
	// la fonction f doit prendre un seul argument de type int
	template<typename Func>
	void forEachAdjacentVertex(int v, Func f) const  {
		for(const Edge& e : this->adjacentEdges(v))
			f(e.Other(v));
	}
	
	// Parcours de toutes les arêtes du graphe.
	// la fonction f doit prendre un seul argument de type
	// EdgeWeightedGraph::Edge
	template<typename Func>
	void forEachEdge(Func f) const  {
		for(int v=0;v<this->V();++v)
			for(const Edge& e : this->adjacentEdges(v))
				if(e.Other(v) >= v)
					f(e);
	}
};

#endif /* defined(__ASD2__EdgeWeightedGraph__) */
