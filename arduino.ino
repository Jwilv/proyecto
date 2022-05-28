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
    ESTADO_INICIAL,
    ESTADO_BAILE,
    ESTADO_QUIETO = 0,
    ESTADO_CONTROL_ADELANTE,
    ESTADO_CONTROL_ATRAS,
    ESTADO_CONTROL_IZQUIERDA,
    ESTADO_CONTROL_DERECHA};
enum estadoOrden = {
    COMANDOS = 250,
    CONTROL = 500};
int estadoRobot;
int estadoComando;
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
void asignacionMotores(int pin_motores_v)
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
    int periodo 20;
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
void motoresAdelante(int motorEna, int motorIn1, int motorIn2)
{
    analogWrite(motorEna, 100);
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn2, HIGH);
}
// FUNCIOS DE DOBLAR A LA DERECHA INVIRTIENDO UN MOTOR FUNCION MOTOR IZQUIERDO
void motoresAtras(int motorEna, int motorIn1, int motorIn2)
{
    analogWrite(motorEna, 100);
    digitalWrite(motorIn1, HIGH);
    digitalWrite(motorIn2, LOW);
}
// FUNCIOn DE DOBLAR A LA derecha INVIRTIENDO UN MOTOR FUNCION DE MOTOR DERECHO
// RECOPILACION DE FUNCIONES PARA SU MOVIMIENTO AUTONOMO
void movimiento_autonomo()
{
    // DISTANCIA DE RECOLECCION DE DATOS
    int sensor_arriba = sensores(PIN_TRIG_ARRIBA, PIN_ECO_ARRIBA);
    int sensor_abajo = sensores(PIN_TRIG_ABAJO, PIN_ECO_ABAJO);
    // CONDICIONES PARA MOVIMIENTO AUTONOMO
    bool moverse_adelante = sensor_arriba <= DISTANCIA_MAX && sensor_abajo <= DISTANCIA_MAX_ABAJO;
    bool doblar = sensor_arriba <= DISTANCIA_MIN;
    if (moverse_adelante)
    {
        motoresAdelante(PIN_MOTOR_DERECHO_ENA, PIN_MOTOR_DERECHO_IN1, PIN_MOTOR_DERECHO_IN2);
        motoresAdelante(PIN_MOTOR_IZQUIERDO_ENA, PIN_MOTOR_IZQUIERDO_IN1, PIN_MOTOR_IZQUIERDO_IN2);
    }
    else if (doblar) // else if de giro tomando en cuenta el sensor de arriba
    {
        int valor_doblar;
        if (!valor_doblar % 2)
        {
            motoresAdelante(PIN_MOTOR_IZQUIERDO_ENA, PIN_MOTOR_IZQUIERDO_IN1, PIN_MOTOR_IZQUIERDO_IN2);
            motoresAtras(PIN_MOTOR_DERECHO_ENA, PIN_MOTOR_DERECHO_IN1, PIN_MOTOR_DERECHO_IN2);
            valor_doblar++;
        }
        else
        {
            motoresAtras(PIN_MOTOR_IZQUIERDO_ENA, PIN_MOTOR_IZQUIERDO_IN1, PIN_MOTOR_IZQUIERDO_IN2);
            motoresAdelante(PIN_MOTOR_DERECHO_ENA, PIN_MOTOR_DERECHO_IN1, PIN_MOTOR_DERECHO_IN2);
            valor_doblar++;
        }
    }
}
void lecturaEstado()
{
    // lectura de estado de la base de datos
    Firebase.get(firebase, "/CATEGORIA", estadoRobot);
    CATEGORIA = myFireBaseData.IntData;
    bool comandos = CATEGORIA == COMANDOS;
    bool control = CATEGORIA == CONTROL;
    if (comandos)estadoRobot = COMANDOS;
    else if (control)estadoRobot = CONTROL;
    else estadoRobot = ESTADO_QUIETO;
}
void resultadoEstado(bool estado, int valor_estado)
{
    if (estado)
        estadoComando = valor_estado;
}
void lecturaComandos()
{
    Firebase.get(firebase, "/COMANDOS", estadoRobot);
    COMANDOS = myFireBaseData.IntData;
    bool baile = ESTADO_BAILE == COMANDOS;
    bool quieto = ESTADO_QUIETO == COMANDOS;
    bool trompo = ESTADO_TROMPO == COMANDOS;
    bool basura = ESTADO_BASURA == COMANDOS;
    resultadoEstado(baile, ESTADO_BAILE);
    resultadoEstado(quieto, ESTADO_QUIETO);
    resultadoEstado(trompo, ESTADO_TROMPO);
    resultadoEstado(basura, ESTADO_BASURA);

}
    // FUNCIONE DE CONTROL DE MOVIMIENTO
    controlmovimiento()
{ // CONTROL DE MOVIMIENTO
    lecturaEstado();
    lecturaComandos();
    switch (estadoRobot)
    {
    case COMANDOS:
    {
        switch ()
        {
        case ESTADO_INICIAL:
        {
            movimiento_autonomo();
            break;
        }
        case ESTADO_BAILE:
        {
            comandoBaile();
            break;
        }
        case ESTADO_QUIETO:
        {
            comandoQuieto();
            break;
        }
        }
    }
    case CONTROL:
    {
    case ESTADO_CONTROL_ADELANTE:
    {
        controlAdelante();
        break;
    }
    case ESTADO_CONTROL_ATRAS:
    {
        controlAtras();
        break;
    }
    case ESTADO_CONTROL_IZQUIERDA:
    {
        controlIzquierda();
        break;
    }
    case ESTADO_CONTROL_DERECHA:
    {
        controlDerecha();
        break;
    }
    }
    }
}
// LOOP
void loop()
{
    cotrolmovimiento();
}
