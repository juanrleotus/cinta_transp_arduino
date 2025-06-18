//


// Pines del sensor
#define SENSOR1_PIN 2
#define SENSOR2_PIN 3

// Motor DC (Movimiento)
#define ENA_MOTOR1 4
#define IN1_MOTOR1 8
#define IN2_MOTOR1 9

// Bomba de agua
#define ENA_BOMBA 5
#define IN1_BOMBA 6
#define IN2_BOMBA 7

// Variables de control
bool motorActivo = true;
bool bombaActivada = false;
unsigned long tiempoBombaInicio = 0;

void setup() {
  Serial.begin(9600);

  // Configuración de pines
  pinMode(SENSOR1_PIN, INPUT);
  pinMode(SENSOR2_PIN, INPUT);

  pinMode(ENA_MOTOR1, OUTPUT);
  pinMode(IN1_MOTOR1, OUTPUT);
  pinMode(IN2_MOTOR1, OUTPUT);

  pinMode(ENA_BOMBA, OUTPUT);
  pinMode(IN1_BOMBA, OUTPUT);
  pinMode(IN2_BOMBA, OUTPUT);

  avanzarMotor();
}

void loop() {
  // Leer sensores (asumiendo que emiten LOW cuando detectan obstáculo)
  bool obstaculo1 = digitalRead(SENSOR1_PIN) == LOW;
  bool obstaculo2 = digitalRead(SENSOR2_PIN) == LOW;

  if (motorActivo && obstaculo1) {
    Serial.println("Obstáculo detectado por sensor1. Deteniendo motor...");
    detenerMotor();
    motorActivo = false;

    delay(2000);  // Esperar 2 segundos

    Serial.println("Activando bomba...");
    activarBomba(60);  // Velocidad 60 (de 255)
    bombaActivada = true;
    tiempoBombaInicio = millis();
  }

  if (bombaActivada && (millis() - tiempoBombaInicio >= 4000)) {
    Serial.println("Apagando bomba y reanudando motor...");
    detenerBomba();
    bombaActivada = false;

    avanzarMotor();
    motorActivo = true;
  }

  if (motorActivo && obstaculo2) {
    Serial.println("Obstáculo detectado por sensor2. Deteniendo motor...");
    detenerMotor();
    motorActivo = false;
  }
}

// Funciones de control del motor DC
void avanzarMotor() {
  digitalWrite(IN1_MOTOR1, HIGH);
  digitalWrite(IN2_MOTOR1, LOW);
  analogWrite(ENA_MOTOR1, 255);  // Velocidad máxima
}

void detenerMotor() {
  digitalWrite(IN1_MOTOR1, LOW);
  digitalWrite(IN2_MOTOR1, LOW);
  analogWrite(ENA_MOTOR1, 0);
}

// Funciones de control de la bomba
void activarBomba(int velocidad) {
  digitalWrite(IN1_BOMBA, HIGH);
  digitalWrite(IN2_BOMBA, LOW);
  analogWrite(ENA_BOMBA, velocidad);
}

void detenerBomba() {
  digitalWrite(IN1_BOMBA, LOW);
  digitalWrite(IN2_BOMBA, LOW);
  analogWrite(ENA_BOMBA, 0);
}
