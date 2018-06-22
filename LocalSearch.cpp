#include <ctime>
#include <algorithm>
#include <iostream>
#include "Problem.h"
#include "Solution.h"
#include "LocalSearch.h"
#include "Operator.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>   /* time */

using namespace std;


LocalSearch::LocalSearch()
{}


LocalSearch::~LocalSearch()
{}
// std::sort(solution.MTrips.begin(),solution.MTrips.end(),[p](VehicleTrips& VT1,VehicleTrips& VT2)->bool//compare_vehicletype_size_desc
// {
//     if(VT1.TripVehicle.type<VT2.TripVehicle.type) return true;
//     else if(VT2.Multi.size()<VT1.Multi.size()) return true;
//     else return false;
// });
// solution.displaySolution();
//int VTid=-1;

bool SearchforAllFalse(const std::vector<bool>& vec)
{
    cout<<"search all false ";
    for(bool i:vec)
    {
        cout<<i<<" ";
        if (i==0)
        {
            
            return true;
        }
    }
    return false;
}

void LocalSearch::LocalSearch_operator1(Solution& solution,const Problem& p)
{
    //purpose is to shift the trip to lower capacity trip if possible
    Solution oldSolution=solution;//check it
    solution.displaySolution();


//Problem of singletrip??
    for(auto vehicle_trip_it=solution.MTrips.begin();
        vehicle_trip_it!=solution.MTrips.end();vehicle_trip_it++)
    {
        std::vector<int> trips=vehicle_trip_it->Multi;
        cout<<"vehicletripsnumber"<<trips<<endl;
        for(auto singletrip_it=trips.begin();
        singletrip_it<trips.end();singletrip_it++)
        {
            SingleTrip& tempStrip=solution.GlobalTrips[*singletrip_it];
            if(tempStrip.islunchtrip==0)
            {
                for(auto second_vehicle_trip_it=vehicle_trip_it+1;
                    second_vehicle_trip_it!=solution.MTrips.end() && second_vehicle_trip_it != vehicle_trip_it;
                    second_vehicle_trip_it++)
                {
                    //check first for type difference
                    //this guarantees that type_const constraint won't be violated as we are inserting trip from vt1 to vt2
                    if(vehicle_trip_it->TripVehicle.type > second_vehicle_trip_it->TripVehicle.type)
                    {
                        //std::cout<<"vehicle_trip_it->TripVehicle.type" <<" second_vehicle_trip_it->TripVehicle.type"<<std::endl;

                        //check the maximum weight capacity
                        if(tempStrip.max_weight <= second_vehicle_trip_it->TripVehicle.Capacity)//capacity check
                        {
                            std::cout<<"first vehicle totaltrip cost-pre "<<vehicle_trip_it->Calculate_Cost_of_MultiTrip(p,&solution)<<std::endl;
                            std::cout<<"second vehicle totaltrip cost-pre "<<second_vehicle_trip_it->Calculate_Cost_of_MultiTrip(p,&solution)<<std::endl;


                            int tripAddedNumber=*singletrip_it;
                            second_vehicle_trip_it->Multi.push_back(tripAddedNumber);
                            bool multifeas=MultiTripFeasiblity(*second_vehicle_trip_it,p,solution);
                            cout<<"beforemultifeas"<<endl;
                            if(multifeas)
                            {
                                cout<<"aftermultifeas"<<endl;

                                tempStrip.vehicletrip_id=second_vehicle_trip_it->vehicletrip_id;//vehicletripid changed

                                std::vector<int>& vec = vehicle_trip_it->Multi; // using shorter name
                                vec.erase(std::remove(vec.begin(), vec.end(), tripAddedNumber), vec.end());

                                //distance display

                                // vehicle_trip_it->distance_of_vehicletrip-=tempStrip.trip_distance;
                                // second_vehicle_trip_it->distance_of_vehicletrip=+tempStrip.trip_distance;
                                cout<<"tripAddedNumber"<<tripAddedNumber<<endl;
                                cout<<"first_vehicle_trip_it->vehicletrip_id"<<vehicle_trip_it->vehicletrip_id<<std::endl;
                                cout<<"second_vehicle_trip_it->vehicletrip_id"<<second_vehicle_trip_it->vehicletrip_id<<std::endl;
                                std::cout<<"first vehicle totaltrip cost-post "<<vehicle_trip_it->Calculate_Cost_of_MultiTrip(p,&solution)<<std::endl;
                                cout<<"second vehicle totaltrip cost-post "<<second_vehicle_trip_it->Calculate_Cost_of_MultiTrip(p,&solution)<<std::endl;
                                std::cout<<endl;
                                cout<<tempStrip.cust_id;
                                break;
                                

                            }
                            else
                            {
                                cout<<"beforeerase"<<endl;
                                std::vector<int>& vec = second_vehicle_trip_it->Multi; // using shorter name
                                vec.erase(std::remove(vec.begin(), vec.end(), tripAddedNumber), vec.end());
                                cout<<"aftererase"<<endl;   
                            }
                                
                        }
                    }
                    // else if()
                    // {

                    // }
                    // else
                    // {

                    // }
                }
            }
        }
    }
    cout<<"going out of Localsearch1"<<endl;
   // cout<<"The request can't be assigned to already present vehicle so new MultiTrip is being created"<<endl;
}
bool LocalSearch::LocalSearch_operator2(Solution& solution,const Problem& p)//a customer relocation
{

    Solution Sbest=solution;
    int request_id;//gives the id of the 
    unsigned GlobalTrip_size=solution.GlobalTrips.size();
    double cost=0;//vehicle removed(if)+customerunassigned+tripdistanceremoved
    int static seed=0;
    std::vector<SingleTrip>::iterator iter;
    int RandomTripNumber;
    int customer_id;
    while(true)
    {
        // srand(time(NULL));
        bool bookbool=SearchforAllFalse(m_LS2_bookkeep);
        if(bookbool==false)
        {
            return bookbool;
        }
        srand(seed++);  
        /*find a random trip which is non empty*/
        RandomTripNumber=rand()%GlobalTrip_size;
        cout<<"randomtripnumber="<<RandomTripNumber<<endl;//debug

        /* iter represent the current singletrip which customer we want to remove*/
        iter = std::next(solution.GlobalTrips.begin(), RandomTripNumber);
        SingleTrip temps=*iter;
        if(iter->cust_id.size()>2)/*size of randomtrips more than 2*/
        {
            cost=0;
            int remove_id_location=(rand()%(iter->cust_id.size()-2))+1;
            cout<<"remove_id_location=="<<remove_id_location<<endl;
            customer_id=iter->cust_id[remove_id_location];//customer_id is the pickup/delivery id
            customer_id=p.getRequestID(customer_id);//customer_id contain the index of request_id
            cout<<"cust_id_index=="<<customer_id<<endl;
            double old_traveldistance=iter->trip_distance;
            if(m_LS2_bookkeep[customer_id]==0)
            {
                Erase_ID_from_Trip(iter,customer_id,solution,p);

                double new_traveldistance=iter->trip_distance;
                cout<<"old_traveldistance"<<old_traveldistance<<"new_traveldistance"<<new_traveldistance<<endl;
                cost+=((new_traveldistance-old_traveldistance)*solution.MTrips[iter->vehicletrip_id].TripVehicle.variable_cost);
                cout<<"cost == "<<cost<<endl;
                if(cost<0)
                {
                    solution.unrouted_cust_request_id.push_back(customer_id);//inserted the index
                    break;
                }
                else
                {
                    *iter=temps;
                    m_LS2_bookkeep[customer_id]=1;                    
                }
            }
        }
    }
   int request_index=solution.unrouted_cust_request_id[rand()%solution.unrouted_cust_request_id.size()];//contain index of request in request
    bool inserted=BestInsertPlace(request_index, p, solution, cost,m_LS2_bookkeep);
    if(iter->cust_id.size()==2&&inserted)
    {
        solution.MTrips[iter->vehicletrip_id].Multi.erase(std::remove(solution.MTrips[iter->vehicletrip_id].Multi.begin(), solution.MTrips[iter->vehicletrip_id].Multi.end(),//next line
                                                                    RandomTripNumber), solution.MTrips[iter->vehicletrip_id].Multi.end());
        if(solution.MTrips[iter->vehicletrip_id].Multi.size()==1/*check that it is also lunch trip*/)
        {
            cout<<"extra vehicle trip erased+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++vehicleerased"<<endl;
            solution.MTrips.erase(solution.MTrips.begin()+iter->vehicletrip_id);//have to update the solution also and also the removed singletrip vector
            solution.updateMTrips();
            
            // solution.MTrips.erase(std::remove(solution.MTrips.begin(), solution.MTrips.end(),solution.MTrips[iter->vehicletrip_id]), solution.MTrips.end());//have to update the solution also;
        }
        // solution.unallocatedGlobalID.pushback
    }
    if(!inserted)
    {
        m_LS2_bookkeep[request_index]=1;
    }
    
    solution.displaySolution();
    solution.Calculate_Solution_Cost(p);
    if(Sbest.total_solution_cost<solution.total_solution_cost)
    {
        solution=Sbest;
    }
    return inserted;
    // return false;

    //find a random customer from whole of the singletrip

}


void LocalSearch::LocalOpt( Solution& solution, const Problem& p)
{
    m_LS2_bookkeep.resize(p.num_of_request,0);//maybe a local variable
    for(unsigned i=0;i<p.num_of_request;i++)
    {
        m_LS2_bookkeep[i]=0;
    }
    int total_local_search_operators=4;
    //assumed the current solution to be the best solution available
    Solution Sbest=solution;
    //assume there are n types of localsearch operation
    int unsuccesful_attempt=0;
    LocalSearch_operator1(solution,p);
     for(int i=0;unsuccesful_attempt<total_local_search_operators;(++i)%=total_local_search_operators)
    {
        if(i==0)
        {
            while(LocalSearch_operator2( solution,  p))
            {
                unsuccesful_attempt=0;
            }
            unsuccesful_attempt++;
        }
    }
    //     if(i==1)
    //     {
    //         while(LocalSearch_operator2( solution, p))
    //         {
    //             unsuccesful_attempt=0;
    //         }
    //         unsuccesful_attempt++;

    //     }
    //     if (i==2)
    //     {
    //         while(LocalSearch_operator3( solution, p))
    //         {
    //             unsuccesful_attempt=0;
    //         }
    //         unsuccesful_attempt++;
    //     }
    // }
}