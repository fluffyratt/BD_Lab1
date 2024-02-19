#pragma once
#include "Master.h"
#include "Slave.h"
#include "OutputEntities.h"

#define TICKET_DATA "ticket.fl"
#define TICKET_GARBAGE "ticket_garbage.txt"
#define TICKET_SIZE sizeof(struct Ticket)

int updatePassenger(struct Passenger passenger, char* error, int id);

struct Ticket FindLastAddress(FILE* database, struct Passenger* passenger, struct Ticket* previous)
{
    for (int i = 0; i < passenger->ticketCount; i++) {
        fread(previous, TICKET_SIZE, 1, database);
        fseek(database, previous->nextAddress, SEEK_SET);
    }
    return (*previous);
}

void NextAddress(FILE* database, struct Passenger* passenger, struct Ticket* ticket)
{
    fclose(database);
    database = fopen(TICKET_DATA, "r+b") ;
    struct Ticket previous;
    fseek(database, passenger->ticketFirstAddress, SEEK_SET);
    previous = FindLastAddress(database, passenger, &previous);
    previous.nextAddress = ticket->selfAddress;
    fwrite(&previous,TICKET_SIZE, 1, database);
}

void overwriteGarbageAddress(int garbageCount, FILE* garbageZone, struct Ticket* record) 
{
    int* deletedIds = (malloc(garbageCount * sizeof(int)));
    for (int i = 0; i < garbageCount; i++) {
        fscanf(garbageZone, "%d", deletedIds + i);
    }

    record->selfAddress = deletedIds[0];
    record->nextAddress = deletedIds[0];
    fclose(garbageZone);
    garbageZone = fopen(TICKET_GARBAGE, "wb");
    fprintf(garbageZone, "%d", garbageCount - 1);
    for (int i = 1; i < garbageCount; i++)
    {
        fprintf(garbageZone, " %d", deletedIds[i]);
    }
    free(deletedIds);
    fclose(garbageZone);
}

int insertTicket(struct Passenger passenger, struct Ticket ticket, char* error)
{
    FILE* database = fopen(TICKET_DATA, "a+b");
    FILE* garbageZone = fopen(TICKET_GARBAGE, "a+b");
    int garbageCount = 0;
    fscanf(garbageZone, "%d", &garbageCount);
    ticket.exists = 1; // файл присутній 
    struct Ticket ticket1; 
    fseek(database, 0, SEEK_END);
    if (garbageCount != 0)
    {
        overwriteGarbageAddress(garbageCount, garbageZone, &ticket);
        fclose(database);
        //database = fopen(TICKET_DATA, "r+b");
        database = fopen(TICKET_DATA, "rb");
        fseek(database, ticket.selfAddress, SEEK_SET);
        fread(&ticket1, TICKET_SIZE, 1, database);
        ticket.Id = ticket1.Id;
        fclose(database);
        database = fopen(TICKET_DATA, "r+b");
        fseek(database, ticket.selfAddress, SEEK_SET);
    }
    else
    {
        if (ftell(database) != 0) {
            fseek(database, -(TICKET_SIZE), SEEK_END);
            fread(&ticket1, TICKET_SIZE, 1, database);
            ticket.Id = ticket1.Id + 1;
        }
        else
        {
            ticket.Id = 1;
        }
        int dbSize = ftell(database);
        ticket.selfAddress = dbSize;
        ticket.nextAddress = dbSize;
        fseek(database, 0, SEEK_END);
    }
    printf("Your ticket  id is %d \n", ticket.Id);
    fwrite(&ticket, TICKET_SIZE, 1, database);
    if (!passenger.ticketCount)
    {
        passenger.ticketFirstAddress = ticket.selfAddress;
    }
    else
    {
        NextAddress(database, &passenger, &ticket);
    }
    fclose(database);
    int count = passenger.ticketCount + 1;
    passenger.ticketCount = count;
    updatePassenger(passenger, error, passenger.id);
    return 1;
}
int getTicket(struct Passenger passenger, struct Ticket* ticket, int ticketId, char* error) {

    FILE* database = fopen(TICKET_DATA, "rb");
    fseek(database, passenger.ticketFirstAddress, SEEK_SET);
    fread(ticket, TICKET_SIZE, 1, database);

    for (int i = 0; i < passenger.ticketCount; i++) {
        if (ticket->Id == ticketId)
        {
            fclose(database);
            return 1;
        }
        fseek(database, ticket->nextAddress, SEEK_SET);
        fread(ticket, TICKET_SIZE, 1, database);
    }
    fclose(database);
    return 0;
}
int updateTicket(struct Ticket ticket)
{
    FILE* database = fopen(TICKET_DATA, "r+b");
    fseek(database, ticket.selfAddress, SEEK_SET);
    fwrite(&ticket, TICKET_SIZE, 1, database);
    fclose(database);
    return 1;
}
void noteDeletedTicket(int address)
{
    FILE* garbageZone = fopen(TICKET_GARBAGE, "rb");
    int garbageCount;
    fscanf(garbageZone, "%d", &garbageCount);
    int* deletedAddresses = (malloc(garbageCount * sizeof(long)));

    for (int i = 0; i < garbageCount; i++) {
        fscanf(garbageZone, "%d", deletedAddresses + i);
    }
    fclose(garbageZone);
    garbageZone = fopen(TICKET_GARBAGE, "wb");
    fprintf(garbageZone, "%d", garbageCount + 1);
    for (int i = 0; i < garbageCount; i++) {
        fprintf(garbageZone, " %d", deletedAddresses[i]);
    }

    fprintf(garbageZone, " %d", address);
    free(deletedAddresses);
    fclose(garbageZone);
}
void relinkAddresses(FILE* database, struct Ticket previous, struct Ticket ticket, struct Passenger* passenger) {
    if (ticket.selfAddress == passenger->ticketFirstAddress) {
        if (ticket.selfAddress == ticket.nextAddress) {
            passenger->ticketFirstAddress = -1; // пусте поле
        }
        else {
            passenger->ticketFirstAddress = ticket.nextAddress;
        }
    }
    else {
        if (ticket.selfAddress == ticket.nextAddress) {
            previous.nextAddress = previous.selfAddress;
        }
        else {
            previous.nextAddress = ticket.nextAddress;
        }
        fseek(database, previous.selfAddress, SEEK_SET);
        fwrite(&previous, TICKET_SIZE, 1, database);
    }
}

void deleteTicket(struct Passenger passenger, struct Ticket ticket, char* error)
{
    FILE* database = fopen(TICKET_DATA, "r+b");
    struct Ticket previous;
    fseek(database, passenger.ticketFirstAddress, SEEK_SET);
    do {
        fread(&previous, TICKET_SIZE, 1, database);
        fseek(database, previous.nextAddress, SEEK_SET);
    } while (previous.nextAddress != ticket.selfAddress && ticket.selfAddress != passenger.ticketFirstAddress);
    relinkAddresses(database, previous, ticket, &passenger);
    noteDeletedTicket(ticket.selfAddress);
    ticket.exists = 0; // було видалено

    fseek(database, ticket.selfAddress, SEEK_SET);
    fwrite(&ticket, TICKET_SIZE, 1, database);
    fclose(database);

    passenger.ticketCount--;
    updatePassenger(passenger, error, passenger.id);
}
void PrintList(struct Passenger passenger)
{
    FILE* ticketsDb = fopen(TICKET_DATA, "a+b");
    struct Ticket ticket;
    fseek(ticketsDb, passenger.ticketFirstAddress, SEEK_SET);
    for (int i = 0; i < passenger.ticketCount; i++) {
        fread(&ticket, TICKET_SIZE, 1, ticketsDb);
        if (ticket.exists)
        {
            OutputTicket(ticket, passenger);
            printf("<====================================>\n");
        }
        fseek(ticketsDb, ticket.nextAddress, SEEK_SET);
    }
    fclose(ticketsDb);
}
