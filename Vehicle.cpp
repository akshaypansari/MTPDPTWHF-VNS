#include "Vehicle.h"

Vehicle::Vehicle()
{
    fixed_cost=0;
    variable_cost=0;
    Capacity=0;
    type=-1;
    lunch_start_time=lunch_end_time=lunch_duration=-1;
}

Vehicle::~Vehicle()
{
}