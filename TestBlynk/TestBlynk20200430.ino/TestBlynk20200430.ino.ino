#define BLYNK_PRINT Serial
#include <SoftwareSerial.h>

#include <BlynkSimpleSerialBLE.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "lzpzhMPA9jEOyfUD8Qod2ZsbGZXOD_J8";

// Use Virtual pin 5 for uptime display
#define PIN_UPTIME V5
SoftwareSerial SerialBLE(2, 3); // RX, TX
// This function tells Arduino what to do if there is a Widget
// which is requesting data for Virtual Pin (5)
BLYNK_READ(PIN_UPTIME)
{
  // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  Blynk.virtualWrite(PIN_UPTIME, millis() / 1000);
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  
  SerialBLE.begin(9600);
  Blynk.begin(SerialBLE, auth);

  Serial.println("Waiting for connections...");
}

void loop()
{
  Blynk.run();
}
