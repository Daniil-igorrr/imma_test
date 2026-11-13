#ifndef LISTAPUNTOSGPS_H
#define LISTAPUNTOSGPS_H
#include "PuntoGPS.h"

#include<string>

using namespace std;

//---------------------NodoLista------------------------

class NodoLista{

private:
NodoLista* siguiente;
PuntoGPS puntoGPS;

public:
//Constructor por defecto
NodoLista();
//Constructor sobrecargado
NodoLista(NodoLista*, const PuntoGPS&);
//Constructor de copia
NodoLista(const NodoLista&);
//Operador de asignación
NodoLista& operator=(const NodoLista &);
//Destructor
~NodoLista();
//Getters y setters
NodoLista* getSiguiente() const;
PuntoGPS getPuntoGPS() const;
void setSiguiente(NodoLista*);
void setPuntoGPS(const PuntoGPS&);

};


//---------------------IteradorLista------------------------

class IteradorLista{

private:
NodoLista* pt;
friend class ListaPuntosGPS;    
public:
//Constructor por defecto: puntero a nullptr
IteradorLista();
//Constructor de copia
IteradorLista(const IteradorLista&);
//Destructor
~IteradorLista();
//Operador de asignación
IteradorLista& operator=(const IteradorLista&);
//Avanza una posición en la lista (apunta al siguiente nodo)
void step();
//Operadores de comparación de iteradores
bool operator==(const IteradorLista&) const;
bool operator!=(const IteradorLista&) const;

};

//---------------------ListaPuntosGPS------------------------
class ListaPuntosGPS{
    
private:
NodoLista *head;


public:
//Constructor por defecto: lista vacía
ListaPuntosGPS();
//Constructor de copia
ListaPuntosGPS(const ListaPuntosGPS&);  
//Operador de asignación

ListaPuntosGPS& operator=(const ListaPuntosGPS &);
//Destructor
~ListaPuntosGPS();
//Devuelve un iterador al primer elemento de la lista
IteradorLista begin() const;
//Devuelve un iterador al final de la lista (puntero a nullptr)
IteradorLista end() const;
//Devuelve el punto GPS apuntado por el iterador
PuntoGPS getPuntoGPS(IteradorLista) const;
//Comprueba si la lista está vacía
bool esVacia() const;
//Devuelve la cantidad de elementos en la lista
int longitud() const;
//Comprueba si existe un punto con la marca temporal dada
bool existeTimestamp(unsigned long) const;
//Devuelve un iterador al punto con dicho timestamp (o end() si no existe)
IteradorLista buscarPorTimestamp(unsigned long) const;
//Inserta un punto manteniendo el orden por timestamp.
//Devuelve false si ya existe un punto con el mismo timestamp.
bool insertar(const PuntoGPS&);
//Asigna un punto GPS en la posición apuntada manteniendo la ordenación.
//Devuelve false si viola el orden o genera timestamps duplicados.
bool asignar(IteradorLista, const PuntoGPS&);
//Borra el primer punto GPS de la lista
bool borrarPrimero();
//Borra el último punto GPS de la lista
bool borrarUltimo();
//Borra el punto GPS al que apunta el iterador
bool borrar(IteradorLista &);
//Borra el punto GPS con el timestamp indicado
bool borrarPorTimestamp(unsigned long);
//Cuenta los puntos que coinciden en ubicación (latitud, longitud y altitud)
int contarMismaUbicacion(const PuntoGPS &) const;

//Filtra y devuelve una nueva lista en el rango de timestamp [inicio, fin]
ListaPuntosGPS filtrarRangoTimestamp(unsigned long, unsigned long) const;
//Filtra y devuelve una nueva lista asociada a un dispositivo concreto
ListaPuntosGPS filtrarDispositivo(const string &) const;
//Cálculos sobre la trayectoria completa
double distanciaTotal() const;
double velocidadMedia() const;
double velocidadMaxima() const;
//Devuelve una cadena con la representación formateada de la lista
string aCadena() const;


};
#endif