#include "Aircraft.h"
#include <cmath>

double Aircraft::distance_to_next_point(){
  return edge_path[pos.first].length - pos.second;
}

double Aircraft::get_velocity(){
  // TODO Car following model joins here

  double T = 1.0/tick_per_time_unit;
  if (command == STOP_COMMAND){

    if (prev_command != STOP_COMMAND){
      stop_received += 1;
    }

    wait_tick += 1;

    acceleration = - model.a_brake;
    return velocity + acceleration * T;
  }


  if (prev_aircraft == nullptr){
    acceleration = model.a_max;
    return velocity + acceleration * T;
  }

  double distance = distance_to_prev;


  // if (prev_aircraft -> acceleration < 0){
  //   acceleration = -model.a_brake;
  //   return velocity + acceleration * T;
  // }

  // Accelerating 
  double v_other = prev_aircraft->velocity;
  double a_brake_other = prev_aircraft ->model.a_brake;



  double h = distance + (v_other * v_other / (2 * a_brake_other)) - model.safety_distance;


  double a_eq = 2 * T * T;
  double b_eq = model.a_brake * T * T + 2 * velocity * T;
  double c_eq = velocity * velocity + 2 * model.a_brake * (velocity * T - h);

  double acc = 0;
  if (b_eq * b_eq - 4 * a_eq * c_eq < 0){
    // b2 - 4ac < 0 implies even deceleration will lead to collision
    // TODO need check
    acc = -model.a_brake;
  }else{
    acc = (-b_eq + sqrt(b_eq * b_eq - 4 * a_eq * c_eq))/ (2 * a_eq);
  }

  acceleration =  max(-model.a_brake, min(model.a_max, acc));

  return velocity + acceleration * T;
}

void Aircraft::move(){
  passed_check_point.clear();
  int edge_idx = pos.first;

  // velocity * timestep
  double v = get_velocity();
  if (v > model.v_max){
    v = model.v_max;
  }
  if (v < 0){
    v = 0;
  }

  if (v == 0){
    zero_velocity_tick += 1;
  }


  velocity = v;

  double l = pos.second + velocity / (double)tick_per_time_unit;
  while (l > edge_path[edge_idx].length &&
         edge_idx + 1 < edge_path.size()){
    l = l - edge_path[edge_idx].length;
    passed_check_point.push_back(edge_path[edge_idx].name);
    edge_idx += 1;
  }
  if (l > edge_path[edge_idx].length){
    passed_check_point.push_back(edge_path[edge_idx].name);
    // arrive at runway
    ready_for_runway = true;
  }
  pos = {edge_idx, l};
  prev_command = command;
  command = NO_COMMAND;
}

string Aircraft::position_str(){
  return edge_path[pos.first].name + string(" - ")
    + to_string(pos.second);
}


void Aircraft::init_expr_data(){
  stop_received = 0;
  zero_velocity_tick = 0;
  wait_tick = 0;
}

void Aircraft::simulation_init(){
  // generate_actual_appear_time();

  /*
    init exp data
   */
  init_expr_data();

  command = NO_COMMAND;
  prev_command = NO_COMMAND;
  ready_for_runway = false;
  actual_runway_time = 0;
}

void Aircraft::simulation_begin(){
  ready_for_runway = false;
  actual_runway_time = 0;
  pos = {0, 0};
}

void Aircraft::generate_actual_appear_time(){
  actual_appear_time = path[0].time[0]; 
}

string Aircraft::current_edge_name(){
  return  edge_path[pos.first].name;
}


void Aircraft::send_command(int given_command){
  command = given_command;
}

vector<string> Aircraft::intersection_in_sight(double sight_length){
  /*
    return the edge that aircraft will cross in coming dist of
    sight_length
   */
  vector<string> edge_list;

  if (sight_length > distance_to_next_point()){
    sight_length -= distance_to_next_point();

    int i = pos.first;
    // we have to_node in edge_path
    edge_list.push_back(edge_path[i].name);
    i++;

    while (sight_length > 0 && i < edge_path.size()){
      if (sight_length > edge_path[i].length){
        edge_list.push_back(edge_path[i].name);
      }
      sight_length -= edge_path[i].length;
      i ++;
    }
  }
  // cout << id << endl;
  // for (auto e: edge_list){
  //   cout << e << endl;
  // }

  return edge_list;
}