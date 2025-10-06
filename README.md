# Warehouse-Management-System
# Overview

This project is a C++ simulation of a Food Warehouse Management System that manages volunteers, customers, and orders.
It demonstrates object-oriented programming principles, dynamic memory management, and the design of a modular simulation system.

The program models a food distribution warehouse, where:

Customers place food orders.

Volunteers (collectors and drivers) handle and deliver those orders.

The system progresses step-by-step, simulating the workflow of the warehouse in real time.

# Simulation Flow

The program receives the path to a configuration file as a command-line argument

The configuration file defines the initial state of the warehouse — including its customers, volunteers, and other parameters — before the simulation begins.

When the program starts:

The warehouse is initialized according to the given configuration file.

It prints Warehouse is open! to the console.

It enters a loop, waiting for user actions (commands).

The actions include: Creating new customers or placing new orders, Performing simulation steps to advance time, Checking the status of orders, volunteers, or customers, and Closing the warehouse (ending the simulation).

# how to run

In terminal: make

Then run: bin/warehouse <config_path>


Example: bin/warehouse config.txt

