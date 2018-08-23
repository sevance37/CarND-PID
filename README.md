# CarND-PID-Controller
Drive a race car around a track using a PID controller.  
The program is written in C++.  This Project is from Udacity's Self-Driving Car Engineer Nanodegree Program.

## Basic Set-up
1. Clone this repo.
2. Make sure [uWebSocketIO](https://github.com/uWebSockets/uWebSockets) is installed.  Two install scripts are included for MAC and Linux.  For Windows use [Windows 10 Bash on Ubuntu](https://www.howtogeek.com/249966/how-to-install-and-use-the-linux-bash-shell-on-windows-10/) and following the Linux instructions.
3. Make a build directory: `mkdir build && cd build`
4. Compile: `cmake .. && make`
5. Run the programs: Run `./pid`. Open [Term 2 Simulator](https://github.com/udacity/self-driving-car-sim/releases) and run the corresponding page.  

## The Parameters
#### This PID (proportional–integral–derivative) controller has 3 parameters that depend on the error (the cross track error). 
1. **P Component**:  The P or proportional component scales linearly with the error. The larger the error, the larger the steering angle to get the car back to the track. This causes the car to contually steer toward the path and results in overshoots or oscillations.

2. **I Component**:  The I or integral component is the sum of all of the errors over time. If car stays to one side of the path too long, this term will grow large and increase the steering angle to get the car back to the path. An example of this would be the car going around a curve. The I term will then continue to push the car to the other side of the path until the I term goes to zero. This behavior results in overshoots and oscillations. 

3. **D Component**:  The D or derivative component is related to the change in the error. When the change in the error is large, this term is large. This dampens the oscillations caused by the P and I components.

The components of this PID controller behaved as described above. This was observed by varying the magnitude of each term individually.

### Setting the hyperparameters.
The hyperparameters were chosen by manual tuning. The throttle was set to 0.3. The PID parameters were then systematically varied. The set with the least average absolute cross track error as well as a smooth ride (few oscillations) was chosen. The values of those parameters were `K_P=0.24`, `K_I=0.0048` and `K_d=3.6`. 

## Other Important Dependencies

* cmake >= 3.5
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)




