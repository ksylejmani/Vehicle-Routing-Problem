#pragma once
#include<vector>
#include<unordered_map>

struct Solution {
	std::vector<int> routeEvents;
	int routePickupEvent;
	std::vector<int> nonRouteEvents;
	std::pair<bool, std::vector<int>> routeLoad;
	double routeLength;

	Solution() :routePickupEvent(0), routeLength(0){};

	Solution(const std::vector<int>& _routeEvents, const int & _routePickupEvent,
		const std::vector<int>& _nonRouteEvents,
		const std::pair<bool, std::vector<int>>& _routeLoad, const double& _routeLength):
		routeEvents(_routeEvents), routePickupEvent(_routePickupEvent), 
		nonRouteEvents(_nonRouteEvents),
		routeLoad(_routeLoad), routeLength(_routeLength) {};

	Solution(const Solution& _solution):routeEvents(_solution.routeEvents),
		routePickupEvent(_solution.routePickupEvent),nonRouteEvents(_solution.nonRouteEvents),
		routeLoad(_solution.routeLoad),routeLength(_solution.routeLength){}
};