#include "Solution.h"
#include "Problem.h"
#include "LoadRequest.h"
#include <iostream>
#include <algorithm>
#include <stdlib.h>

#include <vector>
using std::vector;
#include <iostream>
using std::ostream;

template<typename T>
ostream& operator<< (ostream& out, const vector<T>& v) {
    out << "{";
    size_t last = v.size() - 1;
    for(size_t i = 0; i < v.size(); ++i) {
        out << v[i];
        if (i != last) 
            out << ", ";
    }
    out << "}";
    return out;
}

SingleTrip::SingleTrip(){
    depot_early_start_time=trip_cost=trip_duration=max_weight=0;
    vid=0;//number_of_nodes_visited// trip_number
    fix_trip=0;
    vehicletrip_id=-1;
    depot_late_start_time=1440;
    islunchtrip=0;
    trip_distance=0;
    servicetime=0;
    waitingtime=0;
    //v_type=0;
    
}
SingleTrip::~SingleTrip(){
    //cout<<"SingleTrip is destroyed\n\n";
}

double SingleTrip::Calculate_Trip_Distance(const Problem& p)
{
    //cout<<"ST"<<" ";
    trip_distance=0;
    for(unsigned i=1;i<cust_id.size();i++)
        trip_distance+=p.distance[cust_id[i]][cust_id[i-1]];
    return trip_distance;
}
//vector<SingleTrip> GlobalTrips;

//--------------------vehicletrips definitions below
VehicleTrips::VehicleTrips(){
    num_of_trips=0;//size()-1;
    cost_of_vehicletrip=0;
    vehicletype=0;
    distance_of_vehicletrip=0;
    vehicletrip_id=-1;    
}
VehicleTrips::~VehicleTrips(){
   // cout<<"VehicleTrip is destroyed\n\n";
}
double VehicleTrips::Calculate_Cost_of_MultiTrip(const Problem& p,  Solution* S)
{
    cost_of_vehicletrip=TripVehicle.fixed_cost;
    distance_of_vehicletrip=0;
    for(auto tripiterator=Multi.begin();tripiterator!=Multi.end();tripiterator++)
    {
        distance_of_vehicletrip+=S->GlobalTrips[*tripiterator].Calculate_Trip_Distance(p);
        //cost_of_vehicletrip+=(S->GlobalTrips[*tripiterator].Calculate_Trip_Distance(p))*TripVehicle.variable_cost;
    }
    cost_of_vehicletrip+=distance_of_vehicletrip*TripVehicle.variable_cost;
    return cost_of_vehicletrip;
}
//--------------------------------solution definition below
Solution::Solution(){
    total_solution_cost=0;num_of_vehicles=0;total_solution_cost=0;
    solution_distance_travelled=0;
}

Solution::~Solution(){
  //  cout<<"Solution is destroyed\n\n";
}

void Solution::Calculate_Solution_Cost(const Problem& p)
{
    double C1=1000,C2=1;
    total_solution_cost=0;
    for(auto Vehicletripiterator=MTrips.begin();Vehicletripiterator!=MTrips.end();Vehicletripiterator++)
    {
        // solution_distance_travelled +=(Vehicletripiterator->Calculate_Cost_of_MultiTrip(p,this));

        total_solution_cost+=C2*(Vehicletripiterator->Calculate_Cost_of_MultiTrip(p,this));
        solution_distance_travelled+=Vehicletripiterator->distance_of_vehicletrip;  
        //cout<<"hi"<<endl;
    }    
    total_solution_cost+=C1*unrouted_cust_request_id.size();
    cout<<"Total Solution Cost   "<<total_solution_cost<<"*********----------"<<endl;
}

void Solution::displaySolution()
{
    //display number of vehicletrips; //then vehicle type of each vehicletrip
    //how many trips in each vehicletrip// and for each trip display the start time, and duration
    // and then the node is each route
    cout<<"----------------------Solution Display---------------------------"<<endl;
    cout<<"Number of Vehicle to be used is "<<MTrips.size()<<"."<<endl<<endl;
    for(auto i=0;i<MTrips.size();i++)
    {
        cout<<"The vehicle Type is "<<MTrips[i].TripVehicle.type<<endl;

     //   cout<<"Total Trip(including lunch) in the VehicleTrip Number "<<i+1<<" is "<<MTrips[i].Multi.size()<<endl;
       // cout<<"VehicleTypeused is"<< MTrips[i].TripVehicle.type<<endl;
        int k=0;
        Solution * s = this;
        //int a =0, b = 0;
        
        std::sort(MTrips[i].Multi.begin(),MTrips[i].Multi.end(),[s](int a, int b)->bool {
            // Lambda function
         //   s->GlobalTrips
        if(s->GlobalTrips[a].depot_early_start_time<s->GlobalTrips[b].depot_early_start_time)
            return true;
        else if(s->GlobalTrips[a].depot_early_start_time==s->GlobalTrips[b].depot_early_start_time
                &&  s->GlobalTrips[a].trip_duration<s->GlobalTrips[b].trip_duration)
            return true;
        return false;
        });

        double starttime=GlobalTrips[ MTrips[i].Multi[0]].depot_early_start_time;                  
        for(auto j:MTrips[i].Multi)
        {
            //we can also make a display function for singletrip
            k++;      
            SingleTrip& temps=GlobalTrips[j];
            if(!temps.islunchtrip)
            {
                cout<<"SingleTripNumber="<<j<<" ";
                cout<<"This is the Trip Number "<<k;
                cout<<temps.cust_id<<"     ";
                cout<<" earlystarttime "<< temps.depot_early_start_time<<" " <<starttime<<" <latestarttime"<<temps.depot_late_start_time<<"  && duration"<<temps.trip_duration<<endl;
                starttime=starttime+temps.trip_duration+30.;//MTrips[i].TripVehicle.lunch_end;
                starttime=max(starttime,temps.depot_early_start_time);    
            }
            else
            {
                cout<<"This is Lunch Trip for this vehicle";
                cout<<starttime<<" <latestarttime"<<temps.depot_late_start_time<<endl;
                starttime=starttime+temps.trip_duration;                
            }
            
        }
        for(auto i:unrouted_cust_request_id)
        {
            cout<<i<<" ";
        }
        cout<<endl;
    }
    
}




void Solution::updateMTrips()
{
    for(auto i=0;i!=MTrips.size();i++)
    {
        MTrips[i].vehicletrip_id=i;
        for(auto j=MTrips[i].Multi.begin();j!=MTrips[i].Multi.end();j++)
        {
            GlobalTrips[*j].vehicletrip_id=i;
        }
    }
}
//https://stackoverflow.com/questions/2029272/how-to-declare-a-global-variable-that-could-be-used-in-the-entire-program

// void MultipleTrip::Calculate_Cost_of_MultiTrip(const Problem& p)
// {
//     unsigned i=0;
//     while(i!=Trips.size())
//     {
//         cost_of_mtrip+=Trips[i].trip_cost;
//         i++;
//     }
// }
// void VehicleTrips::createSingleTrip(Problem &p, LoadRequest &Req)
// {
//     SingleTrip temp_strip;
//     temp_strip.cust_id.push_back(p.depot.id);
//     temp_strip.cust_id.push_back(Req.pickup.id);
//     temp_strip.cust_id.push_back(Req.delivery.id);
//     temp_strip.cust_id.push_back(p.depot.id);
// //    temp_strip.begin_time.push_back(Req.pickup.start_time-p.distance[0][Req.pickup.id]);
//   //  temp_strip.begin_time.push_back(Req.pickup.start_time);
//     //temp_strip.begin_time.push_back();
//     temp_strip.v_type=vehicletype;
//     //temp_strip.check_fix_trip();
//     Trips.push_back(temp_strip);
    
// }
// bool compareTripEarlyStartTime_duration2(int a, int b)
// {
//     if(GlobalTrips[a].depot_early_start_time<GlobalTrips[b].depot_early_start_time)
//         return true;
//     else if(GlobalTrips[a].depot_early_start_time==GlobalTrips[b].depot_early_start_time
//             &&  GlobalTrips[a].trip_duration<GlobalTrips[b].trip_duration)
//         return true;
//     return false;
// }