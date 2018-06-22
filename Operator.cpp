/*
created on 19June, 2018
author: Akshay
contains the general function and operators
*/
#include "Operator.h"
#include <iostream>


template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
    out<<"{";
    size_t last=v.size()-1;
    for (int i=0;i<v.size();i++)
    {
        out<<v[i]<<" ";
    }
    out<<" }";
    std::cout<<std::endl;
    return out;

}
void ShrinkTrip(SingleTrip& stemp,const Problem& p, Solution& S)
{
    double early_start=p.depot.start_time ,late_start=p.depot.end_time,duration=0;
    double totaltripdistance=0,max_capacity=0,l_waittime=0;
    for (auto i=1;i<stemp.cust_id.size();i++)//checking and feasibility and finding the time window
    {
        Node N0=p.getNode(stemp.cust_id[i-1]);//NO=
        Node Ni=p.getNode(stemp.cust_id[i]);
        double s0=N0.service_time;
        double d01=p.distance[stemp.cust_id[i-1]][stemp.cust_id[i]];
        double tempweight=Ni.demand;
        double ei=Ni.start_time;
        double li=Ni.end_time;
        //totaltripdistance+=d01;
        // store service time and distancehere and add them;
        // also find li and ei and ci and typecons-i
        early_start+=d01+s0;
        late_start+=d01+s0;
        duration+=d01+s0;
       // capacity+=tempweight;declared below
//--------------------------------checking for time window
        if(early_start>li)
        {
            cout<<"Infeasible new SingleTrip due to time window in ShrinkTrip: Request  id is ="<<N0.id<<endl;
        }
        else if (late_start<ei)
        {
            l_waittime+=(ei-early_start);
            duration=duration+(ei-early_start);
            early_start=ei;late_start=ei;
        }
        else
        {
            early_start=max(early_start,ei);
            late_start=min(late_start,li);
        }
        if(duration>p.max_travel_time)//to complete
        {
            cout<<"Infeasible new SingleTrip due to duration in ShrinkTrip: Request  id is ="<<p.getNode(stemp.cust_id[1]).id<<endl;
        }
        //capacity+=tempweight;
        max_capacity=std::max(max_capacity,max_capacity+tempweight);
        totaltripdistance+=d01;
    }

    //update all the value of the stemp
    stemp.waitingtime=l_waittime;
    stemp.depot_early_start_time=early_start-duration;
    stemp.depot_late_start_time=late_start-duration;
    stemp.trip_duration=duration;
    stemp.max_weight=max_capacity;
    stemp.trip_distance=totaltripdistance;
    cout<<"Shrink Trip duration"<<duration;
    //cout<<"hi"<<endl;
    //cout<<"ID-----"<<p.getNode(stemp.cust_id[1]).id<<"updated"<<endl;
}




//------------------------ShrinkTripBi--------------------------

double ShrinkTripBI(int pindex,int dindex,SingleTrip& temps,const LoadRequest& tempreq, const Problem& p,Solution& S)
{
    temps.cust_id.insert(temps.cust_id.begin()+pindex,tempreq.pickup.id);//.insert(temps.cust_id.begin()+pindex);
    temps.cust_id.insert(temps.cust_id.begin()+dindex,tempreq.delivery.id);//
    
    double old_distancetravelled=temps.trip_distance;
    double early_start=p.depot.start_time ,late_start=p.depot.end_time,duration=0;
    double totaltripdistance=0,max_capacity=0;
    for (auto i=1;i<temps.cust_id.size();i++)//checking and feasibility and finding the time window
    {
        Node N0=p.getNode(temps.cust_id[i-1]);//NO=
        Node Ni=p.getNode(temps.cust_id[i]);
        double s0=N0.service_time;
        double d01=p.distance[temps.cust_id[i-1]][temps.cust_id[i]];
        double tempweight=Ni.demand;
        double ei=Ni.start_time;
        double li=Ni.end_time;

        early_start=early_start+d01+s0;
        late_start=late_start+d01+s0;
        duration=duration+d01+s0;
        if(duration>p.max_travel_time)//to complete
        {
            cout<<"duration"<<duration<<endl;

            temps.cust_id.erase(temps.cust_id.begin()+dindex);
            temps.cust_id.erase(temps.cust_id.begin()+pindex);
            return 1000000000;
        }
       // capacity+=tempweight;declared below
//--------------------------------checking for time window
        if(early_start>li)
        {

           // cout<<"early_start>li"<<endl;            
            temps.cust_id.erase(temps.cust_id.begin()+dindex);
            temps.cust_id.erase(temps.cust_id.begin()+pindex);
            return 1000000000;

        }
        else if (late_start<ei)
        {
           // cout<<"late_start<ei"<<endl;
            duration=duration+(ei-early_start);
            early_start=ei;late_start=ei;
        }
        else
        {
           // cout<<"3rd case"<<endl;            
            early_start=max(early_start,ei);
            late_start=min(late_start,li);
            
        }
//--------------------------------capacity constraint checking
        totaltripdistance+=d01;        
        max_capacity=std::max(max_capacity,max_capacity+tempweight);
        //cout<<"capacity"<<capacity<<"S.MTrips[temps.vehicletrip_id].TripVehicle.Capacity"<<S.MTrips[temps.vehicletrip_id].TripVehicle.Capacity<<endl;
        if(max_capacity>S.MTrips[temps.vehicletrip_id].TripVehicle.Capacity) //if current cap is more than cehicle capacity then reurn false[temps.vehicletrip_id].TripVehicle.Capacity) //if current cap is more than cehicle capacity then reurn false
        {
            //cout<<"temps.vehicletrip_id"<<temps.vehicletrip_id<<"##############################################"<<endl;
            
            // cout<<"capacity overflow"<<endl;
            temps.cust_id.erase(temps.cust_id.begin()+dindex);
            temps.cust_id.erase(temps.cust_id.begin()+pindex);
            return 1000000000;
        }
        // if(duration>p.max_travel_time)//to complete
        // {
        //     cout<<"duration"<<duration<<endl;
        //     temps.cust_id.erase(temps.cust_id.begin()+dindex);
        //     temps.cust_id.erase(temps.cust_id.begin()+pindex);
        //     return 1000000000;
        // }
    }


    double olddepotstarttime=temps.depot_early_start_time;        
    temps.depot_early_start_time=early_start-duration;

    double olddepotendtime=temps.depot_late_start_time;                
    temps.depot_late_start_time=late_start-duration;
    
    double oldduration=temps.trip_duration;
    temps.trip_duration=duration;
    bool feasmulti=MultiTripFeasiblity(S.MTrips[temps.vehicletrip_id],p,S);
        
    temps.trip_duration=oldduration;            
    temps.depot_early_start_time=olddepotstarttime;
    temps.depot_late_start_time=olddepotendtime;
    temps.cust_id.erase(temps.cust_id.begin()+dindex);
    temps.cust_id.erase(temps.cust_id.begin()+pindex);
    // cout<<tripindex<<" "<<pindex<<" "<<dindex<<" "<<"multitrip insertion feasibility= "<<int(feasmulti)<<endl;
    if(feasmulti)
    {
        //cout<<"   I reached here "<<" ";
        return S.MTrips[temps.vehicletrip_id].TripVehicle.variable_cost*(totaltripdistance-old_distancetravelled);
    }
    else
    {
        // cout<<" multiinfeasiblbe ";
        return 1000000000;
    }
}



bool MultiTripFeasiblity(VehicleTrips& VT,const Problem& p, Solution& S)//this contains total trip of single variable
{
  //  cout<<"Yout are now in MultiTripFeasiblity"<<endl;
    // sort multitrip on the basis of trip starttime window and then on duration;
    //cout<<"old VT.multi.size()"<< VT.Multi.size()<<endl;   
    //std::sort(VT.Multi.begin(),VT.Multi.end(), compareTripEarlyStartTime_duration);

        std::sort(VT.Multi.begin(),VT.Multi.end(),[S](int a, int b)->bool {
            // Lambda function
         //   s->GlobalTrips
        if(S.GlobalTrips[a].depot_early_start_time<S.GlobalTrips[b].depot_early_start_time)
            return true;
        else if(S.GlobalTrips[a].depot_early_start_time==S.GlobalTrips[b].depot_early_start_time
                &&  S.GlobalTrips[a].trip_duration<S.GlobalTrips[b].trip_duration)
            return true;
        return false;
        });
    double temp;
    temp= S.GlobalTrips[ VT.Multi[0]].depot_early_start_time;
   // cout<<"GlobalTrips[ VT.Multi[0]].depot_early_start_time "<<S.GlobalTrips[ VT.Multi[0]].depot_early_start_time<<endl;
    for(auto it=VT.Multi.begin()+1;it!=VT.Multi.end();it++)
    {
        //cout<<"new loop ="<<*it<<endl;
        //cout<<"GlobalTrips[*(it-1)].trip_duration"<<S.GlobalTrips[*(it-1)].trip_duration<<endl;
        //cout<<"oldtemp"<<temp<<endl;
        temp=max(temp+S.GlobalTrips[*(it-1)].trip_duration+VT.TripVehicle.lunch_duration*!(S.GlobalTrips[*(it-1)].islunchtrip),
                            S.GlobalTrips[*it].depot_early_start_time );
       // cout<<"newtemp"<<temp<<endl<<"GlobalTrips[*it].depot_late_start_time"<<S.GlobalTrips[*it].depot_late_start_time<<endl;
        
        if( temp>S.GlobalTrips[*it].depot_late_start_time)
        {
            return false;
        }
    }
    return true;
}

void Erase_ID_from_Trip(std::vector<SingleTrip>::iterator temp_strip,
                        const int customer_index,Solution& solution,const Problem& p)
{
    SingleTrip oldTrip=*temp_strip;
    //iterate through all the nodes
    for(std::vector<int>::iterator node_it=temp_strip->cust_id.begin()+1;
        node_it!=temp_strip->cust_id.end();)
    {
        if ( p.getRequestID(*node_it) == customer_index)
        {
            /*the LHS below */
            node_it=temp_strip->cust_id.erase(node_it);
            cout<<*node_it<<endl;//debug
        }
        else{
            node_it++;
        }
    }
    std::cout<<temp_strip->cust_id;
    //if(temp_strip->cust_id.size()>2)
    ShrinkTrip(*temp_strip,p,solution);//i guess no need to check multifeasibility
}
//------------------------------

// bool BestInsertPlace(int request_index,const Problem& p, Solution& S, double cost, std::vector<bool>& bookkeep )
// {
//     LoadRequest req=p.requests[request_index];
//     double PNodeSerTime=req.pickup.service_time;
//     double DNodeSerTime=req.delivery.service_time;
//     double curr_cost=1000000000;
//     int pbest=-1,dbest=-1;
//     int nooftimesreloopdone=0;
//     int reloop;
//     int tripnum=-1;
//     // cout<<"req.pickup.type_const="<<req.pickup.type_const<<endl;
//     // cout<<"req.pickup.demand == "<<req.pickup.demand<<endl;
//     std::vector<SingleTrip>::iterator best_it;//for singletrip
//     std::vector<VehicleTrips>::iterator vbest_it;//for vehicle trips
    
//                 for(auto singletrip_it=S.GlobalTrips.begin();
//                     singletrip_it!=S.GlobalTrips.end();singletrip_it++)
//                 {
//                     tripnum++;
//                     cout<<" tripnum== "<<tripnum;
//                     if(!singletrip_it->islunchtrip)
//                     {
//                         cout<<"1";
//                         // cout<<"S.MTrips[singletrip_it->vehicletrip_id].TripVehicle.type = "<<S.MTrips[singletrip_it->vehicletrip_id].TripVehicle.type
//                         // <<" req.pickup.type_const == "<<req.pickup.type_const<<endl;
//                         // cout<<" req.pickup.demand == "<<req.pickup.demand<<" S.MTrips[singletrip_it->vehicletrip_id].TripVehicle.Capacity == "<<S.MTrips[singletrip_it->vehicletrip_id].TripVehicle.Capacity<<endl;
//                         //feasibility check of request with this singletrip
//                         if(S.MTrips[singletrip_it->vehicletrip_id].TripVehicle.type<req.pickup.type_const&& 
//                             req.pickup.demand<=S.MTrips[singletrip_it->vehicletrip_id].TripVehicle.Capacity)
//                         {
//                             cout<<"2";
//                             if( singletrip_it->trip_duration-singletrip_it->waitingtime+PNodeSerTime+DNodeSerTime<p.max_travel_time//trip duration-waiting time+service<=duration)
//                                 && (singletrip_it->depot_late_start_time+p.max_travel_time)>std::max(req.pickup.start_time,req.delivery.start_time))
//                             {
//                                 cout<<"3";
//                                 int customersize=singletrip_it->cust_id.size();
//                                 for(auto m=1;m<customersize-1;m++)
//                                 {
//                                     if( p.request_feasibility[p.getRequestID(req.pickup.id)][p.getRequestID(singletrip_it->cust_id[m])]==0)
//                                     {
//                                         reloop=1;
//                                         break;
//                                     }     
//                                 }
//                                 if(reloop==1)
//                                 {
//                                     nooftimesreloopdone++;
//                                     continue;
//                                 }
//                                 cout<<"4"<<endl;
//                                 for(auto pindex=1 ; pindex < customersize ; pindex++)
//                                 {
                                    
//                                     for(auto dindex=pindex+1 ; dindex < customersize+1 ; dindex++)
//                                         {
//                                             // cout<<"pindex == "<<pindex<<"dindex == "<<dindex;
//                                             double newaddedcost=ShrinkTripBI(pindex,dindex,*singletrip_it,req,p,S);//returns cost of insertion
//                                             if(newaddedcost<curr_cost)
//                                             {
//                                                 cout<<"***************************************************\n";
//                                                 cout<<"inside deepest loop"<<"-----------------------------------------------------"<<endl;                                    
//                                                 cout<<"newaddedcost"<<newaddedcost<< endl;
//                                                 cout<<" tripnum = "<<tripnum<<"\n";
//                                                 best_it=singletrip_it;
//                                                 pbest=pindex;
//                                                 dbest=dindex;
//                                                 curr_cost=newaddedcost;
//                                             }
//                                         }
//                                         // cout<<endl;
//                                 }
                                
//                             }
//                         }
//                     }
//                 }
//     // cout<<"outofloop"<<"--------------------------------------------------------"<<endl;
//     // cout<<"curr_cost== "<<curr_cost<<"cost= "<<cost<<endl;
//     //     cout<<"pindex=="<<pbest<<endl<<"dindex=="<<dbest<<endl;
//         // cout<<"iterator trip"<<best_it->vehicletrip_id<<"  other info"<<best_it->trip_cost<<endl;
// // 
//     if(curr_cost+cost<0)
//     {
//         cout<<"curr_cost+cost"<<endl;
//         cout<<"pindex=="<<pbest<<endl<<"dindex=="<<dbest<<endl;
//         cout<<"iterator trip"<<best_it->vehicletrip_id<<"  other info"<<best_it->trip_cost<<endl;
//         //add request to the singletrip_it
//         best_it->cust_id.insert(best_it->cust_id.begin()+pbest,req.pickup.id);//.insert(temps.cust_id.begin()+pindex);
//         best_it->cust_id.insert(best_it->cust_id.begin()+dbest,req.delivery.id);//
//         if(best_it->cust_id.size()==4)
//         {
//             //
//         }
//         ShrinkTrip(*best_it,p,S);
//         //remove request from the unserved request
//         cout<<"iterator trip"<<best_it->vehicletrip_id<<"  other info"<<best_it->trip_cost<<endl;
//         S.unrouted_cust_request_id.erase(std::remove(S.unrouted_cust_request_id.begin(), S.unrouted_cust_request_id.end(),request_index), S.unrouted_cust_request_id.end());
//         //
//         cout<<"Size of unrouted customer="<<S.unrouted_cust_request_id.size()<<endl;
//         return true;
//     }
//     else if(curr_cost+cost==0)
//     {
//         cout<<"curr_cost+cost"<<endl;
//         cout<<"pindex=="<<pbest<<endl<<"dindex=="<<dbest<<endl;
//         cout<<"iterator trip"<<best_it->vehicletrip_id<<"  other info"<<best_it->trip_cost<<endl;
//         //add request to the singletrip_it
//         best_it->cust_id.insert(best_it->cust_id.begin()+pbest,req.pickup.id);//.insert(temps.cust_id.begin()+pindex);
//         best_it->cust_id.insert(best_it->cust_id.begin()+dbest,req.delivery.id);//
//         if(best_it->cust_id.size()==4)
//         {
//             //
//         }
//         ShrinkTrip(*best_it,p,S);
//         //remove request from the unserved request
//         cout<<"iterator trip"<<best_it->vehicletrip_id<<"  other info"<<best_it->trip_cost<<endl;
//         S.unrouted_cust_request_id.erase(std::remove(S.unrouted_cust_request_id.begin(), S.unrouted_cust_request_id.end(),request_index), S.unrouted_cust_request_id.end());
//         //
//         cout<<"Size of unrouted customer="<<S.unrouted_cust_request_id.size()<<endl;
//         bookkeep[request_index]=1;
//     }
//     else
//     {
//         bookkeep[request_index]=1;
//         return false;
//     }
// }




bool BestInsertPlace(int request_index,const Problem& p, Solution& S, double cost, std::vector<bool>& bookkeep )
{
    LoadRequest req=p.requests[request_index];
    double PNodeSerTime=req.pickup.service_time;
    double DNodeSerTime=req.delivery.service_time;
    double curr_cost=1000000000;
    int pbest=-1,dbest=-1;
    int nooftimesreloopdone=0;
    int reloop;
    int tripnum=-1;
    // cout<<"req.pickup.type_const="<<req.pickup.type_const<<endl;
    // cout<<"req.pickup.demand == "<<req.pickup.demand<<endl;
    int best_it;//for singletrip
    std::vector<VehicleTrips>::iterator vbest_it;//for vehicle trips

    for(auto vehicle_trip_it=S.MTrips.begin();
        vehicle_trip_it!=S.MTrips.end();vehicle_trip_it++)
    {
        std::vector<int> trips(vehicle_trip_it->Multi);

                 for(auto singletrip_it=trips.begin();
                    singletrip_it!=trips.end();singletrip_it++)
                {
                    // tripnum++;
                    // cout<<" tripnum== "<<tripnum;
                    int tempnum=*singletrip_it;
                    SingleTrip& tempStrip=S.GlobalTrips[tempnum];
                    // cout<<"singletrip== "<<tempnum<<" \n";
                    if(tempStrip.islunchtrip==0)
                    {
                        cout<<"1";
                        // cout<<"S.MTrips[tempStrip.vehicletrip_id].TripVehicle.type = "<<S.MTrips[tempStrip.vehicletrip_id].TripVehicle.type
                        // <<" req.pickup.type_const == "<<req.pickup.type_const<<endl;
                        // cout<<" req.pickup.demand == "<<req.pickup.demand<<" S.MTrips[tempStrip.vehicletrip_id].TripVehicle.Capacity == "<<S.MTrips[singletrip_it->vehicletrip_id].TripVehicle.Capacity<<endl;
                        //feasibility check of request with this singletrip
                        if(vehicle_trip_it->TripVehicle.type<req.pickup.type_const&& 
                            req.pickup.demand<=vehicle_trip_it->TripVehicle.Capacity)
                        {
                            cout<<"2";
                            if( tempStrip.trip_duration-tempStrip.waitingtime+PNodeSerTime+DNodeSerTime<p.max_travel_time//trip duration-waiting time+service<=duration)
                                && (tempStrip.depot_late_start_time+p.max_travel_time)>std::max(req.pickup.start_time,req.delivery.start_time))
                            {
                                cout<<"3";
                                int customersize = tempStrip.cust_id.size();
                                for(auto m=1;m<customersize-1;m++)
                                {
                                    if( p.request_feasibility[p.getRequestID(req.pickup.id)][p.getRequestID(tempStrip.cust_id[m])]==0)
                                    {
                                        reloop=1;
                                        break;
                                    }     
                                }
                                if(reloop==1)
                                {
                                    nooftimesreloopdone++;
                                    continue;
                                }
                                cout<<"4"<<endl;
                                // cout<<"customer_size"<<endl;
                                for(auto pindex=1 ; pindex < customersize ; pindex++)
                                {
                                    
                                    for(auto dindex=pindex+1 ; dindex < customersize+1 ; dindex++)
                                        {
                                            // cout<<"pindex == "<<pindex<<"dindex == "<<dindex;
                                            double newaddedcost=ShrinkTripBI(pindex,dindex,tempStrip,req,p,S);//returns cost of insertion
                                            if(newaddedcost<curr_cost)
                                            {
                                                // cout<<"***************************************************\n";
                                                // cout<<"inside deepest loop"<<"-----------------------------------------------------"<<endl;                                    
                                                cout<<"newaddedcost"<<newaddedcost<< endl;
                                                // cout<<" falsetripnum = "<<*singletrip_it<<"\n";
                                                // cout<<" realtripnum = "<<tempnum<<"\n\n";
                                                
                                                best_it=tempnum;
                                                vbest_it=vehicle_trip_it;
                                                pbest=pindex;
                                                dbest=dindex;
                                                curr_cost=newaddedcost;
                                            }
                                        }
                                        // cout<<endl;
                                }
                                
                            }
                        }
                    }
                }
    // cout<<"outofloop"<<"--------------------------------------------------------"<<endl;
    // cout<<"curr_cost== "<<curr_cost<<"cost= "<<cost<<endl;
    //     cout<<"pindex=="<<pbest<<endl<<"dindex=="<<dbest<<endl;
        // cout<<"iterator trip"<<best_it->vehicletrip_id<<"  other info"<<best_it->trip_cost<<endl;
    }
    if(curr_cost+cost<0)
    {
        cout<<"curr_cost+cost== "<<curr_cost+cost<<endl;
        cout<<"pindex=="<<pbest<<"dindex=="<<dbest<<endl;
        cout<<"iterator trip"<<S.GlobalTrips[best_it].vehicletrip_id<<"  other info"<<S.GlobalTrips[best_it].trip_cost<<endl;
        cout<<"hi3";
        cout<<"size=="<<S.GlobalTrips[best_it].cust_id.size()<<endl;
        //add request to the singletrip_it
        S.GlobalTrips[best_it].cust_id.insert(S.GlobalTrips[best_it].cust_id.begin()+pbest,req.pickup.id);//.insert(temps.cust_id.begin()+pindex);
        // cout<<"hi4";        
        S.GlobalTrips[best_it].cust_id.insert(S.GlobalTrips[best_it].cust_id.begin()+dbest,req.delivery.id);//
        // cout<<"hi2"<<endl;
        if(S.GlobalTrips[best_it].cust_id.size()==4)
        {
            //
        }
        cout<<"hi"<<endl;
        ShrinkTrip(S.GlobalTrips[best_it],p,S);
        //remove request from the unserved request
        cout<<"iterator trip"<<S.GlobalTrips[best_it].vehicletrip_id<<"  other info"<<S.GlobalTrips[best_it].trip_cost<<endl;
        S.unrouted_cust_request_id.erase(std::remove(S.unrouted_cust_request_id.begin(), S.unrouted_cust_request_id.end(),request_index), S.unrouted_cust_request_id.end());
        //
        cout<<"Size of unrouted customer="<<S.unrouted_cust_request_id.size()<<endl;
        return true;
    }
    else if(curr_cost+cost==0)
    {
        cout<<"curr+cost==0"<<endl;
        cout<<"curr_cost+cost== "<<curr_cost+cost<<endl;
        cout<<"pindex=="<<pbest<<endl<<"dindex=="<<dbest<<endl;
        cout<<"iterator trip"<<S.GlobalTrips[best_it].vehicletrip_id<<"  other info"<<S.GlobalTrips[best_it].trip_cost<<endl;
        //add request to the singletrip_it
        S.GlobalTrips[best_it].cust_id.insert(S.GlobalTrips[best_it].cust_id.begin()+pbest,req.pickup.id);//.insert(temps.cust_id.begin()+pindex);
        S.GlobalTrips[best_it].cust_id.insert(S.GlobalTrips[best_it].cust_id.begin()+dbest,req.delivery.id);//
        if(S.GlobalTrips[best_it].cust_id.size()==4)
        {
            //
        }
        ShrinkTrip(S.GlobalTrips[best_it],p,S);
        //remove request from the unserved request
        cout<<"iterator trip"<<S.GlobalTrips[best_it].vehicletrip_id<<"  other info"<<S.GlobalTrips[best_it].trip_cost<<endl;
        S.unrouted_cust_request_id.erase(std::remove(S.unrouted_cust_request_id.begin(), S.unrouted_cust_request_id.end(),request_index), S.unrouted_cust_request_id.end());
        //
        cout<<"Size of unrouted customer="<<S.unrouted_cust_request_id.size()<<endl;
        bookkeep[request_index]=1;
        return false;
    }
    else
    {
        bookkeep[request_index]=1;
        return false;
    }
}
// #ifndef OPERATOR_H_
// #define OPERATOR_H_

// //SELECT A RANDOM NUMBER
// //
// #endif