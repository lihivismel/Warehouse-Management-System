#include "WareHouse.h"
#include "Customer.h"
#include "Volunteer.h"
#include "Order.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
#include "Action.h"

BaseAction:: BaseAction():errorMsg(""),status(){};

ActionStatus BaseAction:: getStatus() const{
    return status;
}
void BaseAction:: complete() {
    status = ActionStatus::COMPLETED;
}
void BaseAction:: error(string errorMsg) {
    setErrorMsg(errorMsg);
    status=  ActionStatus:: ERROR;
}
string BaseAction:: getErrorMsg() const{
    return errorMsg;
}
void BaseAction:: setErrorMsg(string msg){
    errorMsg = msg;
}




SimulateStep:: SimulateStep (int numOfSteps):
 BaseAction(), numOfSteps(numOfSteps){
 };

 void SimulateStep:: act(WareHouse &wareHouse) {
    for(int n=0;n<numOfSteps;n++){
        vector<Volunteer*> volunteers=wareHouse.getvolunteers();
        int sizePending= (int)wareHouse.getpendingOrders().size();
        for(int i=0; i<sizePending;i++){
            OrderStatus status = wareHouse.getpendings(i)->getStatus();
            if(status==OrderStatus:: PENDING){
                Volunteer* notBusy = wareHouse.collectorAvl(volunteers,*wareHouse.getpendings(i));
                if(notBusy!=nullptr){ 
                    CollectorVolunteer* col= dynamic_cast<CollectorVolunteer*>(notBusy);
                    wareHouse.getpendings(i)->setCollectorId(notBusy->getId());
                    OrderStatus status = OrderStatus:: COLLECTING;
                    wareHouse.getpendings(i)->setStatus(status);
                    col->acceptOrder(*wareHouse.getpendings(i));
                    wareHouse.addToinProcess(wareHouse.getpendings(i));
                    wareHouse.erasePendings(i);
                    vector<Order*> pending2 = wareHouse.getpendingOrders();
                    sizePending--;
                    i--; 
                }
            }
            else {
                if(status==OrderStatus:: COLLECTING){
                    Volunteer* notBusy = wareHouse.driverAvl(volunteers,*wareHouse.getpendings(i));
                    if(notBusy!=nullptr){ 
                        DriverVolunteer* dr= dynamic_cast<DriverVolunteer*>(notBusy);
                        wareHouse.getpendings(i)->setDriverId(notBusy->getId());
                        wareHouse.getpendings(i)->setStatus(OrderStatus::DELIVERING);
                        dr->acceptOrder(*wareHouse.getpendings(i));
                        notBusy->setActiveOrderId(wareHouse.getpendings(i)->getId());
                        wareHouse.addToinProcess(wareHouse.getpendings(i));
                        wareHouse.erasePendings(i);
                        sizePending--;
                        i--; 
                    }
                }
            }
        }
        int sizeInProcessOrders= (int)wareHouse.getinProcessOrders().size();
        for(int i=0; i<sizeInProcessOrders;i++){
            if(wareHouse.getinprocess(i)->getStatus()==OrderStatus:: COLLECTING){
                int id= wareHouse.getinprocess(i)->getCollectorId();

                Volunteer& collector= wareHouse.getVolunteer(id);
                CollectorVolunteer* col= dynamic_cast<CollectorVolunteer*>(&collector);

                col->step();

                if(col->getTimeLeft()<= 0){
                    col->setCompletedOrderId(wareHouse.getinprocess(i)->getId());
                    wareHouse.addOrder(wareHouse.getinprocess(i));
                    wareHouse.eraseInProcess(i);
                    col->setActiveOrderId(-1);
                    if(!col->hasOrdersLeft()){
                        int j=0;
                        while(j<(int)wareHouse.getvolunteers().size() && wareHouse.getvol(j)->getId() != col->getId()){
                            j++;
                        }
                        if(wareHouse.getvol(j)->getId() == col->getId()){
                            delete wareHouse.getvol(j);
                            wareHouse.eraseVolunteer(j);
                        }
                    }

                sizeInProcessOrders--;
                i--;    
                }
            }

            else{ 
                if(wareHouse.getinprocess(i)->getStatus()==OrderStatus:: DELIVERING){
                    int id= wareHouse.getinprocess(i)->getDriverId();
                    Volunteer& driver= wareHouse.getVolunteer(id);
                    DriverVolunteer* dr= dynamic_cast<DriverVolunteer*>(&driver); 
                    bool isO= dr->decreaseDistanceLeft();
                    if(isO){
                            dr->setActiveOrderId(-1);
                            dr->setCompletedOrderId(wareHouse.getinprocess(i)->getId());
                            wareHouse.getinprocess(i)->setStatus(OrderStatus::COMPLETED);
                            wareHouse.addToCompleted(wareHouse.getinprocess(i));
                            wareHouse.eraseInProcess(i);
                            if(!dr->hasOrdersLeft()){
                                int j=0;
                                while(j<(int)wareHouse.getvolunteers().size() && wareHouse.getvol(j)->getId() != dr->getId()){
                                    j++;
                                }
                                if(wareHouse.getvol(j)->getId() == dr->getId()){
                                    delete wareHouse.getvol(j);
                                    wareHouse.eraseVolunteer(j);
                                }
                            }
                            sizeInProcessOrders--;
                            i--;
                        }
                }
            }
        }
    }
    complete();
    SimulateStep *act = clone();
    wareHouse.addAction(act);

    
 }

string SimulateStep :: toString() const {
    string str = "simulateStep "  + to_string(numOfSteps) + " COMPLETED";
    return str;

}
SimulateStep *SimulateStep :: clone() const{
    return new SimulateStep(*this);
}



AddOrder:: AddOrder(int id): BaseAction(), customerId(id){
};

void AddOrder:: act(WareHouse &wareHouse){
    int dis=-1;
    vector<Order*> pending = wareHouse.getpendingOrders();
    vector<Customer*> customers=  wareHouse.getCustomers();
    int i=0;
    while ((i<(int)customers.size())&& (wareHouse.getcus(i)->getId() != customerId)){
        i++;}
        if(i<(int)customers.size()){
            if(wareHouse.getcus(i)->getId() == customerId){
                dis= wareHouse.getcus(i)->getCustomerDistance();
                if(!wareHouse.getcus(i)->canMakeOrder()){
                    error("cannot place this order");
                    std::cout << "error: "+ getErrorMsg() <<endl;
                }
                else if(dis!=-1){
                    wareHouse.getcus(i)->addOrder(wareHouse.getOrderCounter());
                    Order* order = new Order(wareHouse.getOrderCounter() ,customerId, dis);
                    wareHouse.addOrder(order);
                    wareHouse.increaseOrderCounter();

                    complete();}
            }
        }
    if(dis==-1){
        error("cannot place this order"); 
        std::cout << "error: " + getErrorMsg() <<endl; }
        
    BaseAction *act =  clone();
    wareHouse.addAction(act);
}

string AddOrder:: toString() const{
    string s="";
    if(getStatus()== ActionStatus:: COMPLETED){
        s = "order "  + std::to_string(customerId) + " COMPLETED";}
    
    if(getStatus()== ActionStatus:: ERROR){
        s = "order "  + std::to_string(customerId) + " ERROR";}
    return s;
    
}   


AddOrder *AddOrder :: clone() const{
    return new AddOrder(*this);
}


AddCustomer:: AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders):
BaseAction(), customerName(customerName), customerType(type(customerType)), distance(distance),maxOrders(maxOrders){}
      
CustomerType AddCustomer:: type(string s){
    
    if(s=="soldier") return CustomerType:: Soldier;
    else return CustomerType:: Civilian;
}
void AddCustomer::act(WareHouse &wareHouse){
    vector<Customer*> customers=  wareHouse.getCustomers();
    int id= wareHouse.getcustomerCounter();
    wareHouse.increasecustomerCounter();
    if(customerType==CustomerType:: Soldier){
        SoldierCustomer* customer = new SoldierCustomer(id, customerName, distance, maxOrders);
        wareHouse.addToCustomers(customer);

    }
    if(customerType==CustomerType:: Civilian){
        CivilianCustomer* customer = new CivilianCustomer(id, customerName, distance, maxOrders);
        wareHouse.addToCustomers(customer);
    }
    
    complete();
    BaseAction *act =  clone();
    wareHouse.addAction(act);
}
string AddCustomer:: toString() const{
    string str = "";
    if( customerType== CustomerType:: Civilian ){
       str = "customer "  + customerName + " Civilian " + to_string(distance) + " " +to_string(maxOrders) ;
    }
    else if (customerType== CustomerType:: Soldier){
        str =  "customer "  + customerName + " Soldier " + to_string(distance) + " " + to_string(maxOrders) ;
    }
    
    if(getStatus()== ActionStatus:: COMPLETED){
        str = str + " COMPLETED";
    }
    else if (getStatus()== ActionStatus:: ERROR){
        str = str + " ERROR";
    }
    return str;
}


AddCustomer *AddCustomer :: clone() const{
    return new AddCustomer(*this);
}


PrintOrderStatus:: PrintOrderStatus(int id): orderId(id){}


void PrintOrderStatus:: act(WareHouse &wareHouse){
    vector<Order*> pending = wareHouse.getpendingOrders();
    vector<Order*> inProcess = wareHouse.getinProcessOrders();
    vector<Order*> completedOrders = wareHouse.getcompletedOrders();
    bool isFound=false;
    for(int i=0; (i<(int)pending.size())&(!isFound);i++){
        if(wareHouse.getpendings(i)->getId()==orderId){
            std::cout<< "orderID: "  << wareHouse.getpendings(i)->getId() << endl;
            std::cout<< "orderStatus: "  << wareHouse.getpendings(i)->getStatusString() << endl;
            std::cout<< "customerID: "  << wareHouse.getpendings(i)->getCustomerId() << endl;
            if(wareHouse.getpendings(i)->getCollectorId()==-1){
                std::cout<< "CollectorId: None" << endl;}
            else {
                 std::cout<< "CollectorId: "  << wareHouse.getpendings(i)->getCollectorId() << endl;
                 }

            if(wareHouse.getpendings(i)->getDriverId()==-1){
                std::cout<< "driverID = None" << endl;}
            else {std:: cout<< "driverID: " << wareHouse.getpendings(i)->getDriverId()  << endl;}
            isFound=true;
        }
    }
    for(int i=0; (i<(int)inProcess.size())&(!isFound);i++){
        if(wareHouse.getinprocess(i)->getId()==orderId){
            std::cout<< "orderID: "  << wareHouse.getinprocess(i)->getId() << endl;
            std::cout<< "orderStatus: "  << wareHouse.getinprocess(i)->getStatusString() << endl;
            std::cout<< "customerID: "  << wareHouse.getinprocess(i)->getCustomerId() << endl;
            if(wareHouse.getinprocess(i)->getCollectorId()==-1){
                std::cout<< "CollectorId: None" << endl;}
            else {
                 std::cout<< "CollectorId: "  << wareHouse.getinprocess(i)->getCollectorId() << endl;
                 }

            if(wareHouse.getinprocess(i)->getDriverId()==-1){
                std::cout<< "driverID = None" << endl;}
            else {std:: cout<< "driverID: " << wareHouse.getinprocess(i)->getDriverId()  << endl;}
            isFound=true;}

    }
    for(int i=0; (i<(int)completedOrders.size())&(!isFound);i++){
        if(wareHouse.getcompleted(i)->getId()==orderId){
            std::cout<< "orderID: "  << wareHouse.getcompleted(i)->getId() << endl;
            std::cout<< "orderStatus: "  << wareHouse.getcompleted(i)->getStatusString() << endl;
            std::cout<< "customerID: "  << wareHouse.getcompleted(i)->getCustomerId() << endl;
            if(wareHouse.getcompleted(i)->getCollectorId()==-1){
                std::cout<< "CollectorId: None" << endl;}
            else {
                 std::cout<< "CollectorId: "  << wareHouse.getcompleted(i)->getCollectorId() << endl;
                 }

            if(wareHouse.getcompleted(i)->getDriverId()==-1){
                std::cout<< "driverID = None" << endl;}
            else {std:: cout<< "driverID: " << wareHouse.getcompleted(i)->getDriverId()  << endl;}
            isFound=true;

        }
    }
    if(isFound){
        complete();
    }
    else{
        error("order doesn't exist");
        std:: cout << "error: " + getErrorMsg() <<endl;
    }

    BaseAction *act =  clone();
    wareHouse.addAction(act);

    
    
}

string PrintOrderStatus:: toString()  const{
     string s="";
    if(getStatus()== ActionStatus:: COMPLETED){
        s = "orderStatus "  + std::to_string(orderId) + " COMPLETED";}
    
    if(getStatus()== ActionStatus:: ERROR){
        s = "orderStatus "  + std::to_string(orderId) + " ERROR";}
    return s;
    
}   
    
PrintOrderStatus *PrintOrderStatus :: clone() const{
    return new PrintOrderStatus(*this);
}


PrintCustomerStatus:: PrintCustomerStatus(int customerId):customerId(customerId){}

void PrintCustomerStatus:: act(WareHouse &wareHouse){
    vector<Customer*> customers = wareHouse.getCustomers();
    vector<Order*> pending = wareHouse.getpendingOrders();
    vector<Order*> inProcess = wareHouse.getinProcessOrders();
    vector<Order*> completedOrders = wareHouse.getcompletedOrders();
    int i=0;
    bool isFound= false;
    while(i<(int)customers.size() && !isFound){
        if(customers[i]->getId()==customerId){ isFound=true;}
        else { i++; }
    }
    if(isFound){
        std::cout << "Customer Id- " <<customers[i]->getId() << endl;
        Customer* customer= customers[i];
        vector<int> cust_orders =  customer->getOrdersIds();
        bool found;
        for(int j=0; j <(int)cust_orders.size(); j++){
            int orderId=cust_orders[j];
            std::cout << "Order Id- " <<orderId << endl;
            found = false;

            for(int k=0; (k<(int)pending.size()) & (!found);k++){
                if(wareHouse.getpendings(k)->getId()==orderId){
                    string status = wareHouse.getpendings(k)->getStatusString();
                    found = true;
                    std::cout<< "Order status: " << status << endl;
                }
            }
            for(int k=0; (k<(int)inProcess.size()) & (!found);k++){
                if(wareHouse.getinprocess(k)->getId()==orderId){
                    string status = wareHouse.getinprocess(k)->getStatusString();
                    found = true;
                    std::cout<< "Order status: " << status << endl;                }
            }
            for(int k=0;(k<(int)completedOrders.size()) & (!found);k++){
                if(wareHouse.getcompleted(k)->getId()==orderId){
                    found = true;
                    std::cout<< "Order status: COMPLETED" << endl;                }
            }

        }
        std::cout << "number of orders left- " << wareHouse.getcus(i)->getMaxOrders() - customers[i]->getNumOrders() << endl;
        complete();
    }
    else{
        error("Customer doesn't exist");
        std::cout << "error: " + getErrorMsg() <<endl;
    }
    BaseAction *act =  clone();
    wareHouse.addAction(act);

}

string PrintCustomerStatus:: toString() const{
    string str="";
    if(getStatus()== ActionStatus:: COMPLETED){
        str = "customerStatus " + to_string(customerId) + " COMPLETED";    }
    else if (getStatus()== ActionStatus:: ERROR){
        str = "customerStatus " + to_string(customerId) + " ERROR";    }
    return str;
    

}

PrintCustomerStatus *PrintCustomerStatus :: clone() const{
    return new PrintCustomerStatus(*this);
}

PrintVolunteerStatus ::PrintVolunteerStatus(int id) :
BaseAction(),volunteerId(id){} 

void PrintVolunteerStatus:: act(WareHouse &wareHouse){
    vector<Volunteer*> volunteers = wareHouse.getvolunteers();
    bool isFound=false;
     for(int i=0; (i<(int)volunteers.size())&&(!isFound);i++){
        if(wareHouse.getvol(i)->getId()==volunteerId){
            isFound=true;
            std::cout<< "volunteerID: " << wareHouse.getvol(i)->getId()<< endl;
            if(wareHouse.getvol(i)->isBusy()){
                std::cout<< "isBusy: true" << endl;
                std::cout<< "OrderID: " << wareHouse.getvol(i)->getActiveOrderId()<< endl;
                }
            else {
                std::cout<< "isBusy: false" << endl;
                std::cout<< "OrderID: None" << endl;
                }
            CollectorVolunteer *col =  dynamic_cast<CollectorVolunteer*>(volunteers[i]);
            if ((col != nullptr) &(col->isBusy())){
                std::cout<< "TimeLeft: " << col->getTimeLeft()<< endl; }
            else{ std::cout<< "TimeLeft: None"<< endl;}
            LimitedCollectorVolunteer *Lcol =  dynamic_cast<LimitedCollectorVolunteer*>(volunteers[i]);
            if(Lcol != nullptr)
                {std::cout<< "OrdersLeft: " << Lcol->getNumOrdersLeft()<< endl;}
            else {std::cout<< "OrdersLeft: No Limit" << endl;}
                
            DriverVolunteer *dr =  dynamic_cast<DriverVolunteer*>(wareHouse.getvol(i));
            if (dr != nullptr){
                if(dr->isBusy()){
                    std::cout<< "distance_left: " << dr->getDistanceLeft()<< endl; }   
                else{ std::cout<< "distance_left: None" << endl; } 
                LimitedDriverVolunteer *Ldr =  dynamic_cast<LimitedDriverVolunteer*>(volunteers[i]);
                if(Ldr != nullptr)
                    {std::cout<< "OrdersLeft: " << Ldr->getMaxOrders()<< endl;}
                else {std::cout<< "OrdersLeft: No Limit" << endl;}    
            }
        }
    
        complete();}  
    
     if (!isFound){
        error("Volunteer doesnt exist");
        std::cout << "error: " + getErrorMsg() <<endl;}
    BaseAction *act =  clone();
    wareHouse.addAction(act);
}

string PrintVolunteerStatus:: toString() const{
    string str="";
    if(getStatus()== ActionStatus:: COMPLETED){
        str = "volunteerStatus " + to_string(volunteerId) + " COMPLTED";    }
    else if (getStatus()== ActionStatus:: ERROR){
        str = "volunteerStatus " + to_string(volunteerId) + " ERROR";    }
    return str;
}

PrintVolunteerStatus *PrintVolunteerStatus :: clone() const{
    return new PrintVolunteerStatus(*this);
}

PrintActionsLog ::PrintActionsLog(): BaseAction(){}

void  PrintActionsLog:: act(WareHouse &wareHouse){
    vector<BaseAction*> actionLog = wareHouse.getActions();
    for(int i=0; i<(int)actionLog.size();i++){
        std::cout<< actionLog[i]->toString()<< endl;
    }
    complete();
    BaseAction *act =  clone();
    wareHouse.addAction(act);
}
string PrintActionsLog:: toString() const{
    string str= " log completed";
    return str;
}
PrintActionsLog *PrintActionsLog :: clone() const{
    return new PrintActionsLog(*this);
}


Close:: Close(): BaseAction(){
}
void Close:: act(WareHouse &wareHouse){
    vector<Order*> pending = wareHouse.getpendingOrders();
    vector<Order*> inProcess = wareHouse.getinProcessOrders();
    vector<Order*> completedOrders = wareHouse.getcompletedOrders();
    string s="";
    
    for(int i=0; i<(int)pending.size();i++){
        if(wareHouse.getpendings(i)->getStatus() == OrderStatus:: PENDING){
            s="Order ID:"+to_string(wareHouse.getpendings(i)->getId()) + " Customer ID: " + to_string(pending[i]->getCustomerId())+ "  status: PENDING " ;
            std::cout << s << endl;
        }
        if(wareHouse.getpendings(i)->getStatus() == OrderStatus:: COLLECTING){
            s="Order ID:"+to_string(wareHouse.getpendings(i)->getId()) + " Customer ID: " + to_string(pending[i]->getCustomerId())+ "  status: COLLECTING " ;
            std::cout << s << endl;
        } 
    }
    for(int i=0; i<(int)inProcess.size();i++){
        if(wareHouse.getinprocess(i)->getStatus() == OrderStatus:: COLLECTING){
            s="Order ID:"+to_string(wareHouse.getinprocess(i)->getId()) + " Customer ID: " + to_string(inProcess[i]->getCustomerId())+ "  status: COLLECTING" ;
            std::cout << s << endl;
            }
        if(wareHouse.getinprocess(i)->getStatus() == OrderStatus:: DELIVERING){
            s="Order ID:"+to_string(wareHouse.getinprocess(i)->getId()) + " Customer ID: " + to_string(inProcess[i]->getCustomerId())+ "  status: DELIVERING" ;
            std::cout << s << endl;
            }

    }
    for(int i=0; i<(int)completedOrders.size();i++){
        s="Order ID:"+to_string(wareHouse.getcompleted(i)->getId()) + " Customer ID: " + to_string(completedOrders[i]->getCustomerId())+ "  status: COMPLETED " ;
        std::cout << s << endl;
    }
    complete();
    wareHouse.close();
}
string Close:: toString() const{
    string str= "close completed";
    return str;
}
Close *Close :: clone() const{
    return new Close(*this);
}

 BackupWareHouse :: BackupWareHouse(): BaseAction(){

 }

void BackupWareHouse:: act(WareHouse &wareHouse){
    extern WareHouse* backup;
    if(backup != nullptr){
        delete backup;
    }
    backup= new WareHouse(wareHouse); 
    complete();

    BaseAction *act =  clone();
    wareHouse.addAction(act);
    BaseAction *act2 =  clone();
    backup->addAction(act2);
}

BackupWareHouse *BackupWareHouse::  clone() const{
    return new BackupWareHouse();
}
string BackupWareHouse:: toString() const{
   string str= "backup COMPLETED";
    return str; 
}

RestoreWareHouse :: RestoreWareHouse():BaseAction(){}

void RestoreWareHouse :: act(WareHouse &wareHouse){
    extern WareHouse* backup;
    if (backup != nullptr){
        wareHouse.operator=(*backup);
        complete();

    }
    else{
        error("No backup available");
        std::cout << "error: " << getErrorMsg() <<endl;
    }
    
    BaseAction *act =  clone();
    wareHouse.addAction(act);
}

RestoreWareHouse *RestoreWareHouse::  clone() const{
    return new RestoreWareHouse(*this);
}
string RestoreWareHouse:: toString() const{

    string str="";
    if(getStatus()== ActionStatus:: COMPLETED){
        str = "restore COMPLETED";    }
    if (getStatus()== ActionStatus:: ERROR){
        str = "restore ERROR";    }
    return str;

}






