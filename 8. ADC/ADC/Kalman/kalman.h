/*
 SimpleKalmanFilter - a Kalman Filter implementation for single variable models.
 Created by Denys Sene, January, 1, 2017.
 Released under MIT License - see LICENSE file for details.
 */ 
 #ifndef KALMAN_H_
 #define KALMAN_H_
 // class SimpleKalmanFilter
 void SimpleKalmanFilter(float mea_e, float est_e, float q);
   float updateEstimate(float mea);
   void setMeasurementError(float mea_e);
   void setEstimateError(float est_e);
   void setProcessNoise(float q);
   float getKalmanGain();
   float getEstimateError();
 #endif