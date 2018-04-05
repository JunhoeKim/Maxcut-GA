#include "Chromosomes.h"
using namespace std;

Chromosomes::Chromosomes(int size, Graph graph): size(size), graph(graph) {
  srand((unsigned int)time(NULL));
  int vCount = graph.getVCount();
  scores.resize(size);
  for (int i = 0; i < size; i++) {
    vector<int> elem;
    for (int j = 0; j < vCount; j++) {
      int value = rand() % 2;
      elem.push_back(value);
    }
    elements.push_back(elem);
  }
}

void Chromosomes::printElems() {
  cout << "Elements ---------" << endl;
  for (int i = 0; i < size; i++) {
    vector<int> elem = elements[i];
    cout << i << " : ";
    for (int j = 0; j < elem.size(); j++) {
      cout << elem[j] << ' ';
    }
    cout << endl;
  }

  cout << "Scores --------" << endl;
  for (int i = 0; i < size; i++) {
    cout << i << " : " << scores[i] << endl;
  }
}

void Chromosomes::updateScore() {
  int vCount = graph.getVCount();
  for (int i = 0; i < size; i++) {
    int currScore = 0;
    for (int j = 0; j < vCount; j++) {
    int currValue = elements[i][j];
      for (int k = 0; k < vCount; k++) {
        if (k == j) {
          continue;
        }
        int otherValue = elements[i][k];
        if (otherValue != currValue) {
          currScore += graph.getMatrix()[j][k];
        }
      }
    }
    scores[i] = currScore / 2;
  }
}

pair<ELEM, ELEM> Chromosomes::selectElemPair() {
  return make_pair(elements[0], elements[1]);
}
