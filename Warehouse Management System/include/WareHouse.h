#pragma once
#include <string>
#include <vector>



#include "Order.h"
#include "Customer.h"

class BaseAction;
class Volunteer;



class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        ~WareHouse();
        WareHouse(const WareHouse &other);
        WareHouse& operator=(const WareHouse &other);
        WareHouse (WareHouse&& other)noexcept;
        WareHouse& operator=(WareHouse&& other)noexcept;
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer& getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Volunteer *collectorAvl(vector<Volunteer*>& volunteers, Order &order); 
        Volunteer *driverAvl(vector<Volunteer*>& volunteers, Order &order);
        Order &getOrder(int orderId) const;
        int getOrderCounter() const;
        void increaseOrderCounter();
        int getcustomerCounter() const;
        void increasecustomerCounter();
        int getvolunteerCounter() const;
        void increasevolunteerCounter();
        const vector<BaseAction*> &getActions() const;
        const vector<Order*> &getpendingOrders() const; 
        const vector<Order*> &getinProcessOrders() const; 
        const vector<Order*> &getcompletedOrders() const; 
        const vector<Volunteer*> &getvolunteers() const; 
        const vector<Customer*> &getCustomers() const; 

        void addToinProcess(Order* order);
        void addToCompleted(Order* order);
        void addToVolunteers(Volunteer* volunteer);
        void addToCustomers(Customer* customer);
        void erasePendings(int i);
        void eraseInProcess(int i);
        void eraseVolunteer(int i);
        Order *getpendings(int i);
        Order *getinprocess(int i);
        Order *getcompleted(int i);
        Volunteer *getvol(int i);
        Customer *getcus(int i);

        

        void close();
        void open();

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter; 
};