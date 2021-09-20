#pragma once
#include"input.h"

#include<map>
#include<unordered_map>

class Preprocess {
protected:
	Input input;

	std::vector<int> primaryEvents;
	std::vector<int> substitutingEvents;
	std::vector<std::pair<int, int>> substitutingPairs;
	std::unordered_map<int, std::vector<int>> eventMapping;
	std::vector<int> pickupEvents;
	std::map<std::pair<int, int>, double> distances;
	std::map<int, double> startEndDistances;
	std::multimap<double, std::pair<int, int>> deliveryDistanceComponents;
	std::multimap<double, std::pair<int, int>> pickupDistanceComponents;
	std::multimap<int, int> deliveryDemandComponents;
	std::multimap<int, int> pickupDemandComponents;

	std::multimap<int, int> createDemandComponents(const bool& _isDemand);
	void filterDeliveryEvents();
	std::vector<int> getSubstitutingEvents();
	bool areCandidates(int _event1ID, int _event2ID);
	bool isCandidate(int _eventID);
	std::unordered_map<int, std::vector<int>> getEventMapping();
	std::vector<int> getPickupEvents();

	double getDistance(const int& firstEvnetID, const int& secondEventID);
	std::map<std::pair<int, int>, double> calculateDistances();
	std::map<int, double> calculateStartEndDistances();
	std::multimap<double, std::pair<int, int>> createDeliveryDistanceComponents();
	std::multimap<double, std::pair<int, int>> createPickupDistanceComponents();

public:
	Preprocess(Input _input);
};
