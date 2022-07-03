
// librerias
#include <WiFi.h>


#define ssid "Matías"                                            // NOMBRE DE RED
#define pass "valentina"                                            // passWORD DE RED
#define URL "wall-e-app-default-rtdb.firebaseio.com"          // URL DE FIREBASE
#define SECRETBASE "zHQFVPY4ZS7Ay0wrOeiWTKbhXTySFRXwKasZ0H4f" // SECRET DE FIREBASE
// CONSTANTES DE DISTANCIA
/*
// fire base
FirebaseData myFirebaseData;

// INFORMACION DE DISTNCIA RECOLECTADA POR LOS Sensores

int lecturaComandos()
{
    Firebase.get(myFirebaseData,"/COMANDOS");
    return myFirebaseData.intData(); // aca tenes 2doble ;<-------------------------------------------------------------------
}

void standby()
{
    Firebase.set(myFirebaseData, "/COMANDOS", 0);
}
*/

void setup() //<--------------------------------------- Esto deberia estar abajo de todo con el Loop
{
    Serial.begin(115200);
    // CONECTAR A WIFI
    WiFi.begin(ssid, pass);
    Serial.println("conectando...");
    while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(500);

    }
    Serial.println("CONECTADO");
    //Firebase.begin(URL, SECRETBASE);
   // Firebase.reconnectWiFi(true);
}

// LOOP
void loop()
{
  //  int cmdMov = lecturaComandos();
   // Serial.println(cmdMov);
}
