# ArduinoBasics
Sketches básicos de uso de diferentes componentes Arduino

1. [WakeUpAlarm](#wakeupalarm)

***

## WakeUpAlarm
Sketch que utiliza el modo sleep y las interrupciones para despertar la placa Arduino haciendo uso de un reloj RTC. El Arduino entra en modo sleep, se despierta cada minuto durante 5 segundos para hacer parpadear el led y vuelve a entrar en modo sleep.

### Componentes principales
- [Placa Arduino UNO](https://www.arduino.cc/en/Main/ArduinoBoardUno)
- Reloj RTC3231 (I2C)

### Dependencias
- [Wire Library](https://www.arduino.cc/en/Reference/Wire) *-- Uso del bus I2C*
- [A modified arduino library for DS3231 RTC](https://github.com/FabioCuomo/FabioCuomo-DS3231) *-- Uso del RTC*
- [Low Power Library for Arduino](https://github.com/rocketscream/Low-Power) *-- Uso del modo sleep de Arduino*

***
