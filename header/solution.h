#pragma once
#include<vector>

struct Solution {
	std::vector<int> routeEvents;
	std::vector<std::pair<int, int>> interchangeEvents;
	double vehicleLoad;
	double routeLength;
	Solution() :vehicleLoad(0) {};
	Solution(const std::vector<int>& _routeEvents, const std::vector<std::pair<int, int>>& _interchangeEvents,
		const double& _vehicleLoad, double _routeLength):
		routeEvents(_routeEvents), interchangeEvents(_interchangeEvents),vehicleLoad(_vehicleLoad), routeLength(_routeLength) {};
};