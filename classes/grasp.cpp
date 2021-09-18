#include"../header/grasp.h"
#include"../header/parameters.h"

#include<math.h>
#include<vector>
#include<iostream>
#include<random>
#include<algorithm>


// Do not forget to do the maping between real pickupID and id used in the representation (i.e. -1)

double GRASP::getDistance(const int& firstEvnetID, const int& secondEventID) {
	double ans;
	if (firstEvnetID == encodedPickupEventID) {
		ans = sqrt(pow(input.pickups[pickupEventID].coordinates.first - input.deliveries[secondEventID].coordinates.first, 2) +
			pow(input.pickups[pickupEventID].coordinates.second - input.deliveries[secondEventID].coordinates.second, 2));
	}
	else if (secondEventID == encodedPickupEventID)
	{
		ans = sqrt(pow(input.deliveries[firstEvnetID].coordinates.first - input.pickups[pickupEventID].coordinates.first, 2) +
			pow(input.deliveries[firstEvnetID].coordinates.second - input.pickups[pickupEventID].coordinates.second,2));
	}
	else
	{
		int minID = (firstEvnetID < secondEventID) ? (firstEvnetID) : (secondEventID);
		int maxID = (firstEvnetID > secondEventID) ? (firstEvnetID) : (secondEventID);
		ans = sqrt(pow(input.deliveries[minID].coordinates.first - input.deliveries[maxID].coordinates.first, 2) +
			pow(input.deliveries[minID].coordinates.second - input.deliveries[maxID].coordinates.second,2));
	}
	return ans;
}

std::map<std::pair<int, int>, double> GRASP:: calculateDeliveryDistances() {
	std::map<std::pair<int, int>, double> ans;
	// Calculate distances between DELIVERY events
	for (unsigned int i = 0; i <input.deliveries.size()-1; i++) {
		for (unsigned int j = i + 1; j < input.deliveries.size(); j++) {
			ans[std::make_pair(i,j)] = getDistance(i, j);
		}
	}
	return ans;
}

std::map<int, double> GRASP::calculatePickupDistances() {
	std::map<int, double> ans;
	// Calculate distances between selected PICKUP event against every DELVERY event
	for (unsigned int i = 0; i < input.deliveries.size(); i++) {
		ans[i] = getDistance(encodedPickupEventID, i);
	}
	return ans;
}

std::map<int, double> GRASP::calculateStartEndDistances() {
	std::map<int, double> ans;
	// Calculate distances between start/end point and PICKUP events, and also between start/end and DELIVERY events
	ans[encodedPickupEventID] = sqrt(pow(input.pickups[pickupEventID].coordinates.first,2)+
		pow(input.pickups[pickupEventID].coordinates.second,2));
	for (unsigned int deliveryEventID = 0; deliveryEventID < input.deliveries.size(); deliveryEventID++) {
		ans[deliveryEventID] = sqrt(pow(input.deliveries[deliveryEventID].coordinates.first, 2) +
			pow(input.deliveries[deliveryEventID].coordinates.second, 2));
	}
	return ans;
}
std::multimap<double, std::pair<int, int>> GRASP::createDeliveryComponents(const std::map<std::pair<int, int>, double>& distances) {
	std::multimap<double, std::pair<int, int>> ans;
	for (auto it = distances.begin(); it != distances.end(); it++) {
		// Save components based on their distance in non decreasing order
		ans.insert(std::pair<double,std::pair<int,int>> (it->second,it->first)); 
	}
	return ans;
}

std::multimap<double, int> GRASP::createPickupComponents(const std::map<int, double>& distances) {
	std::multimap<double, int> ans;
	for (auto it = distances.begin(); it != distances.end(); it++) {
		// Save components based on their distance in non decreasing order
		ans.insert(std::pair<double, int>(it->second, it->first));
	}
	return ans;
}

std::multimap<int, int> GRASP::createDemandComponents() {
	std::multimap<int, int> ans;
	for (auto it = input.deliveries.begin(); it != input.deliveries.end(); it++) {
		ans.insert(std::pair<int, int>(it->demand, it->id));
	}
	return ans;
}

GRASP::GRASP(Input _input, double _percentageOfComponents, int _hillClimbingLength, int _pickupEventID):
	input(_input), percentageOfComponents(_percentageOfComponents), hillClimbingLength(_hillClimbingLength),
	pickupEventID(_pickupEventID){
	srand(seed); 
	deliveryDistances = this->calculateDeliveryDistances();
	deliveryComponents = this->createDeliveryComponents(deliveryDistances);
	pickupDistances = this->calculatePickupDistances();
	pickupComponents = this->createPickupComponents(pickupDistances);
	startEndDistances = this->calculateStartEndDistances();
	demandComponents = this->createDemandComponents();
}

std::vector<std::pair<double, std::pair<int, int>>> GRASP::getDeliveryRestrictedCandidateList() {
	std::vector<std::pair<double, std::pair<int, int>>> ans;
	unsigned int numberOfCandidates = (int)(percentageOfComponents * deliveryComponents.size());
	unsigned int countCandidates = 0;
	for (auto it = deliveryComponents.begin(); it != deliveryComponents.end(); it++) {
		ans.push_back(*it);
		countCandidates++;
		if (countCandidates >= numberOfCandidates)
			break;
	}
	return ans;
}

std::vector<std::pair<double, int>> GRASP::getPickupRestrictedCandidateList() {
	std::vector<std::pair<double, int>> ans;
	unsigned int numberOfCandidates = (int)(percentageOfComponents * pickupComponents.size());
	unsigned int countCandidates = 0;
	for (auto it = pickupComponents.begin(); it != pickupComponents.end(); it++) {
		ans.push_back(*it);
		countCandidates++;
		if (countCandidates >= numberOfCandidates)
			break;
	}
	return ans;
}

std::pair<double, std::pair<int, int>> GRASP::selectDeliveryComponent(std::vector<std::pair<double, std::pair<int,
	int>>>& restrictedCandidateList) {
	std::pair<double, std::pair<int, int>> ans;
	int randomIndex = rand() % restrictedCandidateList.size();
	ans = restrictedCandidateList[randomIndex];
	return ans;
}

std::pair<double, int> GRASP::selectPickupComponent(std::vector<std::pair<double, int>>& restrictedCandidateList) {
	std::pair<double, int> ans;
	int randomIndex = rand() % restrictedCandidateList.size();
	ans = restrictedCandidateList[randomIndex];
	return ans;
}

double GRASP::calculateRouteLength(const std::vector<int>& routeEvents){
	double ans = 0;
	ans += (this->startEndDistances[routeEvents[0]]);
	for (int i = 1; i < routeEvents.size()-1; i++) {
		ans += (this->getDistance(routeEvents[i], routeEvents[i + 1]));
	}
	ans += (this->pickupDistances[routeEvents[routeEvents.size() - 1]]);
	ans += (this->startEndDistances[encodedPickupEventID]);
	return ans;
}

Solution GRASP::getDemandBasedSolution() {
	std::vector<int> routeEvents;
	std::vector<std::pair<int,int>> interchangeEvents;
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
				interchangeEvents.push_back(std::make_pair(routeIt->second,spareIt->second));
				routeIt--;
			}
			// In case the last event in routeEvents can not be subsituted by the current Event, than no preceding evnents can do that
			// This is because all preciding events have equal or smaller demand value
			if (routeIt == it) {
				break;
			}
			spareIt++;
		}
		while (spareIt != demandComponents.end());
	}
	std::shuffle(routeEvents.begin(), routeEvents.end(), std::default_random_engine(seed));
	double routeLength = this->calculateRouteLength(routeEvents);
	Solution ans(routeEvents, interchangeEvents, vehicleLoad,routeLength);
	return ans;
}


Solution GRASP::construct() {
	std::vector<std::pair<double, std::pair<int, int>>> deliveryRestrictedCandidateList=this->getDeliveryRestrictedCandidateList();
	Solution current = this->getDemandBasedSolution();
	return current;
}
