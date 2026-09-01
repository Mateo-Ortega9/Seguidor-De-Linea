// ==========================================
// ROBOT SEGUIDOR DE LÍNEA VELOCISTA
// Arduino Nano - 5 sensores IR
// Mateo, Andrés, Tadeo
// ==========================================

// ---------- SENSORES ----------
const int S1 = 2;
const int S2 = 3;
const int S3 = 4;
const int S4 = 5;
const int S5 = 6;

// ---------- MOTOR IZQUIERDO ----------
const int IN1 = 7;
const int IN2 = 8;
const int ENA = 9;

// ---------- MOTOR DERECHO ----------
const int IN3 = 11;
const int IN4 = 12;
const int ENB = 10;

// ---------- VELOCIDADES ----------
const int VELOCIDAD_BASE = 180;
const int VELOCIDAD_RAPIDA = 220;
const int VELOCIDAD_SUAVE = 150;
const int VELOCIDAD_FUERTE = 210;

// ---------- ÚLTIMA DIRECCIÓN ----------
enum Direccion {
  IZQUIERDA,
  CENTRO,
  DERECHA
};

Direccion ultimaDireccion = CENTRO;


// ==========================================
// CONFIGURACIÓN
// ==========================================

void setup() {

  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  detener();
}


// ==========================================
// CONTROL DE MOTORES
// ==========================================

void motorIzquierdo(int velocidad) {

  if (velocidad >= 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    velocidad = -velocidad;
  }

  velocidad = constrain(velocidad, 0, 255);
  analogWrite(ENA, velocidad);
}


void motorDerecho(int velocidad) {

  if (velocidad >= 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    velocidad = -velocidad;
  }

  velocidad = constrain(velocidad, 0, 255);
  analogWrite(ENB, velocidad);
}


void mover(int izquierda, int derecha) {

  motorIzquierdo(izquierda);
  motorDerecho(derecha);
}


void detener() {

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}


// ==========================================
// BÚSQUEDA DE LA LÍNEA
// ==========================================

void buscarLinea() {

  if (ultimaDireccion == IZQUIERDA) {

    // Buscar hacia la izquierda
    motorIzquierdo(-130);
    motorDerecho(160);

  } 
  else if (ultimaDireccion == DERECHA) {

    // Buscar hacia la derecha
    motorIzquierdo(160);
    motorDerecho(-130);

  } 
  else {

    // Si nunca sabemos dónde estaba,
    // avanzar lentamente
    mover(100, 100);
  }
}


// ==========================================
// PROGRAMA PRINCIPAL
// ==========================================

void loop() {

  // Leer sensores
  int s1 = digitalRead(S1);
  int s2 = digitalRead(S2);
  int s3 = digitalRead(S3);
  int s4 = digitalRead(S4);
  int s5 = digitalRead(S5);


  // ========================================
  // CENTRO
  // ========================================

  if (s3 == HIGH && s2 == LOW && s4 == LOW) {

    // Línea perfectamente centrada
    mover(VELOCIDAD_RAPIDA, VELOCIDAD_RAPIDA);

    ultimaDireccion = CENTRO;
  }


  // ========================================
  // CORRECCIÓN SUAVE IZQUIERDA
  // ========================================

  else if (s2 == HIGH && s3 == HIGH) {

    // La línea está un poco a la izquierda
    mover(VELOCIDAD_SUAVE, VELOCIDAD_RAPIDA);

    ultimaDireccion = IZQUIERDA;
  }


  else if (s2 == HIGH) {

    // Corrección izquierda
    mover(VELOCIDAD_BASE, VELOCIDAD_RAPIDA);

    ultimaDireccion = IZQUIERDA;
  }


  // ========================================
  // GIRO FUERTE IZQUIERDA
  // ========================================

  else if (s1 == HIGH) {

    // Curva fuerte
    mover(-50, VELOCIDAD_FUERTE);

    ultimaDireccion = IZQUIERDA;
  }


  // ========================================
  // CORRECCIÓN SUAVE DERECHA
  // ========================================

  else if (s4 == HIGH && s3 == HIGH) {

    // La línea está un poco a la derecha
    mover(VELOCIDAD_RAPIDA, VELOCIDAD_SUAVE);

    ultimaDireccion = DERECHA;
  }


  else if (s4 == HIGH) {

    // Corrección derecha
    mover(VELOCIDAD_RAPIDA, VELOCIDAD_BASE);

    ultimaDireccion = DERECHA;
  }


  // ========================================
  // GIRO FUERTE DERECHA
  // ========================================

  else if (s5 == HIGH) {

    // Curva fuerte
    mover(VELOCIDAD_FUERTE, -50);

    ultimaDireccion = DERECHA;
  }


  // ========================================
  // LÍNEA PERDIDA
  // ========================================

  else if (
    s1 == LOW &&
    s2 == LOW &&
    s3 == LOW &&
    s4 == LOW &&
    s5 == LOW
  ) {

    buscarLinea();
  }


  // ========================================
  // CASO ESPECIAL:
  // MUCHOS SENSORES DETECTAN LA LÍNEA
  // ========================================

  else if (s1 == HIGH && s2 == HIGH && s3 == HIGH) {

    // Probablemente una curva muy cerrada
    mover(80, VELOCIDAD_FUERTE);

    ultimaDireccion = IZQUIERDA;
  }


  else if (s3 == HIGH && s4 == HIGH && s5 == HIGH) {

    // Probablemente una curva muy cerrada
    mover(VELOCIDAD_FUERTE, 80);

    ultimaDireccion = DERECHA;
  }


  // Pequeña pausa para mantener
  // una respuesta rápida
  delay(2);
}
