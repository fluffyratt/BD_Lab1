#pragma once
#include <stdio.h>
#include "Master.h"
#include "Slave.h"

void OutputPassenger (struct Passenger passenger)
{
    printf("Passenger's first name: %s \n", passenger.firstName);
    printf("Passenger's last name: %s \n", passenger.lastName);
    printf("Passenger's birthday: %s \n", passenger.birthday);
    printf("Passenger's mobile number: %s \n", passenger.phoneNumber);
    printf("Passenger's email address: %s \n", passenger.email);
}

void OutputTicket (struct Ticket ticket, struct Passenger passenger)
{
    printf("The passenger id %d \n", passenger.id);
    printf("Passenger info first name and last name: %s, %s \n", passenger.firstName, passenger.lastName);
    printf("Ticket's price %d \n", ticket.priceOfTicket);
    printf("Flight number: %d \n", ticket.numOfFlight);
    printf("Number of seat: %d \n", ticket.numOfSeat);
    printf("Class of ticket: %s \n", ticket.classOfTicket);
    printf("Ticket purchase time : % s\n", ticket.date);
}