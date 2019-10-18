/* 
 * File:   EdgeWeightedGraphCommon.h
 * Author: Olivier Cuisenaire
 *
 * Created on 11. novembre 2014, 16:17
 */

#ifndef ASD2_EdgeWeightedGraphCommon_h
#define ASD2_EdgeWeightedGraphCommon_h
#include <iostream>
#include <vector>
#include <list>
#include <functional>
#include <limits>
#include <fstream>

//  Classe regroupant toutes les parties communes de
//  Edge et Directed Edge.

template< typename T> // Type du poids, par exemple int ou double
// T doit etre comparable, et être un parametre
// valide pour std::numeric_limits
struct EdgeCommon {
public:
    // Type de donnée pour les poids.
    typedef T WeightType;
    
protected:
    // v1 et v2 sont les deux sommets de l'arc/arête
    int v1,v2;
    
    // weight en est le poids
    WeightType weight;
    
public:
    // Constructeur par défaut. Le poids est infini
    EdgeCommon() : v1(-1), v2(-1), weight(std::numeric_limits<T>::max()) { }
    
    // Constructeur spécifiant les deux sommets et le poids
    EdgeCommon(int v, int w, WeightType _weight) : v1(v), v2(w), weight(_weight) { }
    
    // renvoie la valeur du poids
    WeightType Weight() const { return weight; }
    
    // opérations de comparaison entre edges en fonction de leur poids.
    inline bool operator< (const EdgeCommon<T>& rhs) const { return weight < rhs.Weight(); }
    inline bool operator> (const EdgeCommon<T>& rhs) const {return rhs < *this;}
    inline bool operator<=(const EdgeCommon<T>& rhs) const {return !(*this > rhs);}
    inline bool operator>=(const EdgeCommon<T>& rhs) const {return !(*this < rhs);}
};


//  Classe regroupant toutes les parties communes de
//  EdgeWeightedGraph et EdgeWeightedDiGraph

template< typename T>  // type des edges, par exemple DirectedEdge<double> ou Edge<int> ou ...
// T doit définir le type T::WeightType
class EdgeWeightedGraphCommon {
public:
    // Type des arcs/arêtes.
    typedef T Edge;
    
    // Type de donnée pour les poids
    typedef typename Edge::WeightType WeightType;
    
protected:
    // Type pour une liste d'arcs/arêtes
    typedef std::list<Edge> EdgeList;
    
    // Structure de donnée pour les listes d'adjacences. Une EdgeList par sommet.
    std::vector<EdgeList> edgeAdjacencyLists;
    
public:
    
    // Constructeur par defaut.
    EdgeWeightedGraphCommon() { }
    
    // Constructeur specifiant le nombre de sommets V
    EdgeWeightedGraphCommon(int N) {
        edgeAdjacencyLists.resize(N);
    }
    
    // Renvoie le nombre de sommets V
    int V() const {
        return int(edgeAdjacencyLists.size());
    }
    
    // fonction a definir par les classes filles. Rend cette class abstraite
    virtual void addEdge(int v, int w, WeightType weight) = 0;
    
    // Parcours des arcs/arêtes adjacentes au sommet v.
    // la fonction f doit prendre un seul argument de type
    // EdgeWeightedGraphCommon::Edge
    template<typename Func>
    void forEachAdjacentEdge(int v, Func f) const  {
        for(const Edge& e : adjacentEdges(v))
            f(e);
    }
    
    // Parcours de tous les sommets du graphe.
    // la fonction f doit prendre un seul argument de type int
    template<typename Func>
    void forEachVertex(Func f) const  {
        for(int v=0;v<V();++v)
            f(v);
    }
    
protected:
    // Renvoie une const reference a la list d'arcs/aretes
    // adjacent-es au sommet v
    const EdgeList& adjacentEdges(int v) const {
        return edgeAdjacencyLists.at(v);
    }
    
    // lecture depuis un fichier.
    void ReadFromFile(const std::string& filename) {
        std::ifstream s(filename);
        this->ReadFromStream(s);
        s.close();
    }
    
    // lecture depuis un stream. Utilise addEdge(...)
    // a definir par les classe fille
    void ReadFromStream(std::istream& s) {
        int V,E;
        
        s >> V >> E;
        
        edgeAdjacencyLists.resize(V);
        
        for (int i = 0; i < E; i++) {
            int v,w;
            WeightType weight;
            
            s >> v >> w >> weight;
            addEdge(v, w, weight);
        }
    }
};

#endif
