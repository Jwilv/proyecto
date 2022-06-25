
// librerias
#include <WiFi.h>
#include <FirebaseESP32.h>
//#include <wall-e app.h>
// instalar libreria de wifi en caso de ser necesario
//  instalar firebase esp32 client
/*
explicacion: P_(pin),M_(motor),DER(DERECHA),IZQ(IZQUIERDA),
*/

#define SSDI "XXX"                                            // NOMBRE DE RED
#define PASS "XXX"                                            // PASSWORD DE RED
#define URL "wall-e-app-default-rtdb.firebaseio.com"          // URL DE FIREBASE
#define SECRETBASE "zHQFVPY4ZS7Ay0wrOeiWTKbhXTySFRXwKasZ0H4f" // SECRET DE FIREBASE
// CONSTANTES DE DISTANCIA
#define DISTANCIA_MIN 15
// DECLARAMOS PINES DE ULTRA SONIDO
#define P_TRIG 10
#define PIN_ECO_ABAJO 14
#define PIN_ECO_ARRIBA 11
// PINES DEL MOTOR IZQUIERDO CON PUENTE H
#define P_M_IZQ_IN1 7
#define P_M_IZQ_IN2 6
#define P_M_IZQ_ENA 8
// PINES DEL MOTOR DERECHO CON PUENTE H
#define P_M_DER_IN3 5
#define P_M_DER_IN4 4
#define P_M_DER_ENB 3
// pin servo
#define SERVO 14
// pwm
int frecuencia = 5000; // frecuencia del pwm
int canal0 = 0;        // canal pwm para el servo motor
int canal1 = 1;        // canal pwm para el motor
int Rmotor = 100;      // resolucion motor
// fire base
FirebaseData myFirebaseData;

// INFORMACION DE DISTNCIA RECOLECTADA POR LOS Sensores
int sensor_arriba;
int sensor_abajo;
int tiempoAc = 0;
int periodo4 = 4000;
int periodo2 = 2000;
int pin_motores[6] =
    {P_M_IZQ_ENA, P_M_IZQ_IN1, P_M_IZQ_IN2,
     P_M_DER_ENB, P_M_DER_IN3, P_M_DER_IN4};
enum ESTADOWALLE  {
    ESTADO_BAILE = 75,
    ESTADO_RECOLECTAR = 100,
    ESTADO_QUIETO = 170,
    ESTADO_TROMPO = 150,
    ESTADO_CONTROL_ADELANTE = 550,
    ESTADO_CONTROL_ATRAS = 600,
    ESTADO_CONTROL_IZQUIERDA = 700,
    ESTADO_CONTROL_DERECHA = 650
};
int valor_doblar;
void DeclaracionPwm()
{
    ledcSetup(canal1, frecuencia, Rmotor);
   ledcSetup(canal1, frecuencia, Rmotor);
    ledcSetup(canal0, frecuencia, Rmotor);
    ledcAttachPin(P_M_IZQ_ENA, canal1);
    ledcAttachPin(P_M_DER_ENB, canal1);
    ledcAttachPin(SERVO, canal0);
}
void asignacionMotores(int pin_motores_v[])
{
    for (int fila = 0; fila < 5; fila++)
    {
        int pinMotor = pin_motores_v[fila];
        pinMode(pinMotor, OUTPUT);
    }
}
// INICIO DE FUNCIONES
void pinesUltrasonido(int trig, int eco_abajo, int eco_arriba)
{
    pinMode(trig, OUTPUT);
    pinMode(eco_abajo, INPUT);
    pinMode(eco_arriba, INPUT);
}
void setup()
{   DeclaracionPwm();
    // pinMode DE ULTRASONIDOS
    pinesUltrasonido(P_TRIG, PIN_ECO_ABAJO, PIN_ECO_ARRIBA);
    // pin mode de motores
    asignacionMotores(pin_motores);
    // CONECTAR A WIFI
     WiFi.begin(SSDI,PASS);
    Firebase.begin(URL,SECRETBASE);
    Firebase.reconnectWiFi(true);
}
// FUNCIOS DE RECOLECCION DE DATOS DE LOS Sensores
int Sensores(int PIN_TRIG, int PIN_ECO)
{
    int distancia;
    int duracion;
    int periodo = 20;
    unsigned long tiempo_actual;
    if (millis() > tiempo_actual + periodo)
    {
        tiempo_actual = millis();
        // SENSOR
        digitalWrite(PIN_TRIG, HIGH);
        delayMicroseconds(15);
        digitalWrite(PIN_TRIG, LOW);
        duracion = pulseIn(PIN_ECO, HIGH);
        distancia = duracion / 58.2;
        return distancia;
    }
}
// FUNCIOS PARA MOVER LOS MOTORES PARA ADELANTE
void motoresAdelante(int motorEna, int motorIn1, int motorIn2, int motorEna_v, int motorIn1_v, int motorIn2_v)
{
    ledcWrite(motorEna, Rmotor);
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn2, HIGH);

    ledcWrite(motorEna_v, Rmotor);
    digitalWrite(motorIn1_v, LOW);
    digitalWrite(motorIn2_v, HIGH);
}
void motoresAtras(int motorEna, int motorIn1, int motorIn2, int motorEna_v, int motorIn1_v, int motorIn2_v)
{
    ledcWrite(motorEna, Rmotor);
    digitalWrite(motorIn1, HIGH);
    digitalWrite(motorIn2, LOW);

    ledcWrite(motorEna_v, Rmotor);
    digitalWrite(motorIn1_v, HIGH);
    digitalWrite(motorIn2_v, LOW);
}
// FUNCIOS DE DOBLAR A LA DERECHA INVIRTIENDO UN MOTOR FUNCION MOTOR IZQUIERDO
void giroizquierda(int motorEna, int motorIn1, int motorIn2, int motorEna_v, int motorIn1_v, int motorIn2_v)
{
    ledcWrite(motorEna, Rmotor);
    digitalWrite(motorIn1, HIGH);
    digitalWrite(motorIn2, LOW);

    ledcWrite(motorEna_v, Rmotor);
    digitalWrite(motorIn1_v, LOW);
    digitalWrite(motorIn2_v, HIGH);
}
void giroderecha(int motorEna, int motorIn1, int motorIn2, int motorEna_v, int motorIn1_v, int motorIn2_v)
{
    ledcWrite(motorEna, Rmotor);
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn2, HIGH);

    ledcWrite(motorEna_v, Rmotor);
    digitalWrite(motorIn1_v, HIGH);
    digitalWrite(motorIn2_v, LOW);
}
// FUNCIOn DE DOBLAR A LA derecha INVIRTIENDO UN MOTOR FUNCION DE MOTOR DERECHO
// RECOPILACION DE FUNCIONES PARA SU MOVIMIENTO AUTONOMO
void estadoAutonomo()
{
    // DISTANCIA DE RECOLECCION DE DATOS
    int sensor_arriba = Sensores(P_TRIG, PIN_ECO_ARRIBA);
    int sensor_abajo = Sensores(P_TRIG, PIN_ECO_ABAJO);
    // CONDICIONES PARA MOVIMIENTO AUTONOMO
    bool moverse_adelante = sensor_arriba > DISTANCIA_MIN && sensor_abajo > DISTANCIA_MIN;
    bool doblar = sensor_arriba <= DISTANCIA_MIN;
    if (moverse_adelante)
    {
        motoresAdelante(P_M_IZQ_ENA, P_M_IZQ_IN1, P_M_IZQ_IN2, P_M_DER_ENB, P_M_DER_IN3, P_M_DER_IN4);
    }
    else if (doblar) // else if de giro tomando en cuenta el sensor de arriba
    {

        if (!valor_doblar++ % 2)
        {
            giroizquierda(P_M_IZQ_ENA, P_M_IZQ_IN1, P_M_IZQ_IN2, P_M_DER_ENB, P_M_DER_IN3, P_M_DER_IN4);
            valor_doblar++;
        }
        else
        {
            giroderecha(P_M_IZQ_ENA, P_M_IZQ_IN1, P_M_IZQ_IN2, P_M_DER_ENB, P_M_DER_IN3, P_M_DER_IN4);

            valor_doblar++;
        }
    }
}
void bailar()
{
    for (int angulo = 0; angulo <= 255; angulo++)
    {
        ledcWrite(canal0, angulo);
    }
    for (int angulo = 0; angulo <= 255; angulo--)
    {
        ledcWrite(canal0, angulo);
    }
    giroderecha(P_M_IZQ_ENA, P_M_IZQ_IN1, P_M_IZQ_IN2, P_M_DER_ENB, P_M_DER_IN3, P_M_DER_IN4);

}

int lecturaComandos(String cmd)
{
  Firebase.get(myFirebaseData, "/cmd");
    return myFirebaseData.intData(); ;
}

void standby()
{
    Firebase.set(myFirebaseData, "/COMANDOS", 0);
}

void movimiento(int estado)
{
    switch (estado)
    {
    case ESTADO_CONTROL_ADELANTE:
    {
        motoresAdelante(P_M_IZQ_ENA, P_M_IZQ_IN1, P_M_IZQ_IN2, P_M_DER_ENB, P_M_DER_IN3, P_M_DER_IN4);
        if (millis() - tiempoAc > periodo2)
        {
            tiempoAc = millis();
            standby();
        }
    }
    case ESTADO_CONTROL_ATRAS:
    {
        motoresAtras(P_M_IZQ_ENA, P_M_IZQ_IN1, P_M_IZQ_IN2, P_M_DER_ENB, P_M_DER_IN3, P_M_DER_IN4);
        if (millis() - tiempoAc > periodo2)
        {
            tiempoAc = millis();
            standby();
        }
    }
    case ESTADO_CONTROL_IZQUIERDA:
    {
        giroizquierda(P_M_IZQ_ENA, P_M_IZQ_IN1, P_M_IZQ_IN2, P_M_DER_ENB, P_M_DER_IN3, P_M_DER_IN4);
        if (millis() - tiempoAc > periodo2)
        {
            tiempoAc = millis();
            standby();
        }
    }
    case ESTADO_CONTROL_DERECHA:
    {
        giroderecha(P_M_IZQ_ENA, P_M_IZQ_IN1, P_M_IZQ_IN2, P_M_DER_ENB, P_M_DER_IN3, P_M_DER_IN4);
        if (millis() - tiempoAc > periodo2)
        {
            tiempoAc = millis();
            standby();
        }
    }
    case ESTADO_BAILE:
    {
        bailar();
        if (millis() - tiempoAc > periodo4)
        {
            tiempoAc = millis();
            standby();
        }
        break;
    }
    case ESTADO_QUIETO:
    {
        standby();
        break;
    }
    case ESTADO_TROMPO:
    {
        giroderecha(P_M_IZQ_ENA, P_M_IZQ_IN1, P_M_IZQ_IN2, P_M_DER_ENB, P_M_DER_IN3, P_M_DER_IN4);
        if (millis() - tiempoAc > periodo4)
        {
            tiempoAc = millis();
            standby();
        }
        break;
    }
    case ESTADO_RECOLECTAR:
    {
        estadoAutonomo();
        break;
    }
    }
}

// LOOP
void loop()
{
    int cmdMov = lecturaComandos("COMANDOS");
    movimiento(cmdMov);
}
