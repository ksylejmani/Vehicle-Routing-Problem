#include"../header/two_phase_algorithm.h"
#include"../header/parameters.h"
#include"../header/preprocess.h"

#include<math.h>
#include<vector>
#include<iostream>
#include<random>
#include<algorithm>


TwoPhaseAlgorithm::TwoPhaseAlgorithm(const Input& _input) :
	Preprocess(_input){
	srand(SEED);
}

double TwoPhaseAlgorithm::calculateRouteLength(const std::vector<int>& routeEvents){
	double ans = 0;
	for (unsigned int i = 0; i < routeEvents.size()-1; i++) {
		ans += (getDistance(routeEvents[i], routeEvents[i + 1]));
	}
	ans += (getDistance(routeEvents[routeEvents.size()-1], routeEvents[0]));
	return ans;
}

int TwoPhaseAlgorithm::getStartPointIndex(const std::vector<int>& route) {
	for (int i = 0; i < route.size(); i++) {
		if (route[i] == -1) {
			return i;
		}
	}
	return -1;
}

int TwoPhaseAlgorithm::findPickupEvent(const std::vector<int>& route, const int & pickupEventID) {
	for (int i = 0; i < route.size(); i++) {
		if (route[i] == pickupEventID) {
			return i;
		}
	}
	return -1;
}

std::pair<bool, std::vector<int>> TwoPhaseAlgorithm::calculateRouteLoad(const std::vector<int>& routeEvents, const int& routePickupEvnet) {
	/// <summary>
	/// Chechks and returns the route validity and load
	/// </summary>
	/// <param name="routeEvents"></param>
	/// <param name="routePickupEvnet"></param>
	/// <returns>The first value in the pair returns whether the route is valid, whereas the second returns route load in all individual events</returns>
	
	std::pair<bool, std::vector<int>>ans;
	int N = routeEvents.size();
	ans.first = true;
	// Initialize load to zero
	for (int i = 0; i < N; i++)
		ans.second.push_back(0);
	
	int startPointIndex=this->getStartPointIndex(routeEvents);
	ans.second[startPointIndex]=input.vehicleCapacity;

	for (unsigned int i = startPointIndex+1; i < N+startPointIndex; i++) {
		if (routeEvents[i % N] == routePickupEvnet) {
			ans.second[i % N] = ans.second[(i - 1) % N] + input.events[routeEvents[i % N]].demand;
		}
		else {
			ans.second[i % N] = ans.second[(i - 1) % N] - input.events[routeEvents[i % N]].demand;
		}
		if (ans.second[i % N] > input.vehicleCapacity) {
			ans.first = false;
			break;
		}
	}
	return ans;
}

Solution TwoPhaseAlgorithm::construct() {
	/// <summary>
	/// Constructs a solution by inserting maximum possible delivery events
	/// </summary>
	/// <returns></returns>
	Solution ans ;

	// Insert delivery events based on their demand in non-decreasing order
	for (auto it = primaryEvents.begin(); it != primaryEvents.end(); it++) {
		ans.routeEvents.push_back(*it);
	}
	
	// Randomly shuffle delivery events
	std::shuffle(ans.routeEvents.begin(), ans.routeEvents.end(), std::default_random_engine(SEED));

	// Insert start point at the begining of route and a pickup event (selected at random) at the end of the route
	// so that the route is guaranted to be feasible (in terms of vehicle capacity)
	ans.routeEvents.insert(ans.routeEvents.begin(),-1); // The start point is labeled with an index of -1
	int rndPickupIndex = rand() % pickupEvents.size();
	ans.routePickupEvent = pickupEvents[rndPickupIndex];
	ans.routeEvents.push_back(ans.routePickupEvent);

	// Calculate route length, load and its feasiblity
	ans.routeLength = this->calculateRouteLength(ans.routeEvents);
	ans.routeLoad = this->calculateRouteLoad(ans.routeEvents, ans.routePickupEvent);
	return ans;
}

void TwoPhaseAlgorithm::printRoute(std::vector<int> route) {
	for (int i = 0; i < route.size(); i++) {
		std::cout << route[i] << " ";
	}
	std::cout << std::endl;
}

void TwoPhaseAlgorithm::printDistances(std::vector<int> route) {
	for (int i = 0; i < route.size() - 1; i++) {
		std::cout << getDistance(route[i], route[i + 1]) << " ";
	}
	std::cout << getDistance(route[route.size() - 1], route[0]);
	std::cout << std::endl;
}

Solution TwoPhaseAlgorithm::twoOptMove(const Solution& s) {
	/// <summary>
	/// Applies two opt operator in the given solution (using the best improving version)
	/// </summary>
	/// <param name="s"></param>
	/// <returns></returns>
	
	Solution ans(s);
	struct BestMove{
		int i = -1, j = -1;
		double gainExpected = 0;
	} bestMove;
	bool localOptimumReached = false;
	while (!localOptimumReached)
	{
		localOptimumReached = true;
		int i, j;
		for (unsigned int count1 = 0; count1 <= ans.routeEvents.size() - 3; count1++) {
			i = count1;
			int x1 = ans.routeEvents[i];
			int x2 = ans.routeEvents[(i + 1) % ans.routeEvents.size()];
			int count2Limit;
			if (i == 0) {
				count2Limit = ans.routeEvents.size() - 2;
			}
			else {
				count2Limit = ans.routeEvents.size() - 1;
			}
			for (unsigned int count2 = count1 + 2; count2 <= count2Limit; count2++) {
				j = count2;
				int y1 = ans.routeEvents[j];
				int y2 = ans.routeEvents[(j + 1) % ans.routeEvents.size()];
				double deleteLength = getDistance(x1, x2) + getDistance(y1, y2);
				double addLength = getDistance(x1, y1) + getDistance(x2, y2);
				double gainExpected = deleteLength - addLength;
				if (gainExpected>bestMove.gainExpected) {
					bestMove.i = i; bestMove.j = j;
					bestMove.gainExpected = deleteLength - addLength;
					localOptimumReached = false;
				}
			}
		}
		if (!localOptimumReached) {
			this->reverseRouteSegement(ans.routeEvents, (bestMove.i + 1) % ans.routeEvents.size(), bestMove.j);
			ans.routeLength = this->calculateRouteLength(ans.routeEvents);
			ans.routeLoad = this->calculateRouteLoad(ans.routeEvents, ans.routePickupEvent);
		}
	}
	// If the new solution is feasible return it
	if (ans.routeLoad.first) {
		return ans;
	}
	else {
		return s;
	}
}

void TwoPhaseAlgorithm::reverseRouteSegement(std::vector<int>& routeEvents, const int& startIndex, const int& endIndex) {
	for (int i = startIndex; i < endIndex; i++) {
		int temp = routeEvents[i];
		routeEvents[i] = routeEvents[endIndex - (i - startIndex)];
		routeEvents[endIndex - (i - startIndex)] = temp;
	}
}

double TwoPhaseAlgorithm::getLengthDifference(const std::vector<int>& route,const int& changeIndex, const int& firstEventID, const int& secondEventID) {
	double ans;
	double removeLength = getDistance(route[changeIndex - 1], firstEventID) +
		getDistance(firstEventID, route[(changeIndex + 1) % route.size()]);
	double addLength = getDistance(route[changeIndex - 1], secondEventID) +
		getDistance(secondEventID, route[(changeIndex + 1) % route.size()]);
	ans = addLength - removeLength;
	return ans;
}

void TwoPhaseAlgorithm::replaceDeliveryEvent(std::vector<int>& _route, double& _routeLength) {
	/// <summary>
	/// Replace the delivery event in place by using the parameter referencing mechanism
	/// </summary>
	/// <param name="_route">List of events in the route</param>
	/// <param name="_routeLength">Current route length</param>

	// Replace an event with it pair if there are such pairs
	if (substitutingPairs.size() > 0) {
		int rndIndex = rand() % substitutingPairs.size();
		for (int i = rndIndex; i < substitutingPairs.size() + rndIndex; i++) {
			std::pair<int, int> currentPair = substitutingPairs[rndIndex % substitutingPairs.size()];
			bool eventReplaced = false;
			for (int j = 0; j < _route.size(); j++) {
				if (_route[j] == currentPair.first) {
					auto it = std::find(_route.begin(),
						_route.end(), currentPair.second);
					if (it == _route.end()) {
						int removeEventID = _route[j];
						int addEventID = currentPair.second;
						_route[j] = addEventID;
						currentPair.first = addEventID;
						currentPair.second = removeEventID;
						_routeLength += (this->getLengthDifference(_route, j, removeEventID, addEventID));
						eventReplaced = true;
						break;
					}
				}
			}
			if (eventReplaced) {
				break;
			}

		}
	}
}

void TwoPhaseAlgorithm::replacePickupEvent(Solution& _s) {
	/// <summary>
	/// Replace the pickup event in place by using the parameter referencing mechanism
	/// </summary>
	/// <param name="_route">List of events in the route</param>
	/// <param name="_routePickupEvent">The ID of pickup event</param>
	
	int pickupEventIndex = this->findPickupEvent(_s.routeEvents, _s.routePickupEvent);
	int rndStartIndex = rand() % pickupEvents.size();
	int removeRoutePickupEvent = _s.routePickupEvent;
	for (int i = rndStartIndex; i < pickupEvents.size() + rndStartIndex; i++) {
		int newRoutePickupEventID = (pickupEvents[rndStartIndex] != _s.routePickupEvent) ? (pickupEvents[rndStartIndex]) :
			(pickupEvents[(rndStartIndex + 1) % pickupEvents.size()]);
		int loadDifference = input.events[newRoutePickupEventID].demand - input.events[_s.routePickupEvent].demand;
		if (_s.routeLoad.second[pickupEventIndex] + loadDifference <= input.vehicleCapacity) {
			_s.routeEvents[pickupEventIndex] = newRoutePickupEventID;
			_s.routePickupEvent = newRoutePickupEventID;
			_s.routeLength += (this->getLengthDifference(_s.routeEvents,pickupEventIndex,removeRoutePickupEvent,newRoutePickupEventID));
			_s.routeLoad.second[pickupEventIndex] += loadDifference;
			break;
		}
	}
}

Solution TwoPhaseAlgorithm::solve() {
	/// <summary>
	/// Solve the Vehicle Routing Problem using two phases: 1-Construction, and 2-Improvement
	/// The generated solution is optimized for visiting maximum possible delivery events with an optimized route length
	/// </summary>
	/// <returns></returns>
	
	Solution current = this->construct(); // Construction phase
	Solution best(current);
	for (int currentIteration = 1; currentIteration <= IMPROVEMENT_PHASE_LENGTH; currentIteration++) { // Improvement phase
		int operatorSelector =1+rand() % 100;
		if (operatorSelector <= TWO_OPT_OPERATOR_PROBABILITY) {
			// Apply TwoOptMove
			current = twoOptMove(current);
		}
		else if (operatorSelector <= REPLACE_DELIVERY_EVENT_PROBABILITY) {
			// Replace delivery event
			this->replaceDeliveryEvent(current.routeEvents,current.routeLength);
		}
		else {
			// Replace pickup event
			this->replacePickupEvent(current);
		}
		if (current.routeLength < best.routeLength) {
			best = Solution(current);
			this->printRoute(current.routeEvents);
			std::cout << current.routeLength << std::endl;
		}
	}
	std::cout << "Number of delivery events: " << best.routeEvents.size()-2 << std::endl;
	std::cout << "Route length: " << best.routeLength << std::endl;
	this->printRoute(best.routeEvents);
	return best;
}