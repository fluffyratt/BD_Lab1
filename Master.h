#pragma once
struct Passenger
{
    int id;
    char firstName[10];
    char lastName[10];
    char phoneNumber[20];
    char birthday[10];
    char email[20];
    int ticketCount;
    int ticketFirstAddress; 
};
struct Indexer // чи існує запис (індексований файл)
{
    int id;
    int address;
    int exists;
};

