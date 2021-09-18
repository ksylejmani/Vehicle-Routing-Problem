#pragma once
#include"solution.h"
#include"input.h"
#include"preprocess.h"

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

	std::vector<std::pair<double, std::pair<int, int>>> getDeliveryRestrictedCandidateList();
	std::vector<std::pair<double, int>> getPickupRestrictedCandidateList();


	std::pair<double, std::pair<int, int>> selectDeliveryComponent(std::vector<std::pair<double, std::pair<int,
		int>>>& restrictedCandidateList);
	std::pair<double, int> selectPickupComponent(std::vector<std::pair<double, int>>& restrictedCandidateList);
	double calculateRouteLength(const std::vector<int>& routeEvents);
public:
	GRASP(Input _input, double _percentageOfComponents, int _hillClimbingLength, int _pickupEventID);
	Solution construct();
	Solution hillClimb();
	Solution solve();
};