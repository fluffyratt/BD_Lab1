#include <stdio.h>
#include "Master.h"
#include "Input.h"
#include "OutputEntities.h"
#include "Slave.h"
#include "Passenger.h"
#include "Ticket.h"

#define Passenger_IND "passenger.ind"
#define INDEXER_SIZE sizeof(struct Indexer)
int main()
{
    struct Passenger passenger;
    struct Ticket ticket;
    while (1)
    {
        int choice;
        int id;
        char error[20];
        printf("Acceptable Options:\n0 Insert Passenger\n1 Get Passenger\n2 Update Passenger\n3 Delete Passenger\n4 Print List of Passenger\n5 Insert Ticket\n6 Get Ticket\n7 Update Ticket\n8 Delete Ticket \n9 Output List of Tickets\n10 Info\n11 Exit\n");
        scanf("%d", &choice);
        switch (choice) {
            case 0:
                inputPassenger(&passenger);
                insertPassenger(passenger);
                break;
            case 1:
                printf("Enter ID: \n");
                scanf("%d", &id);
                if(getPassenger(&passenger, id, error) != 0)
                {
                    OutputPassenger(passenger);
                }
                else
                {
                    printf("Error: %s\n", error);
                }
                break;
            case 2:
                printf("Enter ID: \n");
                scanf("%d", &id);
                getPassenger(&passenger, id, error);
                inputPassenger(&passenger);
                if(updatePassenger(passenger, error, id)==0)
                    printf("%s", error);
                else
                    printf("Update was successfully\n");
                break;
            case 3:
                printf("Enter ID: \n");
                scanf("%d", &id);
                if(deletePassenger(id, error) != 0)
                    printf("Deleted successfully\n");
                else
                    printf("Error: %s\n", error);
                break;
            case 4:
                PrintListOfPassenger(error);
                break;
            case 5:
                printf("Enter the passenger id\n");
                scanf("%d", &id);
                if(getPassenger(&passenger, id, error) != 0)
                {
                    inputTicket(&ticket);
                    ticket.PassengerId = passenger.id;
                    insertTicket(passenger, ticket, error);
                    printf("Insert was successfully \n");
                }
                else
                {
                    printf("Error %s\n", error);
                }
                break;
            case 6:
                printf("Enter the passenger id \n");
                scanf("%d", &id);
                if(getPassenger(&passenger, id, error))
                {
                    printf("Enter the id of ticket \n");
                    scanf("%d", &id);
                    if(checkIfRecordExist(passenger, id,error))
                    {
                        getTicket(passenger, &ticket, id, error);
                        OutputTicket(ticket, passenger);
                    }
                    else
                    {
                        printf("Error %s\n", error);
                    }
                }
                else
                {
                    printf("Error %s\n", error);
                }
                break;
            case 7:
                printf("Enter the passenger id \n");
                scanf("%d", &id);
                if(getPassenger(&passenger, id, error))
                {
                    printf("Enter the id of ticket\n ");
                    scanf("%d", &id);
                    if(checkIfRecordExist(passenger, id,error))
                    {
                        getTicket(passenger, &ticket, id, error);
                        inputTicket(&ticket);
                        updateTicket(ticket);
                        printf("Updating was successfully \n ");
                    }
                    else
                    {
                        printf("Error %s\n", error);
                    }
                }
                else
                {
                    printf("Error %s \n", error);
                }

                break;
            case 8:
                printf("Enter the passenger id \n");
                scanf("%d", &id);
                if(getPassenger(&passenger, id, error))
                {
                    printf("Enter the id of ticket \n");
                    scanf("%d", &id);
                    if(checkIfRecordExist(passenger, id,error))
                    {
                        getTicket(passenger, &ticket, id, error);
                        deleteTicket(passenger,ticket, error);
                        printf("Deleted was successfully \n");
                    }
                    else
                    {
                        printf("Error %s \n ", error);
                    }
                }
                else
                {
                    printf("Error %s \n ", error);
                }
                break;
            case 9:
                printf("Enter the passenger id \n");
                scanf("%d", &id);
                if(getPassenger(&passenger, id, error))
                {
                    if(passenger.ticketCount != 0)
                    {
                        PrintList(passenger);
                    }
                    else
                    {
                        printf("This passenger has no tickets\n");
                    }
                }
                else
                {
                    printf("Error %s\n ", error);
                }
                break;
            case 10:
                info();
                break;
            case 11:
                return 0;
            default:
                printf("No command :( \n");
        }
        printf("\n");
    }
}

