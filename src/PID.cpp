#include "PID.h"
#include <cmath>

using namespace std;


PID::PID() {
  // errors
  p_error = 0.0;
  i_error = 0.0;
  d_error = 0.0;
  pid_error = 0.0;

  // previous cross track error
  prev_cte = NAN;
  
  // previous pid error
  prev_pid_error = NAN;
  
  // coefficients
  Kp = 0.0;
  Ki = 0.0;
  Kd = 0.0;
  
  // average absolute cte
  avg_abs_cte = 0;
  avg_abs_n = 0;
}

PID::~PID() {}

void PID::Init(double Kp_in, double Ki_in, double Kd_in) {
  Kp = Kp_in;
  Ki = Ki_in;
  Kd = Kd_in;
}

void PID::UpdateError(double cte) {
  
  // proportional term
  p_error = cte;
  
  // integral term
  i_error += cte; 
  
  // derivative term
  if (isnan(prev_cte)) { 
    prev_cte = cte;
  }
  d_error = cte-prev_cte;
  prev_cte = cte;

  // the pid_error
  prev_pid_error = pid_error;
  pid_error = -Kp*p_error - Ki*i_error - Kd*d_error; 
 
  // the average of absolute cte
  double tot_abs_cte = abs(cte) + avg_abs_n*avg_abs_cte;
  avg_abs_n += 1;
  avg_abs_cte = tot_abs_cte/avg_abs_n;
}

double PID::TotalError() {
  return pid_error;
}

double PID::ChangeError() {
  return pid_error - prev_pid_error;
}

double PID::AvgAbsCte() {
  return avg_abs_cte;
}
