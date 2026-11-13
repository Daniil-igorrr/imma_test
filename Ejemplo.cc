#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <initializer_list>
#include "PuntoGPS.h"
#include "ListaPuntosGPS.h"

using namespace std;

const double EPSILON = 1e-6;

int pruebasPasadas = 0;
int pruebasTotales = 0;

bool sonIguales(double a, double b) {
    return abs(a - b) < EPSILON;
}

// Función auxiliar para registrar e imprimir errores sin repetir código
void registrarResultado(bool correcto, const string& mensaje, const string& txtObtenido, const string& txtEsperado) {
    pruebasTotales++;
    if (correcto) {
        pruebasPasadas++;
        cout << "[OK] " << mensaje << endl;
    } else {
        cerr << "[ERROR EN PRUEBA] " << mensaje << endl;
        cerr << "   -> Obtenido: " << txtObtenido << " | Esperado: " << txtEsperado << endl;
    }
}

// ------------------------------------------------------------------
// Sobrecargas concretas de 'probar' (sin uso de plantillas C++)
// ------------------------------------------------------------------

// 1. Enteros
void probar(int obtenido, int esperado, const string& mensaje) {
    registrarResultado(obtenido == esperado, mensaje, to_string(obtenido), to_string(esperado));
}

// 2. Marcas de tiempo (unsigned long)
void probar(unsigned long obtenido, unsigned long esperado, const string& mensaje) {
    registrarResultado(obtenido == esperado, mensaje, to_string(obtenido), to_string(esperado));
}

// 3. Cadenas de texto
void probar(const string& obtenido, const string& esperado, const string& mensaje) {
    registrarResultado(obtenido == esperado, mensaje, obtenido, esperado);
}

// 4. Booleanos
void probar(bool obtenido, bool esperado, const string& mensaje) {
    string txtObt = obtenido ? "true" : "false";
    string txtEsp = esperado ? "true" : "false";
    registrarResultado(obtenido == esperado, mensaje, txtObt, txtEsp);
}

// 5. Flotantes (double) con tolerancia EPSILON
void probar(double obtenido, double esperado, const string& mensaje) {
    registrarResultado(sonIguales(obtenido, esperado), mensaje, to_string(obtenido), to_string(esperado));
}

// 6. Punteros a NodoLista
void probar(NodoLista* obtenido, NodoLista* esperado, const string& mensaje) {
    string txtObt = (obtenido == nullptr) ? "nullptr" : "puntero_valido";
    string txtEsp = (esperado == nullptr) ? "nullptr" : "puntero_valido";
    registrarResultado(obtenido == esperado, mensaje, txtObt, txtEsp);
}

// 7. Objetos PuntoGPS
void probar(const PuntoGPS& obtenido, const PuntoGPS& esperado, const string& mensaje) {
    pruebasTotales++;
    if (obtenido == esperado) {
        pruebasPasadas++;
        cout << "[OK] " << mensaje << endl;
    } else {
        cerr << "[ERROR EN PRUEBA] " << mensaje << endl;
        cerr << "   -> Obtenido:\n" << obtenido.aCadena() << endl;
        cerr << "   -> Esperado:\n" << esperado.aCadena() << endl;
    }
}

// 8. Objetos IteradorLista
void probar(const IteradorLista& obtenido, const IteradorLista& esperado, const string& mensaje) {
    string txtObt = (obtenido == IteradorLista()) ? "end/nullptr" : "iterador_valido";
    string txtEsp = (esperado == IteradorLista()) ? "end/nullptr" : "iterador_valido";
    registrarResultado(obtenido == esperado, mensaje, txtObt, txtEsp);
}

// ------------------------------------------------------------------
// Funciones auxiliares para las pruebas adicionales (Sesión 5)
// ------------------------------------------------------------------

// Crea un punto con ubicación fija y el timestamp / dispositivo / altitud indicados
PuntoGPS mk(unsigned long t, const string& d = "D", double alt = 100.0) {
    return PuntoGPS(38.0, -0.5, alt, t, d);
}

// Comprueba que la lista contiene exactamente estos timestamps y en este orden
bool tiene(const ListaPuntosGPS& l, initializer_list<unsigned long> esperado) {
    initializer_list<unsigned long>::iterator e = esperado.begin();
    for (IteradorLista it = l.begin(); it != l.end(); it.step()) {
        if (e == esperado.end() || l.getPuntoGPS(it).getTimestamp() != *e) return false;
        ++e;
    }
    return e == esperado.end();
}

// Comprueba que la lista está estrictamente ordenada por timestamp
bool estrictamenteOrdenada(const ListaPuntosGPS& l) {
    IteradorLista sig = l.begin();
    sig.step();
    for (IteradorLista it = l.begin(); sig != l.end(); it.step(), sig.step()) {
        if (l.getPuntoGPS(it).getTimestamp() >= l.getPuntoGPS(sig).getTimestamp()) return false;
    }
    return true;
}

int main() {
    cout << "==========================================================" << endl;
    cout << "  Batería de Pruebas para Práctica 2: ListaPuntosGPS" << endl;
    cout << "==========================================================" << endl << endl;

    // ------------------------------------------------------------------
    // SESIÓN 1: Auxiliares (NodoLista, IteradorLista) y Estructura Básica de ListaPuntosGPS
    // ------------------------------------------------------------------
    cout << "--- SESIÓN 1: Clases Auxiliares y Estructura Básica de ListaPuntosGPS ---" << endl;
    
    PuntoGPS p1(38.3854, -0.5127, 120.5, 1000, "GARMIN-01");
    PuntoGPS p2(38.3860, -0.5120, 125.0, 1050, "GARMIN-01");
    PuntoGPS p3(38.3900, -0.5100, 130.0, 1200, "DEV-B");
    PuntoGPS p4(38.4000, -0.5000, 140.0, 1500, "GARMIN-01");

    // 1.1 Pruebas de PuntoGPS
    probar(p1.getTimestamp(), 1000UL, "PuntoGPS inicializado correctamente con timestamp 1000");

    // 1.2 Pruebas de NodoLista aisladas
    NodoLista nDef;
    probar(nDef.getSiguiente(), (NodoLista*)nullptr, "NodoLista por defecto inicializa siguiente a nullptr");
    
    NodoLista* n1 = new NodoLista(nullptr, p1);
    probar(n1->getPuntoGPS(), p1, "NodoLista sobrecargado almacena correctamente el PuntoGPS");
    probar(n1->getSiguiente(), (NodoLista*)nullptr, "NodoLista sobrecargado inicializa siguiente a nullptr");

    NodoLista* n2 = new NodoLista(n1, p2);
    probar(n2->getSiguiente(), n1, "NodoLista sobrecargado enlaza correctamente con el nodo siguiente");

    n1->setPuntoGPS(p3);
    probar(n1->getPuntoGPS(), p3, "NodoLista::setPuntoGPS modifica el punto correctamente");
    
    delete n2;
    probar(n1->getPuntoGPS(), p3, "Destructor de NodoLista no borra el nodo siguiente (memoria aislada)");
    delete n1;

    // 1.3 Pruebas de IteradorLista
    IteradorLista itDef;
    probar(itDef, IteradorLista(), "IteradorLista por defecto es equivalente a end() / nullptr");

    // 1.4 Estructura básica de ListaPuntosGPS (esVacia, longitud, begin, end, getPuntoGPS)
    ListaPuntosGPS lista;
    probar(lista.esVacia(), true, "Una lista recién creada está vacía");
    probar(lista.longitud(), 0, "Una lista recién creada tiene longitud 0");
    probar(lista.begin(), lista.end(), "En una lista vacía, begin() == end()");
    probar(lista.getPuntoGPS(lista.begin()), PuntoGPS(), "getPuntoGPS sobre iterador nulo (lista vacía) devuelve PuntoGPS por defecto");

    // Robustez: getPuntoGPS con iterador nulo explícito
    IteradorLista itNull;
    probar(lista.getPuntoGPS(itNull), PuntoGPS(), "getPuntoGPS con iterador nulo devuelve PuntoGPS por defecto");

    // 1.5 Pruebas de inserción rudimentaria y verificación progresiva de longitud
    // Estas pruebas dan por válida la inserción en cualquier posición de la lista, incluso
    // si no se respeta el orden cronológico
    cout << "--- Subpruebas Sesión 1: Inserción rudimentaria y longitud ---" << endl;
    ListaPuntosGPS listaRudimentaria;
    probar(listaRudimentaria.esVacia(), true, "Lista rudimentaria recién creada está vacía");
    probar(listaRudimentaria.longitud(), 0, "Longitud inicial de la lista es 0");

    probar(listaRudimentaria.insertar(p1), true, "Inserción rudimentaria del primer elemento");
    probar(listaRudimentaria.esVacia(), false, "esVacia() devuelve false tras insertar un elemento");
    probar(listaRudimentaria.longitud(), 1, "longitud() devuelve 1 tras la primera inserción");

    probar(listaRudimentaria.insertar(p2), true, "Inserción rudimentaria del segundo elemento");
    probar(listaRudimentaria.longitud(), 2, "longitud() devuelve 2 tras la segunda inserción");

    probar(listaRudimentaria.insertar(p3), true, "Inserción rudimentaria del tercer elemento");
    probar(listaRudimentaria.longitud(), 3, "longitud() devuelve 3 tras la tercera inserción");

    // ------------------------------------------------------------------
    // SESIÓN 2: Inserción Ordenada Completa, aCadena y Métodos de Búsqueda y Consulta
    // ------------------------------------------------------------------
    cout << endl << "--- SESIÓN 2: Inserción Ordenada, aCadena y Métodos de Búsqueda ---" << endl;

    // 2.1 Inserción manteniendo orden por timestamp
    probar(lista.insertar(p3), true, "Inserta correctamente el primer punto (p3, ts: 1200)");
    probar(lista.esVacia(), false, "La lista ya no está vacía tras la primera inserción");
    probar(lista.longitud(), 1, "La longitud de la lista es 1");

    // Robustez: getPuntoGPS con iterador nulo en lista NO vacía
    probar(lista.getPuntoGPS(itNull), PuntoGPS(), "getPuntoGPS con iterador nulo en lista con elementos devuelve PuntoGPS por defecto");

    probar(lista.insertar(p1), true, "Inserta un punto con timestamp menor al principio (p1, ts: 1000)");
    probar(lista.longitud(), 2, "La longitud de la lista es 2");

    probar(lista.insertar(p4), true, "Inserta un punto con timestamp mayor al final (p4, ts: 1500)");
    probar(lista.longitud(), 3, "La longitud de la lista es 3");

    probar(lista.insertar(p2), true, "Inserta un punto en posición intermedia (p2, ts: 1050)");
    probar(lista.longitud(), 4, "La longitud de la lista es 4");

    // Restricción de clave única (timestamps duplicados)
    PuntoGPS pDuplicado(39.0000, -0.4000, 200.0, 1050, "OTRO-DEV");
    probar(lista.insertar(pDuplicado), false, "insertar devuelve false al intentar insertar un timestamp duplicado (1050)");
    probar(lista.longitud(), 4, "La longitud de la lista se mantiene intacta tras el intento duplicado");

    // Recorrido secuencial con IteradorLista
    IteradorLista itRec = lista.begin();
    probar(lista.getPuntoGPS(itRec).getTimestamp(), 1000UL, "El primer nodo es p1 (ts: 1000)");
    itRec.step();
    probar(lista.getPuntoGPS(itRec).getTimestamp(), 1050UL, "El segundo nodo es p2 (ts: 1050)");
    itRec.step();
    probar(lista.getPuntoGPS(itRec).getTimestamp(), 1200UL, "El tercer nodo es p3 (ts: 1200)");
    itRec.step();
    probar(lista.getPuntoGPS(itRec).getTimestamp(), 1500UL, "El cuarto nodo es p4 (ts: 1500)");
    itRec.step();
    probar(itRec, lista.end(), "it.step() desde el último elemento alcanza lista.end()");

    // 2.2 Método aCadena()
    string cadenaEsperada = p1.aCadena() + "\n" + p2.aCadena() + "\n" + p3.aCadena() + "\n" + p4.aCadena();
    probar(lista.aCadena(), cadenaEsperada, "aCadena() genera la representación multilínea ordenada sin salto final");

    // 2.3 Métodos de búsqueda y consulta (existeTimestamp, buscarPorTimestamp, contarMismaUbicacion)
    probar(lista.existeTimestamp(1050), true, "existeTimestamp devuelve true para un timestamp existente (1050)");
    probar(lista.existeTimestamp(9999), false, "existeTimestamp devuelve false para un timestamp inexistente (9999)");

    IteradorLista itBusqueda = lista.buscarPorTimestamp(1200);
    probar(itBusqueda != lista.end() && lista.getPuntoGPS(itBusqueda).getDispositivo() == "DEV-B", true, 
           "buscarPorTimestamp devuelve el iterador correcto al punto buscado");

    probar(lista.buscarPorTimestamp(9999), lista.end(), "buscarPorTimestamp devuelve end() si no existe el timestamp");

    // Probar contarMismaUbicacion con punto coincidente (misma posición que p1 pero ts 2000)
    PuntoGPS p5(38.3854 + 1e-7, -0.5127 - 1e-7, 120.5, 2000, "GARMIN-02");
    lista.insertar(p5);

    probar(lista.contarMismaUbicacion(p1), 2, "contarMismaUbicacion cuenta 2 puntos en la misma ubicación (p1 y p5 con tolerancia 1e-6)");
    probar(lista.contarMismaUbicacion(p2), 1, "contarMismaUbicacion cuenta 1 punto para la ubicación de p2");
    PuntoGPS pUbicacionLejana(40.0000, -3.0000, 600.0, 3000, "DEV-X");
    probar(lista.contarMismaUbicacion(pUbicacionLejana), 0, "contarMismaUbicacion devuelve 0 para una ubicación sin visitas en la lista");

    // ------------------------------------------------------------------
    // SESIÓN 3: Métodos de Eliminación y Gestión de Memoria / Copia Profunda
    // ------------------------------------------------------------------
    cout << endl << "--- SESIÓN 3: Métodos de Eliminación y Copia Profunda ---" << endl;

    // 3.1 Pruebas de borrarPrimero() y borrarUltimo()
    // La lista contiene: 1000 (p1), 1050 (p2), 1200 (p3), 1500 (p4), 2000 (p5)
    probar(lista.borrarPrimero(), true, "borrarPrimero elimina el primer elemento (p1, ts: 1000)");
    probar(lista.longitud(), 4, "Longitud es 4 tras borrar el primero");
    probar(lista.getPuntoGPS(lista.begin()).getTimestamp(), 1050UL, "El nuevo primer elemento es p2 (ts: 1050)");

    probar(lista.borrarUltimo(), true, "borrarUltimo elimina el último elemento (p5, ts: 2000)");
    probar(lista.longitud(), 3, "Longitud es 3 tras borrar el último");

    // 3.2 Pruebas exhaustivas de borrarPorTimestamp()
    cout << "--- Subpruebas: borrarPorTimestamp ---" << endl;
    {
        // Caso A: Lista con un único elemento
        ListaPuntosGPS lUnica;
        lUnica.insertar(p1); // ts 1000
        probar(lUnica.borrarPorTimestamp(1000), true, "borrarPorTimestamp en lista de un único elemento devuelve true");
        probar(lUnica.esVacia(), true, "Lista queda vacía tras borrar su único elemento por timestamp");
        probar(lUnica.borrarPorTimestamp(1000), false, "borrarPorTimestamp devuelve false en lista recién vaciada");

        // Caso B: Cabeza, nodo intermedio y cola en lista de varios elementos
        ListaPuntosGPS lTres;
        lTres.insertar(p1); // 1000
        lTres.insertar(p2); // 1050
        lTres.insertar(p3); // 1200

        // Borrar al principio (cabeza)
        probar(lTres.borrarPorTimestamp(1000), true, "borrarPorTimestamp borra correctamente el primer nodo (cabeza)");
        probar(lTres.getPuntoGPS(lTres.begin()).getTimestamp(), 1050UL, "El nuevo primer nodo tras borrarPorTimestamp es ts 1050");

        // Restablecer y borrar en punto intermedio
        lTres.insertar(p1); // 1000, 1050, 1200
        probar(lTres.borrarPorTimestamp(1050), true, "borrarPorTimestamp borra correctamente un nodo intermedio");
        probar(lTres.longitud(), 2, "La longitud es 2 tras borrar un nodo intermedio");
        probar(lTres.existeTimestamp(1050), false, "El timestamp 1050 ya no existe en la lista");

        // Borrar al final (cola)
        probar(lTres.borrarPorTimestamp(1200), true, "borrarPorTimestamp borra correctamente el último nodo (cola)");
        probar(lTres.longitud(), 1, "La longitud es 1 tras borrar el último nodo por timestamp");
        probar(lTres.getPuntoGPS(lTres.begin()).getTimestamp(), 1000UL, "Solo queda el nodo con ts 1000");
    }

    // 3.3 Pruebas exhaustivas de borrar(IteradorLista&)
    cout << "--- Subpruebas: borrar(IteradorLista&) ---" << endl;
    {
        // Robustez: iterador nulo o end()
        ListaPuntosGPS lTres;
        lTres.insertar(p1);
        lTres.insertar(p2);
        lTres.insertar(p3);

        IteradorLista itNull;
        probar(lTres.borrar(itNull), false, "borrar(iterador) devuelve false cuando el iterador apunta a nullptr (nulo)");
        probar(lTres.longitud(), 3, "La longitud de la lista no cambia tras intentar borrar un iterador nulo");

        IteradorLista itEnd = lTres.end();
        probar(lTres.borrar(itEnd), false, "borrar(iterador) devuelve false cuando recibe end()");

        // Caso A: Lista con un único elemento
        ListaPuntosGPS lUnica;
        lUnica.insertar(p1);
        IteradorLista it = lUnica.begin();
        probar(lUnica.borrar(it), true, "borrar(iterador) en lista de un único elemento devuelve true");
        probar(lUnica.esVacia(), true, "Lista queda vacía tras borrar su único elemento con iterador");
        probar(it, lUnica.end(), "El iterador pasa a ser nullptr (end) tras borrar el único elemento");

        // Caso B: Cabeza, intermedio y cola en lista de varios elementos
        IteradorLista itIni = lTres.begin();
        probar(lTres.borrar(itIni), true, "borrar(iterador) borra el primer nodo (cabeza)");
        probar(lTres.getPuntoGPS(lTres.begin()).getTimestamp(), 1050UL, "El nuevo primer nodo tras borrar por iterador es ts 1050");
        probar(itIni, lTres.end(), "El iterador inicial pasa a ser end() tras ser borrado");

        // Reconstruir lista y borrar intermedio
        lTres.insertar(p1); // 1000, 1050, 1200
        IteradorLista itMid = lTres.begin();
        itMid.step(); // apunta a p2 (1050)
        probar(lTres.borrar(itMid), true, "borrar(iterador) borra un nodo intermedio");
        probar(lTres.existeTimestamp(1050), false, "El nodo intermedio borrado ya no existe");
        probar(itMid, lTres.end(), "El iterador intermedio pasa a ser end() tras borrado");

        // Borrar al final (cola con iterador)
        IteradorLista itFin = lTres.begin();
        itFin.step(); // apunta a p3 (1200)
        probar(lTres.borrar(itFin), true, "borrar(iterador) borra el último nodo (cola)");
        probar(lTres.longitud(), 1, "La longitud es 1 tras borrar la cola");
        probar(itFin, lTres.end(), "El iterador final pasa a ser end() tras borrado");
    }

    // 3.4 Copia Profunda (Constructor de copia, operator= y destructor)
    cout << "--- Subpruebas: Copia Profunda y Asignación ---" << endl;
    {
        ListaPuntosGPS lBase;
        lBase.insertar(p1);
        lBase.insertar(p2);
        lBase.insertar(p4);

        ListaPuntosGPS listaCopia(lBase);
        probar(listaCopia.longitud(), lBase.longitud(), "El constructor de copia crea una lista de la misma longitud");
        probar(listaCopia.aCadena(), lBase.aCadena(), "La lista copiada tiene idéntico contenido en aCadena()");

        listaCopia.borrarPrimero();
        probar(listaCopia.longitud() != lBase.longitud(), true, "La modificación de la lista copiada no afecta a la original (copia profunda)");

        ListaPuntosGPS listaAsignada;
        listaAsignada = lBase;
        probar(listaAsignada.longitud(), lBase.longitud(), "El operador de asignación realiza una copia correcta de la lista");
        listaAsignada = listaAsignada;
        probar(listaAsignada.longitud(), lBase.longitud(), "La auto-asignación (obj = obj) mantiene la integridad de la lista");
    }

    // ------------------------------------------------------------------
    // SESIÓN 4: Método Asignar, Filtros y Métodos Globales de Trayectoria
    // ------------------------------------------------------------------
    cout << endl << "--- SESIÓN 4: Método Asignar, Filtros y Métodos de Trayectoria ---" << endl;

    // 4.1 Pruebas exhaustivas de asignar(IteradorLista, const PuntoGPS&)
    cout << "--- Subpruebas: asignar ---" << endl;
    {
        ListaPuntosGPS lAsig;
        lAsig.insertar(p1); // 1000
        lAsig.insertar(p2); // 1050
        lAsig.insertar(p4); // 1500

        // Robustez: iterador nulo o end()
        IteradorLista itNull;
        PuntoGPS pPrueba(38.0, -0.5, 100.0, 1100, "PRUEBA");
        probar(lAsig.asignar(itNull, pPrueba), false, "asignar devuelve false cuando recibe un iterador nulo");

        IteradorLista itEnd = lAsig.end();
        probar(lAsig.asignar(itEnd, pPrueba), false, "asignar devuelve false cuando recibe el iterador end()");

        // Caso A: Sin nodo anterior (nodo Cabeza en lista de varios elementos)
        IteradorLista itHead = lAsig.begin(); // apunta a 1000 (sin nodo anterior)
        PuntoGPS pHeadValido(38.0, -0.5, 100.0, 500, "HEAD-OK");
        probar(lAsig.asignar(itHead, pHeadValido), true, "asignar en cabeza modifica correctamente con ts menor al siguiente (500 < 1050)");
        probar(lAsig.getPuntoGPS(itHead).getTimestamp(), 500UL, "El timestamp en cabeza se actualizó correctamente a 500");

        PuntoGPS pHeadInvalido(38.0, -0.5, 100.0, 1200, "HEAD-BAD");
        probar(lAsig.asignar(itHead, pHeadInvalido), false, "asignar en cabeza devuelve false si el nuevo ts viola el orden con el siguiente (1200 > 1050)");

        // Caso B: Sin nodo siguiente (nodo Cola en lista de varios elementos)
        IteradorLista itTail = lAsig.begin();
        itTail.step(); // 1050
        itTail.step(); // 1500 (sin nodo siguiente)

        PuntoGPS pTailValido(38.0, -0.5, 100.0, 2000, "TAIL-OK");
        probar(lAsig.asignar(itTail, pTailValido), true, "asignar en cola modifica correctamente con ts mayor al anterior (2000 > 1050)");
        probar(lAsig.getPuntoGPS(itTail).getTimestamp(), 2000UL, "El timestamp en cola se actualizó correctamente a 2000");

        PuntoGPS pTailInvalido(38.0, -0.5, 100.0, 800, "TAIL-BAD");
        probar(lAsig.asignar(itTail, pTailInvalido), false, "asignar en cola devuelve false si el nuevo ts viola el orden con el anterior (800 < 1050)");

        // Caso C: Con nodo anterior y siguiente (nodo Intermedio)
        IteradorLista itMid = lAsig.begin();
        itMid.step(); // apunta a 1050 (anterior: 500, siguiente: 2000)

        PuntoGPS pMidValido(38.0, -0.5, 100.0, 1000, "MID-OK");
        probar(lAsig.asignar(itMid, pMidValido), true, "asignar en nodo intermedio modifica con ts válido (500 < 1000 < 2000)");

        PuntoGPS pMidMenorAnterior(38.0, -0.5, 100.0, 400, "MID-BAD1");
        probar(lAsig.asignar(itMid, pMidMenorAnterior), false, "asignar en nodo intermedio devuelve false si ts < anterior (400 < 500)");

        PuntoGPS pMidMayorSiguiente(38.0, -0.5, 100.0, 2500, "MID-BAD2");
        probar(lAsig.asignar(itMid, pMidMayorSiguiente), false, "asignar en nodo intermedio devuelve false si ts > siguiente (2500 > 2000)");

        // Caso D: Sin anterior ni siguiente (Único elemento en la lista)
        ListaPuntosGPS lUnicoAsig;
        lUnicoAsig.insertar(p1); // ts 1000
        IteradorLista itUnico = lUnicoAsig.begin();
        PuntoGPS pUnicoNuevo(38.0, -0.5, 100.0, 9999, "SOLO-OK");
        probar(lUnicoAsig.asignar(itUnico, pUnicoNuevo), true, "asignar en lista de un único elemento modifica sin restricciones de orden");
        probar(lUnicoAsig.getPuntoGPS(itUnico).getTimestamp(), 9999UL, "El timestamp del único elemento se actualizó correctamente a 9999");
    }

    // 4.2 Métodos de Filtrado (filtrarRangoTimestamp, filtrarDispositivo)
    cout << "--- Subpruebas: Métodos de Filtrado ---" << endl;
    {
        ListaPuntosGPS trayecto;
        trayecto.insertar(p1); // 1000 (GARMIN-01)
        trayecto.insertar(p2); // 1050 (GARMIN-01)
        trayecto.insertar(p3); // 1200 (DEV-B)
        trayecto.insertar(p4); // 1500 (GARMIN-01)
        trayecto.insertar(p5); // 2000 (GARMIN-02)

        ListaPuntosGPS filtroRango = trayecto.filtrarRangoTimestamp(1020, 1300);
        probar(filtroRango.longitud(), 2, "filtrarRangoTimestamp [1020, 1300] devuelve exactamente 2 puntos (1050 y 1200)");
        probar(filtroRango.existeTimestamp(1050) && filtroRango.existeTimestamp(1200), true, "Los puntos filtrados por rango coinciden con los esperados");

        ListaPuntosGPS filtroDev = trayecto.filtrarDispositivo("GARMIN-01");
        probar(filtroDev.longitud(), 3, "filtrarDispositivo('GARMIN-01') devuelve exactamente 3 puntos");
        probar(filtroDev.contarMismaUbicacion(p3), 0, "El punto de DEV-B fue excluido correctamente en la lista filtrada por dispositivo");

        // 4.3 Métodos globales sobre la trayectoria completa
        double dist12 = p1.distancia3D(p2);
        double dist23 = p2.distancia3D(p3);
        double dist34 = p3.distancia3D(p4);
        double dist45 = p4.distancia3D(p5);
        double distTotalEsperada = dist12 + dist23 + dist34 + dist45;

        probar(trayecto.distanciaTotal(), distTotalEsperada, "distanciaTotal() coincide exactamente con la suma de tramos 3D");

        double velMediaEsperada = distTotalEsperada / (2000.0 - 1000.0);
        probar(trayecto.velocidadMedia(), velMediaEsperada, "velocidadMedia() calcula correctamente distanciaTotal / (ts_fin - ts_inicio)");

        double v12 = p1.velocidad(p2);
        double v23 = p2.velocidad(p3);
        double v34 = p3.velocidad(p4);
        double v45 = p4.velocidad(p5);
        double velMaxEsperada = max({v12, v23, v34, v45});

        probar(trayecto.velocidadMaxima(), velMaxEsperada, "velocidadMaxima() identifica correctamente el tramo con velocidad más alta");

        // Casos frontera (lista con 1 solo elemento)
        ListaPuntosGPS trayectoUnico;
        trayectoUnico.insertar(p1);
        probar(trayectoUnico.distanciaTotal(), 0.0, "distanciaTotal en lista con 1 solo elemento devuelve 0.0");
        probar(trayectoUnico.velocidadMedia(), 0.0, "velocidadMedia en lista con 1 solo elemento devuelve 0.0");
        probar(trayectoUnico.velocidadMaxima(), 0.0, "velocidadMaxima en lista con 1 solo elemento devuelve 0.0");
    }

    // ------------------------------------------------------------------
    // SESIÓN 5: PRUEBAS ADICIONALES (casos frontera y robustez)
    // ------------------------------------------------------------------
    cout << endl << "--- SESIÓN 5: Pruebas adicionales ---" << endl;

    // 5.1 NodoLista: copia, asignación y aislamiento de memoria
    cout << "--- 5.1 NodoLista ---" << endl;
    {
        NodoLista a(nullptr, mk(10));
        NodoLista b(&a, mk(20));

        NodoLista copia(b);
        probar(copia.getSiguiente(), &a, "NodoLista copia: copia el puntero siguiente (no clona nodos vecinos)");
        probar(copia.getPuntoGPS(), mk(20), "NodoLista copia: copia el punto GPS");

        NodoLista asig;
        asig = b;
        probar(asig.getSiguiente(), &a, "NodoLista operator=: copia el puntero siguiente");
        probar(asig.getPuntoGPS(), mk(20), "NodoLista operator=: copia el punto GPS");
        asig = asig;
        probar(asig.getPuntoGPS(), mk(20), "NodoLista auto-asignación mantiene el punto");
        probar(asig.getSiguiente(), &a, "NodoLista auto-asignación mantiene el siguiente");

        asig.setSiguiente(nullptr);
        probar(asig.getSiguiente(), (NodoLista*)nullptr, "NodoLista::setSiguiente(nullptr) funciona");
        probar(b.getSiguiente(), &a, "Modificar la copia no afecta al nodo original");

        // Datos extra dentro del nodo (copia profunda del PuntoGPS)
        PuntoGPS pe(38.0, -0.5, 10.0, 5, "X", 3);
        pe.setDatoExtra(1, 7.5);
        NodoLista ne(nullptr, pe);
        NodoLista nc(ne);
        probar(nc.getPuntoGPS().getDatoExtra(1), 7.5, "NodoLista copia conserva los datos extra del punto");
        pe.setDatoExtra(1, 99.0);
        probar(ne.getPuntoGPS().getDatoExtra(1), 7.5, "El nodo guarda una copia del punto (no depende del original)");
        nc.setPuntoGPS(mk(1));
        probar(ne.getPuntoGPS().getNumDatosExtra(), 3, "setPuntoGPS en la copia no afecta al nodo original");
    }

    // 5.2 IteradorLista
    cout << "--- 5.2 IteradorLista ---" << endl;
    {
        ListaPuntosGPS l;
        l.insertar(mk(1));
        l.insertar(mk(2));

        IteradorLista a = l.begin();
        IteradorLista b(a);
        probar(a == b, true, "Iterador copia: == al original");
        probar(a != b, false, "Iterador copia: != es false respecto al original");
        b.step();
        probar(a != b, true, "step() sobre la copia no mueve el original");
        probar(l.getPuntoGPS(a).getTimestamp(), 1UL, "El original sigue apuntando al primer nodo");
        probar(l.getPuntoGPS(b).getTimestamp(), 2UL, "La copia avanzó al segundo nodo");

        IteradorLista c;
        c = b;
        probar(c == b, true, "Iterador operator=: queda igual al asignado");
        c = c;
        probar(c == b, true, "Iterador auto-asignación no altera el iterador");
        c.step();
        probar(c, l.end(), "step() desde el último nodo llega a end()");
        c.step();
        c.step();
        probar(c, l.end(), "step() sobre end() no hace nada (no falla)");
        probar(l.getPuntoGPS(c), PuntoGPS(), "getPuntoGPS(end()) devuelve PuntoGPS por defecto");

        probar(l.begin() == l.begin(), true, "Dos begin() de la misma lista son iguales");
        probar(l.begin() != l.end(), true, "begin() != end() en lista no vacía");

        // Iteradores de dos listas con el mismo contenido NO son iguales (distintos nodos)
        ListaPuntosGPS l2(l);
        probar(l.begin() == l2.begin(), false, "Iteradores de listas distintas (aunque con igual contenido) no son iguales");
        probar(l.end() == l2.end(), true, "end() de listas distintas es siempre igual (nullptr)");
    }

    // 5.3 insertar: casos frontera
    cout << "--- 5.3 insertar ---" << endl;
    {
        ListaPuntosGPS l;
        probar(l.insertar(mk(0)), true, "Insertar timestamp 0 en lista vacía");
        probar(tiene(l, {0}), true, "La lista contiene solo el timestamp 0");
        probar(l.insertar(mk(0)), false, "Insertar timestamp 0 duplicado devuelve false");
        probar(l.insertar(mk(4000000000UL)), true, "Insertar un timestamp muy grande");
        probar(l.insertar(mk(50)), true, "Insertar entre el primero y el último");
        probar(tiene(l, {0, 50, 4000000000UL}), true, "Orden correcto: 0, 50, 4000000000");

        probar(l.insertar(mk(0, "OTRO")), false, "Duplicado en la cabeza (aunque cambie el dispositivo) devuelve false");
        probar(l.insertar(mk(4000000000UL, "OTRO")), false, "Duplicado en la cola devuelve false");
        probar(l.insertar(mk(50, "OTRO", 500.0)), false, "Duplicado en posición intermedia devuelve false");
        probar(l.longitud(), 3, "Los duplicados rechazados no cambian la longitud");
        probar(l.getPuntoGPS(l.buscarPorTimestamp(50)).getDispositivo(), string("D"), "El duplicado rechazado no sobrescribe el original");
        probar(tiene(l, {0, 50, 4000000000UL}), true, "La lista no se altera tras rechazar duplicados");

        // Inserción en orden inverso
        ListaPuntosGPS inv;
        bool todoTrue = true;
        for (unsigned long i = 100; i >= 1; i--) {
            todoTrue = inv.insertar(mk(i)) && todoTrue;
        }
        probar(todoTrue, true, "100 inserciones en orden descendente devuelven true");
        probar(inv.longitud(), 100, "Longitud 100 tras insertar en orden descendente");
        probar(estrictamenteOrdenada(inv), true, "La lista queda estrictamente ordenada tras insertar en orden descendente");
        probar(inv.getPuntoGPS(inv.begin()).getTimestamp(), 1UL, "El primero es el timestamp 1");

        // Inserción en orden ascendente (siempre al final) con comprobación de último
        ListaPuntosGPS asc;
        for (unsigned long i = 1; i <= 50; i++) asc.insertar(mk(i));
        probar(estrictamenteOrdenada(asc) && asc.longitud() == 50, true, "50 inserciones en orden ascendente correctas");

        // Inserción desordenada (permutación)
        ListaPuntosGPS perm;
        unsigned long orden[] = {5, 1, 9, 3, 7, 2, 8, 4, 6, 10};
        for (int i = 0; i < 10; i++) perm.insertar(mk(orden[i]));
        probar(tiene(perm, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}), true, "Inserción desordenada produce lista ordenada 1..10");

        // La lista guarda una copia del punto
        ListaPuntosGPS lc;
        PuntoGPS orig = mk(7, "ORIG");
        lc.insertar(orig);
        orig.setDispositivo("CAMBIADO");
        orig.setLatitud(10.0);
        probar(lc.getPuntoGPS(lc.begin()).getDispositivo(), string("ORIG"), "La lista almacena una copia: cambiar el original no la afecta");
        probar(lc.getPuntoGPS(lc.begin()).getLatitud(), 38.0, "La latitud almacenada no cambia al modificar el original");

        // Puntos con datos extra
        PuntoGPS pe(38.0, -0.5, 100.0, 15, "EXTRA", 2);
        pe.setDatoExtra(0, 1.5);
        pe.setDatoExtra(1, 2.5);
        ListaPuntosGPS le;
        le.insertar(mk(10));
        le.insertar(pe);
        le.insertar(mk(20));
        PuntoGPS rec = le.getPuntoGPS(le.buscarPorTimestamp(15));
        probar(rec.getNumDatosExtra(), 2, "El punto insertado conserva su número de datos extra");
        probar(rec.getDatoExtra(0), 1.5, "El punto insertado conserva el dato extra 0");
        probar(rec.getDatoExtra(1), 2.5, "El punto insertado conserva el dato extra 1");

        // Un punto inválido se convierte en PuntoGPS por defecto (timestamp 0)
        ListaPuntosGPS li;
        PuntoGPS invalido(100.0, 0.0, 0.0, 500, "INV");
        probar(li.insertar(invalido), true, "Insertar un punto inválido (queda con ts 0 por constructor)");
        probar(li.getPuntoGPS(li.begin()).getTimestamp(), 0UL, "El punto inválido se almacena con timestamp 0");
        probar(li.insertar(mk(0)), false, "Un segundo punto con ts 0 es duplicado del inválido");

        // Reinserción tras vaciar
        ListaPuntosGPS lv;
        lv.insertar(mk(1));
        lv.borrarPrimero();
        probar(lv.insertar(mk(1)), true, "Se puede reinsertar el mismo timestamp tras borrarlo");
        probar(lv.longitud(), 1, "Longitud 1 tras reinsertar");
    }

    // 5.4 Búsqueda y consulta
    cout << "--- 5.4 existeTimestamp / buscarPorTimestamp / contarMismaUbicacion ---" << endl;
    {
        ListaPuntosGPS vacia;
        probar(vacia.existeTimestamp(0), false, "existeTimestamp en lista vacía es false");
        probar(vacia.existeTimestamp(100), false, "existeTimestamp(100) en lista vacía es false");
        probar(vacia.buscarPorTimestamp(100), vacia.end(), "buscarPorTimestamp en lista vacía devuelve end()");
        probar(vacia.contarMismaUbicacion(mk(1)), 0, "contarMismaUbicacion en lista vacía devuelve 0");

        ListaPuntosGPS l;
        l.insertar(mk(0));
        l.insertar(mk(100));
        l.insertar(mk(200));
        probar(l.existeTimestamp(0), true, "existeTimestamp(0) con el primero en ts 0");
        probar(l.existeTimestamp(200), true, "existeTimestamp del último elemento");
        probar(l.existeTimestamp(150), false, "existeTimestamp entre dos elementos es false");
        probar(l.existeTimestamp(201), false, "existeTimestamp justo después del último es false");

        probar(l.buscarPorTimestamp(0), l.begin(), "buscarPorTimestamp del primero == begin()");
        IteradorLista ult = l.buscarPorTimestamp(200);
        probar(ult != l.end(), true, "buscarPorTimestamp del último no devuelve end()");
        ult.step();
        probar(ult, l.end(), "Tras step() desde el último encontrado se llega a end()");
        probar(l.buscarPorTimestamp(150), l.end(), "buscarPorTimestamp inexistente intermedio == end()");
        probar(l.buscarPorTimestamp(5000), l.end(), "buscarPorTimestamp mayor que todos == end()");

        // El iterador de buscarPorTimestamp se puede usar para borrar
        IteradorLista itB = l.buscarPorTimestamp(100);
        probar(l.borrar(itB), true, "borrar con el iterador devuelto por buscarPorTimestamp");
        probar(tiene(l, {0, 200}), true, "La lista queda con 0 y 200");

        // contarMismaUbicacion
        ListaPuntosGPS u;
        u.insertar(PuntoGPS(38.0, -0.5, 100.0, 1, "A"));
        u.insertar(PuntoGPS(38.0, -0.5, 100.0, 2, "B", 3));            // otro dispositivo y con datos extra
        u.insertar(PuntoGPS(38.0 + 5e-7, -0.5 - 5e-7, 100.0 + 5e-7, 3, "C")); // dentro de tolerancia
        u.insertar(PuntoGPS(38.0 + 2e-6, -0.5, 100.0, 4, "D"));        // fuera por latitud
        u.insertar(PuntoGPS(38.0, -0.5 + 2e-6, 100.0, 5, "E"));        // fuera por longitud
        u.insertar(PuntoGPS(38.0, -0.5, 100.0 + 2e-6, 6, "F"));        // fuera por altitud
        u.insertar(PuntoGPS(38.0, -0.5, 101.0, 7, "G"));               // otra altitud
        PuntoGPS ref(38.0, -0.5, 100.0, 9999, "REF");
        probar(u.contarMismaUbicacion(ref), 3, "contarMismaUbicacion ignora ts/dispositivo/extra y respeta la tolerancia");
        probar(u.contarMismaUbicacion(PuntoGPS(38.0, -0.5, 101.0, 1, "X")), 1, "Solo un punto a altitud 101");
        probar(u.contarMismaUbicacion(PuntoGPS(0.0, 0.0, 0.0, 1, "X")), 0, "Ningún punto en (0,0,0)");
        probar(u.longitud(), 7, "contarMismaUbicacion no modifica la lista");
    }

    // 5.5 asignar: casos frontera
    cout << "--- 5.5 asignar ---" << endl;
    {
        ListaPuntosGPS l;
        l.insertar(mk(100, "A"));
        l.insertar(mk(200, "B"));
        l.insertar(mk(300, "C"));

        // Mismo timestamp con contenido distinto: permitido en cualquier posición
        IteradorLista it = l.begin();
        probar(l.asignar(it, mk(100, "A2", 500.0)), true, "asignar con su mismo timestamp en la cabeza (cambia contenido)");
        probar(l.getPuntoGPS(it).getDispositivo(), string("A2"), "El contenido de la cabeza cambió");
        it.step();
        probar(l.asignar(it, mk(200, "B2")), true, "asignar con su mismo timestamp en posición intermedia");
        probar(l.getPuntoGPS(it).getDispositivo(), string("B2"), "El contenido del nodo intermedio cambió");
        it.step();
        probar(l.asignar(it, mk(300, "C2")), true, "asignar con su mismo timestamp en la cola");
        probar(l.getPuntoGPS(it).getDispositivo(), string("C2"), "El contenido de la cola cambió");
        probar(l.longitud(), 3, "asignar no cambia la longitud");

        // Duplicados con los vecinos
        string antes = l.aCadena();
        IteradorLista itMid = l.buscarPorTimestamp(200);
        probar(l.asignar(itMid, mk(300, "X")), false, "asignar con timestamp igual al del siguiente devuelve false");
        probar(l.asignar(itMid, mk(100, "X")), false, "asignar con timestamp igual al del anterior devuelve false");
        probar(l.aCadena(), antes, "Tras asignaciones fallidas la lista no cambia");

        IteradorLista itHead = l.begin();
        probar(l.asignar(itHead, mk(200, "X")), false, "asignar en cabeza con timestamp igual al del siguiente devuelve false");
        IteradorLista itTail = l.buscarPorTimestamp(300);
        probar(l.asignar(itTail, mk(200, "X")), false, "asignar en cola con timestamp igual al del anterior devuelve false");
        probar(l.aCadena(), antes, "Tras más asignaciones fallidas la lista sigue igual");

        // Límites exactos (anterior+1 y siguiente-1)
        probar(l.asignar(itMid, mk(101, "LIM1")), true, "asignar con ts = anterior + 1 es válido");
        probar(l.asignar(itMid, mk(299, "LIM2")), true, "asignar con ts = siguiente - 1 es válido");
        probar(l.asignar(itMid, mk(100, "LIM3")), false, "asignar con ts = anterior es inválido (100)");
        probar(l.asignar(itMid, mk(300, "LIM4")), false, "asignar con ts = siguiente es inválido (300)");
        probar(l.asignar(itMid, mk(99, "LIM5")), false, "asignar con ts < anterior es inválido");
        probar(l.asignar(itMid, mk(301, "LIM6")), false, "asignar con ts > siguiente es inválido");
        probar(tiene(l, {100, 299, 300}), true, "Timestamps finales 100, 299, 300");
        probar(l.existeTimestamp(200), false, "El timestamp viejo (200) ya no existe");
        probar(l.existeTimestamp(299), true, "El timestamp nuevo (299) existe");

        // Cabeza y cola con límites abiertos
        probar(l.asignar(itHead, mk(0, "H0")), true, "asignar en cabeza con ts 0 es válido");
        probar(l.asignar(itTail, mk(4000000000UL, "TG")), true, "asignar en cola con un ts enorme es válido");
        probar(estrictamenteOrdenada(l), true, "La lista sigue estrictamente ordenada");

        // Datos extra en asignar
        PuntoGPS pe(38.0, -0.5, 100.0, 299, "EX", 2);
        pe.setDatoExtra(0, 4.5);
        pe.setDatoExtra(1, 5.5);
        probar(l.asignar(itMid, pe), true, "asignar un punto con datos extra");
        probar(l.getPuntoGPS(itMid).getNumDatosExtra(), 2, "El punto asignado conserva el número de datos extra");
        probar(l.getPuntoGPS(itMid).getDatoExtra(1), 5.5, "El punto asignado conserva los datos extra");
        probar(l.asignar(itMid, mk(299, "SIN")), true, "Reasignar un punto sin datos extra sobre uno con ellos");
        probar(l.getPuntoGPS(itMid).getNumDatosExtra(), 0, "Ya no quedan datos extra tras reasignar");

        // Iterador ya invalidado tras borrar
        IteradorLista itBorrado = l.begin();
        l.borrar(itBorrado);
        probar(l.asignar(itBorrado, mk(1)), false, "asignar con un iterador que quedó nulo tras borrar devuelve false");

        // Asignar en lista con dos elementos
        ListaPuntosGPS dos;
        dos.insertar(mk(10));
        dos.insertar(mk(20));
        IteradorLista d1 = dos.begin();
        IteradorLista d2 = dos.begin();
        d2.step();
        probar(dos.asignar(d1, mk(19)), true, "Lista de 2: asignar cabeza con ts < siguiente");
        probar(dos.asignar(d1, mk(20)), false, "Lista de 2: asignar cabeza con ts == siguiente");
        probar(dos.asignar(d2, mk(19)), false, "Lista de 2: asignar cola con ts == anterior");
        probar(dos.asignar(d2, mk(21)), true, "Lista de 2: asignar cola con ts > anterior");
        probar(tiene(dos, {19, 21}), true, "Lista de 2 queda con 19 y 21");
    }

    // 5.6 Borrado: casos frontera
    cout << "--- 5.6 Borrado ---" << endl;
    {
        ListaPuntosGPS vacia;
        probar(vacia.borrarPrimero(), false, "borrarPrimero en lista vacía devuelve false");
        probar(vacia.borrarUltimo(), false, "borrarUltimo en lista vacía devuelve false");
        probar(vacia.borrarPorTimestamp(0), false, "borrarPorTimestamp en lista vacía devuelve false");
        IteradorLista itV = vacia.begin();
        probar(vacia.borrar(itV), false, "borrar(begin()) en lista vacía devuelve false");
        probar(vacia.esVacia(), true, "La lista vacía sigue vacía tras los intentos de borrado");

        // Lista de un elemento
        ListaPuntosGPS uno;
        uno.insertar(mk(5));
        probar(uno.borrarUltimo(), true, "borrarUltimo en lista de un elemento");
        probar(uno.esVacia(), true, "Lista vacía tras borrarUltimo del único elemento");
        probar(uno.begin(), uno.end(), "begin() == end() tras vaciar con borrarUltimo");
        probar(uno.insertar(mk(5)), true, "Se puede insertar tras vaciar con borrarUltimo");
        probar(uno.borrarPrimero(), true, "borrarPrimero en lista de un elemento");
        probar(uno.begin(), uno.end(), "begin() == end() tras vaciar con borrarPrimero");
        probar(uno.borrarPrimero(), false, "borrarPrimero otra vez devuelve false");
        probar(uno.insertar(mk(6)), true, "Se puede insertar tras vaciar con borrarPrimero");

        // borrarUltimo deja la cola bien enlazada
        ListaPuntosGPS l;
        l.insertar(mk(10));
        l.insertar(mk(20));
        probar(l.borrarUltimo(), true, "borrarUltimo en lista de 2 elementos");
        probar(tiene(l, {10}), true, "Queda solo el elemento 10");
        l.insertar(mk(30));
        probar(tiene(l, {10, 30}), true, "Tras borrarUltimo se puede insertar al final correctamente");
        l.borrarUltimo();
        l.borrarUltimo();
        probar(l.esVacia(), true, "Vaciada con borrarUltimo repetido");
        probar(l.borrarUltimo(), false, "borrarUltimo sobre lista ya vacía devuelve false");

        // Vaciado completo con borrarPrimero
        ListaPuntosGPS m;
        for (unsigned long i = 1; i <= 20; i++) m.insertar(mk(i));
        int borrados = 0;
        while (m.borrarPrimero()) borrados++;
        probar(borrados, 20, "borrarPrimero repetido borra exactamente 20 elementos");
        probar(m.longitud(), 0, "Longitud 0 tras vaciar con borrarPrimero");

        // borrarPorTimestamp
        ListaPuntosGPS n;
        n.insertar(mk(10));
        n.insertar(mk(20));
        n.insertar(mk(30));
        probar(n.borrarPorTimestamp(25), false, "borrarPorTimestamp inexistente devuelve false");
        probar(n.longitud(), 3, "La longitud no cambia tras un borrarPorTimestamp fallido");
        probar(n.borrarPorTimestamp(20), true, "borrarPorTimestamp(20) devuelve true");
        probar(n.borrarPorTimestamp(20), false, "Borrar dos veces el mismo timestamp: segunda vez false");
        probar(tiene(n, {10, 30}), true, "Quedan 10 y 30");
        probar(n.insertar(mk(20)), true, "Se puede reinsertar el timestamp borrado");
        probar(tiene(n, {10, 20, 30}), true, "Vuelven a estar 10, 20, 30");

        // borrarPrimero/borrarUltimo mezclados con borrar por iterador
        ListaPuntosGPS o;
        for (unsigned long i = 1; i <= 6; i++) o.insertar(mk(i * 10));
        o.borrarPrimero();   // 20 30 40 50 60
        o.borrarUltimo();    // 20 30 40 50
        IteradorLista it = o.buscarPorTimestamp(30);
        o.borrar(it);        // 20 40 50
        probar(tiene(o, {20, 40, 50}), true, "Combinación borrarPrimero + borrarUltimo + borrar(it)");
        probar(o.aCadena(), mk(20).aCadena() + "\n" + mk(40).aCadena() + "\n" + mk(50).aCadena(), "aCadena coherente tras borrados mixtos");

        // Borrar elementos alternos recorriendo con iterador (reposicionando tras borrar)
        ListaPuntosGPS p;
        for (unsigned long i = 1; i <= 10; i++) p.insertar(mk(i));
        for (unsigned long i = 2; i <= 10; i += 2) {
            IteradorLista ib = p.buscarPorTimestamp(i);
            p.borrar(ib);
        }
        probar(tiene(p, {1, 3, 5, 7, 9}), true, "Borrar pares con iteradores deja solo los impares");
    }

    // 5.7 Constructor de copia, operator= y destructor
    cout << "--- 5.7 Copia profunda y asignación ---" << endl;
    {
        ListaPuntosGPS vacia;
        ListaPuntosGPS copiaVacia(vacia);
        probar(copiaVacia.esVacia(), true, "Copia de una lista vacía es vacía");
        copiaVacia.insertar(mk(1));
        probar(vacia.esVacia(), true, "Insertar en la copia no afecta a la original vacía");

        ListaPuntosGPS grande;
        for (unsigned long i = 1; i <= 30; i++) grande.insertar(mk(i, "G"));
        ListaPuntosGPS pequena;
        pequena.insertar(mk(999, "P"));

        pequena = grande;
        probar(pequena.longitud(), 30, "operator= de lista grande sobre pequeña: longitud 30");
        probar(pequena.aCadena(), grande.aCadena(), "operator= copia todo el contenido");
        pequena.insertar(mk(31, "NUEVO"));
        probar(grande.longitud(), 30, "Insertar en el destino no afecta a la lista origen");
        probar(grande.existeTimestamp(999), false, "El contenido previo del destino fue descartado");

        grande = vacia;
        probar(grande.esVacia(), true, "operator= de lista vacía sobre una no vacía la vacía");
        probar(grande.begin(), grande.end(), "begin() == end() tras asignar una lista vacía");
        probar(pequena.longitud(), 31, "La lista que se copió antes no se ve afectada");

        // Asignación encadenada
        ListaPuntosGPS a, b, c;
        c.insertar(mk(1));
        c.insertar(mk(2));
        a = b = c;
        probar(a.aCadena(), c.aCadena(), "Asignación encadenada a = b = c (a)");
        probar(b.aCadena(), c.aCadena(), "Asignación encadenada a = b = c (b)");
        a.borrarPrimero();
        probar(b.longitud(), 2, "Las listas encadenadas son independientes");

        // Auto-asignación con contenido
        c = c;
        probar(c.longitud(), 2, "Auto-asignación conserva la longitud");
        probar(tiene(c, {1, 2}), true, "Auto-asignación conserva el contenido");

        // La copia sobrevive a la destrucción del original
        ListaPuntosGPS* orig = new ListaPuntosGPS;
        orig->insertar(mk(1, "X"));
        orig->insertar(mk(2, "Y"));
        ListaPuntosGPS copia(*orig);
        ListaPuntosGPS asignada;
        asignada = *orig;
        delete orig;
        probar(copia.aCadena(), mk(1, "X").aCadena() + "\n" + mk(2, "Y").aCadena(), "La copia es válida tras destruir la original");
        probar(asignada.aCadena(), copia.aCadena(), "La lista asignada es válida tras destruir la original");

        // Copia profunda de datos extra
        ListaPuntosGPS conExtra;
        PuntoGPS pe(38.0, -0.5, 100.0, 10, "EX", 3);
        pe.setDatoExtra(0, 1.0);
        pe.setDatoExtra(1, 2.0);
        pe.setDatoExtra(2, 3.0);
        conExtra.insertar(pe);
        ListaPuntosGPS cpy(conExtra);
        ListaPuntosGPS asg;
        asg = conExtra;
        IteradorLista it = conExtra.begin();
        conExtra.asignar(it, mk(10, "SIN"));
        PuntoGPS r1 = cpy.getPuntoGPS(cpy.begin());
        PuntoGPS r2 = asg.getPuntoGPS(asg.begin());
        probar(r1.getNumDatosExtra(), 3, "La copia conserva 3 datos extra aunque se reasigne el original");
        probar(r1.getDatoExtra(2), 3.0, "La copia conserva el valor del dato extra 2");
        probar(r2.getNumDatosExtra(), 3, "La lista asignada conserva 3 datos extra");
        probar(r2.getDatoExtra(0), 1.0, "La lista asignada conserva el valor del dato extra 0");
        probar(r1.aCadena(), pe.aCadena(), "El punto copiado coincide con el original en aCadena()");
    }

    // 5.8 Filtros
    cout << "--- 5.8 Filtros ---" << endl;
    {
        ListaPuntosGPS vacia;
        probar(vacia.filtrarRangoTimestamp(0, 1000).esVacia(), true, "filtrarRangoTimestamp sobre lista vacía devuelve lista vacía");
        probar(vacia.filtrarDispositivo("A").esVacia(), true, "filtrarDispositivo sobre lista vacía devuelve lista vacía");

        ListaPuntosGPS t;
        t.insertar(mk(100, "A"));
        t.insertar(mk(200, "B"));
        t.insertar(mk(300, "A"));
        t.insertar(mk(400, "B"));
        t.insertar(mk(500, "A"));

        probar(tiene(t.filtrarRangoTimestamp(100, 500), {100, 200, 300, 400, 500}), true, "Rango que incluye toda la lista (ambos extremos inclusivos)");
        probar(tiene(t.filtrarRangoTimestamp(0, 1000000), {100, 200, 300, 400, 500}), true, "Rango mucho mayor que la lista devuelve todo");
        probar(tiene(t.filtrarRangoTimestamp(200, 400), {200, 300, 400}), true, "Rango [200,400] incluye los dos extremos");
        probar(tiene(t.filtrarRangoTimestamp(201, 399), {300}), true, "Rango [201,399] excluye los extremos");
        probar(tiene(t.filtrarRangoTimestamp(300, 300), {300}), true, "Rango [300,300] con un timestamp existente devuelve 1 punto");
        probar(t.filtrarRangoTimestamp(301, 301).esVacia(), true, "Rango [301,301] sin puntos devuelve lista vacía");
        probar(t.filtrarRangoTimestamp(250, 260).esVacia(), true, "Rango entre dos puntos sin ninguno dentro devuelve lista vacía");
        probar(t.filtrarRangoTimestamp(0, 99).esVacia(), true, "Rango anterior a todos los puntos devuelve lista vacía");
        probar(t.filtrarRangoTimestamp(501, 9999).esVacia(), true, "Rango posterior a todos los puntos devuelve lista vacía");
        probar(t.filtrarRangoTimestamp(400, 200).esVacia(), true, "Rango invertido (inicio > fin) devuelve lista vacía");
        probar(tiene(t.filtrarRangoTimestamp(0, 100), {100}), true, "Rango [0,100] incluye el primer elemento");
        probar(tiene(t.filtrarRangoTimestamp(500, 9999), {500}), true, "Rango [500,9999] incluye el último elemento");

        probar(tiene(t.filtrarDispositivo("A"), {100, 300, 500}), true, "filtrarDispositivo('A') devuelve 100, 300, 500 en orden");
        probar(tiene(t.filtrarDispositivo("B"), {200, 400}), true, "filtrarDispositivo('B') devuelve 200, 400 en orden");
        probar(t.filtrarDispositivo("Z").esVacia(), true, "filtrarDispositivo con dispositivo inexistente devuelve lista vacía");
        probar(t.filtrarDispositivo("a").esVacia(), true, "filtrarDispositivo distingue mayúsculas de minúsculas");
        probar(t.filtrarDispositivo("").esVacia(), true, "filtrarDispositivo('') sin puntos de ese tipo devuelve lista vacía");
        t.insertar(mk(600, ""));
        probar(tiene(t.filtrarDispositivo(""), {600}), true, "filtrarDispositivo('') encuentra el punto con dispositivo vacío");
        t.borrarUltimo();

        // La original no se modifica y la filtrada es independiente
        probar(tiene(t, {100, 200, 300, 400, 500}), true, "Los filtros no modifican la lista original");
        ListaPuntosGPS f = t.filtrarDispositivo("A");
        f.borrarPrimero();
        f.insertar(mk(123, "NUEVO"));
        probar(tiene(t, {100, 200, 300, 400, 500}), true, "Modificar la lista filtrada no afecta a la original");
        probar(t.filtrarDispositivo("A").aCadena(), mk(100, "A").aCadena() + "\n" + mk(300, "A").aCadena() + "\n" + mk(500, "A").aCadena(),
               "aCadena de la lista filtrada por dispositivo es correcta");

        // Filtros sobre una lista con datos extra
        ListaPuntosGPS e;
        PuntoGPS pe(38.0, -0.5, 100.0, 10, "EX", 2);
        pe.setDatoExtra(0, 8.0);
        pe.setDatoExtra(1, 9.0);
        e.insertar(pe);
        e.insertar(mk(20, "OTRO"));
        ListaPuntosGPS fr = e.filtrarRangoTimestamp(0, 15);
        ListaPuntosGPS fd = e.filtrarDispositivo("EX");
        probar(fr.getPuntoGPS(fr.begin()).getDatoExtra(1), 9.0, "filtrarRangoTimestamp conserva los datos extra");
        probar(fd.getPuntoGPS(fd.begin()).getNumDatosExtra(), 2, "filtrarDispositivo conserva el número de datos extra");
        probar(fd.getPuntoGPS(fd.begin()).aCadena(), pe.aCadena(), "filtrarDispositivo conserva el punto completo");

        // Encadenar filtros
        ListaPuntosGPS enc = t.filtrarRangoTimestamp(200, 500).filtrarDispositivo("A");
        probar(tiene(enc, {300, 500}), true, "Encadenar filtrarRangoTimestamp y filtrarDispositivo");
    }

    // 5.9 Cálculos sobre la trayectoria
    cout << "--- 5.9 distanciaTotal / velocidadMedia / velocidadMaxima ---" << endl;
    {
        ListaPuntosGPS vacia;
        probar(vacia.distanciaTotal(), 0.0, "distanciaTotal de lista vacía es 0.0");
        probar(vacia.velocidadMedia(), 0.0, "velocidadMedia de lista vacía es 0.0");
        probar(vacia.velocidadMaxima(), 0.0, "velocidadMaxima de lista vacía es 0.0");

        // Dos puntos
        PuntoGPS a(38.0, -0.5, 0.0, 0, "A");
        PuntoGPS b(38.01, -0.49, 50.0, 100, "A");
        ListaPuntosGPS dos;
        dos.insertar(a);
        dos.insertar(b);
        probar(dos.distanciaTotal(), a.distancia3D(b), "distanciaTotal con 2 puntos == distancia3D entre ellos");
        probar(dos.velocidadMedia(), a.velocidad(b), "velocidadMedia con 2 puntos == velocidad entre ellos");
        probar(dos.velocidadMaxima(), a.velocidad(b), "velocidadMaxima con 2 puntos == velocidad entre ellos");

        // Mismo lugar, distinto instante: no hay desplazamiento
        ListaPuntosGPS quieto;
        quieto.insertar(mk(0));
        quieto.insertar(mk(10));
        quieto.insertar(mk(20));
        probar(quieto.distanciaTotal(), 0.0, "Puntos en la misma posición: distanciaTotal 0.0");
        probar(quieto.velocidadMedia(), 0.0, "Puntos en la misma posición: velocidadMedia 0.0");
        probar(quieto.velocidadMaxima(), 0.0, "Puntos en la misma posición: velocidadMaxima 0.0");

        // Valores exactos usando solo diferencias de altitud (misma latitud y longitud)
        // Caso 1: el tramo más rápido es el primero. Tramos: 300 m/10 s, 200 m/20 s, 0 m/10 s
        ListaPuntosGPS t1;
        t1.insertar(PuntoGPS(0.0, 0.0, 0.0, 0, "T"));
        t1.insertar(PuntoGPS(0.0, 0.0, 300.0, 10, "T"));
        t1.insertar(PuntoGPS(0.0, 0.0, 100.0, 30, "T"));
        t1.insertar(PuntoGPS(0.0, 0.0, 100.0, 40, "T"));
        probar(t1.distanciaTotal(), 500.0, "Caso 1: distanciaTotal = 300 + 200 + 0 = 500");
        probar(t1.velocidadMedia(), 12.5, "Caso 1: velocidadMedia = 500 / 40 = 12.5");
        probar(t1.velocidadMaxima(), 30.0, "Caso 1: velocidadMaxima = 30 (tramo inicial)");

        // Caso 2: el tramo más rápido es el último
        ListaPuntosGPS t2;
        t2.insertar(PuntoGPS(0.0, 0.0, 0.0, 0, "T"));
        t2.insertar(PuntoGPS(0.0, 0.0, 10.0, 10, "T"));
        t2.insertar(PuntoGPS(0.0, 0.0, 110.0, 20, "T"));
        probar(t2.distanciaTotal(), 110.0, "Caso 2: distanciaTotal = 10 + 100 = 110");
        probar(t2.velocidadMedia(), 5.5, "Caso 2: velocidadMedia = 110 / 20 = 5.5");
        probar(t2.velocidadMaxima(), 10.0, "Caso 2: velocidadMaxima = 10 (último tramo)");

        // Caso 3: el tramo más rápido es uno intermedio
        ListaPuntosGPS t3;
        t3.insertar(PuntoGPS(0.0, 0.0, 0.0, 0, "T"));
        t3.insertar(PuntoGPS(0.0, 0.0, 10.0, 10, "T"));
        t3.insertar(PuntoGPS(0.0, 0.0, 210.0, 20, "T"));
        t3.insertar(PuntoGPS(0.0, 0.0, 220.0, 30, "T"));
        probar(t3.distanciaTotal(), 220.0, "Caso 3: distanciaTotal = 10 + 200 + 10 = 220");
        probar(t3.velocidadMedia(), 220.0 / 30.0, "Caso 3: velocidadMedia = 220 / 30");
        probar(t3.velocidadMaxima(), 20.0, "Caso 3: velocidadMaxima = 20 (tramo intermedio)");

        // El orden de inserción no influye
        ListaPuntosGPS t4;
        t4.insertar(PuntoGPS(0.0, 0.0, 220.0, 30, "T"));
        t4.insertar(PuntoGPS(0.0, 0.0, 0.0, 0, "T"));
        t4.insertar(PuntoGPS(0.0, 0.0, 210.0, 20, "T"));
        t4.insertar(PuntoGPS(0.0, 0.0, 10.0, 10, "T"));
        probar(t4.distanciaTotal(), 220.0, "Insertando desordenado: distanciaTotal igual");
        probar(t4.velocidadMaxima(), 20.0, "Insertando desordenado: velocidadMaxima igual");

        // Los resultados se actualizan tras modificar la lista
        t4.borrarPrimero();   // quedan 10, 20, 30
        probar(t4.distanciaTotal(), 210.0, "Tras borrarPrimero: distanciaTotal = 200 + 10");
        probar(t4.velocidadMedia(), 210.0 / 20.0, "Tras borrarPrimero: velocidadMedia = 210 / 20 (primer y último ts)");
        t4.borrarUltimo();    // quedan 10, 20
        probar(t4.distanciaTotal(), 200.0, "Tras borrarUltimo: distanciaTotal = 200");
        probar(t4.velocidadMaxima(), 20.0, "Tras borrarUltimo: velocidadMaxima = 20");
        t4.borrarUltimo();    // queda 1
        probar(t4.distanciaTotal(), 0.0, "Con un solo elemento: distanciaTotal 0.0");
        probar(t4.velocidadMaxima(), 0.0, "Con un solo elemento: velocidadMaxima 0.0");

        // La trayectoria se calcula sobre la lista, no sobre filtros
        ListaPuntosGPS f = t1.filtrarRangoTimestamp(0, 10);
        probar(f.distanciaTotal(), 300.0, "distanciaTotal sobre una lista filtrada (0..10) = 300");

        // Distancia sobre superficie real (la velocidad media depende solo de primer y último ts)
        ListaPuntosGPS real;
        PuntoGPS r1(38.3854, -0.5127, 120.5, 1000, "R");
        PuntoGPS r2(38.3900, -0.5100, 130.0, 1100, "R");
        PuntoGPS r3(38.4000, -0.5000, 140.0, 1400, "R");
        real.insertar(r1);
        real.insertar(r2);
        real.insertar(r3);
        double dEsp = r1.distancia3D(r2) + r2.distancia3D(r3);
        probar(real.distanciaTotal(), dEsp, "Trayectoria real: suma de distancias 3D entre consecutivos");
        probar(real.velocidadMedia(), dEsp / 400.0, "Trayectoria real: velocidadMedia = distanciaTotal / 400");
        double vMax = r1.velocidad(r2) > r2.velocidad(r3) ? r1.velocidad(r2) : r2.velocidad(r3);
        probar(real.velocidadMaxima(), vMax, "Trayectoria real: velocidadMaxima es la mayor de los tramos");
    }

    // 5.10 aCadena
    cout << "--- 5.10 aCadena ---" << endl;
    {
        ListaPuntosGPS vacia;
        probar(vacia.aCadena(), string(""), "aCadena de una lista vacía es la cadena vacía");

        ListaPuntosGPS uno;
        uno.insertar(mk(7, "UNO"));
        probar(uno.aCadena(), mk(7, "UNO").aCadena(), "aCadena de un elemento no lleva salto de línea");
        probar(uno.aCadena().find('\n') == string::npos, true, "aCadena de un elemento no contiene '\\n'");

        ListaPuntosGPS l;
        for (unsigned long i = 1; i <= 5; i++) l.insertar(mk(i));
        string s = l.aCadena();
        probar((int)count(s.begin(), s.end(), '\n'), 4, "aCadena de 5 elementos contiene exactamente 4 saltos de línea");
        probar(s[s.size() - 1] != '\n', true, "aCadena no termina en salto de línea");
        probar(s[0] == '(', true, "aCadena empieza por el primer punto");

        // Con datos extra
        ListaPuntosGPS e;
        PuntoGPS pe(38.3854, -0.5127, 120.5, 1758024000, "GARMIN-01", 2);
        pe.setDatoExtra(0, 12.5);
        pe.setDatoExtra(1, 3.4);
        e.insertar(mk(1758023000));
        e.insertar(pe);
        string esperado = mk(1758023000).aCadena() + "\n" +
                          "(38.385400,-0.512700,120.500000,1758024000) GARMIN-01 12.500000 3.400000";
        probar(e.aCadena(), esperado, "aCadena con datos extra respeta el formato del enunciado");

        // Tras modificar la lista
        l.borrarPrimero();
        l.borrarUltimo();
        string s2 = l.aCadena();
        probar((int)count(s2.begin(), s2.end(), '\n'), 2, "aCadena tras borrar primero y último: 3 elementos, 2 saltos");
    }

    // 5.11 getPuntoGPS devuelve una copia
    cout << "--- 5.11 getPuntoGPS y esVacia/longitud ---" << endl;
    {
        ListaPuntosGPS l;
        l.insertar(mk(1, "ORIG"));
        PuntoGPS copia = l.getPuntoGPS(l.begin());
        copia.setDispositivo("MODIFICADA");
        copia.setLatitud(10.0);
        probar(l.getPuntoGPS(l.begin()).getDispositivo(), string("ORIG"), "Modificar el punto devuelto por getPuntoGPS no cambia la lista (dispositivo)");
        probar(l.getPuntoGPS(l.begin()).getLatitud(), 38.0, "Modificar el punto devuelto por getPuntoGPS no cambia la lista (latitud)");

        // esVacia y longitud coherentes tras secuencia de operaciones
        ListaPuntosGPS s;
        probar(s.esVacia() && s.longitud() == 0, true, "Inicio: vacía y longitud 0");
        s.insertar(mk(1));
        s.insertar(mk(2));
        s.insertar(mk(2));          // duplicado
        probar(s.longitud(), 2, "Tras 3 insertar (1 duplicado): longitud 2");
        s.borrarPorTimestamp(1);
        s.borrarPorTimestamp(1);    // ya no existe
        probar(s.longitud(), 1, "Tras borrar 1 (dos intentos): longitud 1");
        s.borrarUltimo();
        probar(s.esVacia() && s.longitud() == 0, true, "Final: vacía y longitud 0");
    }

    // 5.12 Prueba de estrés
    cout << "--- 5.12 Estrés ---" << endl;
    {
        ListaPuntosGPS l;
        const unsigned long N = 2003; // número primo: i*7919 % N recorre valores distintos
        bool todoOk = true;
        for (unsigned long i = 1; i < N; i++) {
            todoOk = l.insertar(mk((i * 7919) % N + 1)) && todoOk;
        }
        probar(todoOk, true, "2002 inserciones desordenadas sin duplicados devuelven true");
        probar(l.longitud(), 2002, "Longitud 2002 tras las inserciones");
        probar(estrictamenteOrdenada(l), true, "La lista de 2002 elementos está estrictamente ordenada");
        probar(l.insertar(mk(1000)), false, "Insertar un timestamp existente en lista grande devuelve false");

        ListaPuntosGPS copia(l);
        probar(copia.longitud(), 2002, "Copia de lista grande: misma longitud");
        probar(copia.aCadena(), l.aCadena(), "Copia de lista grande: mismo contenido");

        // Borrar todos los pares por timestamp
        bool borradoOk = true;
        for (unsigned long t = 2; t <= N; t += 2) {
            borradoOk = copia.borrarPorTimestamp(t) && borradoOk;
        }
        probar(borradoOk, true, "Se borran todos los timestamps pares");
        probar(copia.longitud(), 1001, "Quedan 1001 elementos impares");
        probar(estrictamenteOrdenada(copia), true, "Tras los borrados la lista sigue ordenada");
        probar(l.longitud(), 2002, "La lista original no se ve afectada por los borrados de la copia");

        ListaPuntosGPS rango = l.filtrarRangoTimestamp(500, 599);
        probar(rango.longitud(), 100, "filtrarRangoTimestamp(500, 599) en lista grande devuelve 100 puntos");
    }

    cout << endl << "==========================================================" << endl;
    cout << "  Resumen: " << pruebasPasadas << " de " << pruebasTotales << " pruebas pasadas correctamente." << endl;
    cout << "==========================================================" << endl;

    return 0;
}