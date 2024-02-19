#pragma once
struct Ticket
{
    int Id;
    int PassengerId;
    int priceOfTicket;
    int numOfFlight; 
    int numOfSeat;
    char classOfTicket;
    char date[10];
    int exists;  // basic 
    int selfAddress;
    int nextAddress;
}; 
