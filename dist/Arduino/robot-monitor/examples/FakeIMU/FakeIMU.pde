/* Robot Monitor Client Library - Fake IMU Example.
 * https://github.com/khantkyawkhaung/robot-monitor
 * 
 * This example code is in the public domain.
 */


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
      float x, y, z;
      if(magMode == 0) {
        x = 400*cos(a)*cos(b);
        y = 300*sin(a)*cos(b);
        z = 350*sin(b);
        x = x + 150;
        y = y - 50;
        z = z + 50;
      }
      else {
        x = cos(a)*cos(b);
        y = sin(a)*cos(b);
        z = sin(b);
      }
      rmSendCommand("mag-data %f %f %f", x, y, z);
    }
    t1b = t2;
  }
  rmProcessMessage();
}
