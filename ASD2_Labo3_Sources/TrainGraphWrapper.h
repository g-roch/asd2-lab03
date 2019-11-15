/* 
 * @file   TrainGraphWrapper.h
 * @author Gabriel Roch
 * @author Gwendoline Dossegger
 * @authro Jean-Luc Blanc
 *
 */


#include "TrainNetwork.h"

/**
 * Classe permettant de voir un TrainNetwork comm un graph
 */
class TrainGraphWrapper {
	private:
		const TrainNetwork & tn;

	public:
		/**
		 * @brief Constructeur de la classe TrainGraphWrapper
		 * @param tn réseau de train
		 */
		TrainGraphWrapper(const TrainNetwork & tn) : tn(tn) {
		}

		/**
		 * @brief renvoit le nombre de sommet du graphe (nombre de gare)
		 * @return taille du graphe
		 */
		int V() const {
			return tn.cities.size();
		}

		/**
		 * @brief Parcours de tous les sommets du graphe. La fonction f doit prendre un seul argument de type int
		 * @param f fonction que l'on veut effectuer sur chaque sommet
		 */
		template<typename Func>
		void forEachVertex(Func f) const  {
			for(int v=0;v<V();++v)
				f(v);
		}


		/**
		 * @brief Parcours de tous les sommets adjacents au sommet v
		 *        la fonction f doit prendre un seul argument de type int
		 * @param v sommet sur lequel il faut itérer les sommets adjacents
		 * @param f fonction que l'on veut effectuer
		 */
		template<typename Func >
		void forEachAdjacentVertex (int v, Func f) const {
			for(int lineid : tn.cities[v].lines) {
				TrainNetwork::Line e = tn.lines[lineid];
				if(e.cities.first == v) {
					f(e.cities.second);
				} else if(e.cities.second == v) {
					f(e.cities.first);
				}
			}
		}

	protected:

		/**
		 * @brief Parcours des arcs/arêtes adjacentes au sommet v.
		 *        la fonction f doit prendre un seul argument de type 
		 *        ...::Edge
		 * @param v sommet sur lequel il faut itérer au arc adjacents
		 * @param f Fonction à appliquer
		 * @param edgeFunc Fonction retournant l'arc dont le n° est passé en premier argument
		 */
		template<typename Func, typename EdgeFunc>
		void forEachAdjacentEdge(int v, Func f, EdgeFunc edgeFunc) const  {
			for(int lineid : tn.cities[v].lines) {
				//for(const TrainNetwork::Line & e : tn.lines) 
				TrainNetwork::Line e = tn.lines[lineid];
				for(const auto & i : edgeFunc(e)) f(i);
			}
		}


		/**
		 * @brief Parcours de toutes les arêtes du graphe.
		 *        la fonction f doit prendre un seul argument de type 
		 *        ...::Edge
		 * @param f Fonction à appliquer
		 * @param edgeFunc Fonction retournant l'arc dont le n° est passé en premier argument
		 */
		template<typename Func, typename EdgeFunc>
		void forEachEdge (Func f, EdgeFunc edgeFunc) const {
			for(const TrainNetwork::Line & e : tn.lines) 
				for(const auto & i : edgeFunc(e))
					f(i);
		}

};

class TrainGraphWrapperCostTrack : public TrainGraphWrapper {
	public:
		// Type des arcs/arêtes.
		typedef WeightedEdge<int> Edge;
	private:
		std::vector<int> cost;

	public:

		/**
		 * @brief Parcours des arcs/arêtes adjacentes au sommet v.
		 *        la fonction f doit prendre un seul argument de type 
		 *        ...::Edge
		 * @param v sommet sur lequel il faut itérer au arc adjacents
		 * @param f Fonction à appliquer
		 */
		template<typename Func>
		void forEachAdjacentEdge(int v, Func f) const  {
			TrainGraphWrapper::forEachAdjacentEdge(v, f, 
					[this] (const TrainNetwork::Line & e) -> std::vector<Edge> { 
					return {
					//Edge(e.cities.first, e.cities.second, cost.at(e.nbTracks)*e.length), 
					Edge(e.cities.second, e.cities.first, cost.at(e.nbTracks)*e.length) 
					//Edge(e.cities.second, e.cities.first, e.nbTracks) 
					};
					}
					);
		}

		/**
		 * @brief Parcours de toutes les arêtes du graphe.
		 *        la fonction f doit prendre un seul argument de type 
		 *        ...::Edge
		 * @param f Fonction à appliquer
		 */
		template<typename Func>
		void forEachEdge(Func f) const {
			TrainGraphWrapper::forEachEdge(f, 
					[this] (const TrainNetwork::Line & e) -> std::vector<Edge> { 
					return {
					//Edge(e.cities.first, e.cities.second, cost.at(e.nbTracks)*e.length),
					Edge(e.cities.second, e.cities.first, cost.at(e.nbTracks)*e.length)
					//Edge(e.cities.second, e.cities.first, e.nbTracks) 
					};
					}
					);
		}

		/**
		 * @brief Constructeur de la classe TrainGraphWrapperCostTrack
		 * @param tn réseau de train
		 * @param cost Tableau des coûts pour le nombre de voix par arc
		 */
		TrainGraphWrapperCostTrack(const TrainNetwork & tn, const std::vector<int> & cost) : TrainGraphWrapper(tn), cost(cost) {
		}
};

class TrainGraphWrapperDistance : public TrainGraphWrapper {
	public:
		// Type des arcs/arêtes.
		typedef WeightedDirectedEdge<int> Edge;
	private:
		/**
		 * Optient un Edge à partir de son id
		 * @param e id de l'edge
		 * @return tableau des deux Edge (aller; retour).
		 */
		static std::vector<Edge> getEdge(const TrainNetwork::Line & e) {
			return {
				Edge(e.cities.first, e.cities.second, e.length),
				Edge(e.cities.second, e.cities.first, e.length)
			};
		}
	public:

		/**
		 * @brief Parcours des arcs/arêtes adjacentes au sommet v.
		 *        la fonction f doit prendre un seul argument de type 
		 *        ...::Edge
		 * @param v sommet sur lequel il faut itérer au arc adjacents
		 * @param f Fonction à appliquer
		 */
		template<typename Func>
		void forEachAdjacentEdge(int v, Func f) const  {
			TrainGraphWrapper::forEachAdjacentEdge(v, f, 
					getEdge
					//[] (const TrainNetwork::Line & e) -> Edge { return Edge(e.cities.first, e.cities.second, e.length); }
					);
		}

		/**
		 * @brief Parcours de toutes les arêtes du graphe.
		 *        la fonction f doit prendre un seul argument de type 
		 *        ...::Edge
		 * @param f Fonction à appliquer
		 * @param edgeFunc Fonction retournant l'arc dont le n° est passé en premier argument
		 */
		template<typename Func>
		void forEachEdge(Func f) const {
			TrainGraphWrapper::forEachEdge(f, 
					getEdge
					//[] (const TrainNetwork::Line & e) -> Edge { return Edge(e.cities.first, e.cities.second, e.length); }
					);
		}

		/**
		 * @brief Constructeur de la classe TrainGraphWrapperDistance
		 * @param tn réseau de train
		 */
		TrainGraphWrapperDistance(const TrainNetwork & tn) : TrainGraphWrapper(tn) {
		}
};

class TrainGraphWrapperDuration : public TrainGraphWrapper {
	public:
		// Type des arcs/arêtes.
		typedef WeightedDirectedEdge<int> Edge;
	private:
		/**
		 * Optient un Edge à partir de son id
		 * @param e id de l'edge
		 * @return tableau des deux Edge (aller; retour).
		 */
		static std::vector<Edge> getEdge(const TrainNetwork::Line & e) {
			return {
				Edge(e.cities.first, e.cities.second, e.duration),
				Edge(e.cities.second, e.cities.first, e.duration)
			};
		}
	public:

		/**
		 * @brief Parcours des arcs/arêtes adjacentes au sommet v.
		 *        la fonction f doit prendre un seul argument de type 
		 *        ...::Edge
		 * @param v sommet sur lequel il faut itérer au arc adjacents
		 * @param f Fonction à appliquer
		 */
		template<typename Func>
		void forEachAdjacentEdge(int v, Func f) const  {
			TrainGraphWrapper::forEachAdjacentEdge(v, f, 
					getEdge
					//[] (const TrainNetwork::Line & e) -> Edge { return Edge(e.cities.first, e.cities.second, e.duration); }
					);
		}

		/**
		 * @brief Parcours de toutes les arêtes du graphe.
		 *        la fonction f doit prendre un seul argument de type 
		 *        ...::Edge
		 * @param f Fonction à appliquer
		 * @param edgeFunc Fonction retournant l'arc dont le n° est passé en premier argument
		 */
		template<typename Func>
		void forEachEdge(Func f) const {
			TrainGraphWrapper::forEachEdge(f, 
					getEdge
					//[] (const TrainNetwork::Line & e) -> Edge { return Edge(e.cities.first, e.cities.second, e.duration); }
					);
		}

		/**
		 * @brief Constructeur de la classe TrainGraphWrapperDuration
		 * @param tn réseau de train
		 */
		TrainGraphWrapperDuration(const TrainNetwork & tn) : TrainGraphWrapper(tn) {
		}
};
