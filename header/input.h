#pragma once
#include<utility>
#include<vector>
#include<string>

struct Event{
	unsigned int id;
	std::pair<int, int> coordinates;
	unsigned int demand;
	bool isDelivery;

	Event():id(0),
		demand(0),
		isDelivery(false) {};

	Event(const unsigned int& _id, 
		const std::pair<int, 
		int>& _coordinates,
		const unsigned int& _demand, 
		const unsigned int& _isDelivery) :
		id(_id), 
		coordinates(_coordinates), 
		demand(_demand), 
		isDelivery(_isDelivery) {};
};
struct Input
{
	unsigned int vehicleCapacity;
	std::pair<int, int> depotCoordinates;
	std::vector<Event> events;

	Input():vehicleCapacity(0) {};

	Input(const unsigned int& _vehicleCapacity, 
		const std::pair<int, 
		int>& _depotCoordinates,
		const std::vector<Event>& _events, 
		const std::vector<Event>& _pickups) :
		vehicleCapacity(_vehicleCapacity), 
		depotCoordinates(_depotCoordinates), 
		events(_events){};
};

Input getInputFromFile(const std::string& instanceName);
unsigned int getNumberFromText(const std::string& lineText);
std::pair<int, int> getDepotCoordinates(const std::string& lineText);
Event getEvent(const std::string& lineText);
