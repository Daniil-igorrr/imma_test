#ifndef PUNTOGPS_H
#define PUNTOGPS_H

#include<string>

using namespace std;
class PuntoGPS{

private:
const double RADIO_TIERRA = 6371000.0; 
const double PI = 3.14159265358979323846;
const double EPSILON = 1e-6;
double latitud;
double altitud;
double longitud;
unsigned long tiempo;
string dispositivo;


unsigned int num_datos_extra;
double *datos_extra;  

bool validar(double,double,double);

public:
//Forma canónica

PuntoGPS();
PuntoGPS(double, double, double, unsigned long, string);
PuntoGPS(double, double, double, unsigned long, string, unsigned int);
PuntoGPS(const PuntoGPS &);
~PuntoGPS();
PuntoGPS& operator=(const PuntoGPS &);

//Getters y setters
double getLatitud() const;
bool setLatitud(double);
double getLongitud() const;
bool setLongitud(double);
double getAltitud() const;
bool setAltitud(double);
unsigned long getTimestamp() const;
void setTimestamp(unsigned long);
string getDispositivo() const;
void setDispositivo(const string&);
int getNumDatosExtra() const; 
double getDatoExtra(int) const;
bool setDatoExtra(int, double);


//Sobrecarga de operadores
bool operator<(const PuntoGPS&) const; //compara timestamp
bool operator>(const PuntoGPS&) const; //compara timestamp
bool operator==(const PuntoGPS&) const; //Si latitud, longitud, altitud y tiempo son iguales
bool operator!=(const PuntoGPS&) const;

//distanciaSuperficie: https://en.wikipedia.org/wiki/Haversine_formula
double distanciaSuperficie(const PuntoGPS& otro) const;

//distancia3D: distancia Euclidea usando la diferencia en altitud y en superficie
double distancia3D(const PuntoGPS& otro) const;

//Velocidad (3D)
double velocidad(const PuntoGPS& otro) const;

//Representación
string aCadena() const;

};

#endif
