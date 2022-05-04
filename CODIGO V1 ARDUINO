//CONSTANTES DE DISTANCIA
#define DISTANCIA_MIN 10 
#define DISTANCIA_MAX_ABAJO 15
#define DISTANCIA_MAX 400 
//DECLARAMOS PINES DE ULTRA SONIDO
#define PIN_TRIG_ABAJO 10 
#define PIN_ECO_ABAJO 9
#define PIN_TRIG_ARRIBA 11
#define PIN_ECO_ARRIBA 12
//PINES DEL MOTOR IZQUIERDO CON PUENTE H
#define PIN_MOTOR_IZQUIERDO_IN1 2
#define PIN_MOTOR_IZQUIERDO_IN2 3
#define PIN_MOTOR_IZQUIERDO_ENA 5
//PINES DEL MOTOR DERECHO CON PUENTE H
#define PIN_MOTOR_DERECHO_IN1 6
#define PIN_MOTOR_DERECHO_IN2 7
#define PIN_MOTOR_DERECHO_ENA 8
//INFORMACION DE DISTNCIA RECOLECTADA POR LOS sensores
int sensor_arriba;
int sensor_abajo;
//INICIO DE FUNCIONES
void setup(){
    //pinMode DE ULTRASONIDOS
 pinMode(PIN_TRIG_ABAJO, OUTPUT);
 pinMode(PIN_ECO_ABAJO, INPUT);
 //pinMode DE MOTOR IZQUIERDO
 pinMode(PIN_MOTOR_IZQUIERDO_IN1, OUTPUT);
 pinMode(PIN_MOTOR_IZQUIERDO_IN2, OUTPUT);
 pinMode(PIN_MOTOR_IZQUIERDO_ENA, OUTPUT);  
 //pinMode DE MOTOR DERECHO
 pinMode(PIN_MOTOR_DERECHO_IN1, OUTPUT);
 pinMode(PIN_MOTOR_DERECHO_IN2, OUTPUT);
 pinMode(PIN_MOTOR_DERECHO_ENA, OUTPUT);
}
//FUNCIOS DE RECOLECCION DE DATOS DE LOS sensores
int sensores(PIN_TRIG, PIN_ECO)
{
    int distancia;
    int duracion;
    int periodo 20;
    unsigned long tiempo_actual;
    if (millis()>tiempo_actual + periodo)
    //SENSOR
    digitalWrite(PIN_TRIG_, HIGH);
    digitalWrite(PIN_TRIG_, LOW);
    duracion = pulseIn(PIN_ECO_, HIGH);
    distancia = duracion / 58.2;
    return distancia;
}

}
//FUNCIOS PARA MOVER LOS MOTORES PARA ADELANTE
void motores_adelante(){
    //control del motor izquierdo
    analogWrite(PIN_MOTOR_IZQUIERDO_ENA, 100);
    digitalWrite(PIN_MOTOR_IZQUIERDO_IN1, LOW);
    digitalWrite(PIN_MOTOR_IZQUIERDO_IN2, HIGH);
    //control del motor DERECHO
   analogWrite(PIN_MOTOR_DERECHO_ENA, 100);
    digitalWrite(PIN_MOTOR_DERECHO_IN1, LOW);
    digitalWrite(PIN_MOTOR_DERECHO_IN2, HIGH);
}
//FUNCIOS DE DOBLAR A LA DERECHA INVIRTIENDO UN MOTOR
void motores_doblar_derecha(){
    //control del motor izquierdo
    analogWrite(PIN_MOTOR_IZQUIERDO_ENA, 100);
    digitalWrite(PIN_MOTOR_IZQUIERDO_IN1, LOW);
    digitalWrite(PIN_MOTOR_IZQUIERDO_IN2, HIGH);
    //control del motor DERECHO
   analogWrite(PIN_MOTOR_DERECHO_ENA, 100);
    digitalWrite(PIN_MOTOR_DERECHO_IN1, HIGH);
    digitalWrite(PIN_MOTOR_DERECHO_IN2, LOW);
}
//FUNCIOS DE DOBLAR A LA IZQUIERDA INVIRTIENDO UN MOTOR
void motores_doblar_izquierda(){
    //control del motor izquierdo
    analogWrite(PIN_MOTOR_IZQUIERDO_ENA, 100);
    digitalWrite(PIN_MOTOR_IZQUIERDO_IN1, HIGH);
    digitalWrite(PIN_MOTOR_IZQUIERDO_IN2, LOW);
    //control del motor DERECHO
   analogWrite(PIN_MOTOR_DERECHO_ENA, 100);
    digitalWrite(PIN_MOTOR_DERECHO_IN1, LOW);
    digitalWrite(PIN_MOTOR_DERECHO_IN2, HIGH);
}
// RECOPILACION DE FUNCIONES PARA SU MOVIMIENTO AUTONOMO 
void movimiento_autonomo(){
    //DISTANCIA DE RECOLECCION DE DATOS
    int sensor_arriba = sensores(PIN_TRIG_ARRIBA, PIN_ECO_ARRIBA);
    int sensor_abajo = sensores(PIN_TRIG_ABAJO, PIN_ECO_ABAJO);
    //CONDICIONES PARA MOVIMIENTO AUTONOMO
    bool moverse_adelante = sensor_arriba <= DISTANCIA_MAX && sensor_abajo <= DISTANCIA_MAX_ABAJO;
    
    if (moverse_adelante){
        motores_adelante();  
    }
    else if (distancia_arriba <= DISTANCIA_MIN) //else if de giro tomando en cuenta el sensor de arriba
    {
        int valor_doblar ;
        if (!valor-dolar %2)
        {
            motores_doblar_derecha();
            valor_doblar ++;
        }
        else
        {
            motores_doblar_izquierda();
            valor_doblar ++;
        }
    }
}
//LOOP
void loop()
{
    movimiento_autonomo();
}
