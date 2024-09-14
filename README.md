# DSA Airline Project

## Overview
The DSA Airline Project is a C++ application designed to simulate basic airline operations, including flight management, reservation systems, and route finding. This project utilizes various data structures such as linked lists, hash tables, and priority queues to efficiently manage flights and reservations.

## Features
- **Flight Management**: Add and manage flights between various cities.
- **Reservation System**: Make reservations for one-way or round trips.
- **Route Finding**: Find direct and connecting routes between cities.
- **Shortest Path Calculation**: Use Dijkstra's algorithm to find the shortest path between two cities.
- **Display Functions**: View flight information, available cities, departures, arrivals, and passenger lists.

## Getting Started

### Prerequisites
- A C++ compiler (e.g., g++, clang++)
- Basic understanding of C++ programming and data structures

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/hamza-amin-4365/Data_Structures_Airline_Project.git
   ```
2. Navigate to the project directory:
   ```bash
   cd Data_Structures_Airline_Project
   ```
3. Compile the project using a C++ compiler:
   ```bash
   g++ -o airline_project "DSA Airline Project.cpp"
   ```

### Running the Application
Run the compiled application:
```bash
./airline_project
```

## Usage
Upon running the application, you'll be presented with a menu of options:
1. Show Cities
2. Show Departures for a City
3. Show Arrivals for a City
4. Show Reachable Cities from a City
5. Show Shortest Path between Cities
6. Find Route between Cities
7. Make Reservation
8. Print Reservation Schedule
9. Delete Reservation
10. Print Passenger List for a Flight
11. Display all flights data in the database
0. Exit

Simply enter the corresponding number to perform an action.

## Data Structures Used
- **Linked Lists**: For managing flights and reservations.
- **Hash Tables**: To efficiently search for cities and flights.
- **Priority Queues**: For implementing Dijkstra's algorithm to find the shortest path.

## Code Structure
- `FlightType`: Structure representing flight details.
- `RouteType`: Structure representing travel routes.
- `ReservationType`: Structure for managing reservations.
- `CityListType`: Structure for managing city names and associated flights.
- Functions for initializing data structures, inserting flights, making reservations, and displaying information.

## Example Flight Data
This project includes sample flight data to demonstrate functionality:
- Flights between cities like Multan, Tokyo, Gawadar, and Gujranwala with various departure and arrival times.

## Contributing
Contributions are welcome! Please fork the repository and submit a pull request with your changes.

## License
This project is open-source and available under the MIT License.

## Acknowledgments
- C++ Standard Library for data structures and algorithms.
- Various online resources for learning about flight management systems and data structures.

## Contact
For any inquiries, please contact Hamza Amin and Ali Vijdaan via email: <br> mh4070685@gmail.com <br> alivijdaan@gmail.com
```

This README provides a comprehensive overview of the DSA Airline Project, detailing its features, usage, data structures, and how to get started. It also includes instructions for installation and contact information for further inquiries.
