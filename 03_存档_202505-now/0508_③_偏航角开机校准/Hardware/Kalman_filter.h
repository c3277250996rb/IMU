#ifndef Kalman_filter
#define Kalman_filter

typedef struct {
    double x;  // 估�?��?
    double p;  // 估�?��??�?协方�?
    double q;  // 过程�?声方�?
    double r;  // 测量�?声方�?
} KalmanFilter;

void KalmanFilter_Init(KalmanFilter* kf, double initial_value, double process_noise, double measurement_noise);
double KalmanFilter_Update(KalmanFilter* kf, double measurement);

#endif
