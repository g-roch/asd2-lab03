/* 
 * @file   ShortestPath.h
 * @author Olivier Cuisenaire
 *
 * @author Gabriel Roch
 * @author Gwendoline Dossegger
 * @author Jean-Luc Blanc
 *
 * Created on 5. novembre 2014, 10:16
 *
 */

#ifndef ASD2_ShortestPath_h
#define ASD2_ShortestPath_h

#include <algorithm>
#include <queue>
#include <vector>
#include <set>
#include <functional>


// Classe parente de toutes les classes de plus court chemin.
// Defini les membres edgeTo et distanceTo commun à toutes ces
// classes, ainsi que les methodes DistanceTo, EdgeTo et PathTo
// qui permettent de les interroger.
//
// Le calcul des plus courts chemins est fait dans les constructeurs
// des classes derivees.

template<typename GraphType>   // Type du graphe pondere oriente a traiter
							   // GraphType doit se comporter comme un
							   // EdgeWeightedDiGraph et definir le
							   // type GraphType::Edge
class ShortestPath {
public:
	// Type des arcs. Normalement ASD2::DirectedEdge<double>
	typedef typename GraphType::Edge Edge;
	
	// Type des poids. Normalement double ou int.
	typedef typename Edge::WeightType Weight;

	// Listes d'arcs et de poids
	typedef std::vector<Edge> Edges;
	typedef std::vector<Weight> Weights;

	
	/**
	 * @brief Renvoie la distance du chemin le plus court du sommet source a v
	 * @param v, index du sommet dont on veut connaitre la distance au sommet source
	 * @return distance entre les 2 sommets
	 */
	Weight DistanceTo(int v) const {
		return distanceTo.at(v);
	}
	
	
	/**
	 * @brief Renvoie le dernier arc u->v du chemin le plus court du sommet source a v
	 * @param v, index du sommet dont on veut connaitre le dernier arc
	 * @return le dernier arc du chemin le plus court entre les 2 sommets
	 */
	Edge EdgeTo(int v) const {
		return edgeTo.at(v);
	}
	

	/**
	 * @brief Renvoie la liste ordonnee des arcs constituant un chemin le plus court du sommet source à v.
	 * @param v, sommet dont on veut connaitre le chemin le plus court constitué d'arc
	 * @return liste des arcs du chemin le plus court entre les 2 sommets
	 */
	Edges PathTo(int v) const {
		Edges e;
		while(DistanceTo(v) != 0) {
			e.push_back(EdgeTo(v));
			v = e.back().From();
		}
		std::reverse(e.begin(), e.end());
		return e;
	}

protected:
	Edges edgeTo;
	Weights distanceTo;
};

// Classe a mettre en oeuvre au labo 3. S'inspirer de BellmaFordSP pour l'API

template<typename GraphType>
class DijkstraSP : public ShortestPath<GraphType> {
public:
	typedef ShortestPath<GraphType> BASE;
	typedef typename BASE::Edge Edge;
	typedef typename BASE::Weight Weight;

	/**
	 * @brief Relachement de l'arc e
	 * @param e, arc que l'on veut relaché
	 */
	void relax(const Edge& e) {
		int v = e.From(), w = e.To();
		Weight distThruE = this->distanceTo[v]+e.Weight();
		
		if(this->distanceTo[w] > distThruE) {
			this->distanceTo[w] = distThruE;
			this->edgeTo[w] = e;
		}
	}

	/**
	 * @brief Algorithme de Dijkstra, sert à déterminer le chemin le plus court dans un graphe
	 * @param g, graphe surlequel on veut effectuer l'algorithme
	 * @param v, index du sommet à partir duquel on veut calculer le chemin le plus court
	 */
	DijkstraSP(const GraphType& g, int v)  {
        //distTo[v] = 0 pour la ou les sources
        //distTo[v] = infini pour les autres sommets
        //E = ensemble de tous les sommets
        // infini = max de unsigned
				//
        //unsigned distTo[g.V()];
        //for(unsigned u : distTo){
        //    if(u == v) distTo[u] = 0;
        //    else{ distTo[u] = -1;}
        //}
        //std::priority_queue<Edge, std::vector<Edge>> pq;
        //g.forEachAdjacentEdge (v, [&pq] (const Edge& e) -> void {pq.push(e);});

        //while(!pq.empty()) {
        //  pq.pop(std::min_element(distTo,distTo+g.V()));

        //  //manque relacher tous les arcs v->w issus de v
        //}

				// Version GRH
		    //std::vector<unsigned> distTo(g.V(), -1);
				
				BASE::distanceTo.resize(g.V(), std::numeric_limits<Weight>::max());
				BASE::edgeTo.resize(g.V());
				BASE::distanceTo.at(v) = 0;

				std::set<int> toTTT;
				g.forEachVertex([& toTTT] (int i) { toTTT.insert(i);});
				while(!toTTT.empty()) {
					int vertexMin = *toTTT.begin();
					// find min
					//
					for(int i : toTTT)
						if(BASE::distanceTo.at(i) < BASE::distanceTo.at(vertexMin))
							vertexMin = i;

					toTTT.erase(vertexMin);

					g.forEachAdjacentEdge(vertexMin, [&toTTT, this] (const Edge& e) {
							if(toTTT.count(e.To()) == 1)	relax(e);
							//Weight newWeight = BASE::distanceTo.at(e.From()) + e.Weight();
							//if(toTTT.count(e.To()) == 1 and BASE::distanceTo.at(e.To()) > newWeight) {
							//	BASE::distanceTo.at(e.To()) = newWeight;
							//}
						});

				}
	}
};

// Algorithme de BellmanFord.

template<typename GraphType> // Type du graphe pondere oriente a traiter
							 // GraphType doit se comporter comme un
							 // EdgeWeightedDiGraph et definir forEachEdge(Func),
							 // ainsi que le type GraphType::Edge. Ce dernier doit
							 // se comporter comme ASD2::DirectedEdge, c-a-dire definir From(),
							 // To() et Weight()

class BellmanFordSP : public ShortestPath<GraphType> {

private:
	typedef ShortestPath<GraphType> BASE;
	typedef typename BASE::Edge Edge;
	typedef typename BASE::Weight Weight;
	
	
	/**
	 * @brief Relachement de l'arc e
	 * @param e, arc que l'on veut relaché
	 */
	void relax(const Edge& e) {
		int v = e.From(), w = e.To();
		Weight distThruE = this->distanceTo[v]+e.Weight();
		
		if(this->distanceTo[w] > distThruE) {
			this->distanceTo[w] = distThruE;
			this->edgeTo[w] = e;
		}
	}
	
public:

	/**
	 * @brief Constructeur a partir du graphe g et du sommet v a la source des plus courts chemins
	 * @param g, graphe dans lequel on veut construire 
	 * @param v, sommet é partir duquel on veut construire
	 */
	BellmanFordSP(const GraphType& g, int v) {
		
		this->edgeTo.resize(g.V());
		this->distanceTo.assign(g.V(),std::numeric_limits<Weight>::max());

		this->edgeTo[v] = Edge(v,v,0);
		this->distanceTo[v] = 0;
		
		for(int i=0;i<g.V();++i)
			g.forEachEdge([this](const Edge& e){
				this->relax(e);
			});
	}
};


/* Fera l'objet d'un exercice de programmation
// Algorithme de BellmanFord avec queue simple reprenant les sommets ayant
// ete modifies par la derniere iteration.
template<typename GraphType> // Type du graphe pondere oriente a traiter
							 // GraphType doit se comporter comme un
							 // EdgeWeightedDiGraph et definir forEachAdjacentEdge(int,Func),
							 // ainsi que le type GraphType::Edge. Ce dernier doit
							 // se comporter comme ASD2::DirectedEdge, c-a-dire definir From(),
							 // To() et Weight()

class BellmanFordQueueSP : public ShortestPath<GraphType> {

private:
	typedef ShortestPath<GraphType> BASE;
	typedef typename BASE::Edge Edge;
	typedef typename BASE::Weight Weight;
	
public:
	BellmanFordQueueSP(const GraphType& g, int v) {
		
	}
};
*/

#endif
