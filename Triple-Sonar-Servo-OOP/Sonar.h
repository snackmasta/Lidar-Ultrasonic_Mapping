#ifndef SONAR_H
#define SONAR_H

#include <NewPing.h>

#define TRIGGER_PIN_1  12  // Sonar 1
#define ECHO_PIN_1     11
#define TRIGGER_PIN_2  10  // Sonar 2
#define ECHO_PIN_2     9
#define TRIGGER_PIN_3   8  // Sonar 3
#define ECHO_PIN_3     7
#define SONAR_NUM 3
#define MAX_DISTANCE 200 

class SonarModule {
public:
  SonarModule();
  int ping(int index);
  int kalmanFilter(int index);

private:
  NewPing sonar[SONAR_NUM];
  float Xt[SONAR_NUM], Xt_update[SONAR_NUM], Xt_prev[SONAR_NUM];
  float Pt[SONAR_NUM], Pt_update[SONAR_NUM], Pt_prev[SONAR_NUM];
  float Kt[SONAR_NUM], R, Q;
};

#endif
