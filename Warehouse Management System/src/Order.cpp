#include "Order.h"
#include <iostream>
#include <string>

using namespace std;

Order::Order (int id, int customerId, int distance):
  id(id), customerId(customerId), distance(distance),status(OrderStatus::PENDING),collectorId(NO_VOLUNTEER),
  driverId(NO_VOLUNTEER){}
Order::Order (const Order& other): id(other.id),customerId(other.customerId), distance(other.distance),status(other.getStatus()),
collectorId(other.collectorId), driverId(other.driverId){}



int Order::getId() const {
    return id;
}

int Order::getCustomerId() const{
    return customerId;
}
int Order::getDistance() const{
    return distance;
}

void Order::setStatus(OrderStatus st){
    status = st;

}
void Order::setCollectorId(int colId){
    collectorId = colId;
}
void  Order::setDriverId(int drId){
    driverId = drId;
}
int  Order::getCollectorId() const{
    return collectorId;
}
 int Order:: getDriverId () const{
   return driverId; 
 }
 OrderStatus Order:: getStatus() const{
    return status;
 }
string Order:: getStatusString(){
    string output="";
    switch (status){
        case OrderStatus::PENDING:
             output= "PENDING";
            break;
        case OrderStatus::COLLECTING:
            output= "COLLECTING";
            break;
        case OrderStatus::DELIVERING:
            output= "DELIVERING";
            break;
        case OrderStatus::COMPLETED:
           output= "COMPLETED";
    }
    return output;
}

 const string Order:: toString() const{
    std:: string s = to_string(id) +","+ to_string(customerId) +",";
    if(collectorId==-1){
        s= s + "None, ";}
    else { s=s+to_string(collectorId)+ ",";}

    if(driverId==-1){
        s= s + "None";}
    else { s=s+to_string(driverId);}

    switch (status){
        case OrderStatus::PENDING:
            s=s +","+ "PENDING";
            break;
        case OrderStatus::COLLECTING:
            s=s +","+ "COLLECTING";
            break;
        case OrderStatus::DELIVERING:
            s=s +","+ "DELIVERING";
            break;
        case OrderStatus::COMPLETED:
            s=s +","+ "COMPLETED";
    }
    return s;
 }







