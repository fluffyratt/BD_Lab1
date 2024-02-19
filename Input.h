#pragma once

#include <stdio.h>
#include <string.h>
#include "Master.h"
#include "Slave.h"

void inputPassenger (struct Passenger *passenger)
{
    char firstName[10];
    char lastName[10];
    char phoneNumber[20];
    char birthday[10];
    char email[20];

    printf("Enter passenger's first Name: ");
    scanf("%s", firstName);
    strcpy(passenger->firstName, firstName);

    printf("Enter passenger's last Name: ");
    scanf("%s", lastName);
    strcpy(passenger->lastName, lastName);

    printf("Enter passenger's phone Number: ");
    scanf("%s", phoneNumber);
    strcpy(passenger->phoneNumber, phoneNumber);

    printf("Enter passenger's birthday: ");
    scanf("%s", birthday);
    strcpy(passenger->birthday, birthday);

    printf("Enter passenger's email address: ");
    scanf("%s", email);
    strcpy(passenger->email, email);
}

void inputTicket(struct Ticket *ticket)
{
    char date[12];
    int priceOfTicket;
    int numOfFlight;
    int numOfSeat;
    char classOfTicket;


    printf("Enter number of flight ");
    scanf("%d", &numOfFlight);
    ticket ->numOfFlight = numOfFlight;

    printf("Enter number of seat ");
    scanf("%d", &numOfSeat);
    ticket ->numOfSeat = numOfSeat;

    printf("Enter class of ticket ");
    scanf("%d", &classOfTicket);
    ticket ->classOfTicket = classOfTicket;

    printf("Enter price of ticket ");
    scanf("%d", &priceOfTicket);
    ticket ->priceOfTicket = priceOfTicket;

    printf("Enter ticket's date: ");
    scanf("%d", &date);
    strcpy(ticket->date, date);



}
