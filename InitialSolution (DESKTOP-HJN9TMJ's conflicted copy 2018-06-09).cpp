#include "Solution.h"
#include "Problem.h"
#include "InitialSolution.h"
#include <vector>
using namespace std;
InitialSolution::InitialSolution()
{}
InitialSolution::~InitialSolution()
{}
Solution InitialSolution::InitialSolve(Problem& p, Solution& S)
{
//    vector<SingleTrip> temptrip;
    vector<int> temp_unrouted_custumer;
    temp_unrouted_custumer.resize(p.num_of_request);
    //assign unrouted customer to temp_unrouted_customer
    int j;
    for(unsigned i=0;i< p.sorted_request_id.size();i++)
    {
        temp_unrouted_custumer[i]=p.sorted_request_id[i];
    }
    cout<<"temp_unrouted_and_sorted_custumer assigned"<<endl;
    LoadRequest tempreq=p.requests[temp_unrouted_custumer[0]-1];//-1 done to get index
    int required_vehicle=p.getVehicle(tempreq.pickup);
    while(!temp_unrouted_custumer.empty())
    {
        for(auto i=0;i<=GlobalTrips[i].size();i++)
        {
            if(GlobalTrips[i].lunchservice!=1)//to check using a variable;

            //define new variable;
            {
                int index_i=-1,index_j=-1;
                for(auto j=1;j<GlobalTrips[i].size()-1;i++)//first index
                {
                    for(auto k=j+1;k<GlobalTrips[i].size();k++)//second index
                    bool feas=Checkfeasibilisty(i,j,k,p,S);
                    
                }
            }
        }
    }
}

bool ShrinkTrip_FeasiblityCheckingforSingleTrip(int pos1,int pos2,  Problem& p,requestidindex)
{
    //see the structure of entering the number in the vector;
    double early_start=p.depot.start_time ,late_start=p.depot.end_time,duration=0,capacity=0;
    bool feasible = true;
    
    for (auto i=1;i<trip.size();i++)//checking and feasibility and finding the time window
    {
        Node N0=p.getNode(trip[i-1]);
        Node Ni=p.getNode(trip[i]);
        double s0=N0.service_time;
        double d01=p.distance[trip[i-1]][trip[i]];
        double tempweight=Ni.demand;
        double ei=Ni.start_time;
        double li=Ni.end_time;
        // store service time and distancehere and add them;
        // also find li and ei and ci and typecons-i
        early_start+=d01+s0;
        late_start+=d01+s0;
        duration+=d01+s0;
        capacity+=tempweight;
        if(early_start>li)
        {
            feasible=false;
        }
        else if (late_start<ei)
        {
            duration=duration+(ei-early_start);
            early_start=ei;late_start=ei;
        }
        else
        {
            early_start=max(early_start,ei);
            late_start=min(late_start,li);
        }
        if(duration>)//to complete
        {
            feasible=false;
        }
        else{
            trip.depot_start_timen=early_start-duratio;
            trip.late_start_time=late_start-duration;
        }
        if(feasible)//checking for capacity constraint
        {
            capacity+=tempweight;
            if(capacity<=)//complete this
        }
        if(feasible)//checking for multitrip constraint
        {
            feasible=MultiTripFeasibility(multitrip(vector of singletrip), vehicleno, )
        }
        //check capacity constraint everytime, check duration contraint everytime; check time wndow constraint
        //can we use static here to define a fix quantity;
    }
    //feasible is true then check the feasibility of multitrip
}

// void MultiTripFeasiblity(vectorofSingleTrips)//singletripshould contain 
// {
//     //sort the multitrip according to the time earlier time window in the vector;tie broken based on minimum duration
//     //greedy method to obtain the start time windows;    
//     sort multitrip on the basis of trip starttime window and then on duration;
//     for(auto i1=sortedtrip.begin()+1;i1!=sortedtrip.end();i1++)
//     {
//         i1.depot_start_time=max((i1-1).depot_start_time+(i1-1).trip_duration+rest_time,il.depot_start_time);
//         if(E1>i1.late_start_time)
//         {
//             feasible=false;
//         }
//     }
//     if(feasible)
//     {
//         //reduce the time windows
//         //for (auto it = my_vector.rbegin(); it != my_vector.rend(); ++it)
// //         for (vector<my_class>::reverse_iterator i = my_vector.rbegin(); 
// //         i != my_vector.rend(); ++i ) { 
// // } 
//         for(auto it=my_vector.rbegin()-1; it != my_vector.rend(); ++it)
//         {
//             it.late_start_time=min(it.late_start_time, (it+1).late_start_time-it.duration-rest_time;

//         }
//     }
//     else(
//         //permute the combination
//     )
//     return feasible;
//     //E(i)=max(E(i-1)+D(i-1)+delta,E(i))
//     //L(i)=min::(L(i+1)-D(i)-delta,L(i))
//     //
// }

// //multitrip contain vectors of singletrip but we should always be dealing with vector of whole single trip
// //we should be able to delete singletrip from the multitrip vector and that should remain in the singletrip vector
// //we can sort the singletrip on the basis of different things;
// // https://stackoverflow.com/questions/31179517/c-same-object-in-two-different-vectors