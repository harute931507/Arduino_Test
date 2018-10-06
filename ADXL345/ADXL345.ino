#include <Wire.h>

// ADXL345 device address
#define DEVICE (0x53)

#define POWER_CTL   0x2D  // Power Control Register
#define DATA_FORMAT 0x31  // Data format control
#define DATAX0      0x32  // X-Axis Data 0
#define DATAX1      0x33  // X-Axis Data 1
#define DATAY0      0x34  // Y-Axis Data 0
#define DATAY1      0x35  // Y-Axis Data 1
#define DATAZ0      0x36  // Z-Axis Data 0
#define DATAZ1      0x37  // Z-Axis Data 1


int delayTime = 5;
float Fliter = 0.5;
byte X0;
byte X1;
byte Y0;
byte Y1;
byte Z0;
byte Z1;
float fX;
float fY;
float fZ;
float X;
float Y;
float Z;
float Roll;
float Pitch;
float Yaw;


void setup() {
  Serial.begin(9600);
  
  // Join I2C 
  Wire.begin();   

  // ADXL345 Setting
  writeTo(DEVICE, POWER_CTL, 0x08); // Measurement Mode
  writeTo(DEVICE, DATA_FORMAT, 0x09); // +- 4g

  // Init
  X = 0;
  Y = 0;
  Z = 0;
  Roll = 0;
  Pitch = 0;
}

void loop() {
  XYZ_calculate();
  RP_calculate();
 
  Serial.print(X);
  Serial.print(",");
  Serial.print(Y);
  Serial.print(",");
  Serial.print(Z);
  Serial.print(",");
  Serial.print(Roll);
  Serial.print(",");
  Serial.println(Pitch);
  
  delay(delayTime);
  
}

void writeTo(int device, byte address, byte val) {
  // Start Transmission to Device 
  Wire.beginTransmission(device);
  
  // Send Register Address
  Wire.write(address);        
  // Send Value to Write
  Wire.write(val);       
  // End Transmission 
  Wire.endTransmission(); 
}

int getData(byte address){
  // Start Transmission to Device 
  Wire.beginTransmission(DEVICE);
  // Send Value to Write
  Wire.write(address); 
  // End Transmission 
  Wire.endTransmission();
    
  // Return Value in the Address
  Wire.requestFrom(DEVICE,1);
  if(Wire.available()<=1)
  {
    return Wire.read();
  }
}

void XYZ_calculate(){
  X0 = getData(DATAX0); 
  X1 = getData(DATAX1);  
  Y0 = getData(DATAY0);
  Y1 = getData(DATAY1);
  Z0 = getData(DATAZ0);
  Z1 = getData(DATAZ1);

  X = ((X1 << 8)  + X0) / 256.0;
  Y = ((Y1 << 8)  + Y0) / 256.0;
  Z = ((Z1 << 8)  + Z0) / 256.0 - 0.095;

  if (X > 128.0){
    X = X - 256.0; 
  }
  if (Y > 128.0){
    Y = Y - 256.0; 
  }
  if (Z > 128.0){
    Z = Z - 256.0; 
  }
  /*
  // Low Pass Fliter
  fX = X * Fliter + fX * (1.0 - Fliter);
  fY = Y * Fliter + fY * (1.0 - Fliter);
  fZ = Z * Fliter + fZ * (1.0 - Fliter);
  X = fX;
  Y = fY;
  Z = fZ;*/
}

//calculate the Roll&Pitch
void RP_calculate(){
  double x_Buff = float(X);
  double y_Buff = float(Y);
  double z_Buff = float(Z);
  int sign = (z_Buff > 0)?1:-1;
  Roll = atan2(y_Buff, z_Buff) * 180.0 / PI;
  Pitch = atan2(-x_Buff, sign * sqrt(y_Buff * y_Buff + z_Buff * z_Buff)) * 180.0 / PI;
  //Yaw = atan2(z_Buff, sqrt(x_Buff * x_Buff + y_Buff * y_Buff)) * 180.0 / PI;
}
