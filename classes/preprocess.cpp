#include"../header/preprocess.h"
#include"../header/parameters.h"

#include<math.h>
#include<vector>
#include<iostream>
#include<random>
#include<algorithm>

Preprocess::Preprocess(Input _input):
	input(_input) {
	srand(SEED);
	deliveryDemandComponents = this->createDemandComponents(true);
	filterDeliveryEvents();
	substitutingEvents = this->getSubstitutingEvents();
	distances = this->calculateDistances();
	startEndDistances = this->calculateStartEndDistances();
	pickupEvents = this->getPickupEvents();
}

std::multimap<int, int> Preprocess::createDemandComponents(const bool& _isDemand) {
	std::multimap<int, int> ans;
	for (auto it = input.events.begin(); it != input.events.end(); it++) {
		if (it->isDelivery==_isDemand)
			ans.insert(std::pair<int, int>(it->demand, it->id));
	}
	return ans;
}

void Preprocess::filterDeliveryEvents() {
	/// <summary>
	/// This procedure does two operations:
	/// 1. Sorts the delivery events based on their demand, so that it be easily calculated what is the maximum number of delivery events that can visited
	/// 2. Make a list of pairs of delivery events which can substitute one another in reference to their demand values
	/// </summary>
	
	double vehicleLoad = 0;
	// Get list of Delivery events in non decreaing order of capacity, which can be loaded into the vehicle
	std::multimap<int, int>::iterator it;
	for (it = deliveryDemandComponents.begin(); it != deliveryDemandComponents.end(); it++) {
		if (it->first + vehicleLoad <= input.vehicleCapacity) {
			primaryEvents.push_back(it->second);
			vehicleLoad += it->first;
		}
		else {
			break;
		}
	}

	// Get the list of Delivery events which can subsitute a given event in the route (a.k.a. spareEvents)
	if (it != deliveryDemandComponents.end()) {
		std::multimap<int, int>::iterator spareIt = it;
		spareIt++;
		do
		{
			std::multimap<int, int>::iterator routeIt = it;
			while (routeIt!= deliveryDemandComponents.end() && vehicleLoad - routeIt->first + spareIt->first <= input.vehicleCapacity)
			{
				substitutingPairs.push_back(std::make_pair(routeIt->second, spareIt->second));
				routeIt--;
			}
			// In case the last event in routeEvents can not be subsituted by the current Event, than no preceding evnents can do that
			// This is because all preciding events have equal or smaller demand value
			if (routeIt == it) {
				break;
			}
			spareIt++;
		} while (spareIt != deliveryDemandComponents.end());
	}
}

std::vector<int> Preprocess::getPickupEvents() {
	std::vector<int> ans;
	for (auto it = input.events.begin(); it != input.events.end(); it++) {
		if (!it->isDelivery)
			ans.push_back(it->id);
	}
	return ans;
}

double Preprocess::getDistance(const int& firstEvnetID, const int& secondEventID) {
	double ans;
	if (firstEvnetID == -1) {
		ans= startEndDistances[secondEventID];
	}
	else if (secondEventID == -1) {
		ans = startEndDistances[firstEvnetID];
	}
	else {
		int minID = (firstEvnetID < secondEventID) ? (firstEvnetID) : (secondEventID);
		int maxID = (firstEvnetID > secondEventID) ? (firstEvnetID) : (secondEventID);
		ans = sqrt(pow(input.events[minID].coordinates.first - input.events[maxID].coordinates.first, 2) +
			pow(input.events[minID].coordinates.second - input.events[maxID].coordinates.second, 2));
	}
	return ans;
}

std::map<std::pair<int, int>, double> Preprocess::calculateDistances() {
	/// <summary>
	/// Calculate distances between events
	/// </summary>
	/// <returns></returns>
	
	std::map<std::pair<int, int>, double> ans;
	for (unsigned int leftEventID = 0; leftEventID < input.events.size() - 1; leftEventID++) {
		for (unsigned int rightEventID = leftEventID + 1; rightEventID < input.events.size(); rightEventID++) {
			// See whether both events are part of primaryEvents or substituting Events (i.e. candidates),
			// Or they are pickup events
			if ((!this->isCandidate(leftEventID) && input.events[leftEventID].isDelivery) || 
				(!this->isCandidate(rightEventID) && input.events[rightEventID].isDelivery)) {
				continue;
			}
			else {
				ans[std::make_pair(leftEventID, rightEventID)] = getDistance(leftEventID, rightEventID);
			}
		}
	}
	return ans;
}

std::map<int, double> Preprocess::calculateStartEndDistances() {
	/// <summary>
	/// Calculate distances between start/end point and events
	/// </summary>
	/// <returns></returns>
	
	std::map<int, double> ans;
	for (unsigned int eventID = 0; eventID < input.events.size(); eventID++) {
		// See whether the event is part of primaryEvents or substitutingEvents (i.e. candidates),
		// Or it is a pickup event
		if (!this->isCandidate(eventID) && input.events[eventID].isDelivery) {
			continue;
		}
		else {
			ans[eventID] = sqrt(pow(input.events[eventID].coordinates.first, 2) +
				pow(input.events[eventID].coordinates.second, 2));
		}
	}
	return ans;
}

std::vector<int> Preprocess::getSubstitutingEvents() {
	std::vector<int> ans;
	for (auto it = this->substitutingPairs.begin(); it != this->substitutingPairs.end(); it++) {
		auto p = std::find(ans.begin(), ans.end(), it->second);
		if (p == ans.end())
			ans.push_back(it->second);
	}
	return ans;
}

bool Preprocess::areCandidates(int _event1ID, int _event2ID) {
	auto p1PE = std::find(primaryEvents.begin(), primaryEvents.end(), _event1ID);
	auto p1SE = std::find(substitutingEvents.begin(), substitutingEvents.end(), _event1ID);
	auto p2PE = std::find(primaryEvents.begin(), primaryEvents.end(), _event2ID);
	auto p2SE = std::find(substitutingEvents.begin(), substitutingEvents.end(), _event2ID);
	if ((p1PE != primaryEvents.end() || p1SE != substitutingEvents.end()) &&
		(p2PE != primaryEvents.end() || p2SE != substitutingEvents.end())) {
		return true;
	}
	else {
		return false;
	}
}

std::multimap<double, std::pair<int, int>> Preprocess::createDeliveryDistanceComponents() {
	std::multimap<double, std::pair<int, int>> ans;
	for (auto it = distances.begin(); it != distances.end(); it++) {
		// See whether both events are part of pimraryEvents or substitutingEvents (i.e. candidates)
		if(this->areCandidates(it->first.first,it->first.second))
			ans.insert(std::pair<double, std::pair<int, int>>(it->second, it->first));
	}
	return ans;
}

bool Preprocess::isCandidate(int _eventID) {
	auto p1RE = std::find(primaryEvents.begin(), primaryEvents.end(), _eventID);
	auto p1SE = std::find(substitutingEvents.begin(), substitutingEvents.end(), _eventID);
	if (p1RE != primaryEvents.end() || p1SE != substitutingEvents.end()) {
		return true;
	}
	else {
		return false;
	}
}

std::multimap<double, std::pair<int, int>> Preprocess::createPickupDistanceComponents() {
	std::multimap<double, std::pair<int, int>> ans;
	for (auto it = distances.begin(); it != distances.end(); it++) {
		// See whether either of the events is a pickup event, but not both
		if (input.events[it->first.first].isDelivery ^ input.events[it->first.second].isDelivery)
				ans.insert(std::pair<double, std::pair<int, int>>(it->second, it->first));
	}
	return ans;
}

std::unordered_map<int, std::vector<int>> Preprocess::getEventMapping() {
	std::unordered_map<int, std::vector<int>> ans;
	for (auto it = substitutingPairs.begin(); it != substitutingPairs.end(); it++) {
		ans[it->first].push_back(it->second);
	}
	return ans;
}