//This is to define a class of Vehicle with t types // header file

#ifndef VEHICLE_H_
#define VEHICLE_H_

class Vehicle
{
public:
    Vehicle();
    ~Vehicle();

    int type;// represent the size of the vehicle; bigger number represent bigger size
    double fixed_cost;//fixed cost to hire a vehicle for a day
    double variable_cost;// cost to travel a unit distance
    double Capacity; // maximum weight a vehicle can handle
    double lunch_start_time, lunch_end_time;//should we define it here or in the problem
    double lunch_duration;//should we define it here // it is ok here
    //vehicle is defined separately and and in vehicletrips
};

#endif

