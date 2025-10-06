#include "WareHouse.h"
#include "Customer.h"
#include "Volunteer.h"
#include "Action.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;



 WareHouse::WareHouse(const string& configFilePath): isOpen(false),actionsLog(),volunteers(),
 pendingOrders(),inProcessOrders(),completedOrders(),customers(),customerCounter(0),volunteerCounter(0),orderCounter(0){

    std::ifstream configFile(configFilePath);

    string line;
    while(getline(configFile, line)) {
        std::istringstream iss(line);
        std::string type;
        iss>> type;
        string name;
        string category;
        int distance;
        int maxOrders;

         if (type == "customer") {
                iss >> name >> category >> distance >> maxOrders;
                if(category=="soldier") {
                    Customer* soldier= new SoldierCustomer (customerCounter, name, distance, maxOrders);
                    customers.push_back(soldier);}

                if(category=="civilian") { 
                    Customer* civilian= new CivilianCustomer (customerCounter, name, distance, maxOrders);
                    customers.push_back(civilian);}
                
                customerCounter++;
        } 
        else if (type == "volunteer") {
                int coolDown;
                int maxDistance;
                int distancePerStep;

                iss >> name >> category;
                if (category == "collector") {
                    iss >> coolDown >> maxOrders;
                    Volunteer* collector=new CollectorVolunteer (volunteerCounter,name,coolDown);
                    volunteers.push_back(collector);
                 }

                if (category == "limited_collector") {
                    iss >> coolDown >> maxOrders; 
                    Volunteer* lim_collector=new LimitedCollectorVolunteer(volunteerCounter, name, coolDown,maxOrders);
                    volunteers.push_back(lim_collector);

                    }
                if (category == "driver"){
                    iss >> maxDistance >>distancePerStep;
                    Volunteer* driver = new DriverVolunteer(volunteerCounter, name, maxDistance,distancePerStep);
                    volunteers.push_back(driver);
                }
                if (category == "limited_driver"){
                    iss >> maxDistance >>distancePerStep>> maxOrders;
                    Volunteer* lim_driver = new LimitedDriverVolunteer(volunteerCounter, name, maxDistance,distancePerStep,maxOrders);
                    volunteers.push_back(lim_driver);
                }
                volunteerCounter++;
            }
    }


        
 }


 WareHouse:: ~WareHouse() { 
    for (std::vector<BaseAction*>::iterator it = actionsLog.begin(); it != actionsLog.end(); ++it) {
        delete *it;
    }
    for (std::vector<Volunteer*>::iterator it = volunteers.begin(); it != volunteers.end(); ++it) {
        delete *it;
    }
    for (std::vector<Order*>::iterator it = pendingOrders.begin(); it != pendingOrders.end(); ++it) {
        delete *it;
    }
    for (std::vector<Order*>::iterator it = inProcessOrders.begin(); it != inProcessOrders.end(); ++it) {
        delete *it;
    }
    for (std::vector<Order*>::iterator it = completedOrders.begin(); it != completedOrders.end(); ++it) {
        delete *it;
    }
    for (std::vector<Customer*>::iterator it = customers.begin(); it != customers.end(); ++it) {
        delete *it;
    }
       
    actionsLog.clear();volunteers.clear();pendingOrders.clear();
    inProcessOrders.clear();completedOrders.clear();customers.clear();

 }
 
WareHouse:: WareHouse(const WareHouse &other):isOpen(other.isOpen),actionsLog(),volunteers(),pendingOrders(),inProcessOrders(),completedOrders(),customers(),
 customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter),
 orderCounter(other.orderCounter){

    for (BaseAction* action : other.actionsLog) {
            actionsLog.push_back(action->clone());
        }
    for (Volunteer* volunteer : other.volunteers) {
            volunteers.push_back(volunteer->clone());
        }
    for (Order* pending : other.pendingOrders) {
            pendingOrders.push_back(new Order(*pending));
        }
    for (Order* inProcess : other.inProcessOrders) {
            inProcessOrders.push_back(new Order(*inProcess));
        }
    for (Order* completed : other.completedOrders) {
            completedOrders.push_back(new Order(*completed));
        }
    for (Customer* customer : other.customers) {
            customers.push_back(customer->clone());
        }

}
WareHouse &WareHouse::operator=(const WareHouse &other){
    

    if( this!=&other){

        for (BaseAction* action : actionsLog) {
            delete action;
        }
        for (Volunteer* volunteer : volunteers) {
                delete volunteer;
        }
        for (Order* pending : pendingOrders) {
                delete pending;
        }
        for (Order* inProcess : inProcessOrders) {
                 delete inProcess;
        }
        for (Order* completed : completedOrders) {
                delete completed;
        }
        for (Customer* customer : customers) {
                delete customer;
        }
       
            actionsLog.clear();
            volunteers.clear();
            pendingOrders.clear();
            inProcessOrders.clear();
            completedOrders.clear();
            customers.clear();

        for (BaseAction* action : other.actionsLog) {
            actionsLog.push_back(action->clone());
        }
        for (Volunteer* volunteer : other.volunteers) {
                volunteers.push_back(volunteer->clone());
        }
        for (Order* pending : other.pendingOrders) {
                pendingOrders.push_back(new Order(*pending));
        }
        for (Order* inProcess : other.inProcessOrders) {
                inProcessOrders.push_back(new Order(*inProcess));
        }
        for (Order* completed : other.completedOrders) {
                completedOrders.push_back(new Order(*completed));
        }
        for (Customer* customer : other.customers) {
                customers.push_back(customer->clone());
        }
            customerCounter= other.customerCounter;
            volunteerCounter= other.volunteerCounter;
            orderCounter= other.orderCounter;
            isOpen = other.isOpen;
    }
    return *this;

}

WareHouse:: WareHouse(WareHouse&& other)noexcept: isOpen(other.isOpen),actionsLog(),volunteers(),pendingOrders(),inProcessOrders(),completedOrders(),customers(),
  customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter){
    
        for (BaseAction* action : other.actionsLog) {
            actionsLog.push_back(action->clone());
            delete action;
        }
        for (Volunteer* volunteer : other.volunteers) {
                volunteers.push_back(volunteer->clone());
                delete volunteer;
        }
        for (Order* pending : other.pendingOrders) {
                pendingOrders.push_back(new Order(*pending));
                delete pending;
        }
        for (Order* inProcess : other.inProcessOrders) {
                inProcessOrders.push_back(new Order(*inProcess));
                 delete inProcess;
        }
        for (Order* completed : other.completedOrders) {
                completedOrders.push_back(new Order(*completed));
                delete completed;
        }
        for (Customer* customer : other.customers) {
                customers.push_back(customer->clone());
                delete customer;
        }
        other.actionsLog.clear();
        other.volunteers.clear();
        other.pendingOrders.clear();
        other.inProcessOrders.clear();
        other.completedOrders.clear();
        other.customers.clear();
        isOpen = other.isOpen;
        other.customerCounter=0;
        other.volunteerCounter=0;
        other.orderCounter=0;
        other.isOpen=false;  
}

   
    



WareHouse& WareHouse::operator=(WareHouse&& other)noexcept{

    if( this!=&other){
        for (std::vector<BaseAction*>::iterator it = actionsLog.begin(); it != actionsLog.end(); ++it) {
            delete *it;
        }
        for (std::vector<Volunteer*>::iterator it = volunteers.begin(); it != volunteers.end(); ++it) {
            delete *it;
        }
        for (std::vector<Order*>::iterator it = pendingOrders.begin(); it != pendingOrders.end(); ++it) {
            delete *it;
        }
        for (std::vector<Order*>::iterator it = inProcessOrders.begin(); it != inProcessOrders.end(); ++it) {
            delete *it;
        }
        for (std::vector<Order*>::iterator it = completedOrders.begin(); it != completedOrders.end(); ++it) {
            delete *it;
        }
        for (std::vector<Customer*>::iterator it = customers.begin(); it != customers.end(); ++it) {
            delete *it;
        }
        actionsLog.clear();
        volunteers.clear();
        pendingOrders.clear();
        inProcessOrders.clear();
        completedOrders.clear();
        customers.clear();

        for (BaseAction* action : other.actionsLog) {
            actionsLog.push_back(action->clone());
        }
        for (Volunteer* volunteer : other.volunteers) {
                volunteers.push_back(volunteer->clone());
        }
        for (Order* pending : other.pendingOrders) {
                pendingOrders.push_back(new Order(*pending));
        }
        for (Order* inProcess : other.inProcessOrders) {
                inProcessOrders.push_back(new Order(*inProcess));
        }
        for (Order* completed : other.completedOrders) {
                completedOrders.push_back(new Order(*completed));
        }
        for (Customer* customer : other.customers) {
                customers.push_back(customer->clone());
        }

            customerCounter= other.customerCounter;
            volunteerCounter= other.volunteerCounter;
            orderCounter= other.orderCounter;
            isOpen = other.isOpen;
        
            for (std::vector<BaseAction*>::iterator it = other.actionsLog.begin(); it != other.actionsLog.end(); ++it) {
                delete *it;
            }
            for (std::vector<Volunteer*>::iterator it = other.volunteers.begin(); it != other.volunteers.end(); ++it) {
                delete *it;
            }
            for (std::vector<Order*>::iterator it = other.pendingOrders.begin(); it != other.pendingOrders.end(); ++it) {
                delete *it;
            }
            for (std::vector<Order*>::iterator it = other.inProcessOrders.begin(); it != other.inProcessOrders.end(); ++it) {
                delete *it;
            }
            for (std::vector<Order*>::iterator it = other.completedOrders.begin(); it != other.completedOrders.end(); ++it) {
                delete *it;
            }
            for (std::vector<Customer*>::iterator it = other.customers.begin(); it != other.customers.end(); ++it) {
                delete *it;
            }
            
            other.actionsLog.clear();
            other.volunteers.clear();
            other.pendingOrders.clear();
            other.inProcessOrders.clear();
            other.completedOrders.clear();
            other.customers.clear();
            isOpen = other.isOpen;
            other.customerCounter=0;
            other.volunteerCounter=0;
            other.orderCounter=0;
            other.isOpen=false;
            }

        return *this;
}
       



void WareHouse:: start() { 
    cout << "warehouse is open!" << endl ;
    open();
    while (isOpen){
            string act;
            cin >> act;
            int id;
            int numofstep; 

            if (act == "step"){
                cin >> numofstep;
                SimulateStep step =  SimulateStep(numofstep);
                step.act(*this);  
            }
            
            else if(act=="order"){
                cin >>id;
                AddOrder order = AddOrder(id);
                order.act(*this);
            }

            else if(act=="customer"){
                string name, type;
                cin >>name >> type;
                int dist, maxOrd;
                cin>> dist >> maxOrd;
                
                AddCustomer customer = AddCustomer(name, type, dist, maxOrd);
                customer.act(*this);
            }
            else if(act=="orderStatus"){
                cin >>id;
                PrintOrderStatus orderstatus = PrintOrderStatus(id);
                orderstatus.act(*this);
            }
            else if(act=="customerStatus"){
                cin >>id;
                PrintCustomerStatus customerStatus = PrintCustomerStatus(id);
                customerStatus.act(*this);
            }
            else if(act=="volunteerStatus"){
                cin >>id;
                PrintVolunteerStatus volunteerStatus = PrintVolunteerStatus(id);
                volunteerStatus.act(*this);
            }
            else if(act=="log"){
                PrintActionsLog actionsLog = PrintActionsLog();
                actionsLog.act(*this);
            }
            else if(act=="close"){
                Close close = Close();
                close.act(*this);
            }
            else if(act=="backup"){
                BackupWareHouse backup = BackupWareHouse();
                 backup.act(*this);
            }
            else if(act=="restore"){
                RestoreWareHouse Restore = RestoreWareHouse();
                 Restore.act(*this);
            }
            
        }

     
 }
 void WareHouse:: addOrder(Order* order){
    pendingOrders.push_back(order);
 }
 void WareHouse:: addAction(BaseAction* action){
    actionsLog.push_back(action);
 }
 Customer &WareHouse:: getCustomer(int customerId) const{
    for(int i=0; i<(int)customers.size();i++)
    {
        Customer* cust = customers[i];
        if( cust->getId() == customerId){
            return *cust;
        }
    }

    throw runtime_error("Customer not found");
 }
Volunteer &WareHouse:: getVolunteer(int volunteerId) const{
    for(int i=0; i<(int)volunteers.size();i++)
    {
        Volunteer* vol = volunteers[i];
        if( vol->getId() == volunteerId){
            return *vol;
        }
    }
    throw runtime_error("Volunteer not found");

}
 Volunteer *WareHouse:: collectorAvl(vector<Volunteer*>& volunteers, Order &order){
    for(int i=0; i<(int)getvolunteers().size(); i++){
        Volunteer* collector = dynamic_cast<CollectorVolunteer*>(getvol(i));
        Volunteer* limcollector = dynamic_cast<LimitedCollectorVolunteer*>(getvol(i));
        if(collector != nullptr){
            if(collector->canTakeOrder(order)) return collector;}
        else if(limcollector != nullptr){
            if(limcollector->canTakeOrder(order)) return limcollector;}

    }
    return nullptr;

 }
 Volunteer *WareHouse:: driverAvl(vector<Volunteer*>& volunteers, Order &order){
    for(int i=0; i< (int)getvolunteers().size(); i++){
        DriverVolunteer* driver = dynamic_cast<DriverVolunteer*>(getvol(i));
        if(driver!=nullptr) {
            if(getvol(i)->canTakeOrder(order)) return getvol(i);
        }
    }
    return nullptr;
 }


Order &WareHouse:: getOrder(int orderId) const{
    for(int i=0; i<(int)pendingOrders.size(); i++){
            if(pendingOrders[i]->getId() == orderId){
                return *pendingOrders[i];
            }
        }

    for(int i=0; i<(int)inProcessOrders.size(); i++)
        {
            if(inProcessOrders[i]->getId() == orderId){
                return *inProcessOrders[i];
            }
        }
    for(int i=0; i<(int)completedOrders.size(); i++)
        {
            if(completedOrders[i]->getId() == orderId){
                return *completedOrders[i];
            }
        }
    throw runtime_error("Order not found");
}

int WareHouse:: getOrderCounter ()const {
    return orderCounter;
}
void WareHouse:: increaseOrderCounter(){
    orderCounter = orderCounter+1;
}
int WareHouse:: getcustomerCounter ()const {
    return customerCounter;
}
void WareHouse:: increasecustomerCounter(){
    customerCounter = customerCounter+1;
}
int WareHouse:: getvolunteerCounter ()const {
    return volunteerCounter;
}
void WareHouse:: increasevolunteerCounter(){
    volunteerCounter = volunteerCounter+1;
}
const vector<BaseAction*> &WareHouse:: getActions() const{
    return actionsLog;

}
const vector<Order*> &WareHouse:: getpendingOrders() const{
    return pendingOrders;

}
const vector<Order*> &WareHouse:: getinProcessOrders() const{
    return inProcessOrders;

}
const vector<Order*> &WareHouse:: getcompletedOrders() const{
    return completedOrders;

}
const vector<Volunteer*> &WareHouse:: getvolunteers() const{
    return volunteers;
}
const vector<Customer*> &WareHouse:: getCustomers() const{
    return customers;
}


void WareHouse:: addToinProcess(Order* order){
   inProcessOrders.push_back(order);

}
void WareHouse:: addToCompleted(Order* order){
    completedOrders.push_back(order);

}
void WareHouse:: addToCustomers(Customer* customer){
    customers.push_back(customer);

}
void WareHouse:: addToVolunteers(Volunteer* volunteer){
    volunteers.push_back(volunteer);

}

void WareHouse:: erasePendings(int i){
    pendingOrders.erase(pendingOrders.begin()+i);
}
void WareHouse:: eraseInProcess(int i){
    inProcessOrders.erase(inProcessOrders.begin()+i);
}
void WareHouse:: eraseVolunteer(int i){
    volunteers.erase(volunteers.begin()+i);
}
Order *WareHouse:: getpendings(int i){
    return pendingOrders[i];
}
Order *WareHouse::getinprocess(int i){
     return inProcessOrders[i];
}
Order *WareHouse::getcompleted(int i){
     return completedOrders[i];
}
Volunteer *WareHouse::getvol(int i){
    return volunteers[i];
}
 Customer *WareHouse::getcus(int i){
    return customers[i];
 }






void WareHouse :: close(){
    isOpen = false;
}

void WareHouse:: open(){
    isOpen = true;
    
}


