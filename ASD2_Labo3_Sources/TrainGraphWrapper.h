
#include "TrainNetwork.h"

class TrainGraphWrapper {
	public:
		// Type de donnée pour les poids
		typedef int WeightType;

	private:
		const TrainNetwork & tn;

	public:
		TrainGraphWrapper(const TrainNetwork & tn) : tn(tn) {
		}

		int V() const {
			return tn.cities.size();
		}

		// Parcours de tous les sommets du graphe.
		// la fonction f doit prendre un seul argument de type int
		template<typename Func>
			void forEachVertex(Func f) const  {
				for(int v=0;v<V();++v)
					f(v);
			}


		// Parcours de tous les sommets adjacents au sommet v
		// la fonction f doit prendre un seul argument de type int
		template<typename Func >
			void forEachAdjacentVertex (int v, Func f) const {
				for(const TrainNetwork::Line & e : tn.lines) 
					if(e.cities.first == v) {
						f(e.cities.second);
					} else if(e.cities.second == v) {
						f(e.cities.first);
					}
			}

	protected:
		// Parcours des arcs/arêtes adjacentes au sommet v.
		// la fonction f doit prendre un seul argument de type
		// ...::Edge
		template<typename Func, typename EdgeFunc>
			void forEachAdjacentEdge(WeightType v, Func f, EdgeFunc edgeFunc) const  {
				for(const TrainNetwork::Line & e : tn.lines) 
					if(e.cities.first == v or e.cities.second == v)
						f(edgeFunc(e));
			}

		// Parcours de toutes les arêtes du graphe.
		// la fonction f doit prendre un seul argument de type
		// ...::Edge
		template<typename Func, typename EdgeFunc>
			void forEachEdge (Func f, EdgeFunc edgeFunc) const {
				for(const TrainNetwork::Line & e : tn.lines) 
					f(edgeFunc(e));
			}

};

class TrainGraphWrapperCostTrack : public TrainGraphWrapper {
	private:
		std::vector<int> cost;
	public:
		// Type des arcs/arêtes.
		typedef WeightedEdge<int> Edge;

		template<typename Func>
			void forEachAdjacentEdge(WeightType v, Func f) const  {
				TrainGraphWrapper::forEachAdjacentEdge(v, f, 
						[this] (const TrainNetwork::Line & e) -> Edge { return Edge(e.cities.first, e.cities.second, cost.at(e.nbTracks)); }
					);
			}
		template<typename Func>
			void forEachEdge(Func f) const {
				TrainGraphWrapper::forEachEdge(f, 
						[this] (const TrainNetwork::Line & e) -> Edge { return Edge(e.cities.first, e.cities.second, cost.at(e.nbTracks)); }
					);
			}
		TrainGraphWrapperCostTrack(const TrainNetwork & tn, const std::vector<int> & cost) : TrainGraphWrapper(tn), cost(cost) {
		}
};

class TrainGraphWrapperDistance : public TrainGraphWrapper {
	public:
		// Type des arcs/arêtes.
		typedef WeightedEdge<int> Edge;

		template<typename Func>
			void forEachAdjacentEdge(WeightType v, Func f) const  {
				TrainGraphWrapper::forEachAdjacentEdge(v, f, 
						[] (const TrainNetwork::Line & e) -> Edge { return Edge(e.cities.first, e.cities.second, e.length); }
					);
			}
		template<typename Func>
			void forEachEdge(Func f) const {
				TrainGraphWrapper::forEachEdge(f, 
						[] (const TrainNetwork::Line & e) -> Edge { return Edge(e.cities.first, e.cities.second, e.length); }
					);
			}
		TrainGraphWrapperDistance(const TrainNetwork & tn) : TrainGraphWrapper(tn) {
		}
};

class TrainGraphWrapperDuration : public TrainGraphWrapper {
	public:
		// Type des arcs/arêtes.
		typedef WeightedEdge<int> Edge;

		template<typename Func>
			void forEachAdjacentEdge(WeightType v, Func f) const  {
				TrainGraphWrapper::forEachAdjacentEdge(v, f, 
						[] (const TrainNetwork::Line & e) -> Edge { return Edge(e.cities.first, e.cities.second, e.duration); }
					);
			}
		template<typename Func>
			void forEachEdge(Func f) const {
				TrainGraphWrapper::forEachEdge(f, 
						[] (const TrainNetwork::Line & e) -> Edge { return Edge(e.cities.first, e.cities.second, e.duration); }
					);
			}
		TrainGraphWrapperDuration(const TrainNetwork & tn) : TrainGraphWrapper(tn) {
		}
};
