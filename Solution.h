#ifndef SOLUTION_H_
#define SOLUTION_H_


#include <vector>
#include "Problem.h"
#include "Vehicle.h"
using namespace std;
class Solution;
class SingleTrip
{
    public:
    SingleTrip();
    ~SingleTrip();

    double depot_early_start_time;//time when the car leave the depot
    double depot_late_start_time;//time when the car reach the depot
    double trip_duration;//trip duration
    double trip_cost;//calculate the cost of the trip
    double weight_to_carry;//check the maximum weight that is carried
    bool islunchtrip;
    double trip_distance;

    std::vector<double> start_time_trip;//start time of trip
    //int number_of_nodes_visited;//how many nodes has been visited including the depot;
    //int v_type;//to which type the trip has been assigned
    //int trip_number;//which trip number of the vehicle is it
    int vid;//what is the index or the id of the vehicle
    int fix_trip;//if anything can be added to the trip or not
    int vehicletrip_id;
    std::vector<int> cust_id;
    std::vector<double> begin_time;//service begin time
    std::vector<double> depart_time;//service over time


    double Calculate_Trip_Distance(const Problem& p);
};

class VehicleTrips{
public:
    VehicleTrips();
    ~VehicleTrips();

    Vehicle TripVehicle;
    std::vector<int> Multi;//vector of trips
    int num_of_trips;//number of trips by the car in a day ie size of trips
    double cost_of_vehicletrip;
    double Calculate_Cost_of_MultiTrip(const Problem& p, Solution* S);
    
   // void createSingleTrip(Problem &p, LoadRequest &Req);

    int vehicletype;
};

class Solution{
    public:
    Solution();
    ~Solution();
    std::vector<SingleTrip> GlobalTrips;
    std::vector<VehicleTrips> MTrips;
    int num_of_vehicles;
    void Calculate_Solution_Cost(const Problem& p);
    vector<int> unrouted_cust_request_id;
    double total_solution_cost;
    VehicleTrips createMultipleTrip(VehicleTrips &mtrip, Problem &p,LoadRequest &Req);
    void displaySolution();
//     bool compareTripEarlyStartTime_duration(int a, int b)
// {
//     if(GlobalTrips[a].depot_early_start_time<GlobalTrips[b].depot_early_start_time)
//         return true;
//     else if(GlobalTrips[a].depot_early_start_time==GlobalTrips[b].depot_early_start_time
//             &&  GlobalTrips[a].trip_duration<GlobalTrips[b].trip_duration)
//         return true;
//     return false;
// }
};


#endif
