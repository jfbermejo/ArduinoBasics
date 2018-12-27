#include <Wire.h>
#include <RTClibExtended.h>
#include <LowPower.h>

#define wakePin 2    // Usa la interrupción 0 (pin 2) y ejecuta la funcion cuando pin 2 está a LOW
#define ledPin 13    // Usa el LED integrado para indicar que Arduino está despierto

RTC_DS3231 RTC;      // Usa el RTC DS3231

volatile byte AlarmFlag = 0;          // Bandera que indica si ha saltado la alarma
byte ledStatus = 1;                   // Estado del led
unsigned long marcaTiempo;            // Marca de tiempo para contar los milisegundos
const long duracionParpadeo = 5000;   // Cuánto tiempo parpadea el led


//-------------------------------------------------
// Función que se ejecuta al despertar el Arduino
//-------------------------------------------------
void wakeUp(){
    AlarmFlag++;  // Fija la bandera para indicar que ha saltado la alarma
}


//-------------------------------------------------
// Función que limpia las alarmas al salir del modo sleep
//-------------------------------------------------
void clearAlarmFunction(){
  // Elimina cualquier alarma pendiente
  RTC.armAlarm(1, false);
  RTC.clearAlarm(1);
  RTC.alarmInterrupt(1, false);
  RTC.armAlarm(2, false);
  RTC.clearAlarm(2);
  RTC.alarmInterrupt(2, false);
}


//-------------------------------------------------
// Función que fija una nueva alarma
//-------------------------------------------------
void setAlarmFunction(){
   RTC.setAlarm(ALM2_EVERY_MINUTE, 0, 0, 0);
   RTC.alarmInterrupt(2, true);
}


//-------------------------------------------------
// Función de configuración (se ejecuta 1 vez)
//-------------------------------------------------
void setup() {

  // Iniciamos comunicación serie
  Serial.begin(9600);
  
  // Fija el pin D2 como INPUT para aceptar la señal de interrupción desde el DS3231
  pinMode( wakePin , INPUT );

  // Enciende el led de la placa durante 1 segundo para indicar que se ha iniciadoel sketch
  pinMode( ledPin , OUTPUT );
  digitalWrite( ledPin , HIGH );
  delay( 1000 );

  // Inicializa el reloj
  Wire.begin();
  RTC.begin();
  RTC.adjust(DateTime(__DATE__, __TIME__));   // Fija la hora del RTC con la de compilación
  
  // Limpia cualquier alarma pendiente
  clearAlarmFunction();

  // Fija el pin SQW del RTC a OFF (por defecto está fijado a 1Hz)
  // La salida del pin INT del DS3231 está conectada a este pin
  // Debe conectarse al pin D2 de arduino para despertarse
  RTC.writeSqwPinMode(DS3231_OFF);

  Serial.println( "Test de interrupciones, modo sleep y RTC" );
  Serial.println();
  Serial.flush();
  
}


//-------------------------------------------------
// Función que se ejecuta contínuamente
//-------------------------------------------------
void loop() {

  // En el primer loop la placa entra en modo sleep
  if( AlarmFlag == 0 ){

    Serial.println( "Fijando alarma y entrando en modo sleep..." );
    Serial.flush();
    
    setAlarmFunction();
    attachInterrupt(0, wakeUp, LOW);                       // Usa la interrupción 0 (pin 2) y ejecuta función wakeUp cuando el pin 2 va a LOW 
    digitalWrite(ledPin, LOW);                             // Apaga el led para indicar que se entra en modo sleep
    ledStatus = 0;                                         // Fija el estatus del led
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);   // El Arduino entra en modo sleep aquí
    detachInterrupt(0);                                    // La ejecución continua desde aquí después de despertar

    // Obtenemos la hora
    DateTime tiempoReferencia = RTC.now();

    // Mostramos la fecha por el monitor serie
    Serial.print( "...saliendo de modo sleep a las " );
    Serial.print(tiempoReferencia.hour(), DEC);
    Serial.print(':');
    Serial.print(tiempoReferencia.minute(), DEC);
    Serial.print(':');
    Serial.print(tiempoReferencia.second(), DEC);
    Serial.println();
    Serial.println();

    clearAlarmFunction();

    // Comenzamos a contar el tiempo
    marcaTiempo = millis();
  }

  // Comprueba si está dentro de la duración especificada
  if( millis() - marcaTiempo < duracionParpadeo ){

      Serial.print( millis() );
      Serial.print( "-" );
      Serial.print(marcaTiempo);
      Serial.print( " < " );
      Serial.println( duracionParpadeo);
    
      // Enciende y apaga el led para indicar que el Arduino está despierto
      if( ledStatus == 0 ){
        ledStatus = 1;
        digitalWrite( ledPin , HIGH );
      } else {
        ledStatus = 0;
        digitalWrite( ledPin , LOW );
      }
    
      delay(500);
      
  } else {
      // Ha pasado la duración especificada, fija condición para entrar en modo sleep
      AlarmFlag = 0;
      marcaTiempo = 0;

      Serial.println();
  }

}
