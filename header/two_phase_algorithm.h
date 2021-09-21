#pragma once
#include"solution.h"
#include"input.h"
#include"preprocess.h"

#include<map>

// Extends the Preprocess class
class TwoPhaseAlgorithm: Preprocess {
private:

	double calculateRouteLength(const std::vector<int>& routeEvents);
	std::pair<bool,std::vector<int>> calculateRouteLoad(const std::vector<int>& routeEvents, const int& routePickupEvnet);
	int getStartPointIndex(const std::vector<int>& route);
	int findPickupEvent(const std::vector<int>& route, const int& pickupEventID);
	double getLengthDifference(const std::vector<int>& route, const int& changeIndex, const int& firstEventID, const int& secondEventID);
	void replaceDeliveryEvent(std::vector<int>& route, double& routeLength);
	void replacePickupEvent(Solution& s);
	void printRoute(std::vector<int> route);
	void printDistances(std::vector<int> route);
	void reverseRouteSegement(std::vector<int>& routeEvents, const int& startIndex, const int& endIndex);
	Solution construct();
	Solution twoOptMove(const Solution& s);

public:
	TwoPhaseAlgorithm(const Input& _input);
	Solution solve();
};