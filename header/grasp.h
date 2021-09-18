#pragma once
#include"solution.h"
#include"input.h"

#include<map>

class GRASP {
private:
	Input input;
	double percentageOfComponents;
	int hillClimbingLength;
	int pickupEventID;
	std::map<std::pair<int, int>, double> deliveryDistances;
	std::map<int, double> pickupDistances;
	std::map<int, double> startEndDistances;
	std::multimap<double, std::pair<int, int>> deliveryComponents;
	std::multimap<double, int> pickupComponents;
	std::multimap<int, int> demandComponents;
	std::vector<std::pair<double, std::pair<int, int>>> getDeliveryRestrictedCandidateList();
	std::vector<std::pair<double, int>> getPickupRestrictedCandidateList();
	double getDistance(const int& firstEvnetID, const int& secondEventID);
	std::map<std::pair<int, int>, double> calculateDeliveryDistances();
	std::map<int, double> calculatePickupDistances();
	std::map<int, double> calculateStartEndDistances();
	std::multimap<double, std::pair<int, int>> createDeliveryComponents(const std::map<std::pair<int, int>, double>& distances);
	std::multimap<double, int> createPickupComponents(const std::map<int, double>& distances);
	std::multimap<int, int> createDemandComponents();
	std::pair<double, std::pair<int, int>> selectDeliveryComponent(std::vector<std::pair<double, std::pair<int,
		int>>>& restrictedCandidateList);
	std::pair<double, int> selectPickupComponent(std::vector<std::pair<double, int>>& restrictedCandidateList);
	Solution getDemandBasedSolution();
	double calculateRouteLength(const std::vector<int>& routeEvents);
public:
	GRASP(Input _input, double _percentageOfComponents, int _hillClimbingLength, int _pickupEventID);
	Solution construct();
	Solution hillClimb();
	Solution solve();
};