
#include "TrainNetwork.h"

class TrainGraphWrapper {
private:
  const TrainNetwork & tn;
  EdgeWeightedGraph<int> g;
public:
  typedef EdgeCommon<int> BASE;
  typedef BASE::Edge Edge;
  TrainGraphWrapper(const TrainNetwork & tn) : tn(tn), g(tn.cities.size()) {
    for(auto line : tn.lines) {
      g.addEdge(line.cities.first, line.cities.second, line.length);
    }
  }
  int V() const {
    return g.V();
  }
  template<typename Func >
  void forEachAdjacentVertex (int v, Func f) const {
    g.forEachAdjacentVertex(v, f);
  }
  template<typename Func >
  void forEachEdge (Func f) const {
    g.forEachEdge(f);
  }


};
