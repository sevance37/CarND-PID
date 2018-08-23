#ifndef PID_H
#define PID_H

class PID {
public:
  /*
  * Errors
  */
  double p_error;
  double i_error;
  double d_error;
  double pid_error;
  
  /*
  * Previous PID error
  */
  double prev_pid_error;
  
  /*
  * Previous cross track error
  */
  double prev_cte;

  /*
  * Average of the absolute CTE
  */
  double avg_abs_cte;
  double avg_abs_n;
  
  /*
  * Coefficients
  */ 
  double Kp;
  double Ki;
  double Kd;
  
  /*
  * Constructor
  */
  PID();

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void Init(double Kp, double Ki, double Kd);

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */
  double TotalError();

  /*
  * Calculate the change in the PID error.
  */
  double ChangeError(); 
  
  /*
  * Calculate the average abs cte.
  */
  double AvgAbsCte(); 
  
};

#endif /* PID_H */
