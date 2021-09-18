#pragma once
#include<utility>
#include<vector>
#include<string>

struct Event{
	unsigned int id;
	std::pair<int, int> coordinates;
	unsigned int demand;
	unsigned int serviceTime;
	Event():id(0),demand(0),serviceTime(0) {};
	Event(const unsigned int& _id, const std::pair<int, int>& _coordinates,
		const unsigned int& _demand, const unsigned int& _serviceTime) :
		id(_id), coordinates(_coordinates), demand(_demand), serviceTime(_serviceTime) {};
};
struct Input
{
	unsigned int vehicleCapacity;
	std::pair<int, int> depotCoordinates;
	std::vector<Event> deliveries, pickups;
	Input():vehicleCapacity(0) {};
	Input(const unsigned int& _vehicleCapacity, const std::pair<int, int>& _depotCoordinates,
		const std::vector<Event>& _deliveries, const std::vector<Event>& _pickups) :
		vehicleCapacity(_vehicleCapacity), depotCoordinates(_depotCoordinates), deliveries(_deliveries), pickups(_pickups) {};
};

Input getInputFromFile(const std::string& instanceName);
unsigned int getNumberFromText(const std::string& lineText);
std::pair<int, int> getDepotCoordinates(const std::string& lineText);
Event getEvent(const std::string& lineText);
