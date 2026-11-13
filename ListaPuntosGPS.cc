#include "ListaPuntosGPS.h"
#include "PuntoGPS.h"
#include <cmath>


//---------------------NodoLista------------------------

NodoLista::NodoLista(){
    siguiente = nullptr;
    puntoGPS= PuntoGPS();
}

NodoLista::NodoLista(NodoLista *ptr, const PuntoGPS &punto){

    siguiente=ptr;
    puntoGPS=PuntoGPS(punto);
}

NodoLista::NodoLista(const NodoLista &otro){
    puntoGPS=otro.puntoGPS;
    siguiente=otro.siguiente;
    
}

NodoLista &NodoLista::operator=(const NodoLista &otro){
if (this != &otro){
    puntoGPS=otro.puntoGPS;
    siguiente=otro.siguiente;
    
    

}
return *this;
}

NodoLista::~NodoLista(){

}

NodoLista* NodoLista::getSiguiente() const {
    return siguiente;
}

PuntoGPS NodoLista::getPuntoGPS() const{
    return puntoGPS;
}

void NodoLista::setSiguiente(NodoLista* nd){
siguiente=nd;
}

void NodoLista::setPuntoGPS(const PuntoGPS&otro){
puntoGPS=otro;
}


//---------------------IteradorLista------------------------

IteradorLista::IteradorLista(){
    pt = nullptr;
}

IteradorLista::IteradorLista(const IteradorLista &otro) {
    pt=otro.pt;
}

IteradorLista::~IteradorLista(){
}

IteradorLista& IteradorLista::operator=(const IteradorLista &otro){
    if (this != &otro){
        pt = otro.pt;
    }
    return *this;
}

void IteradorLista::step(){
    if (pt != nullptr){
            pt = pt->getSiguiente();
        }
}

bool IteradorLista::operator==(const IteradorLista &otro) const{
    return pt == otro.pt;
}

bool IteradorLista::operator!=(const IteradorLista &otro) const{
    return pt != otro.pt;
}


//---------------------ListaPuntosGPS------------------------

ListaPuntosGPS::ListaPuntosGPS(){
    head=nullptr;
}

ListaPuntosGPS::ListaPuntosGPS(const ListaPuntosGPS &otro){
    head = nullptr;
    NodoLista *ultimo = nullptr;
    for (IteradorLista it = otro.begin(); it != otro.end(); it.step()){
        NodoLista *nuevo = new NodoLista(nullptr, otro.getPuntoGPS(it));
        if (ultimo == nullptr){
            head = nuevo;
        } else {
            ultimo->setSiguiente(nuevo);
        }
        ultimo = nuevo;
    }
}

ListaPuntosGPS& ListaPuntosGPS ::operator=(const ListaPuntosGPS &otro){
    if (this != &otro){
        while (head != nullptr){
        NodoLista *nodoABorrar = head;
        head = head->getSiguiente();
        delete nodoABorrar;
        }

        NodoLista *ultimo = nullptr;
        for (IteradorLista it = otro.begin(); it != otro.end(); it.step()){
            NodoLista *nuevo = new NodoLista(nullptr, otro.getPuntoGPS(it));
            if (ultimo == nullptr){
                head = nuevo;
            } else {
                ultimo->setSiguiente(nuevo);
            }
            ultimo = nuevo;
        }
    }
    return *this;
}

ListaPuntosGPS::~ListaPuntosGPS(){
    while (head != nullptr){
        NodoLista *nodoABorrar = head;
        head = head->getSiguiente();
        delete nodoABorrar;
    }
}

IteradorLista ListaPuntosGPS::begin() const{
    IteradorLista it;
    it.pt = head;      
    return it;
}

IteradorLista ListaPuntosGPS::end() const{
    return IteradorLista();
}

PuntoGPS ListaPuntosGPS::getPuntoGPS(IteradorLista it) const{
    if (it.pt == nullptr){
        return PuntoGPS();
    }
    return it.pt->getPuntoGPS();
}

bool ListaPuntosGPS::esVacia() const{
    return head == nullptr;
}
int ListaPuntosGPS::longitud() const{
    int n = 0;
    for (IteradorLista it = begin(); it != end(); it.step()){
        n++;
    }
    return n;
}

bool ListaPuntosGPS :: existeTimestamp(unsigned long t) const{
    IteradorLista it =begin();

    while (it.pt!=nullptr){
        PuntoGPS p= getPuntoGPS(it);
        if (p.getTimestamp()==t){
            return true;
        }
        it.step();
    }
    return false;
}

IteradorLista ListaPuntosGPS ::buscarPorTimestamp(unsigned long t) const{
    IteradorLista it =begin();
    while (it.pt!=nullptr){
        PuntoGPS p= getPuntoGPS(it);
        if (p.getTimestamp()==t){
            return it;
        }
        it.step();
    }
    it=end();
    return it;

}

bool ListaPuntosGPS::insertar( const PuntoGPS& p){
    NodoLista *actual= head;
    NodoLista * anterior=nullptr;
    while (actual != nullptr && actual->getPuntoGPS().getTimestamp() < p.getTimestamp()){
        
    
        anterior = actual;
        actual = actual->getSiguiente();
    }


    
    if (actual != nullptr && actual->getPuntoGPS().getTimestamp() == p.getTimestamp()){
        return false;

    }

    NodoLista *nuevo = new NodoLista(actual, p);

    if (anterior == nullptr){
        head=nuevo;
    } else {
        anterior->setSiguiente(nuevo); 
    }
    return true;
}

bool ListaPuntosGPS::asignar(IteradorLista it, const PuntoGPS &p){
    if (it.pt == nullptr){
        return false;
    }
    NodoLista *siguiente = it.pt->getSiguiente();
    if (siguiente != nullptr && p.getTimestamp() >= siguiente->getPuntoGPS().getTimestamp()){
        return false;
    }
    NodoLista *anterior = nullptr;
    NodoLista *actual = head;
    while (actual != nullptr && actual != it.pt){
        anterior = actual;
        actual = actual->getSiguiente();
    }
    if (actual == nullptr){
        return false;
    }
    if (anterior != nullptr && p.getTimestamp() <= anterior->getPuntoGPS().getTimestamp()){
        return false;
    }
    it.pt->setPuntoGPS(p);
    return true;
}

bool ListaPuntosGPS::borrarPrimero(){
    if (head==nullptr){
        return false;
    }
    NodoLista *nodoAborrar=head;
    head=head->getSiguiente();
    delete nodoAborrar;
    return true;
}

bool ListaPuntosGPS::borrarUltimo(){
    if (head==nullptr){
        return false;
    }
    if (head->getSiguiente() == nullptr){   
        delete head;
        head = nullptr;
        return true;
    }
    NodoLista *actual= head;
    NodoLista * anterior=nullptr;
    
    while (actual->getSiguiente()!=nullptr){
        anterior = actual;
        actual=actual->getSiguiente();
        
    }
    anterior->setSiguiente(nullptr);
    delete actual;
    
    return true;
}

bool ListaPuntosGPS::borrar(IteradorLista & it){
    if (it.pt == nullptr){
            return false;
        }

    if (it.pt == head){
        head = head->getSiguiente();
        delete it.pt;
        it.pt = nullptr;
        return true;
    }

    
    NodoLista *actual=head;
    NodoLista *anterior=nullptr;
    while (actual!=nullptr && actual !=it.pt){
        anterior=actual;
        actual=actual->getSiguiente();
    }
    
    if (actual==nullptr){
        return false;
    }
    anterior->setSiguiente(actual->getSiguiente());
    delete actual;
    it.pt=nullptr;
    return true;
}

bool ListaPuntosGPS::borrarPorTimestamp(unsigned long t){
    IteradorLista it = buscarPorTimestamp(t);
    if (it == end()){
        return false;          
    }
    return borrar(it);
}

int ListaPuntosGPS :: contarMismaUbicacion(const PuntoGPS &compar) const{
    IteradorLista it=begin();
    const double EPS = 1e-6;
    int cont=0;
    while (it.pt != nullptr){
        PuntoGPS p = getPuntoGPS(it);   
        if (fabs(p.getLatitud()  - compar.getLatitud()) < EPS && fabs(p.getLongitud() - compar.getLongitud()) < EPS && fabs(p.getAltitud()  - compar.getAltitud())  < EPS){
            cont++;
        }
        it.step();
    }
    return cont;
}

ListaPuntosGPS ListaPuntosGPS::filtrarRangoTimestamp(unsigned long inicio, unsigned long fin) const{
    ListaPuntosGPS resultado;
    for (IteradorLista it = begin(); it != end(); it.step()){
        PuntoGPS p = getPuntoGPS(it);
        if (p.getTimestamp() >= inicio && p.getTimestamp() <= fin){
            resultado.insertar(p);
        }
    }
    return resultado;
}

ListaPuntosGPS ListaPuntosGPS::filtrarDispositivo(const string &disp) const{
    ListaPuntosGPS resultado;
    for (IteradorLista it = begin(); it != end(); it.step()){
        PuntoGPS p = getPuntoGPS(it);
        if (p.getDispositivo() == disp){
            resultado.insertar(p);
        }
    }
    return resultado;
}

double ListaPuntosGPS::distanciaTotal() const{
    double total = 0.0;
    IteradorLista sig = begin();
    sig.step();
    for (IteradorLista it = begin(); sig != end(); it.step(), sig.step()){
        total += getPuntoGPS(it).distancia3D(getPuntoGPS(sig));
    }
    return total;
}

double ListaPuntosGPS::velocidadMedia() const{
    if (longitud() < 2){
        return 0.0;
    }
    IteradorLista ultimo = begin();
    for (IteradorLista it = begin(); it != end(); it.step()){
        ultimo = it;
    }
    unsigned long diferencia = getPuntoGPS(ultimo).getTimestamp() - getPuntoGPS(begin()).getTimestamp();
    if (diferencia == 0){
        return 0.0;
    }
    return distanciaTotal() / diferencia;
}

double ListaPuntosGPS::velocidadMaxima() const{
    double maxima = 0.0;
    IteradorLista sig = begin();
    sig.step();
    for (IteradorLista it = begin(); sig != end(); it.step(), sig.step()){
        double v = getPuntoGPS(it).velocidad(getPuntoGPS(sig));
        if (v > maxima){
            maxima = v;
        }
    }
    return maxima;
}

string ListaPuntosGPS::aCadena() const{
    string resultado="";
    
    IteradorLista it =begin();
    while (it.pt!=nullptr){
        PuntoGPS p= getPuntoGPS(it);
        if (it!=begin()){
            resultado = resultado + "\n";
        }
        resultado = resultado + p.aCadena();
        it.step();
    }
    return resultado;
}