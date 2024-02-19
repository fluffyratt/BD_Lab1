#pragma once

#include "Master.h"
#include "Passenger.h"
#include "Check.h"

#define INDEXER_SIZE sizeof(struct Indexer)
#define Passenger_DATA "passenger.fl"
#define Passenger_SIZE sizeof(struct Passenger)
#define TICKET_DATA "ticket.fl"
#define TICKET_SIZE sizeof(struct Ticket)
#define Passenger_IND "passenger.ind"

int getPassenger(struct Passenger* passenger, int id, char* error);

int checkFileExists(FILE* indexTable, FILE* database, char* error)
{
    if (indexTable == NULL || database == NULL) {
        strcpy(error, "DB file do not exits");
        return 0;
    }
    return 1;
}

int checkIndexExists(FILE* indexTable, char* error, int id) {
    fseek(indexTable, 0, SEEK_END);
    long indexTableSize = ftell(indexTable);
    if (indexTableSize == 0 || id * INDEXER_SIZE > indexTableSize) {
        strcpy(error, "no this ID in the table");
        return 0;
    }
    return 1;
}

int checkRecordExists(struct Indexer indexer, char* error) {
    if (indexer.exists == 0) {
        strcpy(error, "the record was deleted");
        return 0;
    }
    return 1;
}

int checkIfRecordExist(struct Passenger passenger, int ticketId, char* error) {
    FILE* ticketsDb = fopen(TICKET_DATA, "a+b");
    struct Ticket ticket;
    fseek(ticketsDb, passenger.ticketFirstAddress, SEEK_SET);
    for (int i = 0; i < passenger.ticketCount; i++) {
        fread(&ticket, TICKET_SIZE, 1, ticketsDb);
        fclose(ticketsDb);
        if (ticket.Id == ticketId)
        {
            if (ticket.exists)
                return 1;
            else
            {
                strcpy(error, "Ticket was deleted");
                return 0;
            }
        }
        ticketsDb = fopen(TICKET_DATA, "r+b");
        fseek(ticketsDb, ticket.nextAddress, SEEK_SET);
    }
    strcpy(error, "The passenger do not have this order, the order has been deleted or does not exist");
    fclose(ticketsDb);
    return 0;
}
void info() {
    FILE* indexTable = fopen(Passenger_IND, "rb");
    if (indexTable == NULL) {
        printf("Error: database files do not exist\n");
        return;
    }
    int passengerCount = 0;
    int ticketCount = 0;
    fseek(indexTable, 0, SEEK_END);
    int indAmount = ftell(indexTable) / sizeof(struct Indexer);
    struct Passenger passenger;
    char error[51];
    for (int i = 1; i <= indAmount; i++)
    {
        if (getPassenger(&passenger, i, error))
        {
            passengerCount++;
            ticketCount += passenger.ticketCount;
            printf("Passenger #%d has %d orders\n", i, passenger.ticketCount);
        }
    }
    fclose(indexTable);

    printf("Total clients: %d\n", passengerCount);
    printf("Total tickets: %d\n", ticketCount);
}




