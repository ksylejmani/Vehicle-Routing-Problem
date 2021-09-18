#pragma once
#include"input.h"

#include<map>



class Preprocess {
private:
	Input input;
	int pickupEventID;

	std::vector<int> routeEvents;
	std::vector<std::pair<int, int>> interchangeEvents;
	std::vector<int> substituteEvents;

	std::map<std::pair<int, int>, double> deliveryDistances;
	std::map<int, double> pickupDistances;
	std::map<int, double> startEndDistances;
	std::multimap<double, std::pair<int, int>> deliveryComponents;
	std::multimap<double, int> pickupComponents;
	std::multimap<int, int> demandComponents;

	std::multimap<int, int> createDemandComponents();
	void filterDeliveryEvents();
	std::vector<int> getSubstituteEvents();
	bool areCandidates(int _event1ID, int _event2ID);
	bool isCandidate(int _eventID);
	double getDistance(const int& firstEvnetID, const int& secondEventID);
	std::map<std::pair<int, int>, double> calculateDeliveryDistances();
	std::map<int, double> calculatePickupDistances();
	std::map<int, double> calculateStartEndDistances();
	std::multimap<double, std::pair<int, int>> createDeliveryComponents(const std::map<std::pair<int, int>, double>& distances);
	std::multimap<double, int> createPickupComponents(const std::map<int, double>& distances);

public:
	Preprocess(Input _input, int _pickupEventID);
};
