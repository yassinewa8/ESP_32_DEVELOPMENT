#include <Wire.h>

#define MPU9250_ADDRESS 0x68
#define AK8963_ADDRESS 0x0C

void setup() {
  Serial.begin(115200);
  Wire.begin();
  initializeMPU9250();
}

void loop() {
  readMPU9250Data();
  delay(100); // Adjust the delay as needed
}

void initializeMPU9250() {
  // Wake up the MPU9250
  Wire.beginTransmission(MPU9250_ADDRESS);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);

  // Configure the accelerometer (+/- 2g)
  Wire.beginTransmission(MPU9250_ADDRESS);
  Wire.write(0x1C);
  Wire.write(0x00);
  Wire.endTransmission(true);

  // Configure the gyroscope (+/- 250 deg/s)
  Wire.beginTransmission(MPU9250_ADDRESS);
  Wire.write(0x1B);
  Wire.write(0x00);
  Wire.endTransmission(true);

  // Configure the magnetometer (16-bit output)
  Wire.beginTransmission(AK8963_ADDRESS);
  Wire.write(0x0A);
  Wire.write(0x16);
  Wire.endTransmission(true);
}

void readMPU9250Data() {
  int16_t accelX, accelY, accelZ;
  int16_t gyroX, gyroY, gyroZ;
  int16_t magX, magY, magZ;
  int16_t temp;

  // Read accelerometer and gyroscope data
  Wire.beginTransmission(MPU9250_ADDRESS);
  Wire.write(0x3B); // Starting register for Accel and Gyro data
  Wire.endTransmission(false);
  Wire.requestFrom(MPU9250_ADDRESS, 14, true);

  accelX = (Wire.read() << 8 | Wire.read());
  accelY = (Wire.read() << 8 | Wire.read());
  accelZ = (Wire.read() << 8 | Wire.read());
  temp = (Wire.read() << 8 | Wire.read());
  gyroX = (Wire.read() << 8 | Wire.read());
  gyroY = (Wire.read() << 8 | Wire.read());
  gyroZ = (Wire.read() << 8 | Wire.read());

  // Read magnetometer data
  Wire.beginTransmission(AK8963_ADDRESS);
  Wire.write(0x03); // Starting register for Mag data
  Wire.endTransmission(false);
  Wire.requestFrom(AK8963_ADDRESS, 7, true);

  magX = (Wire.read() | Wire.read() << 8);
  magY = (Wire.read() | Wire.read() << 8);
  magZ = (Wire.read() | Wire.read() << 8);
  Wire.read(); // Status register ST2 needs to be read to complete the read process

  // Convert raw data to meaningful units
  float accelX_g = accelX / 16384.0;
  float accelY_g = accelY / 16384.0;
  float accelZ_g = accelZ / 16384.0;
  float gyroX_dps = gyroX / 131.0;
  float gyroY_dps = gyroY / 131.0;
  float gyroZ_dps = gyroZ / 131.0;
  float magX_uT = magX * 0.15;
  float magY_uT = magY * 0.15;
  float magZ_uT = magZ * 0.15;
  float temperature = (temp / 333.87) + 21.0;

  // Print the data
  Serial.print("Accel (g): ");
  Serial.print("X = "); Serial.print(accelX_g);
  Serial.print(" Y = "); Serial.print(accelY_g);
  Serial.print(" Z = "); Serial.println(accelZ_g);

  Serial.print("Gyro (dps): ");
  Serial.print("X = "); Serial.print(gyroX_dps);
  Serial.print(" Y = "); Serial.print(gyroY_dps);
  Serial.print(" Z = "); Serial.println(gyroZ_dps);

  Serial.print("Mag (uT): ");
  Serial.print("X = "); Serial.print(magX_uT);
  Serial.print(" Y = "); Serial.print(magY_uT);
  Serial.print(" Z = "); Serial.println(magZ_uT);

  Serial.print("Temperature (C): ");
  Serial.println(temperature);
}

///////////////////////////////////////////////////////////////////////////////////
// Loops through all the addresses and returns the valid address for the mpu 9250
///////////////////////////////////////////////////////////////////////////////////
int GET_I2C_ADDRESS(){
  byte error, address;
  int nDevices = 0;

  Serial.println("Scanning for I2C devices ...");
  for (address = 0x01; address < 0x7f; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.printf("I2C device found at address 0x%02X\n", address);
      return address;
      nDevices++;
    } else if (error != 2) {
      Serial.printf("Error %d at address 0x%02X\n", error, address);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found");
  }

   

}

