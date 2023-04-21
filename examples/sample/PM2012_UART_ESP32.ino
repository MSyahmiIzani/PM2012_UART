#include <HardwareSerial.h>
#include <PM2012_UART.h>



PM2012_UART pm2012(&Serial);

char version[14];
uint8_t serialNumber[10];

void setup() {
  Serial.begin(9600); // initialize serial communication for debugging
  Serial.begin(9600, SERIAL_8N1, 16, 17); // initialize hardware serial for AM1002

  while (!Serial) { // Wait for serial connection
  
  }
}

void loop() {
  
  bool success = pm2012.measure();

  if (success)
  {
    Serial.print("PM 1.0 GRIMM : ");
    Serial.println(pm2012.getPm1p0Grimm());
    Serial.print("PM 2.5 GRIMM : ");
    Serial.println(pm2012.getPm2p5Grimm());
    Serial.print("PM 10 GRIMM : ");
    Serial.println(pm2012.getPm10Grimm());
    Serial.print("PM 1.0 TSI : ");
    Serial.println(pm2012.getPm1p0Tsi());
    Serial.print("PM 2.5 TSI : ");
    Serial.println(pm2012.getPm2p5Tsi());
    Serial.print("PM 10 TSI : ");
    Serial.println(pm2012.getPm10Tsi());
    Serial.print("Particle Number > 0.3 um : ");
    Serial.println(pm2012.getPm0p3Micro());
    Serial.print("Particle Number > 0.5 um : ");
    Serial.println(pm2012.getPm0p5Micro());
    Serial.print("Particle Number > 1.0 um : ");
    Serial.println(pm2012.getPm1p0Micro());
    Serial.print("Particle Number > 2.5 um : ");
    Serial.println(pm2012.getPm2p5Micro());
    Serial.print("Particle Number > 5.0 um : ");
    Serial.println(pm2012.getPm5p0Micro());
    Serial.print("Particle Number > 10 um : ");
    Serial.println(pm2012.getPm10Micro());
  } else {
    Serial.println("Failed to measure sensor data"); // debug message
  }

  success = pm2012.requestSoftwareVersionNumber();

  if (success)
  {
    pm2012.readSoftwareVersionNumber(version);
    Serial.print("Version : ");
    Serial.println(version);
  }

  success = pm2012.requestSerialNumber(); // request serial number

  if(success)
  {
    pm2012.readSerialNumber(serialNumber);
    Serial.print("Serial Number : ");

    Serial.print((serialNumber[0] << 8) + serialNumber[1]);
    Serial.print(" ");

    Serial.print((serialNumber[2] << 8) + serialNumber[3]);
    Serial.print(" ");

    Serial.print((serialNumber[4] << 8) + serialNumber[5]);
    Serial.print(" ");

    Serial.print((serialNumber[6] << 8) + serialNumber[7]);
    Serial.print(" ");

    Serial.println((serialNumber[8] << 8) + serialNumber[9]);
  }

  delay(3000);
}
