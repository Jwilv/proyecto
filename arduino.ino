// librerias
#include <wifi.h>
#include <wall-e app.h>
#include <FirebaseESP32.h>
// instalar libreria de wifi en caso de ser necesario
//  instalar firebase esp32 client

#define SSDI "XXX"                                            // NOMBRE DE RED
#define PASS "XXX"                                            // PASSWORD DE RED
#define URL "wall-e-app-default-rtdb.firebaseio.com"          // URL DE FIREBASE
#define secretbase "zHQFVPY4ZS7Ay0wrOeiWTKbhXTySFRXwKasZ0H4f" // SECRET DE FIREBASE
// CONSTANTES DE DISTANCIA
#define DISTANCIA_MIN 10
#define DISTANCIA_MAX_ABAJO 15
#define DISTANCIA_MAX 400
// DECLARAMOS PINES DE ULTRA SONIDO
#define PIN_TRIG_ABAJO 10
#define PIN_ECO_ABAJO 14
#define PIN_TRIG_ARRIBA 11
#define PIN_ECO_ARRIBA 11
// PINES DEL MOTOR IZQUIERDO CON PUENTE H
#define PIN_MOTOR_IZQUIERDO_IN1 2
#define PIN_MOTOR_IZQUIERDO_IN2 3
#define PIN_MOTOR_IZQUIERDO_ENA 5
// PINES DEL MOTOR DERECHO CON PUENTE H
#define PIN_MOTOR_DERECHO_IN1 6
#define PIN_MOTOR_DERECHO_IN2 7
#define PIN_MOTOR_DERECHO_ENA 8
// fire base
firebaseData myFirebaseData;

// INFORMACION DE DISTNCIA RECOLECTADA POR LOS sensores
int sensor_arriba;
int sensor_abajo;
int pin_motores[1] = {
    {PIN_MOTOR_IZQUIERDO_ENA, PIN_MOTOR_IZQUIERDO_IN1, PIN_MOTOR_IZQUIERDO_IN2,
     PIN_MOTOR_DERECHO_ENA, PIN_MOTOR_DERECHO_IN1, PIN_MOTOR_DERECHO_IN2}};
enum estadoWalle = {
    ESTADO_BAILE = 75,
    ESTADO_RECOLECTAR = 100,
    ESTADO_QUIETO = 170,
    ESTADO_TROMPO = 150,
    ESTADO_CONTROL_ADELANTE = 550,
    ESTADO_CONTROL_ATRAS = 600,
    ESTADO_CONTROL_IZQUIERDA = 700,
    ESTADO_CONTROL_DERECHA = 650,
    ACTIVAR_CAMARA = 750,
    DESACTIVAR_CAMARA = 0,
};
enum estadoOrden = {
    COMANDOS = 250,
    CONTROL = 500};
int estadoRobot;
int estadoComando;
int estadoCAmara;
// tiempo de millis
int tiempoWifi = 0;
int periodoWifi = 500;
void conexionWifiBaseData(int ssdi, int pass, int url, int secret)
{
    // CONECTAR A WIFI
    WiFi.begin(ssdi, pass);
    firebasae.begin(url, secret);
    firebase.reconnectWiFi(true);
}
void asignacionMotores(int pin_motores_v[])
{
    // pinMode(ECHO, INPUT);
    // pinMode(Trigger,salida);
    for (int fila = 0; fila < 9; fila++)
    {
        int pinMotor = pin_motores_v[fila];
        pinMode(pinMotor, INPUT);
    }
}
// INICIO DE FUNCIONES
void pinesUltrasonido(int trig_abajo, int eco_abajo, int trig_arriba, int eco_arriba)
{
    pinMode(trig_abajo, OUTPUT);
    pinMode(eco_abajo, INPUT);
    pinMode(trig_arriba, OUTPUT);
    pinMode(eco_arriba, INPUT);
}
void setup()
{
    // pinMode DE ULTRASONIDOS
    pinesUltrasonido(PIN_TRIG_ABAJO, PIN_ECO_ABAJO, PIN_TRIG_ARRIBA, PIN_ECO_ARRIBA);
    // pin mode de motores
    asignacionMotores(pin_motores);
    // CONECTAR A WIFI
    conexionWifiBaseData(SSDI, PASS, URL, secretbase);
}
// FUNCIOS DE RECOLECCION DE DATOS DE LOS sensores
int sensores(int PIN_TRIG, int PIN_ECO)
{
    int distancia;
    int duracion;
    int periodo = 20;
    unsigned long tiempo_actual;
    if (millis() > tiempo_actual + periodo)
    {
        // SENSOR
        digitalWrite(PIN_TRIG, HIGH);
        digitalWrite(PIN_TRIG, LOW);
        duracion = pulseIn(PIN_ECO, HIGH);
        distancia = duracion / 58.2;
        return distancia;
    }
}
// FUNCIOS PARA MOVER LOS MOTORES PARA ADELANTE
void motoresAdelante(int motorEna, int motorIn1, int motorIn2, int motorEna_v, int motorIn1_v, int motorIn2_v)
{
    analogWrite(motorEna, 100);
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn2, HIGH);

    analogWrite(motorEna_v, 100);
    digitalWrite(motorIn1_v, LOW);
    digitalWrite(motorIn2_v, HIGH);
}
// FUNCIOS DE DOBLAR A LA DERECHA INVIRTIENDO UN MOTOR FUNCION MOTOR IZQUIERDO
void giroizquierda(int motorEna, int motorIn1, int motorIn2, int motorEna_v, int motorIn1_v, int motorIn2_v)
{
    analogWrite(motorEna, 100);
    digitalWrite(motorIn1, HIGH);
    digitalWrite(motorIn2, LOW);

    analogWrite(motorEna_v, 100);
    digitalWrite(motorIn1_v, LOW);
    digitalWrite(motorIn2_v, HIGH);

    
}
void giroderecha(int motorEna, int motorIn1, int motorIn2, int motorEna_v, int motorIn1_v, int motorIn2_v)
{
    analogWrite(motorEna, 100);
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn2, HIGH);

    analogWrite(motorEna_v, 100);
    digitalWrite(motorIn1_v, HIGH);
    digitalWrite(motorIn2_v, LOW);
}
// FUNCIOn DE DOBLAR A LA derecha INVIRTIENDO UN MOTOR FUNCION DE MOTOR DERECHO
// RECOPILACION DE FUNCIONES PARA SU MOVIMIENTO AUTONOMO
void estadoAutonomo()
{
    // DISTANCIA DE RECOLECCION DE DATOS
    int sensor_arriba = sensores(PIN_TRIG_ARRIBA, PIN_ECO_ARRIBA);
    int sensor_abajo = sensores(PIN_TRIG_ABAJO, PIN_ECO_ABAJO);
    // CONDICIONES PARA MOVIMIENTO AUTONOMO
    bool moverse_adelante = sensor_arriba <= DISTANCIA_MAX && sensor_abajo <= DISTANCIA_MAX_ABAJO;
    bool doblar = sensor_arriba <= DISTANCIA_MIN;
    if (moverse_adelante)
    {
        motoresAdelante(PIN_MOTOR_IZQUIERDO_ENA, PIN_MOTOR_IZQUIERDO_IN1, PIN_MOTOR_IZQUIERDO_IN2,PIN_MOTOR_DERECHO_ENA, PIN_MOTOR_DERECHO_IN1, PIN_MOTOR_DERECHO_IN2);
    }
    else if (doblar) // else if de giro tomando en cuenta el sensor de arriba
    {
        int valor_doblar;
        if (!valor_doblar % 2)
        {
            giroizquierda(PIN_MOTOR_IZQUIERDO_ENA, PIN_MOTOR_IZQUIERDO_IN1, PIN_MOTOR_IZQUIERDO_IN2, PIN_MOTOR_DERECHO_ENA, PIN_MOTOR_DERECHO_IN1, PIN_MOTOR_DERECHO_IN2);
            valor_doblar++;
        }
        else
        {
             giroderecha(PIN_MOTOR_IZQUIERDO_ENA, PIN_MOTOR_IZQUIERDO_IN1, PIN_MOTOR_IZQUIERDO_IN2, PIN_MOTOR_DERECHO_ENA, PIN_MOTOR_DERECHO_IN1, PIN_MOTOR_DERECHO_IN2);

            valor_doblar++;
        }
    }
}
int lecturaCamara(int estado)
{
    // lectura de estado de la camara
    Firebase.get(firebase, "/CAMARA");
    e = myFireBaseData.IntData;
    switch (estado)
    {
    case ACTIVAR_CAMARA:
    {
        return ACTIVAR_CAMARA;
    }
    case DESACTIVAR_CAMARA:
    {
        return DESACTIVAR_CAMARA;
    }
    }
}

int lecturaComandos()
{
    Firebase.get(firebase, "/COMANDOS");
    return COMANDOS = myFireBaseData.IntData;
}

void estandby()
{
}
void camara(int estado)
{
    switch (estado)
    {
    case ACTIVAR_CAMARA:
    {
        activarCamara();
        break;
    }
    case DESACTIVAR_CAMARA:
    {
        desactivarCamara();
        break;
    }
    }
    }

void movimiento(int estado)
{
    switch (estado)
    {
    case ESTADO_CONTROL_ADELANTE:
    {
        avanzar();
        estandby();
    }
    case ESTADO_CONTROL_ATRAS:
    {
        retroceder();
        estandby();
    }
    case ESTADO_CONTROL_IZQUIERDA:
    {
        doblarIzquierda();
        estandby();
    }
    case ESTADO_CONTROL_DERECHA:
    {
        doblarDerecha();
        estandby();
    }
    case ESTADO_BAILE:
    {
        bailar();
        estandby();
        break;
    }
    case ESTADO_QUIETO:
    {
        quieto();
        estandby();
        break;
    }
    case ESTADO_TROMPO:
    {
        trompo();
        estandby();
        break;
    }
    case ESTADO_RECOLECTAR:
    {
        estadoAutonomo();
        estandby();
        break;
    }
    }
}

// LOOP
void loop()
{
}
