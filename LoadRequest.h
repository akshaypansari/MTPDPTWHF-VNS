//In this file we are defining the classes for the request and for the delivery and pickup node for the request
//A request consists of a pickup node and a delivery node
//Request class will contain 2 node Classes
#ifndef LOADREQUEST_H_
#define LOADREQUEST_H_

class Node{
public:
    Node();
    ~Node();
    int id;
    double service_time;
    double start_time;
    double end_time;
    double demand;
    int type_const;
    //double x,y;//no need?
};


class LoadRequest{
public:
    LoadRequest();
    ~LoadRequest();
    int rid;//request[0] contains pickup id 1 and delivery id n+1
    Node pickup,delivery;
    //double book_time; for DARP
};

#endif