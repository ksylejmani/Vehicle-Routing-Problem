#pragma once
#include"solution.h"
#include"input.h"
#include"preprocess.h"

#include<map>

class GRASP: Preprocess {
private:
	std::vector<std::pair<double, std::pair<int, int>>> deliveryRestrictedCandidateList;
	std::vector<std::pair<double, std::pair<int, int>>> pickupRestrictedCandidateList;

	std::vector<std::pair<double, std::pair<int, int>>> getDeliveryRestrictedCandidateList();
	std::vector<std::pair<double, std::pair<int, int>>> getPickupRestrictedCandidateList();
	std::pair<double, std::pair<int, int>> selectDeliveryComponent(std::vector<std::pair<double, 
		std::pair<int, int>>>& restrictedCandidateList);
	std::pair<double, std::pair<int, int>> selectPickupComponent(const std::vector<std::pair<double, 
		std::pair<int, int>>>& restrictedCandidateList);
	std::pair<double, std::pair<int, int>> selectPickupComponent(std::multimap<int, int> pickupDemandComponents);
	double calculateRouteLength(const std::vector<int>& routeEvents);
	std::pair<bool,std::vector<int>> calculateRouteLoad(const std::vector<int>& routeEvents, const int& routePickupEvnet);
	Solution _2OptMove(const Solution& s);
	void reverseRouteSegement(std::vector<int>& routeEvents, const int& startIndex, const int& endIndex);
	int getStartPointIndex(const std::vector<int>& route);
	int findPickupEvent(const std::vector<int>& route, const int& pickupEventID);
	double getLengthDifference(const std::vector<int>& route, const int& changeIndex, const int& firstEventID, const int& secondEventID);
	void replaceDeliveryEvent(std::vector<int>& route, double& routeLength);
	void replacePickupEvent(Solution& s);
	void printRoute(std::vector<int> route);
	void printDistances(std::vector<int> route);
	Solution construct();

public:
	GRASP(const Input& _input);
	Solution solve();
};