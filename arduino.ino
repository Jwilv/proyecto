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
// INFORMACION DE DISTNCIA RECOLECTADA POR LOS sensores
int sensor_arriba;
int sensor_abajo;
int estadoDeorden = 0;
int pin_motores[2][3] = {
    {PIN_MOTOR_IZQUIERDO_ENA, PIN_MOTOR_IZQUIERDO_IN1, PIN_MOTOR_IZQUIERDO_IN2},
    {PIN_MOTOR_DERECHO_ENA, PIN_MOTOR_DERECHO_IN1, PIN_MOTOR_DERECHO_IN2}};
enum estadoWalle = {
    ESTADO_INICIAL,
    ESTADO_COMANDOS,
    ESTADO_COMANDOS_VOZ,
    ESTADO_CONTROLREMOTO,
};
void asignacionMotores()
{
  
    for (int fila = 0; fila < 3; fila++)
    {
        int pinMotorIzq = pin_sensores_ultrasonido[0][fila];
        int pinMotorDer = pin_sensores_ultrasonido[1][fila];
        pinMode(pinMotorIzq, INPUT);
        pinMode(pinMotorDer, INPUT);
    }
}
// INICIO DE FUNCIONES
void pinesUltrasonido()
{
    pinMode(PIN_TRIG_ABAJO, OUTPUT);
    pinMode(PIN_ECO_ABAJO, INPUT);
}
void setup()
{
    // pinMode DE ULTRASONIDOS
    pinesUltrasonido();
    asignacionMotores();
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
// FUNCIOS DE DOBLAR A LA derecha INVIRTIENDO UN MOTOR FUNCION DE MOTOR DERECHO
// RECOPILACION DE FUNCIONES PARA SU MOVIMIENTO AUTONOMO
void movimiento_autonomo()
{
    // DISTANCIA DE RECOLECCION DE DATOS
    int sensor_arriba = sensores(PIN_TRIG_ARRIBA, PIN_ECO_ARRIBA);
    int sensor_abajo = sensores(PIN_TRIG_ABAJO, PIN_ECO_ABAJO);
    // CONDICIONES PARA MOVIMIENTO AUTONOMO
    bool moverse_adelante = sensor_arriba <= DISTANCIA_MAX && sensor_abajo <= DISTANCIA_MAX_ABAJO;
    bool doblar = distancia_arriba <= DISTANCIA_MIN;
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
// LOOP
void loop()
{
    switch (estadoDeorden)
    {
    case 0:
    {
        movimiento_autonomo();
        break;
    }

    default:
    {
        break;
    }
    }
}
