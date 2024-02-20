#pragma once
#include <malloc.h>
#include "Master.h"
#include "Input.h"
#include "Check.h"
#include "OutputEntities.h"
#include "Slave.h"
#include "Ticket.h"

#define Passenger_IND "passenger.ind"
#define Passenger_DATA "passenger.fl"
#define Passenger_GARBAGE "passenger_garbage.txt"
#define INDEXER_SIZE sizeof(struct Indexer)
#define Passenger_SIZE sizeof(struct Passenger)
#define TICKET_SIZE sizeof(struct Ticket)
#define TICKET_DATA "ticket.fl"

int PrintListOfPassenger(char* error);
void overwriteGarbagePassengerElement(int garbageCount, FILE* garbageZone, struct Passenger* passenger)
{
    int* deleteIdsFile = (malloc(garbageCount * sizeof(int)));
    for (int i = 0; i < garbageCount; i++)
    {
        fscanf(garbageZone, "%d", deleteIdsFile + i);
    }
    fclose(garbageZone);
    passenger->id = deleteIdsFile[0];
    fopen(Passenger_GARBAGE, "wb");
    fprintf(garbageZone, "%d", garbageCount - 1);
    for (int i = 1; i < garbageCount; i++)
    {
        fprintf(garbageZone, " %d", deleteIdsFile[i]);
    }
    fclose(garbageZone);
    free(deleteIdsFile);
}
int insertPassenger(struct Passenger passenger) {
    FILE* garbageZone = fopen(Passenger_GARBAGE, "a+b");
    FILE* indexTable = fopen(Passenger_IND, "a+b");
    FILE* database = fopen(Passenger_DATA, "a+b");
    int garbageCount = 0;
    fscanf(garbageZone, "%d", &garbageCount);
    struct Indexer indexer;
    if (garbageCount != 0)
    {
        overwriteGarbagePassengerElement(garbageCount, garbageZone, &passenger);
        fclose(database);
        fclose(indexTable);
        indexTable = fopen(Passenger_IND, "r+b");
        database = fopen(Passenger_DATA, "r+b");
        fseek(indexTable, (passenger.id - 1) * INDEXER_SIZE, SEEK_SET);
        fread(&indexer, INDEXER_SIZE, 1, indexTable);
        fseek(database, indexer.address, SEEK_SET);
    }
    else {
        fseek(indexTable, 0, SEEK_END);
        if (ftell(indexTable) != 0) {
            fseek(indexTable, -(INDEXER_SIZE), SEEK_END);
            fread(&indexer, INDEXER_SIZE, 1, indexTable);
            passenger.id = indexer.id + 1;
        }
        else {
            passenger.id = 1;
        }
    }
    passenger.ticketFirstAddress = -1;
    passenger.ticketCount = 0;
    fwrite(&passenger, Passenger_SIZE, 1, database);
    indexer.id = passenger.id;
    indexer.exists = 1;
    indexer.address = (passenger.id - 1) * Passenger_SIZE;
    printf("Your passenger id is %d\n", passenger.id);
    fseek(indexTable, (passenger.id - 1) * INDEXER_SIZE, SEEK_SET);
    fwrite(&indexer, INDEXER_SIZE, 1, indexTable);
    fclose(database);
    fclose(indexTable);
    return 1;
}
int getPassenger(struct Passenger* passenger, int id, char* error)
{
    FILE* indexTable = fopen(Passenger_IND, "rb");
    FILE* database = fopen(Passenger_DATA, "rb");
    if (checkFileExists(indexTable, database, error) == 0)
        return 0;
    struct Indexer indexer;
    if (checkIndexExists(indexTable, error, id) == 0)
        return 0;
    fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fread(&indexer, INDEXER_SIZE, 1, indexTable);
    if (checkRecordExists(indexer, error) == 0)
        return 0;
    fseek(database, indexer.address, SEEK_SET);
    fread(passenger, Passenger_SIZE, 1, database);
    fclose(indexTable);
    fclose(database);
    return 1;
}

int updatePassenger(struct Passenger passenger, char* error, int id) {
    FILE* indexTable = fopen(Passenger_IND, "r+b");
    FILE* database = fopen(Passenger_DATA, "r+b");
    struct Indexer indexer;
    if (checkFileExists(indexTable, database, error) == 0) {
        return 0;
    }
    if (checkIndexExists(indexTable, error, id) == 0) {
        return 0;
    }
    fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fread(&indexer, INDEXER_SIZE, 1, indexTable);
    if (checkRecordExists(indexer, error) == 0)
        return 0;
    passenger.id = id;
    fseek(database, indexer.address, SEEK_SET);
    fwrite(&passenger, Passenger_SIZE, 1, database);
    fclose(indexTable);
    fclose(database);
    return 1;
}
void listOfDeletedPassenger(int id)
{
    FILE* garbage = fopen(Passenger_GARBAGE, "rb");
    if (garbage == NULL)
    {
        printf("Error occured while opening file \n");
        return;
    }
    int garbageCount;
    fscanf(garbage, "%d", &garbageCount);
    int* deleteIds = (malloc(garbageCount * sizeof(int)));

    for (int i = 0; i < garbageCount; i++)
    {
        fscanf(garbage, "%d", deleteIds + i);
    }
    fclose(garbage);
    garbage = fopen(Passenger_GARBAGE, "wb");
    fprintf(garbage, "%d", garbageCount + 1);
    for (int i = 0; i < garbageCount; i++) {
        fprintf(garbage, " %d", deleteIds[i]);
    }
    fprintf(garbage, " %d", id);
    free(deleteIds);
    fclose(garbage);
}
int deletePassenger(int id, char* error) {
    FILE* indexTable = fopen(Passenger_IND, "r+b");
    struct Indexer indexer;
    if (indexTable == NULL) {
        strcpy(error, "database files are not created yet");
        return 0;
    }
    if (checkIndexExists(indexTable, error, id) == 0) {
        return 0;
    }
    fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fread(&indexer, INDEXER_SIZE, 1, indexTable);
    if (checkRecordExists(indexer, error) == 0)
        return 0;
    struct Passenger passenger;
    getPassenger(&passenger, id, error);
    indexer.exists = 0;
    fseek(indexTable, (id - 1) * INDEXER_SIZE, SEEK_SET);
    fwrite(&indexer, INDEXER_SIZE, 1, indexTable);
    fclose(indexTable);
    listOfDeletedPassenger(id);
    if (passenger.ticketCount != 0) {
        FILE* ticketDb = fopen(TICKET_DATA, "r+b");
        struct Ticket ticket;
        fseek(ticketDb, passenger.ticketFirstAddress, SEEK_SET);
        for (int i = 0; i < passenger.ticketCount; i++)
        {
            fread(&ticket, TICKET_SIZE, 1, ticketDb);
            fclose(ticketDb);
            deleteTicket(passenger, ticket, error);
            ticketDb = fopen(TICKET_DATA, "r+b");
            fseek(ticketDb, ticket.nextAddress, SEEK_SET);
        }
        fclose(ticketDb);
    }
    return 1;
}
int PrintListOfPassenger(char* error)
{
    FILE* database = fopen(Passenger_DATA, "rb");
    FILE* indexTable = fopen(Passenger_IND, "rb");
    struct Passenger passenger;
    struct Indexer indexer;
    if (checkFileExists(indexTable, database, error) == 0)
    {
        return 0;
    }
    fseek(indexTable, -INDEXER_SIZE, SEEK_END);
    fread(&indexer, INDEXER_SIZE, 1, indexTable);
    rewind(indexTable);
    int endIndex = 0;
    getPassenger(&passenger, indexer.id, error);
    endIndex = indexer.id;
    int id = 0;
    int count = 0;
    while (id < endIndex)
    {
        fseek(indexTable, id * INDEXER_SIZE, SEEK_SET);
        fread(&indexer, INDEXER_SIZE, 1, indexTable);
        if (indexer.exists)
        {
            printf("<=============================>\n");
            getPassenger(&passenger, indexer.id, error);
            printf("Id %d\n", passenger.id);
            OutputPassenger(passenger);
            count++;
        }
        id++;
    }
    if (count == 0)
    {
        printf("No passengers \n");
    }
    return 1;
}


