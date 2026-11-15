
#include <stdio.h>
#include <string.h>

#define MAX_FLIGHTS 5
#define MAX_ROWS 5
#define MAX_COLS 4
#define MAX_RESERVATIONS 100

struct Flight
{
    int flightID;
    char source[30];
    char destination[30];
    char departure[20];
    float economyPrice;
    float businessPrice;
    int seats[MAX_ROWS][MAX_COLS];
};

struct Reservation
{
    int bookingID;
    int bookedFlightID;
    char passengerName[50];
    int bookedRow;
    int bookedColumn;
    char bookedClass[20];
    float ticketPrice;
    int reservationActive;
};

struct Flight flights[MAX_FLIGHTS];
struct Reservation reservations[MAX_RESERVATIONS];

int flightCount = 5;
int reservationCount = 0;
int nextBookingID = 1001;


void initializeFlights(void);
void loadReservations(void);
void saveReservations(void);
void displayMainMenu(void);

void displayAllFlights(void);
void searchFlights(void);
void displayFlightDetails(int index);
int findFlightByID(int id);

void displaySeats(int index);
int isSeatAvailable(int index, int row, int column);
void bookTicket(void);

int findReservationByID(int id);
void cancelReservation(void);
void updateSeatAfterCancellation(int index, int row, int column);

void checkSeatConflicts(int index);
void displayAllReservations(void);


int main(void)
{
    int choice;

    initializeFlights();
    loadReservations();

    do
    {
        displayMainMenu();

        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input!\n");

            while (getchar() != '\n');

            continue;
        }

        switch (choice)
        {
            case 1:
                displayAllFlights();
                break;

            case 2:
                searchFlights();
                break;

            case 3:
                bookTicket();
                break;

            case 4:
                cancelReservation();
                break;

            case 5:
                displayAllReservations();
                break;

            case 6:
                printf("\nThank you for using Airline Reservation System!\n");
                break;

            default:
                printf("\nInvalid choice! Enter 1 to 6.\n");
        }

    } while (choice != 6);

    return 0;
}


void initializeFlights(void)
{
    int i, j;

    flights[0].flightID = 101;
    strcpy(flights[0].source, "Karachi");
    strcpy(flights[0].destination, "Lahore");
    strcpy(flights[0].departure, "09:00AM");
    flights[0].economyPrice = 15000;
    flights[0].businessPrice = 30000;

    flights[1].flightID = 102;
    strcpy(flights[1].source, "Lahore");
    strcpy(flights[1].destination, "Islamabad");
    strcpy(flights[1].departure, "11:30AM");
    flights[1].economyPrice = 10000;
    flights[1].businessPrice = 22000;

    flights[2].flightID = 103;
    strcpy(flights[2].source, "Karachi");
    strcpy(flights[2].destination, "Islamabad");
    strcpy(flights[2].departure, "02:00PM");
    flights[2].economyPrice = 18000;
    flights[2].businessPrice = 35000;

    flights[3].flightID = 104;
    strcpy(flights[3].source, "Islamabad");
    strcpy(flights[3].destination, "Karachi");
    strcpy(flights[3].departure, "05:00PM");
    flights[3].economyPrice = 17000;
    flights[3].businessPrice = 32000;

    flights[4].flightID = 105;
    strcpy(flights[4].source, "Lahore");
    strcpy(flights[4].destination, "Karachi");
    strcpy(flights[4].departure, "08:00PM");
    flights[4].economyPrice = 16000;
    flights[4].businessPrice = 31000;

    // Initialize all seats as available
    for (i = 0; i < MAX_FLIGHTS; i++)
    {
        for (j = 0; j < MAX_ROWS; j++)
        {
            int k;

            for (k = 0; k < MAX_COLS; k++)
            {
                flights[i].seats[j][k] = 0;
            }
        }
    }
}


void displayMainMenu(void)
{
    printf("\n============================================\n");
    printf("       AIRLINE RESERVATION SYSTEM\n");
    printf("============================================\n");
    printf("1. Flight Schedule Management\n");
    printf("2. Search Flights\n");
    printf("3. Book a Ticket\n");
    printf("4. Cancel a Reservation\n");
    printf("5. View All Reservations\n");
    printf("6. Exit\n");
    printf("============================================\n");
}


int findFlightByID(int id)
{
    int i;

    for (i = 0; i < flightCount; i++)
    {
        if (flights[i].flightID == id)
        {
            return i;
        }
    }

    return -1;
}


void displayAllFlights(void)
{
    int i;

    printf("\n================ FLIGHT SCHEDULE ================\n");

    printf("%-10s %-15s %-15s %-12s %-14s %-14s\n",
           "Flight ID", "From", "To", "Departure",
           "Economy(Rs)", "Business(Rs)");

    for (i = 0; i < flightCount; i++)
    {
        printf("%-10d %-15s %-15s %-12s %-14.2f %-14.2f\n",
               flights[i].flightID,
               flights[i].source,
               flights[i].destination,
               flights[i].departure,
               flights[i].economyPrice,
               flights[i].businessPrice);
    }
}

void searchFlights(void)
{
    char searchSource[30];
    char searchDestination[30];

    int i, found = 0;

    printf("\n================ SEARCH FLIGHTS ================\n");

    printf("Enter source city: ");
    scanf("%29s", searchSource);

    printf("Enter destination city: ");
    scanf("%29s", searchDestination);

    for (i = 0; i < flightCount; i++)
    {
        if (strcmp(flights[i].source, searchSource) == 0 &&
            strcmp(flights[i].destination, searchDestination) == 0)
        {
            displayFlightDetails(i);
            found = 1;
        }
    }

    if (found == 0)
    {
        printf("\nNo flight found for this route.\n");
    }
}


void displayFlightDetails(int index)
{
    printf("\nFlight ID: %d\n", flights[index].flightID);
    printf("Route: %s to %s\n",
           flights[index].source,
           flights[index].destination);

    printf("Departure: %s\n", flights[index].departure);

    printf("Economy Price: Rs. %.2f\n",
           flights[index].economyPrice);

    printf("Business Price: Rs. %.2f\n",
           flights[index].businessPrice);
}


void displaySeats(int index)
{
    int i, j;

    printf("\n================ SEAT MAP ================\n");
    printf("A = Available, X = Reserved\n");
    printf("               ECONOMY       BUSINESS\n");

    for (i = 0; i < MAX_ROWS; i++)
    {
        printf("Row %d:          ", i + 1);

        for (j = 0; j < MAX_COLS; j++)
        {
            if (flights[index].seats[i][j] == 0)
            {
                printf("A ");
            }
            else
            {
                printf("X ");
            }

            if (j == 1)
            {
                printf("      ");
            }
        }

        printf("\n");
    }

    printf("Columns 1-2: Economy\n");
    printf("Columns 3-4: Business\n");
}


int isSeatAvailable(int index, int row, int column)
{
    if (row < 0 || row >= MAX_ROWS ||
        column < 0 || column >= MAX_COLS)
    {
        return 0;
    }

    if (flights[index].seats[row][column] == 0)
    {
        return 1;
    }

    return 0;
}


void bookTicket(void)
{
    int id, index;
    int row, column, classChoice;

    float price;
    char name[50];

    printf("\n================ TICKET RESERVATION ================\n");

    displayAllFlights();

    printf("\nEnter Flight ID: ");
    if (scanf("%d", &id) != 1)
    {
        printf("Invalid Flight ID!\n");
        while (getchar() != '\n');
        return;
    }

    index = findFlightByID(id);

    if (index == -1)
    {
        printf("\nInvalid Flight ID!\n");
        return;
    }

    printf("Enter passenger name (one word): ");
    scanf("%49s", name);

    printf("\n1. Economy Class\n");
    printf("2. Business Class\n");
    printf("Enter class choice: ");

    if (scanf("%d", &classChoice) != 1)
    {
        printf("Invalid class choice!\n");
        while (getchar() != '\n');
        return;
    }

    if (classChoice != 1 && classChoice != 2)
    {
        printf("\nInvalid class choice!\n");
        return;
    }

    displaySeats(index);

    printf("\nEnter seat row (1-5): ");

    if (scanf("%d", &row) != 1)
    {
        printf("Invalid row!\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter seat column (1-4): ");

    if (scanf("%d", &column) != 1)
    {
        printf("Invalid column!\n");
        while (getchar() != '\n');
        return;
    }

    row--;
    column--;

    if (row < 0 || row >= MAX_ROWS ||
        column < 0 || column >= MAX_COLS)
    {
        printf("\nInvalid seat number!\n");
        return;
    }

    if (classChoice == 1 && column > 1)
    {
        printf("\nSelect Economy seat from columns 1 or 2.\n");
        return;
    }

    if (classChoice == 2 && column < 2)
    {
        printf("\nSelect Business seat from columns 3 or 4.\n");
        return;
    }

    if (isSeatAvailable(index, row, column) == 0)
    {
        printf("\nSeat Conflict: Seat is already reserved!\n");
        checkSeatConflicts(index);
        return;
    }

    if (reservationCount >= MAX_RESERVATIONS)
    {
        printf("\nReservation storage is full!\n");
        return;
    }

    if (classChoice == 1)
    {
        price = flights[index].economyPrice;
    }
    else
    {
        price = flights[index].businessPrice;
    }

    reservations[reservationCount].bookingID = nextBookingID;
    reservations[reservationCount].bookedFlightID = id;

    strcpy(reservations[reservationCount].passengerName, name);

    reservations[reservationCount].bookedRow = row + 1;
    reservations[reservationCount].bookedColumn = column + 1;

    if (classChoice == 1)
    {
        strcpy(reservations[reservationCount].bookedClass, "Economy");
    }
    else
    {
        strcpy(reservations[reservationCount].bookedClass, "Business");
    }

    reservations[reservationCount].ticketPrice = price;
    reservations[reservationCount].reservationActive = 1;

    // Mark seat reserved
    flights[index].seats[row][column] = 1;

    printf("\nTicket booked successfully!\n");
    printf("----------------------------------\n");

    printf("Booking ID: %d\n", nextBookingID);
    printf("Passenger: %s\n", name);
    printf("Flight ID: %d\n", id);
    printf("Seat: Row %d, Column %d\n", row + 1, column + 1);

    printf("Class: %s\n",
           reservations[reservationCount].bookedClass);

    printf("Ticket Price: Rs. %.2f\n", price);

    printf("----------------------------------\n");

    reservationCount++;
    nextBookingID++;

    saveReservations();
}


int findReservationByID(int id)
{
    int i;

    for (i = 0; i < reservationCount; i++)
    {
        if (reservations[i].bookingID == id &&
            reservations[i].reservationActive == 1)
        {
            return i;
        }
    }

    return -1;
}


void cancelReservation(void)
{
    int id, reservationIndex, flightIndex;
    int row, column;

    printf("\n================ RESERVATION CANCELLATION ================\n");

    printf("Enter Booking ID: ");

    if (scanf("%d", &id) != 1)
    {
        printf("Invalid Booking ID!\n");
        while (getchar() != '\n');
        return;
    }

    reservationIndex = findReservationByID(id);

    if (reservationIndex == -1)
    {
        printf("\nBooking not found or already cancelled!\n");
        return;
    }

    flightIndex = findFlightByID(
        reservations[reservationIndex].bookedFlightID
    );

    if (flightIndex == -1)
    {
        printf("\nFlight information not found!\n");
        return;
    }

    row = reservations[reservationIndex].bookedRow - 1;
    column = reservations[reservationIndex].bookedColumn - 1;

    // Mark reservation cancelled
    reservations[reservationIndex].reservationActive = 0;

    updateSeatAfterCancellation(flightIndex, row, column);

    saveReservations();

    printf("\nReservation cancelled successfully!\n");
    printf("Booking ID: %d\n", id);
    printf("The seat is now available again.\n");
}


void updateSeatAfterCancellation(int index, int row, int column)
{
    if (row >= 0 && row < MAX_ROWS &&
        column >= 0 && column < MAX_COLS)
    {
        flights[index].seats[row][column] = 0;
    }
}


void checkSeatConflicts(int index)
{
    int i, j;

    printf("\n================ SEAT CONFLICT MANAGEMENT ================\n");

    printf("Available seats for Flight ID %d:\n",
           flights[index].flightID);

    for (i = 0; i < MAX_ROWS; i++)
    {
        printf("Row %d: ", i + 1);

        for (j = 0; j < MAX_COLS; j++)
        {
            if (flights[index].seats[i][j] == 0)
            {
                printf("[Column %d: Available] ", j + 1);
            }
            else
            {
                printf("[Column %d: Reserved] ", j + 1);
            }
        }

        printf("\n");
    }
}


void displayAllReservations(void)
{
    int i, found = 0;

    printf("\n================ ALL RESERVATIONS ================\n");

    printf("%-12s %-12s %-20s %-8s %-8s %-12s %-12s\n",
           "Booking ID", "Flight ID", "Passenger",
           "Row", "Column", "Class", "Price");

    for (i = 0; i < reservationCount; i++)
    {
        if (reservations[i].reservationActive == 1)
        {
            printf("%-12d %-12d %-20s %-8d %-8d %-12s Rs.%-10.2f\n",
                   reservations[i].bookingID,
                   reservations[i].bookedFlightID,
                   reservations[i].passengerName,
                   reservations[i].bookedRow,
                   reservations[i].bookedColumn,
                   reservations[i].bookedClass,
                   reservations[i].ticketPrice);

            found = 1;
        }
    }

    if (found == 0)
    {
        printf("\nNo active reservations found.\n");
    }
}


void loadReservations(void)
{
    FILE *file;
    struct Reservation temp;
    int index;
    int row, column;

    file = fopen("reservations.txt", "r");

    if (file == NULL)
    {
        return;
    }

    while (reservationCount < MAX_RESERVATIONS &&
           fscanf(file, "%d %d %49s %d %d %19s %f %d",
                  &temp.bookingID,
                  &temp.bookedFlightID,
                  temp.passengerName,
                  &temp.bookedRow,
                  &temp.bookedColumn,
                  temp.bookedClass,
                  &temp.ticketPrice,
                  &temp.reservationActive) == 8)
    {
        reservations[reservationCount] = temp;

        // Restore reserved seats
        if (temp.reservationActive == 1)
        {
            index = findFlightByID(temp.bookedFlightID);

            row = temp.bookedRow - 1;
            column = temp.bookedColumn - 1;

            if (index != -1 &&
                row >= 0 && row < MAX_ROWS &&
                column >= 0 && column < MAX_COLS)
            {
                flights[index].seats[row][column] = 1;
            }
        }

        if (temp.bookingID >= nextBookingID)
        {
            nextBookingID = temp.bookingID + 1;
        }

        reservationCount++;
    }

    fclose(file);
}


void saveReservations(void)
{
    FILE *file;
    int i;

    file = fopen("reservations.txt", "w");

    if (file == NULL)
    {
        printf("\nError: Could not save reservation file!\n");
        return;
    }

    for (i = 0; i < reservationCount; i++)
    {
        fprintf(file, "%d %d %s %d %d %s %.2f %d\n",
                reservations[i].bookingID,
                reservations[i].bookedFlightID,
                reservations[i].passengerName,
                reservations[i].bookedRow,
                reservations[i].bookedColumn,
                reservations[i].bookedClass,
                reservations[i].ticketPrice,
                reservations[i].reservationActive);
    }

    fclose(file);
}
