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
bool bool_Check_Feasibility(int vehiclenum,int customer_id, const Problem& p, Solution& solution)
{
    return(solution.MTrips[vehiclenum].TripVehicle.type<p.requests[customer_id].pickup.type_const
            && p.requests[customer_id].pickup.demand<=solution.MTrips[vehiclenum].TripVehicle.Capacity);
// if(vehicle_trip_it->TripVehicle.type<req.pickup.type_const&& 
// req.pickup.demand<=vehicle_trip_it->TripVehicle.Capacity)

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
    // cout<<"Shrink Trip duration"<<duration;
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
            // cout<<"duration"<<duration<<endl;

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
        temp=max(temp+S.GlobalTrips[*(it-1)].trip_duration+p.rest_time *!(S.GlobalTrips[*(it-1)].islunchtrip),
                            S.GlobalTrips[*it].depot_early_start_time );
    //    cout<<"newtemp"<<temp<<endl<<"GlobalTrips[*it].depot_late_start_time"<<S.GlobalTrips[*it].depot_late_start_time<<endl;
        
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
            // cout<<*node_it<<endl;//debug
        }
        else{
            node_it++;
        }
    }
    // std::cout<<temp_strip->cust_id;
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




bool BestInsertPlace(int request_index,const Problem& p, Solution& S, double cost, std::vector<bool>& bookkeep, double fixed_cost )
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
                        // cout<<"1";
                        // cout<<"S.MTrips[tempStrip.vehicletrip_id].TripVehicle.type = "<<S.MTrips[tempStrip.vehicletrip_id].TripVehicle.type
                        // <<" req.pickup.type_const == "<<req.pickup.type_const<<endl;
                        // cout<<" req.pickup.demand == "<<req.pickup.demand<<" S.MTrips[tempStrip.vehicletrip_id].TripVehicle.Capacity == "<<S.MTrips[singletrip_it->vehicletrip_id].TripVehicle.Capacity<<endl;
                        //feasibility check of request with this singletrip
                        if(vehicle_trip_it->TripVehicle.type<req.pickup.type_const&& 
                            req.pickup.demand<=vehicle_trip_it->TripVehicle.Capacity)
                        {
                            // cout<<"2";
                            if( tempStrip.trip_duration-tempStrip.waitingtime+PNodeSerTime+DNodeSerTime<p.max_travel_time//trip duration-waiting time+service<=duration)
                                && (tempStrip.depot_late_start_time+p.max_travel_time)>std::max(req.pickup.start_time,req.delivery.start_time))
                            {
                                // cout<<"3";
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
                                // cout<<"4"<<endl;
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
                                                // cout<<" falsetripnum = "<<*singletrip_it<<"\n";
                                                // cout<<" realtripnum = "<<tempnum<<"\n\n";
                                                // cout<<"newaddedcost"<<newaddedcost<< endl;
                                                
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
        //add request to the singletrip_it
        S.GlobalTrips[best_it].cust_id.insert(S.GlobalTrips[best_it].cust_id.begin()+pbest,req.pickup.id);//.insert(temps.cust_id.begin()+pindex);
        S.GlobalTrips[best_it].cust_id.insert(S.GlobalTrips[best_it].cust_id.begin()+dbest,req.delivery.id);//
        ShrinkTrip(S.GlobalTrips[best_it],p,S);
        //remove request from the unserved request
        S.unrouted_cust_request_id.erase(std::remove(S.unrouted_cust_request_id.begin(), S.unrouted_cust_request_id.end(),request_index), S.unrouted_cust_request_id.end());
        if(-curr_cost-cost==fixed_cost)
        {
            return false;
        }
        return true;
    }
    else if(curr_cost+cost==0)
    {
        
        S.GlobalTrips[best_it].cust_id.insert(S.GlobalTrips[best_it].cust_id.begin()+pbest,req.pickup.id);//.insert(temps.cust_id.begin()+pindex);
        S.GlobalTrips[best_it].cust_id.insert(S.GlobalTrips[best_it].cust_id.begin()+dbest,req.delivery.id);//

        ShrinkTrip(S.GlobalTrips[best_it],p,S);
        
        S.unrouted_cust_request_id.erase(std::remove(S.unrouted_cust_request_id.begin(), S.unrouted_cust_request_id.end(),request_index), S.unrouted_cust_request_id.end());

        bookkeep[request_index]=1;
        return false;
    }
    else
    {
        bookkeep[request_index]=1;
        return false;
    }
}


bool bool_insert_customer(SingleTrip& temps1,SingleTrip& temps2, int customer_id1,int customer_id2,const Problem& p,Solution& solution,  double cost)
{
    LoadRequest req1=p.requests[customer_id1];
    LoadRequest req2=p.requests[customer_id2];
    double PNodeSerTime1=req1.pickup.service_time;
    double DNodeSerTime1=req1.delivery.service_time;


    int customersize2 = temps2.cust_id.size();

    double PNodeSerTime2 = req2.pickup.service_time;
    double DNodeSerTime2 = req2.delivery.service_time;
  
    double curr_cost1 = 1000000000;
    int p1best,d1best;
    int customersize1=temps1.cust_id.size();
    if(temps1.trip_duration-temps1.waitingtime+PNodeSerTime1+DNodeSerTime1<p.max_travel_time//trip duration-waiting time+service<=duration)
    && (temps1.depot_late_start_time+p.max_travel_time)>std::max(req1.pickup.start_time,req1.delivery.start_time))
    {
        for(auto m = 1;  m  < customersize1-1 ;  m++)
        {
            if( p.request_feasibility [ p.getRequestID(req1.pickup.id) ] [ p.getRequestID(temps1.cust_id[m]) ] == 0)
            {
                return false;
            }     
        }
        
        for(auto pindex=1 ; pindex < customersize1 ; pindex++)
        {

            for(auto dindex=pindex+1 ; dindex < customersize1+1 ; dindex++)
                {
                    // cout<<"pindex == "<<pindex<<"dindex == "<<dindex;
                    double newaddedcost = ShrinkTripBI (pindex, dindex, temps1, req1, p, solution);//returns cost of insertion
                    if ( newaddedcost<curr_cost1 )
                    {
                        p1best=pindex;
                        d1best=dindex;
                        curr_cost1=newaddedcost;
                    }
                }
        }
    }
    else{
        return false;
    }

    
    double curr_cost2 = 1000000000;
    int p2best,d2best;
    if(temps2.trip_duration-temps2.waitingtime+PNodeSerTime2+DNodeSerTime2<p.max_travel_time//trip duration-waiting time+service<=duration)
    && (temps2.depot_late_start_time+p.max_travel_time)>std::max(req2.pickup.start_time,req2.delivery.start_time))
    {
        for(auto m = 1;  m  < customersize2-1 ;  m++)
        {

            if( p.request_feasibility [ p.getRequestID(req2.pickup.id) ] [ p.getRequestID(temps2.cust_id[m]) ] == 0)
            {
                return false;
            }     
        }
        for(auto pindex=1 ; pindex < customersize2 ; pindex++)
        {
            for(auto dindex=pindex+1 ; dindex < customersize2+1 ; dindex++)
                {
                    double newaddedcost = ShrinkTripBI (pindex, dindex, temps2, req2, p, solution);//returns cost of insertion
                    if ( newaddedcost<curr_cost2 )
                    {
                        p2best=pindex;
                        d2best=dindex;
                        curr_cost2=newaddedcost;
                    }
                }
        }
    }
    else{
        return false;
    }
    if(curr_cost1+curr_cost2+cost<0)
    {
        cout<<"curr_cost1+curr_cost2+cost== "<<curr_cost1+curr_cost2+cost<<endl;
        temps1.cust_id.insert(temps1.cust_id.begin()+p1best,req1.pickup.id);//.insert(temps.cust_id.begin()+pindex);
        temps1.cust_id.insert(temps1.cust_id.begin()+d1best,req1.delivery.id);//.insert(temps.cust_id.begin()+pindex);

        temps2.cust_id.insert(temps2.cust_id.begin()+p2best,req2.pickup.id);//.insert(temps.cust_id.begin()+pindex);
        temps2.cust_id.insert(temps2.cust_id.begin()+d2best,req2.delivery.id);//.insert(temps.cust_id.begin()+pindex);

        ShrinkTrip(temps1,p,solution);
        ShrinkTrip(temps2,p,solution);

        return true;
        
    }
    else
    {
        return false;
    }



}

void removeCustomer(Solution &solution, const Problem& p)
{
    int request_id;//gives the id of the 
    unsigned TrueSingleTrip_size=solution.servedSingleTrips.size();
    double cost=0;//vehicle removed(if)+customerunassigned+tripdistanceremoved
    int static seed=0;
    std::vector<SingleTrip>::iterator iter;
    // int RandomTripNumber;
    int customer_id;
    int RandomTripNumber=rand()%TrueSingleTrip_size;
    RandomTripNumber=solution.servedSingleTrips[RandomTripNumber];
    iter = std::next(solution.GlobalTrips.begin(), RandomTripNumber);
    SingleTrip temps=*iter;
    if(iter->cust_id.size()>2)/*size of randomtrips more than 2*/
    {
        // cost=0;
        int remove_id_location=(rand()%(iter->cust_id.size()-2))+1;
        cout<<"remove_id_location=="<<remove_id_location<<endl;
        customer_id=iter->cust_id[remove_id_location];//customer_id is the pickup/delivery id
        customer_id=p.getRequestID(customer_id);//customer_id contain the index of request_id
        cout<<"cust_id_index=="<<customer_id<<endl;
        // double old_traveldistance=iter->trip_distance;
        Erase_ID_from_Trip(iter,customer_id,solution,p);
        solution.unrouted_cust_request_id.push_back(customer_id);//inserted the index
        if(iter->cust_id.size()==2)
        {
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
                // cout<<"hi"<<endl;
                solution.unservedLunchTrips.push_back(temp);
                solution.MTrips.erase(solution.MTrips.begin()+iter->vehicletrip_id);//have to update the solution also and also the removed singletrip vector
                solution.updateMTrips();
                
                // solution.MTrips.erase(std::remove(solution.MTrips.begin(), solution.MTrips.end(),solution.MTrips[iter->vehicletrip_id]), solution.MTrips.end());//have to update the solution also;
            }
        }   


    }
}
class custInsertinfo
{
    public:
        int pindex;
        int dindex;
        double cost;
        int tripindex;
        int vehicleindex;
        custInsertinfo()
        {
            pindex=dindex=tripindex=vehicleindex=0;
            cost=1000000000.00;
        }
        ~custInsertinfo(){};
        void Display(){
            cout<<"tripindex="<<tripindex<<"pindex="<<pindex<<"dindex="<<dindex<<"cost="<<cost<<endl;
        }
};

void InsertRequest(int tripindex,int pindex,int dindex,
                    const LoadRequest& tempreq,const Problem& p,Solution& S)
{
    
    SingleTrip& temps= S.GlobalTrips[tripindex];
    cout<<temps.cust_id;
    cout<<"newline"<<endl;
    temps.cust_id.insert(temps.cust_id.begin()+pindex,tempreq.pickup.id);//.insert(temps.cust_id.begin()+pindex);
    temps.cust_id.insert(temps.cust_id.begin()+dindex,tempreq.delivery.id);//
    //cout<<"request inserted permanently"<<endl;
    double early_start=p.depot.start_time ,late_start=p.depot.end_time,duration=0;
    double totaltripdistance=0,max_capacity=0,l_waittime=0;
    //to edit and make it better
    bool feasible = true;
    for (auto i=1;i<S.GlobalTrips[tripindex].cust_id.size();i++)//checking and feasibility and finding the time window
    {
        Node N0=p.getNode(temps.cust_id[i-1]);//NO=
        Node Ni=p.getNode(temps.cust_id[i]);
        double s0=N0.service_time;
        double d01=p.distance[temps.cust_id[i-1]][temps.cust_id[i]];
        double tempweight=Ni.demand;
        double ei=Ni.start_time;
        double li=Ni.end_time;
        // store service time and distancehere and add them;
        // also find li and ei and ci and typecons-i
        early_start+=d01+s0;
        late_start+=d01+s0;
        duration+=d01+s0;
        // capacity+=tempweight;
        max_capacity=std::max(max_capacity,max_capacity+tempweight);
        totaltripdistance+=d01;
//--------------------------------checking for time window
        if(early_start>li)
        {
            cout<<"Error in entering request : start time not matching "<<endl<<endl;
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
    }
    temps.depot_early_start_time=early_start-duration;
    temps.depot_late_start_time=late_start-duration;
    temps.trip_duration=duration;
    temps.max_weight=max_capacity;
    temps.waitingtime=l_waittime;
    temps.trip_distance=totaltripdistance;
    
    
    //-----------------------------------to modify below
    //temps.vehicletrip_id=S.GlobalTrips[tripindex].vehicletrip_id;//to modify
  //     UpdateMultipleTrip(S.MTrips[temps.vehicletrip_id],p);
    
}
void AddCustomers(const Problem& p, Solution& solution)
{
    const double infinity=1000000000.00;
    while(!solution.unrouted_cust_request_id.empty())
    //start here
    {
        cout<<"solution.unrouted_cust_request_id.size()="<<solution.unrouted_cust_request_id.size()<<endl;
        custInsertinfo CostMatrix[solution.unrouted_cust_request_id.size()][solution.servedSingleTrips.size()];
        for(auto unserved_id=0;unserved_id<solution.unrouted_cust_request_id.size();unserved_id++)
        {
            
            LoadRequest req=p.requests[solution.unrouted_cust_request_id[unserved_id]];
            cout<<"req.pickup.id"<<req.pickup.id<<endl;            
            double PNodeSerTime=req.pickup.service_time;
            double DNodeSerTime=req.delivery.service_time;

            
            for(auto trip_id=0;trip_id!=solution.servedSingleTrips.size();trip_id++)
            {
                int nooftimesreloopdone=0;
                int reloop=0;
                int tripnum=-1;
                double curr_cost=infinity;
                SingleTrip& tempStrip=solution.GlobalTrips[solution.servedSingleTrips[ trip_id]];
                if(tempStrip.cust_id.size()==2)
                {
                    cout<<"Error::wrong cust_id_size(if new route not added)"<<endl;
                    std::exit(0);
                }
                VehicleTrips& vtemp=solution.MTrips[tempStrip.vehicletrip_id];
               
                if(vtemp.TripVehicle.type<req.pickup.type_const&& 
                    req.pickup.demand<=vtemp.TripVehicle.Capacity)
                {
                    if( tempStrip.trip_duration-tempStrip.waitingtime+PNodeSerTime+DNodeSerTime<p.max_travel_time//trip duration-waiting time+service<=duration)
                        && (tempStrip.depot_late_start_time+p.max_travel_time)>std::max(req.pickup.start_time,req.delivery.start_time))
                    {
                        cout<<"1"<<endl;
                                int customersize = tempStrip.cust_id.size();
                                
                                for(auto m=1;m<customersize-1;m++)
                                {
                                    if( p.request_feasibility[p.getRequestID(req.pickup.id)][p.getRequestID(tempStrip.cust_id[m])]==0)
                                    {
                                        reloop=1;
                                        CostMatrix[unserved_id][trip_id].cost=infinity;
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
                                            double newaddedcost=ShrinkTripBI(pindex,dindex,tempStrip,req,p,solution);//returns cost of insertion
                                            if(newaddedcost<curr_cost)
                                            {
                                                
                                                cout<<"unserved_id == "<<unserved_id<<"trip_id == "<<trip_id<<"pindex == "<<pindex<<"dindex == "<<dindex<<"customersize == "<<customersize<<endl;
                                                // cout<<"customersize == "<<customersize;
                                                CostMatrix[unserved_id][trip_id].tripindex=trip_id;
                                                CostMatrix[unserved_id][trip_id].vehicleindex=tempStrip.vehicletrip_id;
                                                CostMatrix[unserved_id][trip_id].pindex=pindex;
                                                CostMatrix[unserved_id][trip_id].dindex=dindex;
                                                curr_cost=newaddedcost;
                                                CostMatrix[unserved_id][trip_id].cost=newaddedcost;
                                                if(CostMatrix[unserved_id][trip_id].cost<0.00001)
                                                {
                                                    CostMatrix[unserved_id][trip_id].cost=0;
                                                }
                                                // MinCustCost[unserved_id]=newaddedcost;
                                            }
                                        }
                                        // cout<<endl;
                                }
                    }
                    else{
                        CostMatrix[unserved_id][trip_id].cost=infinity;
                        continue;
                    }
                }
                else
                {
                    CostMatrix[unserved_id][trip_id].cost=infinity;
                    continue;
                }
            }
            //-----------------------------end for the trip
        }
        cout<<"hi"<<endl;
        for(auto unserved_id=0;unserved_id<solution.unrouted_cust_request_id.size();unserved_id++)
        {
            for(auto trip_id=0;trip_id!=solution.servedSingleTrips.size();trip_id++)
            {
                cout<<CostMatrix[unserved_id][trip_id].cost<<" ";
            }
            cout<<endl;
        }
        // getchar();
        std::vector<double> RegretMatrixSum;
        RegretMatrixSum.resize(solution.unrouted_cust_request_id.size());
        for(int unserved_id=0;unserved_id<solution.unrouted_cust_request_id.size();unserved_id++)
        {
            double tempsum=0;
            double rowmin=infinity;
            for(auto trip_id=0;trip_id!=solution.servedSingleTrips.size();trip_id++)
            {
                tempsum+=CostMatrix[unserved_id][trip_id].cost;
                rowmin=std::min(rowmin,CostMatrix[unserved_id][trip_id].cost);
            }
            RegretMatrixSum[unserved_id]=tempsum-(solution.servedSingleTrips.size()*rowmin);
        }
        cout<<RegretMatrixSum;
        // getchar();
        int maxRegret_custID=0;
        double maxregretValue=RegretMatrixSum[0];
        //thing below can be done in the previous loop
        for(int unserved_id=1;unserved_id<solution.unrouted_cust_request_id.size();unserved_id++)
        {
            if(RegretMatrixSum[unserved_id]>maxregretValue)
            {
                maxRegret_custID=unserved_id;
            }
        }
        //Find the minimum in that particular row
        int tripmincost_id=0;
        double addedcost=infinity;
        for(auto trip_id=0;trip_id!=solution.servedSingleTrips.size();trip_id++)
        {
            if(CostMatrix[maxRegret_custID][trip_id].cost<addedcost)
            {
                tripmincost_id=trip_id;
                addedcost=CostMatrix[maxRegret_custID][trip_id].cost;
            }
        }
        cout<<"maxRegretCust Real Id::"<<p.requests[solution.unrouted_cust_request_id[ maxRegret_custID]].pickup.id<<"trip_id::"<<tripmincost_id<<endl;
        if(CostMatrix[maxRegret_custID][tripmincost_id].cost==infinity)
        {
            //Insert it in new route;
            cout<<" the cost of insertion is infinity so have to make a new route:Press enter to move forward"<<endl;
           
            Edited_createSingleTrip(p.requests[solution.unrouted_cust_request_id[ 0]],p,solution);
            solution.unrouted_cust_request_id.erase(std::remove(solution.unrouted_cust_request_id.begin(), solution.unrouted_cust_request_id.end(),//next line
                                                                    solution.unrouted_cust_request_id[ 0]), solution.unrouted_cust_request_id.end());
            continue;
            // std::exit(0);
            
        }
        cout<<"If all numbeer are not infinity"<<endl;
        CostMatrix[maxRegret_custID][tripmincost_id].Display();
        custInsertinfo temp=CostMatrix[maxRegret_custID][tripmincost_id];
       
        InsertRequest(solution.servedSingleTrips[ temp.tripindex],temp.pindex,temp.dindex,p.requests[solution.unrouted_cust_request_id[ maxRegret_custID]],p,solution);
        solution.unrouted_cust_request_id.erase(std::remove(solution.unrouted_cust_request_id.begin(), solution.unrouted_cust_request_id.end(),//next line
                                                                    solution.unrouted_cust_request_id[ maxRegret_custID]), solution.unrouted_cust_request_id.end());
        solution.displaySolution();
        solution.Calculate_Solution_Cost(p);
        cout<<"hierror"<<endl;
    }
}


void Edited_createSingleTrip(const LoadRequest& req,const Problem& p, Solution& S/*,int pushback_id*/)
{
    if(S.unservedSingleTrips.size()==0)
    {
        cout<<"Error in Number of SingleTrips"<<endl;
        // S.unservedSingleTrips.push_back(S.GlobalTrips.size());
        SingleTrip tempSingleTrip;
        S.GlobalTrips.push_back(tempSingleTrip);
        S.unservedSingleTrips.push_back(S.GlobalTrips.size()-1);
    }
    if(S.unservedLunchTrips.size()==0)
    {
        cout<<"Error in Number of LunchTrips"<<endl;
        // S.unservedSingleTrips.push_back(S.GlobalTrips.size());
        SingleTrip tempLunchTrip;
        S.GlobalTrips.push_back(tempLunchTrip);
        S.unservedLunchTrips.push_back(S.GlobalTrips.size()-1);
    }
        cout<<" Id To be inserted is "<<req.rid<<endl;
        int pushback_id=S.unservedSingleTrips.back();
        cout<<"pushback_id=="<<pushback_id<<"------------------------------------<<<<<<"<<endl;
        S.unservedSingleTrips.pop_back();
        S.servedSingleTrips.push_back(pushback_id);
        S.GlobalTrips[pushback_id].Clear();//this undo that trip back to initial position
        SingleTrip& stemp=S.GlobalTrips[pushback_id];//to insert it in the GlobalTrips
        stemp.cust_id.push_back(p.depot.id);
        stemp.cust_id.push_back(req.pickup.id);
        stemp.cust_id.push_back(req.delivery.id);
        stemp.cust_id.push_back(p.returndepot.id);
        stemp.islunchtrip=false;
        int needed_vehicle_type=p.getVehicleID(req);//return type of vehicle
        ShrinkTrip(stemp,p,S);
        // S.GlobalTrips.push_back(stemp);
        // S.servedSingleTrips.push_back(S.GlobalTrips.size()-1);

        //cout<<"You are in createSingleTrip"<<endl;
        //getvehicleID gives the least cost vehicle number for the vehicle
        //try to insert the singletrip in each of the multitrip and get a good solution
        int VTid=-1;
        for(auto it=S.MTrips.begin();it!=S.MTrips.end();it++)
        {
        // cout<<"you are checking old multrips to assign customers"<<endl;
            VTid++;
            cout<<"VTid"<<VTid<<endl;
        // cout<<"it->TripVehicle.type = "<<it->TripVehicle.type<<" needed_vehicle_type = "<<needed_vehicle_type<<endl;
            //assign it to any vehicle already present  but that should less than type_cons
            if(it->TripVehicle.type==needed_vehicle_type&&it->TripVehicle.type< req.pickup.type_const)
            {
                //inserting only if vehicle type matches
                int tripAddedNumber=pushback_id;
                //Try to insert in the current multitrip
                it->Multi.push_back(tripAddedNumber);//changes done here
                bool multifeas=MultiTripFeasiblity(*it,p,S);//sending the vehicletrip object to check for feasibility of multitrip
                if(multifeas)
                {
                    stemp.vehicletrip_id=it->vehicletrip_id;
                    return;
                }
                //else erase the trip from inside the Multi
                std::vector<int>& vec = it->Multi; // use shorter name
                vec.erase(std::remove(vec.begin(), vec.end(), tripAddedNumber), vec.end());
            }
        }
        cout<<"The request can't be assigned to already present vehicle in VehicleTrips, so new MultiTrip is being created"<<endl;
        // getchar();
        stemp.vehicletrip_id=S.MTrips.size();
        VehicleTrips VT;
        VT.TripVehicle=p.vehicles[needed_vehicle_type-1];
        //GlobalTrips.size() gives the total element and '-1' give the index of the last element entered
        VT.Multi.push_back(pushback_id);//to get last element entered index
        
        SingleTrip &lunchtrip=S.GlobalTrips[S.unservedLunchTrips.back()];
        if(S.servedLunchTrips.size()==0)
        {
            cout<<"Error in Number of LunchTrips"<<endl;
            SingleTrip tempLunchTrip;
            S.GlobalTrips.push_back(tempLunchTrip);
            S.unservedLunchTrips.push_back(S.GlobalTrips.size()-1);
        }
        S.servedLunchTrips.push_back(S.unservedLunchTrips.back());
        VT.Multi.push_back(S.unservedLunchTrips.back());
        S.unservedLunchTrips.pop_back();
        lunchtrip.depot_early_start_time=VT.TripVehicle.lunch_start_time;
        lunchtrip.depot_late_start_time=VT.TripVehicle.lunch_end_time;
        lunchtrip.trip_duration=VT.TripVehicle.lunch_duration;
        lunchtrip.islunchtrip=true;
        lunchtrip.vehicletrip_id=S.MTrips.size();
        VT.vehicletrip_id=S.MTrips.size();
        S.updateMTrips();
      
        S.MTrips.push_back(VT);
        int feas=MultiTripFeasiblity(S.MTrips.back(),p,S);
        if(feas)
        {
            cout<<S.MTrips.size()<<endl;
        }
        else
        {
            S.displaySolution();
            cout<<"this customer can't be allocated"<<req.rid<< endl;
            exit(0);
        }
        
    }
    // else
    // {
    //     cout<<"We have to make a new trip altogether ::Edited Insert Customer"<<endl;
    //     getchar();
    // }

// #ifndef OPERATOR_H_
// #define OPERATOR_H_

// //SELECT A RANDOM NUMBER
// //
// #endif
  // CostMatrix[][].~custInsertinfo();
        // getchar();
        
        //Here do the calculation and insert the customer, otherwise put in the insertion list;
        //Subtract the minimum of the row value from the row and 


  // S.GlobalTrips.push_back(lunchtrip);
        // S.servedLunchTrips.push_back(S.GlobalTrips.size()-1);

        // VT.Multi.push_back(S.GlobalTrips.size()-1);
        //cout<<"S.GlobalTrips.size()-1"<<S.GlobalTrips.size()-1<<endl;


        // cout<<"new vehicle trip is assigned and route created"<<endl;
        // cout<<"bi"<<endl;
            // cout<<S.GlobalTrips<<endl;
            // cout<<S.MTrips<<endl;
            //cout<<"bi"<<endl;

                       // cout<<S.GlobalTrips<<endl;
            // cout<<S.MTrips<<endl;