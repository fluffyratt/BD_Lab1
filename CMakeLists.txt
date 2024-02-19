cmake_minimum_required(VERSION 3.27)
project(Lab1_DB)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Lab1_DB
        Check.h
        Input.h
        Master.h
        OutputEntities.h
        Passenger.h
        Slave.h
        Ticket.h
        main.c)
