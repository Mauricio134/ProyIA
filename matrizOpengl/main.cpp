#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

//Librerias Completas
#include <iostream>
#include <random>
#include <stack>
#include <set>
#include <string>
#include <thread>
#include <queue>
#include <utility>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int ini, fin;

struct nodo {
    string id;
    vector<pair<nodo*, float>> listaConexos;
    vector<nodo*> listaCosto;
    map <string, bool> recorrido;
    double dist_euc;
    double costo = 0;
    vector<nodo*> rflags;
    vector<float>coordenadas;
    nodo() {
        id = "";
    }
    nodo(string id_, float x, float y, float z = 0) {
        id = id_;
        coordenadas.push_back(x);
        coordenadas.push_back(y);
        coordenadas.push_back(z);
    }
    void delet() {
        id = "";
    }
    nodo(string id_) { id = id_; }
};

const int FILAS = 20;
const int COLUMNAS = 20;

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

void Profundidad(vector<vector<nodo *>> mt ,int ini, int fin, queue<string> & c, bool & cambio) {
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
    cout << "NO HAY CAMINO" << endl;
    cambio = false;
    return;
  }
  c = pila2.top().second;
}

void Amplitud(vector<vector<nodo *>> mt, int ini, int fin, queue<string> & c, bool & cambio) {
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
    cout << "NO HAY CAMINO" << endl;
    cambio = false;
    return;
  }
  c = cola.front().second;
}

void dibujarNodos(vector<vector<nodo *>> mt) {
    glPointSize(15.0f); // Tamaño de los puntos

    glBegin(GL_POINTS);
    for (int i = 0; i < FILAS; ++i) {
        for (int j = 0; j < COLUMNAS; ++j) {
            if(mt[i][j]->id != "") glColor3f(0.0f, 1.0f, 1.0f);
            else glColor3f(0.0f, 0.0f, 0.0f);
            glVertex2f(mt[i][j]->coordenadas[0], mt[i][j]->coordenadas[1]);
        }
    }
    glEnd();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void dibujarAristas(vector<vector<nodo *>> mt) {
     glLineWidth(3.0f); // Ancho de las líneas

    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f); // Color blanco para las aristas

    // Itera sobre todos los nodos del grafo
    for (int i = 0; i < FILAS; ++i) {
        for (int j = 0; j < COLUMNAS; ++j) {
            // Dibuja las aristas hacia abajo y hacia la derecha para evitar duplicar conexiones
            if(mt[i][j]->id != ""){
                if (i < FILAS - 1 && mt[i+1][j]->id != "") {
                    glVertex2f(mt[i][j]->coordenadas[0], mt[i][j]->coordenadas[1]);
                    glVertex2f(mt[i+1][j]->coordenadas[0], mt[i+1][j]->coordenadas[1]);
                }
                if (j < COLUMNAS - 1 && mt[i][j+1]->id != "") {
                    glVertex2f(mt[i][j]->coordenadas[0], mt[i][j]->coordenadas[1]);
                    glVertex2f(mt[i][j+1]->coordenadas[0], mt[i][j+1]->coordenadas[1]);
                }
                // Dibuja la arista diagonal inferior derecha si es válida
                if (i < FILAS - 1 && j < COLUMNAS - 1 && mt[i+1][j+1]->id != "") {
                    glVertex2f(mt[i][j]->coordenadas[0], mt[i][j]->coordenadas[1]);
                    glVertex2f(mt[i+1][j+1]->coordenadas[0], mt[i+1][j+1]->coordenadas[1]);
                }
                // Dibuja la arista diagonal inferior izquierda si es válida
                if (i < FILAS - 1 && j > 0 && mt[i+1][j-1]->id != "") {
                    glVertex2f(mt[i][j]->coordenadas[0], mt[i][j]->coordenadas[1]);
                    glVertex2f(mt[i+1][j-1]->coordenadas[0], mt[i+1][j-1]->coordenadas[1]);
                }
            }
        }
    }

    glEnd();
}

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

vector<int> generarNumerosAleatoriosUnicos(int minimo, int maximo, int cantidad) {

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
  int porcentaje, profe;
  cout << "Ingresar Porcentaje a Eliminar: ";
  cin >> profe;
  porcentaje = profe * 4;
  num = generarNumerosAleatoriosUnicos(0, 399, porcentaje);
  for (int i = 0; i < num.size(); i++) {
    (*(mt[num[i] / 20][num[i] % 20])).delet();
  }
}

void inicializarGrafo(vector<vector<nodo *>> &mt) {
    int j = 0;
    for (int i = 0;i<20; i++) {
            for (int k = 0;k<20; k++) {
                mt[i][k] = new nodo(to_string(j),-0.9+(1.8/19)*k,0.9-(1.8/19)*i);
                j++;
            }
    }
    thread T1(makeConnection, ref(mt), 0, 5);
    thread T2(makeConnection, ref(mt), 5, 10);
    thread T3(makeConnection, ref(mt), 10, 15);
    thread T4(makeConnection, ref(mt), 15, 20);
    T1.join();
    T2.join();
    T3.join();
    T4.join();
}

void DibujarNodos(vector<vector<nodo *>> mt, queue<string> camino){
    glPointSize(15.0f);

    glBegin(GL_POINTS);
    while(!camino.empty()){
        int nodoActual = stoi(camino.front());
        nodo * c = mt[nodoActual/20][nodoActual%20];
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(c->coordenadas[0], c->coordenadas[1]);
        camino.pop();
    }
    glEnd();
}

void DibujarAristas(vector<vector<nodo *>> mt, queue<string> camino){
    glPointSize(15.0f);
    vector<string> recorr;
    while(!camino.empty()){
        recorr.push_back(camino.front());
        camino.pop();
    }
    glBegin(GL_LINES);
    for(int i= 0; i < recorr.size()-1;i++){
        int primero = stoi(recorr[i]);
        int segundo = stoi(recorr[i+1]);
        nodo * adelante = mt[primero/20][primero%20];
        nodo * atras = mt[segundo/20][segundo%20];
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(adelante->coordenadas[0], adelante->coordenadas[1]);
        glVertex2f(atras->coordenadas[0], atras->coordenadas[1]);
    }
    glEnd();
}

void calc_dis_eu(vector<vector<nodo*>>& mt, int fin) {
    ;
    for (int i = 0; i < 400; i++) {
        if ((*(mt[i / 20][i % 20])).id != "") {
            (*(mt[i / 20][i % 20])).dist_euc = sqrt(pow((i / 20) - (fin / 20), 2) + pow((i % 20) - (fin % 20), 2)) * 10;
        }
    }
}

void detect_fin(nodo*& ini, int fin, vector<nodo*>& camino, bool& S) {
    int cont = 0;
    for (int i = 0; i < ini->listaConexos.size(); i++) {
        if (ini->listaConexos[i].first->id != "") {
            cont++;
        }
    }
    if (ini->rflags.size() == cont || ini->id == to_string(fin)) {
        S = 0;
    }
    cont = 0;
}

bool compararPorDistEuc(const pair<nodo*, float>& a, const pair<nodo*, float>& b) {
    return a.first->dist_euc > b.first->dist_euc;
}

bool compararPorDistCost(nodo*& a, nodo*& b) {
    return a->costo > b->costo;
}

void hill(vector<vector<nodo*>>& mt, int ini, int fin, vector<nodo*>& camino) {
    nodo* start = mt[ini / 20][ini % 20];
    camino.push_back(start);
    if (ini == fin) {
        return;
    }
    bool S = 1;
    while (!camino.empty() && camino[camino.size() - 1]->id != to_string(fin) && S == 1) {
        nodo* minNode = camino.back();
        sort(camino.back()->listaConexos.begin(), camino.back()->listaConexos.end(), compararPorDistEuc);
        for (int i = 0; i < camino[camino.size() - 1]->listaConexos.size(); i++) {
            bool f = 1;
            if (camino.back()->listaConexos[i].first->id != "" /* && camino.back()->listaConexos[i].first->dist_euc > minNode->dist_euc*/) {
                for (int j = 0; j < camino.back()->rflags.size(); j++) {
                    if (camino.back()->listaConexos[i].first->id == camino.back()->rflags[j]->id) {
                        f = 0;
                        break;
                    }
                }
                if (f == 1)
                    minNode = camino.back()->listaConexos[i].first;
            }
        }
        bool p = 1;
        for (int i = 0; i < camino.size(); i++) {
            if (minNode->id == camino[i]->id) {
                camino.back()->rflags.push_back(minNode);
                p = 0;
                break;
            }
        }
        if (p == 1) {
            camino.push_back(minNode);
        }
        else {
            camino.pop_back();
            if (!camino.empty()) {
                camino.back()->rflags.push_back(minNode);
            }
        }
        detect_fin(start, fin, camino, S);
    }
    return;
}

void calcuCost(nodo*& node) {
    int id = stoi(node->id);
    for (int i=0; i<node->listaConexos.size(); i++) {
        if (node->listaConexos[i].first->id != "") {
            node->listaCosto.push_back(node->listaConexos[i].first);
            if (stoi(node->listaConexos[i].first->id) == (id - 21) || stoi(node->listaConexos[i].first->id) == (id - 19) || stoi(node->listaConexos[i].first->id) == (id + 21) || stoi(node->listaConexos[i].first->id) == (id + 19))
                node->listaConexos[i].first->costo = node->listaConexos[i].first->dist_euc + 14.14;
            else
                node->listaConexos[i].first->costo = node->listaConexos[i].first->dist_euc + 10; 
        }
    }
}

void aAst(vector<vector<nodo*>>& mt, int ini, int fin, vector<nodo*>& camino) {
    nodo* start = mt[ini / 20][ini % 20];
    camino.push_back(start);
    if (ini == fin) {
        return;
    }
    bool S = 1;
    while (!camino.empty() && camino[camino.size() - 1]->id != to_string(fin) && S == 1) {
        nodo* minNode = camino.back();
        calcuCost(minNode);
        sort(camino.back()->listaCosto.begin(), camino.back()->listaCosto.end(), compararPorDistCost);
        for (int i = 0; i < camino[camino.size() - 1]->listaCosto.size(); i++) {
            bool f = 1;
            if (camino.back()->listaCosto[i]->id != "" /* && camino.back()->listaConexos[i].first->dist_euc > minNode->dist_euc*/) {
                for (int j = 0; j < camino.back()->rflags.size(); j++) {
                    if (camino.back()->listaCosto[i]->id == camino.back()->rflags[j]->id) {
                        f = 0;
                        break;
                    }
                }
                if (f == 1)
                    minNode = camino.back()->listaCosto[i];
            }
        }
        bool p = 1;
        for (int i = 0; i < camino.size(); i++) {
            if (minNode->id == camino[i]->id) {
                camino.back()->rflags.push_back(minNode);
                p = 0;
                break;
            }
        }
        if (p == 1) {
            camino.push_back(minNode);
        }
        else {
            camino.pop_back();
            if (!camino.empty()) {
                camino.back()->rflags.push_back(minNode);
            }
        }
        detect_fin(start, fin, camino, S);
    }
    return;
}

void purificador(vector<vector<nodo*>> &mt) {
    for (int i = 0; i < 20; i++) {
        for (int k = 0; k < 20; k++) {
            mt[i][k]->rflags.clear();
        }
    }
}

void clearQ(queue<string> & q){
    while (!q.empty())
    {
        q.pop();
    }
}

void Seleccion(GLFWwindow *window, bool &a, vector<vector<nodo *>> &mt, bool &cam, queue<string> &camino1){
    
    purificador(mt);
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        borrar(mt);
        for (int i = 0; i < 20; i++) {
            for (int k = 0; k < 20; k++) {
            cout << (*(mt[i][k])).id << " ";
            }
            cout << endl;
        }
    }
    if(!a){
        if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
            cout << "Digitar Inicio: ";
            cin >> ini;
            cout << "Digitar Final: ";
            cin >> fin;
            if(mt[ini/20][ini%20]->id == "" || mt[fin/20][fin%20]->id == ""){
                cout << "NO HAY CAMINO" << endl;
                cam = false;
                return;
            }
            cam = false;
            a = true;
        }
    }
    else{
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            cout << "Amplitud" << endl;
            bool cambio = true;
            Amplitud(mt,ini, fin, camino1, cambio);
            if(cambio){
                cam = true;
            }
            a = false;
        }
        else if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
            cout << "Profundidad" << endl;
            bool cambio = true;
            Profundidad(mt, ini, fin, camino1, cambio);
            if(cambio){
                cam = true;
            }
            a = false;
        }
        else if(glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS){
            cout << "Hill" << endl;
            calc_dis_eu(mt, fin);
            clearQ(camino1);
            vector<nodo*> camino;
            purificador(mt);
            hill(mt, ini, fin, camino);
            if (camino.empty() || camino.size() == 1 && camino.back()->id != to_string(fin)) {
                cout << "NO HAY CAMINO" << endl;
                cam = false;
                a = false;
                return;
            }
            else {
                for (int o = 0; o < camino.size(); o++) {
                    camino1.push(camino[o]->id);
                }
            }
            cam = true;
            a = false;
        }
        else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            cout << "Asterisco" << endl;
            clearQ(camino1);
            vector<nodo*> camino;
            purificador(mt);
            aAst(mt, ini, fin, camino);
            if (camino.empty() || camino.size() == 1 && camino.back()->id != to_string(fin)) {
                cout << "NO HAY CAMINO" << endl;
                cam = false;
                a = false;
                return;
            }
            else {
                for (int o = 0; o < camino.size(); o++) {
                    camino1.push(camino[o]->id);
                }
            }
            cam = true;
            a = false;
        }
    }
}

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(300, 300, "Grafo OpenGL", NULL, NULL);
    if (window == NULL) {
        cout << "Error al crear la ventana de GLFW" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    vector<vector<nodo *>> mt(20, vector<nodo *>(20));
    inicializarGrafo(mt);
    bool access = false, cam = false;
    queue<string> camino1;
    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        dibujarNodos(mt);
        dibujarAristas(mt);
        Seleccion(window, access, mt, cam, camino1);
        if(cam){
            DibujarAristas(mt,camino1);
            DibujarNodos(mt, camino1);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
