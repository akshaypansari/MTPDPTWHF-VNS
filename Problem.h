#ifndef PROBLEM_H_
#define PROBLEM_H_

#include "Vehicle.h"
#include "LoadRequest.h"
#include <vector>
#include <string>

using namespace std;
class Problem
{
public:
    Problem();
    ~Problem();

    void LoadProblem(const std::string &filename);

    bool IsPickupNode(int cid)const;//input->request Id and output if(Pickup Node)
    bool IsDeliveryNode(int cid)const;//input->request Id and output if(Delivery)
    const Node &getNode(int cid) const;//take input as the request ID
                                       //and return node index
    int getRequestID(int cid)const;//input->request Id and output the Node index
    int checkcapacity(double demand, int typeconst);//to check
    void SortRequest();
    Vehicle get_vehicle(Node& tempnode);
    int getVehicleID(LoadRequest &Req);

    
    //vector of distance and request
    std::vector<LoadRequest> requests;
    std::vector<std::vector<double>> distance;//we didn't initialize the size
    std::vector<Vehicle> vehicles;
    vector<int> unrouted_request_id;
    vector<int> sorted_request_id;//defined in SortRequest
    Node depot;
    Node returndepot;
    std::vector<std::vector<bool>> request_feasibility;

    //
    int p_seed;


    int num_of_request;
    double rest_time;
    double lunch_duration;//to use here or in vehicle
    int vehicle_type_number;
    double max_travel_time;
    std::string instance_name;
};
#endif