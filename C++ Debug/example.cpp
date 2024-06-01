#include <stdio.h>

float SensorData, KalmanFilterData;
float Xt, Xt_update, Xt_prev;
float Pt, Pt_update, Pt_prev;
float Kt, R, Q;

float kalmanFilter(int input);
int add(int x, int y);

int main(){
    printf("Hello World\n");
    for (int i = 0; i < 10; i++){
        printf("%f\n", kalmanFilter(20));
    }
    return 0;
}

float kalmanFilter(int input){
    R=5;
    Q=0.5;
    Pt_prev=1;

    SensorData = (float)input;
    Xt_update = Xt_prev;
    Pt_update = Pt_prev + Q;
    Kt = Pt_update/(Pt_update + R);
    Xt = Xt_update + (Kt * (SensorData - Xt_update));
    Pt = (1-Kt)*Pt_update;

    Xt_prev = Xt;
    Pt_prev = Pt;

    KalmanFilterData = Xt;

    return KalmanFilterData;
}