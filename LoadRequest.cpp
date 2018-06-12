#include "LoadRequest.h"

Node::Node()
{
    id=0;
    service_time=0;
    start_time=0;
    end_time=0;
    demand=0;
    type_const=0;
 //   x,y=0;
}

Node::~Node(){}

LoadRequest::LoadRequest()
{
    rid=-1;
}

LoadRequest::~LoadRequest(){}