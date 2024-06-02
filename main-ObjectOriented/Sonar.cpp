#include "Sonar.h"

SonarModule::SonarModule() :
  sonar{
    NewPing(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE),
    NewPing(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE),
    NewPing(TRIGGER_PIN_3, ECHO_PIN_3, MAX_DISTANCE)
  }
{}

int SonarModule::ping(int index) {
  return sonar[index].ping_cm();
}
