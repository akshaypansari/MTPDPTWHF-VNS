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
    // cout<<"search all false ";
    for(bool i:vec)
    {
        // cout<<i<<" ";
        if (i==0)
        {
            
            return true;
        }
    }
    return false;
}




bool LocalSearch::LocalSearch_operator1(Solution& solution,const Problem& p)
{
    //purpose is to shift the trip to lower capacity trip if possible
    Solution oldSolution=solution;//check it
    bool mainbreak=false;
    for(auto vehicle_trip_it=solution.MTrips.begin();
        vehicle_trip_it!=solution.MTrips.end() &&mainbreak==false;vehicle_trip_it++)
    {
        std::vector<int> trips=vehicle_trip_it->Multi;
        for(auto singletrip_it=trips.begin();
        singletrip_it<trips.end()&&mainbreak==false;singletrip_it++)
        {
        // cout<<"err2 ";

            SingleTrip& tempStrip=solution.GlobalTrips[*singletrip_it];
            if(tempStrip.islunchtrip==0)
            {
                for(auto second_vehicle_trip_it=solution.MTrips.begin();
                    second_vehicle_trip_it!=solution.MTrips.end() && second_vehicle_trip_it != vehicle_trip_it &&mainbreak==false;
                    second_vehicle_trip_it++)
                {
                    if(vehicle_trip_it->TripVehicle.type > second_vehicle_trip_it->TripVehicle.type)
                    {
                       
                    //check first for type difference
                    //this guarantees that type_const constraint won't be violated as we are inserting trip from vt1 to vt2

                                //check the maximum weight capacity
                                if(tempStrip.max_weight <= second_vehicle_trip_it->TripVehicle.Capacity)//capacity check
                                {
                                    // std::cout<<"first vehicle totaltrip cost-pre "<<vehicle_trip_it->Calculate_Cost_of_MultiTrip(p,&solution)<<std::endl;
                                    // std::cout<<"second vehicle totaltrip cost-pre "<<second_vehicle_trip_it->Calculate_Cost_of_MultiTrip(p,&solution)<<std::endl;


                                    int tripAddedNumber=*singletrip_it;
                                    second_vehicle_trip_it->Multi.push_back(tripAddedNumber);

                                    bool multifeas=MultiTripFeasiblity(*second_vehicle_trip_it,p,solution);
                                    if(multifeas)
                                    {
                                        tempStrip.vehicletrip_id=second_vehicle_trip_it->vehicletrip_id;//vehicletripid changed

                                        std::vector<int>& vec = vehicle_trip_it->Multi; // using shorter name
                                        vec.erase(std::remove(vec.begin(), vec.end(), tripAddedNumber), vec.end());
                                        if(vec.size()==1)
                                        {                                        
                                            // mainbreak==true;
                                            // return true;
                                            cout<<"extra vehicle trip erased inside Local Operator1+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++vehicleerased"<<endl;
                                            int temp=vec[0];
                                            // getchar();
                                            solution.servedLunchTrips.erase(std::remove(solution.servedLunchTrips.begin(), solution.servedLunchTrips.end(),//next line
                                                                                        temp), solution.servedLunchTrips.end());
                                            solution.unservedLunchTrips.push_back(temp);
                                            // solution.MTrips.erase(solution.MTrips.begin()+tempStrip.vehicletrip_id);//have to update the solution also and also the removed singletrip vector
                                            solution.MTrips.erase(vehicle_trip_it);//have to update the solution also and also the removed singletrip vector
                                            // getchar();
                                            solution.updateMTrips();

                                            mainbreak==true;
                                            return true;
                            
                                        }

                                        cout<<"tripAddedNumber"<<tripAddedNumber<<endl;
                                         cout<<"first_vehicle_trip_it->vehicletrip_id=="<<vehicle_trip_it->vehicletrip_id<<std::endl;
                                        cout<<"second_vehicle_trip_it->vehicletrip_id=="<<second_vehicle_trip_it->vehicletrip_id<<std::endl;
                                        // std::cout<<"first vehicle totaltrip cost-post "<<vehicle_trip_it->Calculate_Cost_of_MultiTrip(p,&solution)<<std::endl;
                                        cout<<"second vehicle totaltrip cost-post "<<second_vehicle_trip_it->Calculate_Cost_of_MultiTrip(p,&solution)<<std::endl;
                                        std::cout<<endl;
                                        // cout<<tempStrip.cust_id;
                                        return true;
                                        

                                    }
                                    else
                                    {
                                        // cout<<"beforeerase"<<endl;
                                        std::vector<int>& vec = second_vehicle_trip_it->Multi; // using shorter name
                                        vec.erase(std::remove(vec.begin(), vec.end(), tripAddedNumber), vec.end());
                                        // cout<<"aftererase"<<endl;   
                                    }
                                        
                                }
                        
                    }
                else if(vehicle_trip_it->Multi.size()<=second_vehicle_trip_it->Multi.size()&&vehicle_trip_it->TripVehicle.type == second_vehicle_trip_it->TripVehicle.type)
                    {
                
                    //check first for type difference
                    //this guarantees that type_const constraint won't be violated as we are inserting trip from vt1 to vt2
                            // if()
                            // {
                                //std::cout<<"vehicle_trip_it->TripVehicle.type" <<" second_vehicle_trip_it->TripVehicle.type"<<std::endl;

                                //check the maximum weight capacity
                                if(tempStrip.max_weight <= second_vehicle_trip_it->TripVehicle.Capacity)//capacity check
                                {
                                
                                    int tripAddedNumber=*singletrip_it;
                                    second_vehicle_trip_it->Multi.push_back(tripAddedNumber);
                                    // cout<<"5 ";
                                    bool multifeas=MultiTripFeasiblity(*second_vehicle_trip_it,p,solution);
                                    // cout<<"beforemultifeas"<<endl;
                                    if(multifeas)
                                    {
                                        // cout<<"6 ";
                                        // cout<<"aftermultifeas"<<endl;
                                        
                                        tempStrip.vehicletrip_id=second_vehicle_trip_it->vehicletrip_id;//vehicletripid changed

                                        std::vector<int>& vec = vehicle_trip_it->Multi; // using shorter name
                                        vec.erase(std::remove(vec.begin(), vec.end(), tripAddedNumber), vec.end());
                                        if(vec.size()==1)
                                        {
                                            // solution.displaySolution();
                                            cout<<"extra vehicle trip erased inside Local Operator1+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++vehicleerased"<<endl;
                                            int temp=vec[0];
                                            // getchar();
                                            solution.servedLunchTrips.erase(std::remove(solution.servedLunchTrips.begin(), solution.servedLunchTrips.end(),//next line
                                                                                        temp), solution.servedLunchTrips.end());
                                            cout<<"hi"<<endl;
                                            solution.unservedLunchTrips.push_back(temp);
                                            // solution.MTrips.erase(solution.MTrips.begin()+tempStrip.vehicletrip_id);//have to update the solution also and also the removed singletrip vector
                                            solution.MTrips.erase(vehicle_trip_it);//have to update the solution also and also the removed singletrip vector
                                            // getchar();
                                            // cout<<"hi1"<<endl;
                                            solution.updateMTrips();
                                            // cout<<"hi2"<<endl;

                                            mainbreak==true;
                                            return true;
                            
                                        }
                                        // vehicle_trip_it->distance_of_vehicletrip-=tempStrip.trip_distance;
                                        // second_vehicle_trip_it->distance_of_vehicletrip=+tempStrip.trip_distance;
                                        cout<<"tripAddedNumber"<<tripAddedNumber<<endl;
                                         cout<<"first_vehicle_trip_it->vehicletrip_id=="<<vehicle_trip_it->vehicletrip_id<<std::endl;
                                        cout<<"second_vehicle_trip_it->vehicletrip_id=="<<second_vehicle_trip_it->vehicletrip_id<<std::endl;
                                        // std::cout<<"first vehicle totaltrip cost-post "<<vehicle_trip_it->Calculate_Cost_of_MultiTrip(p,&solution)<<std::endl;
                                        cout<<"second vehicle totaltrip cost-post "<<second_vehicle_trip_it->Calculate_Cost_of_MultiTrip(p,&solution)<<std::endl;
                                        std::cout<<endl;
                                        // cout<<tempStrip.cust_id;
                                        return true;                                    
                                    }
                                    else
                                    {
                                        // cout<<"beforeerase"<<endl;
                                        std::vector<int>& vec = second_vehicle_trip_it->Multi; // using shorter name
                                        vec.erase(std::remove(vec.begin(), vec.end(), tripAddedNumber), vec.end());
                                        // cout<<"aftererase"<<endl;   
                                    }
                                        
                                }
                    }
                }
            }
        }
    }
    // cout<<"going out of Localsearch1"<<endl;
    return false;
   // cout<<"The request can't be assigned to already present vehicle so new MultiTrip is being created"<<endl;
}


bool LocalSearch::LocalSearch_operator2(Solution& solution,const Problem& p)//a customer relocation
{

    Solution Sbest=solution;
    int request_id;//gives the id of the 
    unsigned TrueSingleTrip_size=solution.servedSingleTrips.size();
    double cost=0;//vehicle removed(if)+customerunassigned+tripdistanceremoved
    int static seed=0;
    std::vector<SingleTrip>::iterator iter;
    int RandomTripNumber;
    int customer_id;
    int iteration=0;
    double fixedCost=100000;
    while(iteration<p.num_of_request)
    {
        // cout << "In LS 2 while loop" << endl;
        // srand(time(NULL));
        bool bookbool=SearchforAllFalse(m_LS2_bookkeep);
        if(bookbool==false)
        {
            return bookbool;
        }
        // srand(seed++);  
        /*find a random trip which is non empty*/
        RandomTripNumber=rand()%TrueSingleTrip_size;
        RandomTripNumber=solution.servedSingleTrips[RandomTripNumber];
        // cout<<"randomtripnumber="<<RandomTripNumber<<endl;//debug

        /* iter represent the current singletrip which customer we want to remove*/
        iter = std::next(solution.GlobalTrips.begin(), RandomTripNumber);
        
        SingleTrip temps=*iter;
        if(iter->cust_id.size()>2)/*size of randomtrips more than 2*/
        {
            cost=0;
            int remove_id_location=(rand()%(iter->cust_id.size()-2))+1;
            // cout<<"remove_id_location=="<<remove_id_location<<endl;
            customer_id=iter->cust_id[remove_id_location];//customer_id is the pickup/delivery id
            customer_id=p.getRequestID(customer_id);//customer_id contain the index of request_id
            // cout<<"cust_id_index=="<<customer_id<<endl;
            double old_traveldistance=iter->trip_distance;
            if(m_LS2_bookkeep[customer_id]==0)
            {
                Erase_ID_from_Trip(iter,customer_id,solution,p);
                double new_traveldistance=iter->trip_distance;
                // cout<<"old_traveldistance"<<old_traveldistance<<"new_traveldistance"<<new_traveldistance<<endl;
                cost+=((new_traveldistance-old_traveldistance)*solution.MTrips[iter->vehicletrip_id].TripVehicle.variable_cost);
                if(iter->cust_id.size()==2 &&solution.MTrips[iter->vehicletrip_id].Multi.size()==2)
                {
                    fixedCost=solution.MTrips[iter->vehicletrip_id].TripVehicle.fixed_cost;
                    cost-=fixedCost;
                }
                // cout<<"cost == "<<cost<<endl;
                if(cost<0)
                {
                    solution.unrouted_cust_request_id.push_back(customer_id);//inserted the index
                    break;
                }
                else
                {
                    iteration++;
                    *iter=temps;
                    m_LS2_bookkeep[customer_id]=1;                    
                }
            }
            else{
                iteration++;
            }
        }
        else
        {
            cout<<"trip size mismatch"<<endl;
            getchar();
            iteration++;
        }
    }
    if(iteration==p.num_of_request)
    {
        return false;
    }
    int request_index=solution.unrouted_cust_request_id[rand()%solution.unrouted_cust_request_id.size()];//contain index of request in request
    bool inserted=BestInsertPlace(request_index, p, solution, cost,m_LS2_bookkeep, fixedCost);
    cout<<"iter->custid.size()=="<<iter->cust_id.size()<<endl;
    if(iter->cust_id.size()==2&&inserted)
    {
        cout<<"vehicleTripIDthat is erased==="<<RandomTripNumber<<" Vehicle_ID" <<iter->vehicletrip_id<< endl;
        solution.MTrips[iter->vehicletrip_id].Multi.erase(std::remove(solution.MTrips[iter->vehicletrip_id].Multi.begin(), solution.MTrips[iter->vehicletrip_id].Multi.end(),//next line
                                                                    RandomTripNumber), solution.MTrips[iter->vehicletrip_id].Multi.end());
        solution.servedSingleTrips.erase(std::remove(solution.servedSingleTrips.begin(), solution.servedSingleTrips.end(),//next line
                                                                    RandomTripNumber), solution.servedSingleTrips.end());
        solution.unservedSingleTrips.push_back(RandomTripNumber);                                                                                                                               
        if(solution.MTrips[iter->vehicletrip_id].Multi.size()==1/*check that it is also lunch trip*/)
        {
            cout<<"extra vehicle trip erased+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++vehicleerased"<<endl;
            int temp=solution.MTrips[iter->vehicletrip_id].Multi[0];
            // getchar();
            solution.servedLunchTrips.erase(std::remove(solution.servedLunchTrips.begin(), solution.servedLunchTrips.end(),//next line
                                                         temp), solution.servedLunchTrips.end());
            cout<<"hi"<<endl;
            solution.unservedLunchTrips.push_back(temp);
            solution.MTrips.erase(solution.MTrips.begin()+iter->vehicletrip_id);//have to update the solution also and also the removed singletrip vector
            solution.updateMTrips();
            
            // solution.MTrips.erase(std::remove(solution.MTrips.begin(), solution.MTrips.end(),solution.MTrips[iter->vehicletrip_id]), solution.MTrips.end());//have to update the solution also;
        }
    }
    if(!inserted)
    {
        m_LS2_bookkeep[request_index]=1;
    }
    
    // solution.displaySolution();
    solution.Calculate_Solution_Cost(p);
    // cout<<"founderrorfirst "<<endl;    
    if(Sbest.total_solution_cost<solution.total_solution_cost)
    {
        solution=Sbest;
    // cout<<"founderrorinside"<<endl;

    }
    // cout<<"founderror"<<endl;
    return inserted;
    // return false;

    //find a random customer from whole of the singletrip

}

bool LocalSearch :: LocalSearch_operator3(Solution& solution,const Problem& p)//swap
{
        Solution Sbest=solution;

    // for(int noofwrongiteration=0;noofwrongiteration<p.num_of_request/2;noofwrongiteration++)
    // {
        // cout<<"LS3"<<"\n";
        std::vector<SingleTrip>::iterator iter1;
        std::vector<SingleTrip>::iterator iter2;

        LoadRequest req1,req2;
        unsigned TrueSingleTrip_size=solution.servedSingleTrips.size();

        double cost=0.00;//vehicle removed(if)+customerunassigned+tripdistanceremoved
        int static seed=0;
        int RandomTripNumber1,RandomTripNumber2;
        int customer_id1,customer_id2;
        int vehiclenum1,vehiclenum2;
        int iteration=0;
        while (iteration<100)
        {



            RandomTripNumber1=rand()%TrueSingleTrip_size;
            RandomTripNumber1=solution.servedSingleTrips[RandomTripNumber1];
            
            /* iter1 represent the current singletrip which customer we want to remove*/
            iter1 = std::next(solution.GlobalTrips.begin(), RandomTripNumber1);//solution.GlobalTrips[RandomNumber1].islunchtrip==0 continue
            SingleTrip temps1=*iter1;

            if(iter1->cust_id.size()<=2)//
            {
                iteration++;
                continue;
            }
            RandomTripNumber2=rand()%TrueSingleTrip_size;
            RandomTripNumber2=solution.servedSingleTrips[RandomTripNumber2];
            // RandomTripNumber2=rand()%GlobalTrip_size;        
            iter2= std::next(solution.GlobalTrips.begin(), RandomTripNumber2);
            SingleTrip temps2=*iter2;

            if(iter2->cust_id.size()<=2 || RandomTripNumber1==RandomTripNumber2)
            {
                iteration++;
                continue;
            }
            vehiclenum1=iter1->vehicletrip_id;
            vehiclenum2=iter2->vehicletrip_id;
            // cout<<"3"<<endl;

            int remove_id_location1=(rand()%(iter1->cust_id.size()-2))+1;
            customer_id1=iter1->cust_id[remove_id_location1];//customer_id is the pickup/delivery id
            customer_id1=p.getRequestID(customer_id1);//customer_id contain the index of request_id

            int remove_id_location2=(rand()%(iter2->cust_id.size()-2))+1;
            customer_id2=iter2->cust_id[remove_id_location2];//customer_id is the pickup/delivery id
            customer_id2=p.getRequestID(customer_id2);//customer_id contain the index of request_id



            //check cross vehicletrip feasibiity
            bool crossfirst=bool_Check_Feasibility(vehiclenum1,customer_id2,p, solution);
            bool crossSecond=bool_Check_Feasibility(vehiclenum2,customer_id1,p , solution);
            if(crossfirst && crossSecond)
            {
                cost=0;
                double old_traveldistance1=iter1->trip_distance;
                double old_traveldistance2=iter2->trip_distance;

                // Erase from both the customer ids the the respective location;

                Erase_ID_from_Trip(iter1,customer_id1,solution,p);
                Erase_ID_from_Trip(iter2,customer_id2,solution,p);


                double new_traveldistance1=iter1->trip_distance;
                double new_traveldistance2=iter2->trip_distance;
                
                // cost=
                cost+=( ((new_traveldistance1-old_traveldistance1)*solution.MTrips[iter1->vehicletrip_id].TripVehicle.variable_cost)
                    + ((new_traveldistance2-old_traveldistance2)*solution.MTrips[iter2->vehicletrip_id].TripVehicle.variable_cost) );
                if(cost<0)
                {

                    break;
                }
                else
                {
                    *iter1=temps1;
                    *iter2=temps2;
                    iteration++;
                    continue;
                }
                
            }
            else
            {
                    iteration++;
                continue;
            }
        

        }

        bool boolcheck=bool_insert_customer(*iter1,*iter2,customer_id2,customer_id1,p,solution,cost);

        if(boolcheck)
        {
            return true;
        }
        else
        {
            solution=Sbest;
            return false;

        }
    
// return false;


}



void LocalSearch::LocalOpt( Solution& solution, const Problem& p)
{
    m_LS2_bookkeep.resize(p.num_of_request,0);//maybe a local variable
    for(unsigned i=0;i<p.num_of_request;i++)
    {
        m_LS2_bookkeep[i]=0;
    }
    int total_local_search_operators=solution.servedSingleTrips.size()*solution.servedSingleTrips.size();
    //assumed the current solution to be the best solution available
    Solution Sbest=solution;
    //assume there are n types of localsearch operation
    int unsuccesful_attempt=0;
    LocalSearch_operator1(solution,p);
     for(int i=0;unsuccesful_attempt<total_local_search_operators;(++i)%=total_local_search_operators)
    {
        // cout<<"i="<<i<<endl;
         if(i%2==0)  
         {

            while(LocalSearch_operator2( solution,  p))
            {
                cout<<" pp2 ";
                for(unsigned j=0;j<p.num_of_request;j++)
                {
                    m_LS2_bookkeep[j] = 0;
                }
                unsuccesful_attempt = 0;
            }
            cout<< "LS2 ended" <<" unsuccessful attempt= "<<unsuccesful_attempt<<endl;
            unsuccesful_attempt++;
            // if(LocalSearch_operator1 ( solution, p ) )
            // {
            //     // cout<<"LS2&LS1"<<endl;
            //         for(unsigned j=0;j<p.num_of_request;j++)
            //     {
            //         m_LS2_bookkeep[j]=0;
            //     }
            // }
         }
    
        // cout<<"before LS3"<<endl;
        if(i%2==1)
        {
            // cout<<"inside LocalSearch3"<<endl;
            while(LocalSearch_operator3( solution, p))
            {
                // cout<<" pp ";
                for(unsigned j=0;j<p.num_of_request;j++)
                {
                cout<<" pp3 ";

                    m_LS2_bookkeep[j]=0;
                }
                unsuccesful_attempt=0;
            }
            unsuccesful_attempt++;
            if(LocalSearch_operator1( solution, p))
            {
                for(unsigned j=0;j<p.num_of_request;j++)
                {
                    m_LS2_bookkeep[j]=0;
                }
            }
        }  
        // if(i%3==2)
        // {
        //     cout<<"inside LocalSearch3"<<endl;
        //     while(LocalSearch_operator3( solution, p))
        //     {
        //         unsuccesful_attempt=0;
        //     }
        //     unsuccesful_attempt++;
        //     // LocalSearch_operator1( solution, p);
        // } 

        // }
        // if (i==2)
        // {
        //     while(LocalSearch_operator1( solution, p))
        //     {
        //         unsuccesful_attempt=0;
        //     }
        //     unsuccesful_attempt++;
        // }
    }
    // solution.MTrips[3].Multi[0];
    // solution.MTrips[2].Multi.push_back(solution.MTrips[3].Multi[0]);
    // int bool2=MultiTripFeasiblity(solution.MTrips[2],p,solution);
    // cout<<"bool "<<bool2;
    // getchar();
    // total_local_search_operators=0;
//     for(int i=0;unsuccesful_attempt<total_local_search_operators;(++i)%=total_local_search_operators)
//     {


//         if(i%2==1)
//         {
//             cout<<"inside LocalSearch3"<<endl;
//             while(LocalSearch_operator3( solution, p))
//             {
//                 unsuccesful_attempt=0;
//             }
//             unsuccesful_attempt++;
//             LocalSearch_operator1( solution, p);
            

//         }
// }
}
    //    for(int i=0;unsuccesful_attempt<total_local_search_operators;(++i)%=total_local_search_operators)
    // {
    // //      if(i==0)
    // //      {
    //         // while(LocalSearch_operator2( solution,  p))
    //         // {
    //         //     for(unsigned j=0;j<p.num_of_request;j++)
    //         //     {
    //         //         m_LS2_bookkeep[j]=0;
    //         //     }
    //         //     unsuccesful_attempt=0;
    //         // }
    //         // unsuccesful_attempt++;
    //         // LocalSearch_operator1( solution, p);
            
    // // // getchar();
    // //     }
    

    //     if(i==1)
    //     {
    //         cout<<"inside LocalSearch3"<<endl;
    //         while(LocalSearch_operator3( solution, p))
    //         {
    //             unsuccesful_attempt=0;
    //         }
    //         unsuccesful_attempt++;
    //         LocalSearch_operator1( solution, p);
            

    //     }
    //     // if (i==2)
    //     // {
    //     //     while(LocalSearch_operator1( solution, p))
    //     //     {
    //     //         unsuccesful_attempt=0;
    //     //     }
    //     //     unsuccesful_attempt++;
    //     // }
    // }
// }
// void LocalSearch::LocalSearch_operator1(Solution& solution,const Problem& p)
// {
//     //purpose is to shift the trip to lower capacity trip if possible
//     Solution oldSolution=solution;//check it
//     // solution.displaySolution();

//     bool mainbreak=false;
// //Problem of singletrip??
//     for(auto vehicle_trip_it=solution.MTrips.begin();
//         vehicle_trip_it!=solution.MTrips.end() && mainbreak==false;vehicle_trip_it++)
//     {
//         std::vector<int> trips=vehicle_trip_it->Multi;
//         cout<<"vehicletripsnumber"<<trips<<endl;
//         for(auto singletrip_it=trips.begin();
//         singletrip_it<trips.end();singletrip_it++)
//         {
//             SingleTrip& tempStrip=solution.GlobalTrips[*singletrip_it];
//             if(tempStrip.islunchtrip==0)
//             {
//                 for(auto second_vehicle_trip_it=solution.MTrips.begin();
//                     second_vehicle_trip_it!=solution.MTrips.end() && second_vehicle_trip_it != vehicle_trip_it &&mainbreak==false;
//                     second_vehicle_trip_it++)
//                 {
//                     if(vehicle_trip_it->Multi.size()<=second_vehicle_trip_it->Multi.size())
//                     {
//                     //check first for type difference
//                     //this guarantees that type_const constraint won't be violated as we are inserting trip from vt1 to vt2
//                             if(vehicle_trip_it->TripVehicle.type >= second_vehicle_trip_it->TripVehicle.type)
//                             {
//                                 //std::cout<<"vehicle_trip_it->TripVehicle.type" <<" second_vehicle_trip_it->TripVehicle.type"<<std::endl;

//                                 //check the maximum weight capacity
//                                 if(tempStrip.max_weight <= second_vehicle_trip_it->TripVehicle.Capacity)//capacity check
//                                 {
//                                     // std::cout<<"first vehicle totaltrip cost-pre "<<vehicle_trip_it->Calculate_Cost_of_MultiTrip(p,&solution)<<std::endl;
//                                     // std::cout<<"second vehicle totaltrip cost-pre "<<second_vehicle_trip_it->Calculate_Cost_of_MultiTrip(p,&solution)<<std::endl;


//                                     int tripAddedNumber=*singletrip_it;
//                                     second_vehicle_trip_it->Multi.push_back(tripAddedNumber);
//                                     bool multifeas=MultiTripFeasiblity(*second_vehicle_trip_it,p,solution);
//                                     // cout<<"beforemultifeas"<<endl;
//                                     if(multifeas)
//                                     {
//                                         // cout<<"aftermultifeas"<<endl;

//                                         tempStrip.vehicletrip_id=second_vehicle_trip_it->vehicletrip_id;//vehicletripid changed

//                                         std::vector<int>& vec = vehicle_trip_it->Multi; // using shorter name
//                                         vec.erase(std::remove(vec.begin(), vec.end(), tripAddedNumber), vec.end());
//                                         if(vec.size()==1)
//                                         {
//                                             cout<<"extra vehicle trip erased+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++vehicleerased"<<endl;
//                                             solution.MTrips.erase(solution.MTrips.begin()+tempStrip.vehicletrip_id);//have to update the solution also and also the removed singletrip vector

//                                             solution.updateMTrips();
//                                             mainbreak==true;
//                                         }




//                                         //distance display

//                                         // vehicle_trip_it->distance_of_vehicletrip-=tempStrip.trip_distance;
//                                         // second_vehicle_trip_it->distance_of_vehicletrip=+tempStrip.trip_distance;
//                                         cout<<"tripAddedNumber"<<tripAddedNumber<<endl;
//                                         cout<<"first_vehicle_trip_it->vehicletrip_id"<<vehicle_trip_it->vehicletrip_id<<std::endl;
//                                         cout<<"second_vehicle_trip_it->vehicletrip_id"<<second_vehicle_trip_it->vehicletrip_id<<std::endl;
//                                         std::cout<<"first vehicle totaltrip cost-post "<<vehicle_trip_it->Calculate_Cost_of_MultiTrip(p,&solution)<<std::endl;
//                                         cout<<"second vehicle totaltrip cost-post "<<second_vehicle_trip_it->Calculate_Cost_of_MultiTrip(p,&solution)<<std::endl;
//                                         std::cout<<endl;
//                                         cout<<tempStrip.cust_id;
//                                         break;
                                        

//                                     }
//                                     else
//                                     {
//                                         // cout<<"beforeerase"<<endl;
//                                         std::vector<int>& vec = second_vehicle_trip_it->Multi; // using shorter name
//                                         vec.erase(std::remove(vec.begin(), vec.end(), tripAddedNumber), vec.end());
//                                         // cout<<"aftererase"<<endl;   
//                                     }
                                        
//                                 }
//                             }
//                             // else if()
//                             // {

//                             // }
//                             // else
//                             // {

//                             // }
//                     }
//                 }
//             }
//         }
//     }
//     cout<<"going out of Localsearch1"<<endl;
//    // cout<<"The request can't be assigned to already present vehicle so new MultiTrip is being created"<<endl;
// }

//-------------------------------------------------the earlier one below





// void LocalSearch::LocalSearch_operator1(Solution& solution,const Problem& p)
// {
//     //purpose is to shift the trip to lower capacity trip if possible
//     Solution oldSolution=solution;//check it
//     // solution.displaySolution();


// //Problem of singletrip??
//     for(auto vehicle_trip_it=solution.MTrips.begin();
//         vehicle_trip_it!=solution.MTrips.end();vehicle_trip_it++)
//     {
//         std::vector<int> trips=vehicle_trip_it->Multi;
//         cout<<"vehicletripsnumber"<<trips<<endl;
//         for(auto singletrip_it=trips.begin();
//         singletrip_it<trips.end();singletrip_it++)
//         {
//             SingleTrip& tempStrip=solution.GlobalTrips[*singletrip_it];
//             if(tempStrip.islunchtrip==0)
//             {
//                 for(auto second_vehicle_trip_it=solution.MTrips.begin();
//                     second_vehicle_trip_it!=solution.MTrips.end() && second_vehicle_trip_it != vehicle_trip_it;
//                     second_vehicle_trip_it++)
//                 {
//                     //check first for type difference
//                     //this guarantees that type_const constraint won't be violated as we are inserting trip from vt1 to vt2
//                     if(vehicle_trip_it->TripVehicle.type > second_vehicle_trip_it->TripVehicle.type)
//                     {
//                         //std::cout<<"vehicle_trip_it->TripVehicle.type" <<" second_vehicle_trip_it->TripVehicle.type"<<std::endl;

//                         //check the maximum weight capacity
//                         if(tempStrip.max_weight <= second_vehicle_trip_it->TripVehicle.Capacity)//capacity check
//                         {
//                             // std::cout<<"first vehicle totaltrip cost-pre "<<vehicle_trip_it->Calculate_Cost_of_MultiTrip(p,&solution)<<std::endl;
//                             // std::cout<<"second vehicle totaltrip cost-pre "<<second_vehicle_trip_it->Calculate_Cost_of_MultiTrip(p,&solution)<<std::endl;


//                             int tripAddedNumber=*singletrip_it;
//                             second_vehicle_trip_it->Multi.push_back(tripAddedNumber);
//                             bool multifeas=MultiTripFeasiblity(*second_vehicle_trip_it,p,solution);
//                             // cout<<"beforemultifeas"<<endl;
//                             if(multifeas)
//                             {
//                                 // cout<<"aftermultifeas"<<endl;

//                                 tempStrip.vehicletrip_id=second_vehicle_trip_it->vehicletrip_id;//vehicletripid changed

//                                 std::vector<int>& vec = vehicle_trip_it->Multi; // using shorter name
//                                 vec.erase(std::remove(vec.begin(), vec.end(), tripAddedNumber), vec.end());

//                                 //distance display

//                                 // vehicle_trip_it->distance_of_vehicletrip-=tempStrip.trip_distance;
//                                 // second_vehicle_trip_it->distance_of_vehicletrip=+tempStrip.trip_distance;
//                                 cout<<"tripAddedNumber"<<tripAddedNumber<<endl;
//                                 cout<<"first_vehicle_trip_it->vehicletrip_id"<<vehicle_trip_it->vehicletrip_id<<std::endl;
//                                 cout<<"second_vehicle_trip_it->vehicletrip_id"<<second_vehicle_trip_it->vehicletrip_id<<std::endl;
//                                 std::cout<<"first vehicle totaltrip cost-post "<<vehicle_trip_it->Calculate_Cost_of_MultiTrip(p,&solution)<<std::endl;
//                                 cout<<"second vehicle totaltrip cost-post "<<second_vehicle_trip_it->Calculate_Cost_of_MultiTrip(p,&solution)<<std::endl;
//                                 std::cout<<endl;
//                                 cout<<tempStrip.cust_id;
//                                 break;
                                

//                             }
//                             else
//                             {
//                                 // cout<<"beforeerase"<<endl;
//                                 std::vector<int>& vec = second_vehicle_trip_it->Multi; // using shorter name
//                                 vec.erase(std::remove(vec.begin(), vec.end(), tripAddedNumber), vec.end());
//                                 // cout<<"aftererase"<<endl;   
//                             }
                                
//                         }
//                     }
//                     // else if()
//                     // {

//                     // }
//                     // else
//                     // {

//                     // }
//                 }
//             }
//         }
//     }
//     cout<<"going out of Localsearch1"<<endl;
//    // cout<<"The request can't be assigned to already present vehicle so new MultiTrip is being created"<<endl;
// }