/* Robot Monitor Client Library - Fake IMU Example.
 * https://github.com/khantkyawkhaung/robot-monitor
 * 
 * This example code is in the public domain.
 */


#include <robotmonitor.h>


int sync1;

float roll = 0;
float pitch = 0;
float yaw = 0;
float gyroX, gyroY, gyroZ;

bool magScan = false;
int8_t magMode = 0;


void onMagModeChange(int argc, char *argv[]) {
  digitalWrite(LED_BUILTIN, magMode);
}

void calibrateGyro(int argc, char *argv[]) {
  rmEcho("Gyro sensor calibrated");
}

float M[3][4];

void calibrateMag(int argc, char *argv[]) {
    static uint8_t calibrated = 0b000;
    if(argc != 5)
        return;
    uint8_t i = atoi(argv[0]);
    if(i > 2)
        return;
    calibrated |= (1 << i);
    for(int j=0; j<4; j++) {
        M[i][j] = atof(argv[j+1]);
    }
    if(calibrated == 0b111) {
        rmEcho("Magnetometer calibration matrix uploaded");
        calibrated = 0b000;
    }
}


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  rmConnectUART(115200);
  sync1 = rmCreateSync();
  rmCreateOutputAttribute("roll", &roll,
                          RM_ATTRIBUTE_FLOAT, sync1);
  rmCreateOutputAttribute("pitch", &pitch,
                          RM_ATTRIBUTE_FLOAT, sync1);
  rmCreateOutputAttribute("yaw", &yaw,
                          RM_ATTRIBUTE_FLOAT, sync1);
  rmCreateOutputAttribute("gyro-x", &gyroX,
                          RM_ATTRIBUTE_FLOAT, sync1);
  rmCreateOutputAttribute("gyro-y", &gyroY,
                          RM_ATTRIBUTE_FLOAT, sync1);
  rmCreateOutputAttribute("gyro-z", &gyroZ,
                          RM_ATTRIBUTE_FLOAT, sync1);
  rmCreateInputAttribute("mag-scan", &magScan,
                          RM_ATTRIBUTE_BOOL);
  rmCreateInputAttribute("mag-mode", &magMode,
                          RM_ATTRIBUTE_INT8);
  rmInputAttributeSetOnChange("mag-mode",
                              onMagModeChange);
  rmCreateCall("cali-gyro", calibrateGyro);
  rmCreateCall("cali-mag", calibrateMag);
}


void loop() {
  static unsigned long t1a;
  static unsigned long t1b;
  unsigned long t2 = millis();
  if(t2 - t1a > 33) {
    gyroX += random(-25, 25) * 0.033f;
    gyroY += random(-25, 25) * 0.033f;
    gyroZ += random(-25, 25) * 0.033f;
    gyroX = constrain(gyroX, -90, 90);
    gyroY = constrain(gyroY, -90, 90);
    gyroZ = constrain(gyroZ, -90, 90);
    roll += gyroX * 0.033f;
    pitch += gyroY * 0.033f;
    yaw += gyroZ * 0.1f;
    if(roll > 180)
      roll -= 360;
    else if(roll < -180)
      roll += 360;
    if(pitch > 180)
      pitch -= 360;
    else if(pitch < -180)
      pitch += 360;
    if(yaw > 180)
      yaw -= 360;
    else if(yaw < -180)
      yaw += 360;
    rmSyncUpdate(sync1);
    t1a = t2;
  }
  if(t2 - t1b > 100) {
    if(magScan) {
      float a = random(6283) / 1000.0f;
      float b = random(6283) / 1000.0f;
      float x = 400*cos(a)*cos(b);
      float y = 300*sin(a)*cos(b);
      float z = 350*sin(b);
      x = x + 150;
      y = y - 50;
      z = z + 50;
      if(magMode == 1) {
        float p = M[0][0]*x + M[0][1]*y + M[0][2]*z +
                  M[0][3];
        float q = M[1][0]*x + M[1][1]*y + M[1][2]*z +
                  M[1][3];
        float r = M[2][0]*x + M[2][1]*y + M[2][2]*z +
                  M[2][3];
        x = p;
        y = q;
        z = r;
      }
      rmSendCommand("mag-data %f %f %f", x, y, z);
    }
    t1b = t2;
  }
  rmProcessMessage();
}
