#include <iostream>
#include <cstring>
#include <vector>
#include <queue>
#include <unordered_map>
#include <climits>
#include <iomanip>

using namespace std;

#define MAXCITY 30
#define MAXFLIGHT 200
#define MINLAYOVER 30
#define ROUNDTRIP 0

struct FlightType
{
    int FlightNo;
    char* startCity;
    int timeDepart;
    char* endCity;
    int timeArrival;
    int noOfPassengers;
    FlightType* nextDeparture;
    FlightType* nextArrival;
};

struct RouteType
{
    int Day;
    int nHops;
    int FlightNo1;
    int FlightNo2;
    FlightType F1;
};

struct ReservationType
{
    char* firstName;
    char* lastName;
    int tripType;
    RouteType route1;
    RouteType route2;
    ReservationType* nextReserve;
};

ReservationType* reserveHead = nullptr;
ReservationType* reserveTail = nullptr;

struct CityListType
{
    char* cityName;
    FlightType* nextDeparture;
    FlightType* nextArrival;
};

CityListType cityList[MAXCITY];

struct FlightNumberListType
{
    int FlightNo;
    FlightType* flight;
};

FlightNumberListType flightList[MAXFLIGHT];
int flightCount = 0;

void InitializeCityList()
{
    for (int i = 0; i < MAXCITY; ++i)
    {
        cityList[i].cityName = nullptr;
        cityList[i].nextDeparture = nullptr;
        cityList[i].nextArrival = nullptr;
    }
}

int HashCity(const char* cityName)
{
    int hash = 0;
    for (int i = 0; cityName[i] != '\0'; ++i)
    {
        hash = (hash * 7 + cityName[i]) % MAXCITY;
    }
    return hash;
}

FlightType* CityArrivalList(char* cityName)
{
    int index = HashCity(cityName);
    return cityList[index].nextArrival;
}

FlightType* CityDepartureList(const char* cityName)
{
    int index = HashCity(cityName);
    return cityList[index].nextDeparture;
}

void InsertSortedByTime(FlightType** listHead, FlightType* newFlight, bool isDepartureList) {
    // If the list is empty or the new flight should be at the head
    if (*listHead == nullptr || (isDepartureList ? newFlight->timeDepart < (*listHead)->timeDepart
        : newFlight->timeArrival < (*listHead)->timeArrival)) {
        newFlight->nextDeparture = *listHead; // nextDeparture is used for both departure and arrival lists
        *listHead = newFlight;
    }
    else {
        // Find the correct position for the new flight
        FlightType* current = *listHead;
        while (current->nextDeparture != nullptr &&
            (isDepartureList ? current->nextDeparture->timeDepart < newFlight->timeDepart
                : current->nextDeparture->timeArrival < newFlight->timeArrival)) {
            current = current->nextDeparture;
        }

        // Insert the new flight into the list
        newFlight->nextDeparture = current->nextDeparture;
        current->nextDeparture = newFlight;
    }
}

void MakeFlightNode(int FlightNo, const char* startCity, int timeDepart, const char* endCity, int timeArrival) {
    FlightType* newFlight = new FlightType;
    newFlight->FlightNo = FlightNo;
    newFlight->startCity = new char[strlen(startCity) + 1];
    strcpy(newFlight->startCity, startCity);
    newFlight->timeDepart = timeDepart;
    newFlight->endCity = new char[strlen(endCity) + 1];
    strcpy(newFlight->endCity, endCity);
    newFlight->timeArrival = timeArrival;
    newFlight->noOfPassengers = 0; // Initialize to 0
    newFlight->nextDeparture = nullptr;
    newFlight->nextArrival = nullptr;

    // Insert into departure list
    int departIndex = HashCity(startCity);
    if (cityList[departIndex].cityName == nullptr) {
        cityList[departIndex].cityName = new char[strlen(startCity) + 1];
        strcpy(cityList[departIndex].cityName, startCity);
    }
    InsertSortedByTime(&cityList[departIndex].nextDeparture, newFlight, true);

    // Insert into arrival list
    int arrivalIndex = HashCity(endCity);
    if (cityList[arrivalIndex].cityName == nullptr) {
        cityList[arrivalIndex].cityName = new char[strlen(endCity) + 1];
        strcpy(cityList[arrivalIndex].cityName, endCity);
    }
    InsertSortedByTime(&cityList[arrivalIndex].nextArrival, newFlight, false);

    // Insert into flightList
    // Assuming flightList is maintained as a sorted list
    int i = flightCount - 1;
    while (i >= 0 && flightList[i].FlightNo > FlightNo) {
        flightList[i + 1] = flightList[i];
        i--;
    }
    flightList[i + 1].FlightNo = FlightNo;
    flightList[i + 1].flight = newFlight;
    flightCount++;
}

int findRoute(char* startCity, char* endCity, RouteType& route)
{
    // Check for a direct flight
    FlightType* directFlight = CityDepartureList(startCity);
    while (directFlight != nullptr)
    {
        if (strcmp(directFlight->endCity, endCity) == 0)
        {
            // Direct flight found
            route.Day = 1122;            // Sample day of travel
            route.nHops = 1;             // Direct flight
            route.FlightNo1 = directFlight->FlightNo;  // Sample flight number
            return 0;  // Success
        }
        directFlight = directFlight->nextDeparture;
    }

    // If no direct flight found, search for connecting flights
    FlightType* connectingFlight = CityArrivalList(startCity);
    while (connectingFlight != nullptr) {
        // Check for connecting flights with a layover time of at least MINLAYOVER minutes
        if (strcmp(connectingFlight->startCity, startCity) != 0 &&
            strcmp(connectingFlight->endCity, endCity) == 0)
        {
            route.Day = 1122;                   // Sample day of travel
            route.nHops = 2;                    // Connecting flight
            route.FlightNo1 = directFlight->FlightNo;      // Sample first flight number
            route.FlightNo2 = connectingFlight->FlightNo;  // Sample second flight number
            return 0;  // Success
        }
        connectingFlight = connectingFlight->nextArrival;
    }

    // If no route found
    return -1;  // Failure
}

void MakeReservation()
{
    // Get user input
    char firstName[50];
    char lastName[50];
    int tripType;
    char startCity[50];
    char endCity[50];

    cout << "Provide first name: ";
    cin >> firstName;
    cout << "Provide last name: ";
    cin >> lastName;
    cout << "Provide trip type (0 for ROUNDTRIP, 1 for ONEWAY): ";
    cin >> tripType;
    cout << "Provide source city you want to travel from: ";
    cin >> startCity;
    cout << "Provide target city you want to reach: ";
    cin >> endCity;

    // Placeholder for route information
    RouteType route1;
    RouteType route2;

    // Find the route
    int result = findRoute(startCity, endCity, route1);

    if (result == 0)
    {
        // Route found, make reservation
        ReservationType* newReservation = new ReservationType;
        newReservation->firstName = strdup(firstName);
        newReservation->lastName = strdup(lastName);
        newReservation->tripType = tripType;
        newReservation->route1 = route1;

        if (tripType == ROUNDTRIP)
        {
            // If ROUNDTRIP, find and set route for the return trip
            int returnResult = findRoute(startCity, endCity, route2);

            if (returnResult == 0)
            {
                newReservation->route2 = route2;
            }
            else
            {
                // Handle the case where return route is not found
                cout << "Error: Return route not found. Reservation may be incomplete." << endl;
                delete newReservation;  // Cleanup if reservation is incomplete
                return;
            }
        }

        // Add reservation to the linked list
        newReservation->nextReserve = nullptr;

        if (reserveHead == nullptr)
        {
            reserveHead = reserveTail = newReservation;
        }
        else
        {
            reserveTail->nextReserve = newReservation;
            reserveTail = newReservation;
        }

        cout << "Congrats! Reservation successful!" << endl;
    }
    else
    {
        // Handle the case where route is not found
        cout << "We're sorry to announce this route does not exists. Reservation cannot be made." << endl;
    }
}

void ReadFlightData() {

    flightList[0].FlightNo = 65;
    MakeFlightNode(65, "Multan", 1200, "Tokyo", 1600);

    flightList[1].FlightNo = 420;
    MakeFlightNode(420, "Multan", 1800, "Gawadar", 2000);

    flightList[2].FlightNo = 911;
    MakeFlightNode(911, "Gujranwala", 1300, "Multan", 1600);

    flightList[3].FlightNo = 696;
    MakeFlightNode(696, "Chitral", 900, "Gujranwala", 1300);   

    flightList[4].FlightNo = 511;
    MakeFlightNode(511, "Tokyo", 900, "Gujranwala", 1300); 

    // Initialize cityList entries for departure cities
    int indexA = HashCity("Multan");
    cityList[indexA].cityName = strdup("Multan");
    cityList[indexA].nextDeparture = flightList[0].flight;
    flightList[0].flight->nextDeparture = flightList[1].flight;
    cityList[indexA].nextArrival = flightList[2].flight;

    // Initialize cityList entries for arrival cities
    int indexB = HashCity("Gawadar");
    cityList[indexB].cityName = strdup("Gawadar");
    cityList[indexB].nextArrival = flightList[1].flight;

    int indexC = HashCity("Tokyo");
    if (cityList[indexC].cityName == nullptr) {
        cityList[indexC].cityName = strdup("Tokyo");
    }
    cityList[indexC].nextDeparture = flightList[4].flight;
    cityList[indexC].nextArrival = flightList[0].flight;

    int indexD = HashCity("Gujranwala");
    if (cityList[indexD].cityName == nullptr) {
        cityList[indexD].cityName = strdup("Gujranwala");
    }
    cityList[indexC].nextDeparture = flightList[2].flight;
    cityList[indexC].nextArrival = flightList[3].flight;
    flightList[3].flight->nextArrival = flightList[4].flight;

    int indexE = HashCity("Chitral");
    if (cityList[indexE].cityName == nullptr) {
        cityList[indexE].cityName = strdup("Chitral");
    }
    cityList[indexE].nextDeparture = flightList[3].flight;
}


void DisplayFlightInfo(FlightType* flight) {
    if (flight != nullptr) {
        cout << setw(10) << left << flight->FlightNo
             << setw(15) << left << flight->startCity
             << setw(15) << left << flight->timeDepart
             << setw(15) << left << flight->endCity
             << setw(15) << left << flight->timeArrival << endl;
    }
}

void DisplayAllFlightsData() {
    cout << setw(10) << left << "FlightNo"
         << setw(15) << left << "Departure City"
         << setw(15) << left << "Departure Time"
         << setw(15) << left << "Arrival City"
         << setw(15) << left << "Arrival Time" << endl;
    cout << "-------------------------------------------------------------" << endl;

    for (int i = 0; i < MAXFLIGHT; ++i) {
        if (flightList[i].flight != nullptr) {
            DisplayFlightInfo(flightList[i].flight);
        }
    }
}

FlightType* FlightByNumber(int FlightNo)
{
    for (int i = 0; i < MAXFLIGHT; ++i)
    {
        if (flightList[i].flight != nullptr && flightList[i].FlightNo == FlightNo)
        {
            return flightList[i].flight;
        }
    }
    return nullptr; // Flight not found
}

void DisplayAllCities()
{
    cout << "TOM AIRLINE OFFERED CITIES:" << endl;
    cout << "----------------------------" << endl;
    for (int i = 0; i < MAXCITY; ++i)
    {
        if (cityList[i].cityName != nullptr)
        {
            cout << cityList[i].cityName << endl;
        }
    }
}

void DisplayCitiesFrom(char* startCity) {
    int index = HashCity(startCity);
    if (cityList[index].cityName != nullptr && strcmp(cityList[index].cityName, startCity) == 0) {
        FlightType* current = cityList[index].nextDeparture;
        while (current != nullptr) {
            cout << "To " << current->endCity << " at " << current->timeDepart << endl;
            current = current->nextDeparture;
        }
    }
    else {
        cout << "No departures from " << startCity << endl;
    }
}

struct CityDistance
{
    string city;
    int distance;

    bool operator>(const CityDistance& other) const
    {
        return distance > other.distance;
    }
};

void DisplayShortestPath(char* startCity, char* endCity)
{
    // Dijkstra's algorithm
    unordered_map<string, int> distanceMap;
    unordered_map<string, string> previousCityMap;
    priority_queue<CityDistance, vector<CityDistance>, greater<CityDistance>> pq;

    // Initialize distances to infinity for all cities
    for (int i = 0; i < MAXCITY; ++i)
    {
        if (cityList[i].cityName != nullptr)
        {
            distanceMap[cityList[i].cityName] = INT_MAX;
            previousCityMap[cityList[i].cityName] = "";
        }
    }

    // Set distance to the start city as 0
    distanceMap[startCity] = 0;
    pq.push({ startCity, 0 });

    // Dijkstra's algorithm
    while (!pq.empty())
    {
        CityDistance current = pq.top();
        pq.pop();

        // Break if the endCity is reached
        if (current.city == endCity)
        {
            break;
        }

        FlightType* departureList = CityDepartureList(current.city.c_str());
        while (departureList != nullptr)
        {
            int newDistance = distanceMap[current.city] + (departureList->timeArrival - departureList->timeDepart);

            // Update distance if it's shorter
            if (newDistance < distanceMap[departureList->endCity])
            {
                distanceMap[departureList->endCity] = newDistance;
                previousCityMap[departureList->endCity] = current.city;
                pq.push({ departureList->endCity, newDistance });
            }

            departureList = departureList->nextDeparture;
        }
    }

    // Display the shortest path to the endCity
    cout << "Shortest path between " << startCity << " and " << endCity << ":" << endl;
    cout << "-----------------------------------------" << endl;

    // Check if there is a path to the endCity
    if (distanceMap[endCity] == INT_MAX)
    {
        cout << "No path found." << endl;
        return;
    }

    // Reconstruct the path from endCity to startCity
    vector<string> path;
    string currentCity = endCity;

    while (!previousCityMap[currentCity].empty())
    {
        path.push_back(currentCity);
        currentCity = previousCityMap[currentCity];
    }

    // Add the startCity to the path
    path.push_back(startCity);

    // Display the path in reverse order
    for (int i = path.size() - 1; i > 0; --i)
    {
        cout << path[i] << " -> ";
    }
    cout << path[0] << endl;
}

void DisplayDepartureList(char* cityName) {
    FlightType* departure = CityDepartureList(cityName);
    cout << "Departures from " << cityName << ":" << endl;
    while (departure) {
        DisplayFlightInfo(departure);
        departure = departure->nextDeparture;
    }
}

void DisplayArrivalList(char* cityName) {
    FlightType* arrival = CityArrivalList(cityName);
    cout << "Arrivals to " << cityName << ":" << endl;
    while (arrival) {
        DisplayFlightInfo(arrival);
        arrival = arrival->nextArrival;
    }
}



void InsertReservationSorted(ReservationType** reserveHead, ReservationType** reserveTail, ReservationType* newReserve) {
    if (*reserveHead == nullptr) {
        *reserveHead = *reserveTail = newReserve;
    }
    else {
        ReservationType* current = *reserveHead, * previous = nullptr;
        while (current != nullptr && strcmp(current->lastName, newReserve->lastName) < 0) {
            previous = current;
            current = current->nextReserve;
        }
        while (current != nullptr && strcmp(current->lastName, newReserve->lastName) == 0 && strcmp(current->firstName, newReserve->firstName) < 0) {
            previous = current;
            current = current->nextReserve;
        }

        if (previous == nullptr) {
            newReserve->nextReserve = *reserveHead;
            *reserveHead = newReserve;
        }
        else {
            newReserve->nextReserve = current;
            previous->nextReserve = newReserve;
        }

        if (*reserveTail == previous) {
            *reserveTail = newReserve;
        }
    }
}

void printRoute(int ifExists, RouteType route) {
    if (ifExists == 0) {
        cout << "Day of Travel: " << route.Day << endl;
        cout << "Number of Hops: " << route.nHops << endl;
        cout << "First Flight Number: " << route.FlightNo1 << endl;
        if (route.nHops > 1) {
            cout << "Second Flight Number: " << route.FlightNo2 << endl;
        }
    }
    else {
        cout << "Route does not exist." << endl;
    }
}

void PrintReservation(ReservationType* pReserve) {
    if (pReserve != nullptr) {
        cout << "Name: " << pReserve->firstName << " " << pReserve->lastName << endl;
        cout << "Trip Type: " << (pReserve->tripType == ROUNDTRIP ? "Round Trip" : "One Way") << endl;
        printRoute(0, pReserve->route1);  // Assuming route always exists
        if (pReserve->tripType == ROUNDTRIP) {
            printRoute(0, pReserve->route2);
        }
    }
}


void PrintAllReservations() {
    ReservationType* current = reserveHead;
    while (current != nullptr) {
        PrintReservation(current);
        current = current->nextReserve;
    }
}

void PrintPassengers(int FlightNo) {
    ReservationType* current = reserveHead;
    cout << "Passengers on Flight number:" << FlightNo << ":" << endl;
    while (current != nullptr) {
        if (current->route1.FlightNo1 == FlightNo ||
            (current->tripType == ROUNDTRIP && current->route2.FlightNo1 == FlightNo)) {
            cout << current->firstName << " " << current->lastName << endl;
        }
        current = current->nextReserve;
    }
}

void PrintSchedule() {
    char firstName[50], lastName[50];
    cout << "Enter first name: ";
    cin >> firstName;
    cout << "Enter last name: ";
    cin >> lastName;

    ReservationType* current = reserveHead;
    while (current != nullptr) {
        if (strcmp(current->firstName, firstName) == 0 && strcmp(current->lastName, lastName) == 0) {
            PrintReservation(current);
            return;
        }
        current = current->nextReserve;
    }

    cout << "No reservation found for " << firstName << " " << lastName << endl;
}

void DeleteReserve() {
    char firstName[50], lastName[50];
    cout << "Enter first name to delete reservation: ";
    cin >> firstName;
    cout << "Enter last name: ";
    cin >> lastName;

    ReservationType* current = reserveHead, * prev = nullptr;
    while (current != nullptr) {
        if (strcmp(current->firstName, firstName) == 0 && strcmp(current->lastName, lastName) == 0) {
            if (prev == nullptr) {
                reserveHead = current->nextReserve;
            }
            else {
                prev->nextReserve = current->nextReserve;
            }
            delete current;
            cout << "Reservation deleted." << endl;
            return;
        }
        prev = current;
        current = current->nextReserve;
    }

    cout << "Reservation not found." << endl;
}


int main()
{
    InitializeCityList();
    ReadFlightData();

    int choice;
    do
    {
        // Menu
        cout << "\nTOM AIRLINES:" << endl;
        cout << "1. Show Cities" << endl;
        cout << "2. Show Departures for a City" << endl;
        cout << "3. Show Arrivals for a City" << endl;
        cout << "4. Show Reachable Cities from a City" << endl;
        cout << "5. Show Shortest Path between Cities" << endl;
        cout << "6. Find Route between Cities" << endl;
        cout << "7. Make Reservation" << endl;
        cout << "8. Print Reservation Schedule" << endl;
        cout << "9. Delete Reservation" << endl;
        cout << "10.Print Passenger List for a Flight" << endl;
        cout << "11.Display all flights data in the database"<<endl;
        cout << "0. Exit" << endl;

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1: {
            DisplayAllCities();
            break;
        }

        case 2: {
            char departureCity[50];
            cout << "Enter the departure city: ";
            cin >> departureCity;
            DisplayDepartureList(departureCity);
            break;
        }
        case 3: {
            char arrivalCity[50];
            cout << "Enter the arrival city: ";
            cin >> arrivalCity;
            DisplayArrivalList(arrivalCity);
            break;
        }
        case 4: {
            char reachableCity[50];
            cout << "Enter the starting city: ";
            cin >> reachableCity;
            DisplayCitiesFrom(reachableCity);
            break;
        }
        case 5: {
            char startCity[50], endCity[50];
            cout << "Enter the start city: ";
            cin >> startCity;
            cout << "Enter the end city: ";
            cin >> endCity;
            DisplayShortestPath(startCity, endCity);
            break;
        }
        case 6: {
            char routeStartCity[50], routeEndCity[50];
            cout << "Enter the start city: ";
            cin >> routeStartCity;
            cout << "Enter the end city: ";
            cin >> routeEndCity;
            RouteType route;
            int routeResult = findRoute(routeStartCity, routeEndCity, route);
            printRoute(routeResult, route);
            break;
        }
        case 7: {
            MakeReservation();
            break;
        }
        case 8: {
            PrintSchedule();
            break;
        }
        case 9: {
            DeleteReserve();
            break;
        }
        case 10: {
            int flightNumber;
            cout << "Enter the flight number: ";
            cin >> flightNumber;
            PrintPassengers(flightNumber);
            break;
        }
        case 11:
        {
            DisplayAllFlightsData();
            break;
        }
        case 0: {
            cout << "Exiting program. Thank you!" << endl;
            break;
        }
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);

    return 0;
}