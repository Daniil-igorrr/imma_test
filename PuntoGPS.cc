#include "PuntoGPS.h"
#include <cmath>



bool PuntoGPS::validar(double lat, double lon, double alt){
    return (lat >= -90.0 && lat <= 90.0 && lon >= -180.0 && lon <= 180.0 && alt >= 0.0);
}



PuntoGPS::PuntoGPS(){
  latitud = 0;
  altitud = 0;
  longitud = 0;
  tiempo = 0;
  dispositivo = "";
  num_datos_extra=0;


  datos_extra = nullptr;
}

PuntoGPS::PuntoGPS(double lat, double lon, double alt, unsigned long t, string disp){

        latitud = 0;
        altitud = 0;
        longitud = 0;
        tiempo = 0;
        dispositivo = "";
        
        num_datos_extra=0;

        datos_extra = nullptr;

    if (validar(lat, lon, alt)){
        latitud = lat;
        longitud = lon;
        altitud = alt;
        tiempo = t;
        dispositivo = disp;
    }
}

PuntoGPS::PuntoGPS(double lat, double lon, double alt, unsigned long t, string disp, unsigned int num){
        latitud = 0;
        altitud = 0;
        longitud = 0;
        tiempo = 0;
        dispositivo = "";
        num_datos_extra=0;
        datos_extra=nullptr;

        if (validar(lat, lon, alt)){
        latitud = lat;
        longitud = lon;
        altitud = alt;
        tiempo = t;
        dispositivo = disp;
        num_datos_extra = num;
        
        if (num_datos_extra > 0){
            datos_extra = new double[num_datos_extra];
            for (unsigned int i = 0; i < num_datos_extra; i++){
                datos_extra[i] = 0.0;
            }
        }
    }
}

PuntoGPS::PuntoGPS(const PuntoGPS &otro){

        latitud = otro.latitud;
        altitud = otro.altitud;
        longitud = otro.longitud;
        tiempo = otro.tiempo;
        dispositivo = otro.dispositivo;
        num_datos_extra=otro.num_datos_extra;
        
        if (num_datos_extra==0){
          datos_extra=nullptr;
        }else{
          datos_extra = new double [num_datos_extra];
          for (unsigned int i=0;i<=num_datos_extra-1;i++){
            datos_extra[i]=otro.datos_extra[i];
          }
        }

}

PuntoGPS::~PuntoGPS(){
  if (datos_extra != nullptr){
    delete [] datos_extra;
    datos_extra=nullptr;
  }

}

PuntoGPS &PuntoGPS::operator=(const PuntoGPS &otro){
    if (this != &otro){
        latitud = otro.latitud;
        altitud = otro.altitud;
        longitud = otro.longitud;
        tiempo = otro.tiempo;
        dispositivo = otro.dispositivo;
        num_datos_extra=otro.num_datos_extra;
        
        if (num_datos_extra==0){
          delete [] datos_extra;
          datos_extra=nullptr;
        }else{
          delete [] datos_extra;
          datos_extra = new double [num_datos_extra];
          for (unsigned int i=0;i<=num_datos_extra-1;i++){
            datos_extra[i]=otro.datos_extra[i];
          }
        }

    }
    return *this;
}

double PuntoGPS::getLatitud() const { 
  return latitud; 
}

bool PuntoGPS::setLatitud(double lat){
    if (!validar(lat, longitud, altitud)){
    return false;
  }
    latitud = lat;
    return true;
}

double PuntoGPS::getLongitud() const {
   return longitud; 
  }

bool PuntoGPS::setLongitud(double lon){
    if (!validar(latitud, lon, altitud)){ 
      return false;
    }
    longitud = lon;
    return true;
}

double PuntoGPS::getAltitud() const { 
  return altitud; 
}

bool PuntoGPS::setAltitud(double alt){
    if (!validar(latitud, longitud, alt)) {
      return false;
    }
    altitud = alt;
    return true;
}

unsigned long PuntoGPS::getTimestamp() const { 
  return tiempo; 
}

void PuntoGPS::setTimestamp(unsigned long t){ 
  tiempo = t; 
}

string PuntoGPS::getDispositivo() const { 
  return dispositivo; 
}

void PuntoGPS::setDispositivo(const string &disp){ 
  dispositivo = disp; 
}


int PuntoGPS::getNumDatosExtra() const{
  return num_datos_extra;
}

double PuntoGPS::getDatoExtra(int i) const{
    
  if (i < 0 || i >= (int)num_datos_extra) return 0.0;
  return datos_extra[i];
}


bool PuntoGPS::setDatoExtra(int i, double v) {
    if (i < 0 || i >= (int)num_datos_extra){
      return false;
    } else{
      datos_extra[i] = v;
      return true;
    }
    
}

bool PuntoGPS::operator<(const PuntoGPS &otro) const {
    return tiempo < otro.tiempo;
}

bool PuntoGPS::operator>(const PuntoGPS &otro) const {
    return tiempo > otro.tiempo;
}


bool PuntoGPS::operator==(const PuntoGPS &otro) const {
    return fabs(latitud - otro.latitud) < EPSILON &&
           fabs(longitud - otro.longitud) < EPSILON &&
           fabs(altitud - otro.altitud) < EPSILON &&
           tiempo == otro.tiempo;
}

bool PuntoGPS::operator!=(const PuntoGPS &otro) const {
    return fabs(latitud - otro.latitud) >= EPSILON ||
           fabs(longitud - otro.longitud) >= EPSILON ||
           fabs(altitud - otro.altitud) >= EPSILON ||
           tiempo != otro.tiempo;
}

double PuntoGPS::distanciaSuperficie(const PuntoGPS &otro) const{
    double tlatitud = ((otro.latitud-latitud)*PI)/180;
    double tlon = ((otro.longitud-longitud)*PI)/180;
    double latitud1_rad = (latitud*PI)/180;
    double latitud2_rad = (otro.latitud*PI)/180;

    double a=((1-cos(tlatitud))/2)+(cos(latitud1_rad)*cos(latitud2_rad)*((1-cos(tlon))/2));
    if (a > 1.0){
    a = 1.0;
    }
    double res=2*RADIO_TIERRA*(asin(sqrt(a)));

    return res;
}




double PuntoGPS::distancia3D(const PuntoGPS& otro) const{
    double dsup = distanciaSuperficie(otro);
    double daltitud = (otro.altitud-altitud);

  

    double res = sqrt(pow(dsup,2)+pow(daltitud,2));



        return res;}

double PuntoGPS::velocidad(const PuntoGPS& otro) const{
 
 double d3 = distancia3D(otro);
    
  
  if (otro.tiempo<=tiempo){
    return -1.0;
  } else{
    
    return (d3/(otro.tiempo - tiempo));
  }
}


string PuntoGPS:: aCadena() const{
  string concat;
  if (datos_extra!=nullptr){

  concat=" ";
  
  
  for(unsigned int i=0;num_datos_extra-1>=i;i++){
    if (num_datos_extra-1==i){
      concat=concat + to_string(datos_extra[i]);
    }else{
      concat=concat + to_string(datos_extra[i])+" ";
    }
    
  }
  
}else{
  concat="";
}
  string str= "("+to_string(latitud)+","+to_string(longitud)+","+to_string(altitud)+","+to_string(tiempo)+") "+dispositivo+concat;
  return str;


}

