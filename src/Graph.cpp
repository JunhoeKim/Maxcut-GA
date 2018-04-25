#include "Graph.h"

using namespace std;
Graph::Graph(size_t vCount, size_t eCount) : vCount(vCount), eCount(eCount) {
	eArray.reserve(eCount);
	adjacentLists.resize(vCount);
}

void Graph::addEdge(int first, int second, int weight) {
	eArray.emplace_back(make_tuple(first - 1, second - 1, weight));
	adjacentLists[first - 1].emplace_back(make_pair(second - 1, weight));
	adjacentLists[second - 1].emplace_back(make_pair(first - 1, weight));
}

size_t Graph::getVCount() const {
	return vCount;
}

size_t Graph::getECount() const {
	return eCount;
}

Graph::~Graph() {

}