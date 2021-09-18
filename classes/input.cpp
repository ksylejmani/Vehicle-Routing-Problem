#include"../header/input.h"
#include"../header/parameters.h"

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>

Input getInputFromFile(const std::string& instanceName) {
	unsigned int vehicleCapacity;
	std::pair<int, int> depotCoordinates;
	std::vector<Event> deliveries;
	std::vector<Event> pickups;
	std::ifstream fileStream(instanceName);
	try
	{
		if (fileStream.is_open()) {
			std::string currentLine;
			// Skip the metadata in the first three lines
			for(int i=1;i<=3;i++)
				std::getline(fileStream, currentLine);

			// Read VEHICLE CAPACITY
			std::getline(fileStream, currentLine);
			vehicleCapacity=getNumberFromText(currentLine);

			// Skip the metadata in the next three lines
			for (int i = 1; i <= 3; i++)
				std::getline(fileStream, currentLine);
			
			// Read DEPOT details
			std::getline(fileStream, currentLine);
			depotCoordinates = getDepotCoordinates(currentLine);

			// Skip the metadata in the next two lines
			for (int i = 1; i <= 2; i++)
				std::getline(fileStream, currentLine);
			
			// Read Number of DELEVERIES
			std::getline(fileStream, currentLine);
			unsigned int numberOfDeliveries=getNumberFromText(currentLine);

			// Skip the metadata in the next two lines
			for (int i = 1; i <= 2; i++)
				std::getline(fileStream, currentLine);

			// Read details of each DELIVERY
			for (unsigned int i = 1; i <= numberOfDeliveries; i++) {
				std::getline(fileStream, currentLine);
				deliveries.push_back(getEvent(currentLine));
			}

			// Skip the metadata in the next two lines
			for (int i = 1; i <= 2; i++)
				std::getline(fileStream, currentLine);

			// Read Number of PICKUPS
			std::getline(fileStream, currentLine);
			unsigned int numberOfPickups = getNumberFromText(currentLine);

			// Skip the metadata in the next two lines
			for (int i = 1; i <= 2; i++)
				std::getline(fileStream, currentLine);

			// Read details of each PICKUP
			for (unsigned int i = 1; i <= numberOfPickups; i++) {
				std::getline(fileStream, currentLine);
				pickups.push_back(getEvent(currentLine));
			}
		}
		fileStream.close();
	}
	catch (const std::exception& ex)
	{
		std::cout << "Error message: " << ex.what() << std::endl;
	}
	Input ans(vehicleCapacity, depotCoordinates, deliveries, pickups);
	return ans;
}


unsigned int getNumberFromText(const std::string& lineText) {
	std::string strTemp;
	std::stringstream ss(lineText);
	unsigned int ans;
	ss >> ans;
	return ans;
}


std::pair<int, int> getDepotCoordinates(const std::string& lineText) {
	int xCoordinate,yCoordinate;
	std::stringstream ss(lineText);
	ss >> xCoordinate; ss >> yCoordinate;
	std::pair<int, int> ans(xCoordinate,yCoordinate);
	return ans;
}

Event getEvent(const std::string& lineText) {
	unsigned int id;
	int xCoordinate, yCoordinate;
	unsigned int demand;
	unsigned int serviceTime;
	std::stringstream ss(lineText);
	ss >> id; id--; // Convert id to a zero based index
	ss >> xCoordinate; ss >> yCoordinate;
	ss >> demand; ss >> serviceTime;
	std::pair<int, int> coordinates(xCoordinate, yCoordinate);
	Event ans(id, coordinates, demand, serviceTime);
	return ans;
}