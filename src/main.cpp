#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main(int argc, char** argv)
{
  uWS::Hub h;
  
  // Initialize the pid variables and the throttle.
  PID pid;
  double throttle;
  int adj_throttle = 0;
  if (argc >= 6) {
    double Kp = std::atof(argv[1]);
    double Ki = std::atof(argv[2]);
    double Kd = std::atof(argv[3]);
    throttle = std::atof(argv[4]);
    adj_throttle = std::atoi(argv[5]);
    std::cout << "Params " << Kp << " " << Ki << " " << Kd << std::endl;
    pid.Init(Kp,Ki,Kd);
  } 
  else { 
    pid.Init(0.24,0.0048,3.6);
    throttle = 0.3;
  }

  h.onMessage([&pid,&throttle,&adj_throttle](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());

          // update the PID with the cte 
          pid.UpdateError(cte);
          double pid_error = pid.TotalError();

          // calculate the steering angle
          double prev_steer = angle/25.0;   // modify to [-1,1] 
          
          double mix_wt = 1.0;
          double next_steer = mix_wt*pid_error + (1-mix_wt)*prev_steer;
          if (next_steer > 1.0) { 
            next_steer = 1.0;
          }
          else if (next_steer < -1.0) { 
            next_steer = -1.0;
          }
         
          // adjust the throttle when the steering angle is large
          double next_throttle = throttle;
          if (adj_throttle && fabs(next_steer) > 0.4) { 
            double m_thr = -(throttle+0.1)/0.6;
            double b_thr = -0.1 - m_thr;
            next_throttle = m_thr*fabs(next_steer) + b_thr;
          }            

          /* For finding parameters
          std::cout << "CTE: " << cte << " Steering Value: " << next_steer 
                    << "AVG_ABS_CTE: " << pid.AvgAbsCte() << std::endl;          
          */
         
          json msgJson;
          msgJson["steering_angle"] = next_steer;
          msgJson["throttle"] = next_throttle; 
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
