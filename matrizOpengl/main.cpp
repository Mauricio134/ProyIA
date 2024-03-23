#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <stack>
#include <string>
#include <thread>
#include <queue>
#include <utility>
#include <vector>
#include <map>

using namespace std;

struct nodo {
    string id;
    vector<pair<nodo*,float>> listaConexos;
    map <string,bool> recorrido;
    vector<float>coordenadas;
    nodo() {
        id = "";
    }
    nodo(string id_, float x, float y, float z = 0.0f) {
        id = id_;
        coordenadas.push_back(x);
        coordenadas.push_back(y);
        coordenadas.push_back(z);
    }
    void delet() {
        id = "";
    }

  string id;
  vector<pair<nodo *, float>> listaConexos;
  nodo() { id = ""; }
  nodo(string id_) { id = id_; }
  void delet() { id = ""; }
};

void makeConnection(vector<vector<nodo *>> &matriz, int inicio, int fin) {
  for (int i = inicio; i < fin; i++) {
    for (int j = 0; j < 20; j++) {
      if (j < 19)
        matriz[i][j]->listaConexos.push_back(make_pair(matriz[i][j + 1], 10.0));
      if (j > 0)
        matriz[i][j]->listaConexos.push_back(make_pair(matriz[i][j - 1], 10.0));
      if (i > 0)
        matriz[i][j]->listaConexos.push_back(make_pair(matriz[i - 1][j], 10.0));
      if (i < 19)
        matriz[i][j]->listaConexos.push_back(make_pair(matriz[i + 1][j], 10.0));
      if (j < 19 && i < 19)
        matriz[i][j]->listaConexos.push_back(
            make_pair(matriz[i + 1][j + 1], 14.14));
      if (i > 0 && j < 19)
        matriz[i][j]->listaConexos.push_back(
            make_pair(matriz[i - 1][j + 1], 14.14));
      if (j > 0 && i > 0)
        matriz[i][j]->listaConexos.push_back(
            make_pair(matriz[i - 1][j - 1], 14.14));
      if (j > 0 && i < 19)
        matriz[i][j]->listaConexos.push_back(
            make_pair(matriz[i + 1][j - 1], 14.14));
    }
  }
}

vector<int> generarNumerosAleatoriosUnicos(int minimo, int maximo,
                                           int cantidad) {

  if (cantidad > maximo - minimo + 1) {
    cout << "Error: Cantidad de numeros a generar excede el rango posible.\n";
    return {};
  }

  mt19937 generador(time(0));

  vector<int> numeros;

  for (int i = minimo; i <= maximo; ++i) {
    numeros.push_back(i);
  }

  shuffle(numeros.begin(), numeros.end(), generador);

  numeros.resize(cantidad);

  return numeros;
}

void borrar(vector<vector<nodo *>> &mt) {
  vector<int> num;
  int porcentaje, profe = 60;
  // cin >> profe;
  porcentaje = profe * 4;
  num = generarNumerosAleatoriosUnicos(0, 399, porcentaje);

  for (int i = 0; i < num.size(); i++) {
    (*(mt[num[i] / 20][num[i] % 20])).delet();
  }

  cout << endl;
}

void copiarSet(queue<string> primero, queue<string> &segundo){
  while(!primero.empty()){
    segundo.push(primero.front());
    primero.pop();
  }
}

void imprimir(queue<string> cola){
  while(!cola.empty()){
    cout << cola.front() << " ";
    cola.pop();
  }
  cout << endl;
}

bool Existe(queue<string> cola, string valor){
  while (!cola.empty()) {
    if (cola.front() == valor) {
        return true;
    }
    cola.pop();
  }
  return false;
}

void Profundidad(vector<vector<nodo *>> mt, int ini, int fin) {
  stack<pair<nodo *, queue<string>>> pila1, pila2;
  queue<string> visitados;
  nodo * inicio = mt[ini/20][ini%20];
  visitados.push(inicio->id);
  pila2.push(make_pair(inicio, visitados));
  while(!(pila2.empty()) && !(Existe(pila2.top().second,mt[fin/20][fin%20]->id))){
    pila1.push(pila2.top());
    pila2.pop();
    for(int i = pila1.top().first->listaConexos.size()-1; i > -1;i--){
      nodo * actualNodo = pila1.top().first->listaConexos[i].first;
      string value = actualNodo->id;
      if(value == "" || Existe(pila1.top().second,value)) continue;
      queue<string> visitados2;
      copiarSet(pila1.top().second, visitados2);
      visitados2.push(value);
      pila2.push(make_pair(actualNodo, visitados2));
    }
  }
  if(pila2.empty()){
    cout << "No hay camino" << endl;
    return;
  }
  queue<string> camino = pila2.top().second;
  cout << "Profundidad: ";
  imprimir(camino);
}

void Amplitud(vector<vector<nodo *>> mt, int ini, int fin) {
  queue<pair<nodo *, queue<string>>> cola;
  queue<string> visitados;
  nodo * inicio = mt[ini/20][ini%20];
  visitados.push(inicio->id);
  cola.push(make_pair(inicio, visitados));
  while(!cola.empty() && !(Existe(cola.front().second,mt[fin/20][fin%20]->id))){
    pair<nodo *, queue<string>> revisar = cola.front();
    cola.pop();
    for(int i = revisar.first->listaConexos.size()-1; i > -1; i--){
      nodo * actualNodo = revisar.first->listaConexos[i].first;
      string value = actualNodo->id;
      if(value == "" || Existe(revisar.second,value)) continue;
      queue<string> visitados2;
      copiarSet(revisar.second, visitados2);
      visitados2.push(value);
      cola.push(make_pair(actualNodo, visitados2));
    }
  }
  if(cola.empty()){
    cout << "No hay camino" << endl;
    return;
  }
  queue<string> camino = cola.front().second;
  cout << "Amplitud: ";
  imprimir(camino);
}

int main() {
  vector<vector<nodo *>> mt(20, vector<nodo *>(20));
  int j = 0;
  for (int i = 0;i<20; i++) {
        for (int k = 0;k<20; k++) {
            mt[i][k] = new nodo(to_string(j),-0.9+(1.8/19)*k,0.9-(1.8/19)*i);
            j++;
        }
  }
  for (int i = 0; i < 20; i++) {
    for (int k = 0; k < 20; k++) {
      cout << (*(mt[i][k])).id << " ";
    }
    cout << endl;
  }
  // makeConnection(mt,0,20);
  thread T1(makeConnection, ref(mt), 0, 5);
  thread T2(makeConnection, ref(mt), 5, 10);
  thread T3(makeConnection, ref(mt), 10, 15);
  thread T4(makeConnection, ref(mt), 15, 20);
  T1.join();
  T2.join();
  T3.join();
  T4.join();
  borrar(mt);
  for (int i = 0; i < 20; i++) {
    for (int k = 0; k < 20; k++) {
      cout << (*(mt[i][k])).id << " ";
    }
    cout << endl;
  }
  freopen("input.txt", "r", stdin);
  int inicio, fin;
  cin >> inicio >> fin;
  Profundidad(mt, inicio, fin);
  Amplitud(mt, inicio, fin);
  return 0;
}

