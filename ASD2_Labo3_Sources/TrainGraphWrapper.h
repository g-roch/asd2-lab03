/* 
 * @file   TrainGraphWrapper.h
 * @author Gabriel Roch
 * @author Gwendoline Dossegger
 * @author Jean-Luc Blanc
 *
 */

#include "TrainNetwork.h"
#include "EdgeWeightedGraph.h"
#include <functional>

class TrainGraphWrapperCommon {
	public:
    typedef int Weight;
		typedef std::function<Weight(TrainNetwork::Line const &)> FnWeightType;
	protected:
		TrainNetwork const & tn;
		FnWeightType fnWeight;
	protected:
		/**
		 * @brief Constructeur de la classe TrainGraphWrapperCommon
		 * @param tn réseau de train
		 * @param fnWeight Fonction convertisant un TrainNetwork::Line en 
		 *	               poids (int)
		 */
		TrainGraphWrapperCommon(TrainNetwork const & tn, FnWeightType fnWeight)
			: tn(tn), fnWeight(fnWeight) 
		{
		}
	public:
		/**
		 * @brief renvoit le nombre de sommet du graphe (nombre de gare)
		 * @return taille du graphe
		 */
		int V() const { return tn.cities.size(); }

		/**
		 * @brief Parcours de tous les sommets du graphe. La fonction f doit prendre un seul argument de type int
		 * @param f fonction que l'on veut effectuer sur chaque sommet
		 */
		template<typename Func>
		void forEachVertex(Func f) const {
			for(int v=0; v < V(); ++v) {
				f(v);
			}
		}

		/**
		 * @brief Parcours de tous les sommets adjacents au sommet v
		 *        la fonction f doit prendre un seul argument de type int
		 * @param v sommet sur lequel il faut itérer les sommets adjacents
		 * @param f fonction que l'on veut effectuer
		 */
 		template<typename Func >
 		void forEachAdjacentVertex (int v, Func f) const { // À vérifier
 			for(int lineid : tn.cities[v].lines) {
 				TrainNetwork::Line e = tn.lines[lineid];
 				if(e.cities.first == v) {
 					f(e.cities.second);
 				} else {
 					f(e.cities.first);
 				}
 			}
 		}
};

class TrainGraphWrapper : public TrainGraphWrapperCommon {
	public:
    typedef WeightedEdge<Weight> Edge;
	public:
		/**
		 * @brief Constructeur de la classe TrainGraphWrapper
		 * @param tn réseau de train
		 * @param fnWeight Fonction convertisant un TrainNetwork::Line en 
		 *	               poids (int)
		 */
		TrainGraphWrapper(TrainNetwork const & tn, FnWeightType fnWeight)
			: TrainGraphWrapperCommon(tn, fnWeight)
		{
		}


		/**
		 * @brief Parcours des arcs/arêtes adjacentes au sommet v.
		 *        la fonction f doit prendre un seul argument de type 
		 *        ...::Edge
		 * @param v sommet sur lequel il faut itérer au arc adjacents
		 * @param f Fonction à appliquer
		 */
 		template<typename Func>
 		void forEachAdjacentEdge(int v, Func f) const  {
 			for(int lineid : tn.cities[v].lines) {
				Edge e(
						tn.lines[lineid].cities.first, 
						tn.lines[lineid].cities.second,
						fnWeight(tn.lines[lineid])
						);
				f(e);
 			}
 		}

		/**
		 * @brief Parcours de toutes les arêtes du graphe.
		 *        la fonction f doit prendre un seul argument de type 
		 *        ...::Edge
		 * @param f Fonction à appliquer
		 */
 		template<typename Func>
 		void forEachEdge(Func f) const {
 			for(TrainNetwork::Line const & line : tn.lines) {
				f(Edge(
						line.cities.first, 
						line.cities.second,
						fnWeight(line)
						));
			}
		}
};

class TrainDiGraphWrapper : public TrainGraphWrapperCommon{
	public:
    typedef WeightedDirectedEdge<Weight> Edge;
	public:
		/**
		 * @brief Constructeur de la classe TrainDiGraphWrapper
		 * @param tn réseau de train
		 * @param fnWeight Fonction convertisant un TrainNetwork::Line en 
		 *	               poids (int)
		 */
		TrainDiGraphWrapper(TrainNetwork const & tn, FnWeightType fnWeight)
			: TrainGraphWrapperCommon(tn, fnWeight)
		{
		}

		/**
		 * @brief Parcours des arcs/arêtes adjacentes au sommet v.
		 *        la fonction f doit prendre un seul argument de type 
		 *        ...::Edge
		 * @param v sommet sur lequel il faut itérer au arc adjacents
		 * @param f Fonction à appliquer
		 */
 		template<typename Func>
 		void forEachAdjacentEdge(int v, Func f) const  {
 			for(int lineid : tn.cities[v].lines) {
				f(Edge(
						tn.lines[lineid].cities.first, 
						tn.lines[lineid].cities.second,
						fnWeight(tn.lines[lineid])
						));
				f(Edge(
						tn.lines[lineid].cities.second,
						tn.lines[lineid].cities.first, 
						fnWeight(tn.lines[lineid])
						));
 			}
 		}

		/**
		 * @brief Parcours de toutes les arêtes du graphe.
		 *        la fonction f doit prendre un seul argument de type 
		 *        ...::Edge
		 * @param f Fonction à appliquer
		 */
 		template<typename Func>
 		void forEachEdge(Func f) const {
 			for(TrainNetwork::Line const & line : tn.lines) {
				f(Edge(
						line.cities.second,
						line.cities.first, 
						fnWeight(line)
						));
				f(Edge(
						line.cities.first, 
						line.cities.second,
						fnWeight(line)
						));
			}
		}
};

