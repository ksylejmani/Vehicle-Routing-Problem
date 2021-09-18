#include"../header/preprocess.h"
#include"../header/parameters.h"

#include<math.h>
#include<vector>
#include<iostream>
#include<random>
#include<algorithm>

Preprocess::Preprocess(Input _input, int _pickupEventID):
	input(_input),pickupEventID(_pickupEventID) {
	srand(seed);
	demandComponents = this->createDemandComponents();
	filterDeliveryEvents();
	substituteEvents = this->getSubstituteEvents();
	deliveryDistances = this->calculateDeliveryDistances();
	deliveryComponents = this->createDeliveryComponents(deliveryDistances);
	pickupDistances = this->calculatePickupDistances();
	pickupComponents = this->createPickupComponents(pickupDistances);
	startEndDistances = this->calculateStartEndDistances();
}
void Preprocess::filterDeliveryEvents() {
	double vehicleLoad = 0;
	// Get list of Delivery events in non decreaing order of capacity, which can be loaded into the vehicle
	std::multimap<int, int>::iterator it;
	for (it = demandComponents.begin(); it != demandComponents.end(); it++) {
		if (it->first + vehicleLoad <= input.vehicleCapacity) {
			routeEvents.push_back(it->second);
			vehicleLoad += it->first;
		}
		else {
			break;
		}
	}

	// Get the list of Delivery events which can subsitute a given event in the route (a.k.a. spareEvents)
	if (it != demandComponents.end()) {
		std::multimap<int, int>::iterator spareIt = it;
		spareIt++;
		do
		{
			std::multimap<int, int>::iterator routeIt = it;
			while (vehicleLoad - routeIt->first + spareIt->first <= input.vehicleCapacity)
			{
				interchangeEvents.push_back(std::make_pair(routeIt->second, spareIt->second));
				routeIt--;
			}
			// In case the last event in routeEvents can not be subsituted by the current Event, than no preceding evnents can do that
			// This is because all preciding events have equal or smaller demand value
			if (routeIt == it) {
				break;
			}
			spareIt++;
		} while (spareIt != demandComponents.end());
	}
}

std::vector<int> Preprocess::getSubstituteEvents() {
	std::vector<int> ans;
	for (auto it = this->interchangeEvents.begin(); it != this->interchangeEvents.end(); it++) {
		auto p = std::find(ans.begin(), ans.end(), it->second);
		if (p == ans.end())
			ans.push_back(it->second);
	}
	return ans;
}

double Preprocess::getDistance(const int& firstEvnetID, const int& secondEventID) {
	double ans;
	if (firstEvnetID == encodedPickupEventID) {
		ans = sqrt(pow(input.pickups[pickupEventID].coordinates.first - input.deliveries[secondEventID].coordinates.first, 2) +
			pow(input.pickups[pickupEventID].coordinates.second - input.deliveries[secondEventID].coordinates.second, 2));
	}
	else if (secondEventID == encodedPickupEventID)
	{
		ans = sqrt(pow(input.deliveries[firstEvnetID].coordinates.first - input.pickups[pickupEventID].coordinates.first, 2) +
			pow(input.deliveries[firstEvnetID].coordinates.second - input.pickups[pickupEventID].coordinates.second, 2));
	}
	else
	{
		int minID = (firstEvnetID < secondEventID) ? (firstEvnetID) : (secondEventID);
		int maxID = (firstEvnetID > secondEventID) ? (firstEvnetID) : (secondEventID);
		ans = sqrt(pow(input.deliveries[minID].coordinates.first - input.deliveries[maxID].coordinates.first, 2) +
			pow(input.deliveries[minID].coordinates.second - input.deliveries[maxID].coordinates.second, 2));
	}
	return ans;
}


std::map<std::pair<int, int>, double> Preprocess::calculateDeliveryDistances() {
	std::map<std::pair<int, int>, double> ans;
	// Calculate distances between DELIVERY events
	for (unsigned int i = 0; i < input.deliveries.size() - 1; i++) {
		for (unsigned int j = i + 1; j < input.deliveries.size(); j++) {
			ans[std::make_pair(i, j)] = getDistance(i, j);
		}
	}
	return ans;
}

std::map<int, double> Preprocess::calculatePickupDistances() {
	std::map<int, double> ans;
	// Calculate distances between selected PICKUP event against every DELVERY event
	for (unsigned int i = 0; i < input.deliveries.size(); i++) {
		ans[i] = getDistance(encodedPickupEventID, i);
	}
	return ans;
}

std::map<int, double> Preprocess::calculateStartEndDistances() {
	std::map<int, double> ans;
	// Calculate distances between start/end point and PICKUP events, and also between start/end and DELIVERY events
	ans[encodedPickupEventID] = sqrt(pow(input.pickups[pickupEventID].coordinates.first, 2) +
		pow(input.pickups[pickupEventID].coordinates.second, 2));
	for (unsigned int deliveryEventID = 0; deliveryEventID < input.deliveries.size(); deliveryEventID++) {
		ans[deliveryEventID] = sqrt(pow(input.deliveries[deliveryEventID].coordinates.first, 2) +
			pow(input.deliveries[deliveryEventID].coordinates.second, 2));
	}
	return ans;
}

std::multimap<double, std::pair<int, int>> Preprocess::createDeliveryComponents(const std::map<std::pair<int, int>, double>& distances) {
	std::multimap<double, std::pair<int, int>> ans;
	for (auto it = distances.begin(); it != distances.end(); it++) {
		// See whether both events are part of routeEvents or substitueEvents (i.e. candidates)
		if (this->areCandidates(it->first.first, it->first.second)) {
			// Save components based on their distance in non decreasing order
			ans.insert(std::pair<double, std::pair<int, int>>(it->second, it->first));
		}
	}
	return ans;
}

bool Preprocess::areCandidates(int _event1ID, int _event2ID) {
	auto p1RE = std::find(routeEvents.begin(), routeEvents.end(), _event1ID);
	auto p1SE = std::find(substituteEvents.begin(), substituteEvents.end(), _event1ID);
	auto p2RE = std::find(routeEvents.begin(), routeEvents.end(), _event2ID);
	auto p2SE = std::find(substituteEvents.begin(), substituteEvents.end(), _event2ID);
	if ((p1RE != routeEvents.end() || p1SE != substituteEvents.end()) &&
		(p2RE != routeEvents.end() || p2SE != substituteEvents.end())) {
		return true;
	}
	else {
		return false;
	}
}

bool Preprocess::isCandidate(int _eventID) {
	auto p1RE = std::find(routeEvents.begin(), routeEvents.end(), _eventID);
	auto p1SE = std::find(substituteEvents.begin(), substituteEvents.end(), _eventID);

	if (p1RE != routeEvents.end() || p1SE != substituteEvents.end()) {
		return true;
	}
	else {
		return false;
	}
}

std::multimap<double, int> Preprocess::createPickupComponents(const std::map<int, double>& distances) {
	std::multimap<double, int> ans;
	for (auto it = distances.begin(); it != distances.end(); it++) {
		if (this->isCandidate(it->first)) {
			// Save components based on their distance in non decreasing order
			ans.insert(std::pair<double, int>(it->second, it->first));
		}
	}
	return ans;
}

std::multimap<int, int> Preprocess::createDemandComponents() {
	std::multimap<int, int> ans;
	for (auto it = input.deliveries.begin(); it != input.deliveries.end(); it++) {
		ans.insert(std::pair<int, int>(it->demand, it->id));
	}
	return ans;
}
