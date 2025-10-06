#include "Volunteer.h"
using namespace std;
#include <iostream>
#include <fstream>
#include <sstream>

Volunteer :: Volunteer (int id, const string &name): completedOrderId(NO_ORDER),
activeOrderId(NO_ORDER),id(id),name(name){}

int  Volunteer::getId() const{
    return id;
}
const string &Volunteer:: getName() const{
    return name;
}
int  Volunteer::getActiveOrderId() const{
    return activeOrderId;
}
void Volunteer::setActiveOrderId(int activeId){
   activeOrderId = activeId;
}
int  Volunteer::getCompletedOrderId() const{
    return completedOrderId;
}
void Volunteer::setCompletedOrderId(int completedId){
     completedOrderId= completedId;
}
bool Volunteer:: isBusy() const{
    if (activeOrderId == NO_ORDER){
        return false;
    }
    return true;
}



CollectorVolunteer:: CollectorVolunteer (int id, const string &name, int coolDown):
    Volunteer(id, name),coolDown(coolDown), timeLeft(0){}

 CollectorVolunteer *CollectorVolunteer:: clone() const {
    return new CollectorVolunteer(*this);
}
void CollectorVolunteer::step(){
   timeLeft = timeLeft-1;
}
int CollectorVolunteer::getCoolDown() const{
  return coolDown;  
}
int CollectorVolunteer::getTimeLeft() const{
    return timeLeft;
}
void CollectorVolunteer::setTimeLeft(int time){
    timeLeft= time;
}
bool CollectorVolunteer:: decreaseCoolDown(){
    timeLeft = timeLeft-1;
    return getTimeLeft() == 0;
}
bool CollectorVolunteer::hasOrdersLeft() const{
    return true;
}
 bool CollectorVolunteer:: canTakeOrder(const Order &order) const {
    return !isBusy();
 }
 void CollectorVolunteer:: acceptOrder(const Order &order){
    setActiveOrderId(order.getId());
    setTimeLeft(getCoolDown());
 }
string CollectorVolunteer::toString() const{
    std::string str = getName() +", " + std::to_string(getId()) +
    ", " + std::to_string(getCoolDown()) + ", " + std::to_string(getTimeLeft());
        return str;
}

LimitedCollectorVolunteer:: LimitedCollectorVolunteer (int id, const string &name, int coolDown,int maxOrders):
    CollectorVolunteer(id, name,coolDown),maxOrders(maxOrders),ordersLeft(maxOrders){}

LimitedCollectorVolunteer *LimitedCollectorVolunteer:: clone() const {
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer :: hasOrdersLeft() const{
        return (ordersLeft > 0 );
    
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const{
    return ((hasOrdersLeft()) && (!isBusy()));
     
}
void LimitedCollectorVolunteer::acceptOrder(const Order &order){
    setActiveOrderId(order.getId());
    ordersLeft = ordersLeft-1;
    setTimeLeft(getCoolDown());
}

int LimitedCollectorVolunteer::getMaxOrders() const{
    return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}

string LimitedCollectorVolunteer::toString() const{
    std::string str = getName() +", " + std::to_string(getId()) +
    ", " + std::to_string(getCoolDown()) + ", " + std::to_string(getTimeLeft())+ "," + std::to_string(getMaxOrders())
    + "," + std::to_string(getNumOrdersLeft());
        return str;
}

DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep):
    Volunteer(id, name),maxDistance(maxDistance),distancePerStep(distancePerStep),distanceLeft(0){}

DriverVolunteer *DriverVolunteer:: clone() const {
    return new DriverVolunteer(*this);
}
int DriverVolunteer:: getDistanceLeft() const{
    return distanceLeft;
}
void DriverVolunteer:: setDistanceLeft(int Left){
    distanceLeft= Left;
}
int DriverVolunteer:: getMaxDistance() const{
    return maxDistance;
}
int DriverVolunteer:: getDistancePerStep() const{
    return distancePerStep;
}
bool DriverVolunteer:: decreaseDistanceLeft(){
    int distLeft = getDistanceLeft();
    int distPerStep = getDistancePerStep();
    distanceLeft = distLeft - distPerStep;
    if (distanceLeft < 0){distanceLeft=0;}
    return getDistanceLeft() == 0;
}
bool DriverVolunteer::hasOrdersLeft() const{
    return true;
}
bool DriverVolunteer:: canTakeOrder(const Order &order) const{
    return (order.getDistance() <= getMaxDistance() && !isBusy());
}
void DriverVolunteer:: acceptOrder(const Order &order){
    distanceLeft = order.getDistance();
}
void DriverVolunteer:: step(){
    decreaseDistanceLeft();
}
string DriverVolunteer:: toString() const{
   std::string str = getName()+ "," + std::to_string(getId()) + "," +std::to_string(getMaxDistance()) +","
    + std::to_string(getDistancePerStep()) + "," +std::to_string(getDistanceLeft());
    return str;
}
LimitedDriverVolunteer::LimitedDriverVolunteer (int id, const string &name, int maxDistance, int distancePerStep,int maxOrders):
    DriverVolunteer(id, name,maxDistance,distancePerStep),maxOrders(maxOrders),ordersLeft(maxOrders){}

LimitedDriverVolunteer *LimitedDriverVolunteer:: clone() const {
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer:: getMaxOrders() const{
    return maxOrders;
}
int LimitedDriverVolunteer:: getNumOrdersLeft() const{
    return ordersLeft;
}
bool LimitedDriverVolunteer:: hasOrdersLeft() const{
    return getNumOrdersLeft() > 0;
}
bool LimitedDriverVolunteer:: canTakeOrder(const Order &order) const{
    return ((order.getDistance() <= getMaxDistance()) && (!isBusy()) && (hasOrdersLeft()));
}
void LimitedDriverVolunteer:: acceptOrder(const Order &order) {
  setDistanceLeft(order.getDistance());
  ordersLeft--;
}
 string LimitedDriverVolunteer:: toString() const{
    std::string str = getName()+ "," + std::to_string(getId()) + "," +std::to_string(getMaxDistance()) +","
    + std::to_string(getDistancePerStep()) + "," +std::to_string(getDistanceLeft()) + "," +std::to_string(getMaxOrders())+
    "," + std::to_string(getNumOrdersLeft()); 
    return str;
 }