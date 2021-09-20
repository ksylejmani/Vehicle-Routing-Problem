#include"../header/grasp.h"
#include"../header/parameters.h"
#include"../header/preprocess.h"

#include<math.h>
#include<vector>
#include<iostream>
#include<random>
#include<algorithm>


GRASP::GRASP(const Input& _input) :
	Preprocess(_input){
	//input = _input;
	srand(SEED);
	deliveryRestrictedCandidateList = this->getDeliveryRestrictedCandidateList();
	pickupRestrictedCandidateList =	this->getPickupRestrictedCandidateList();

}

std::vector<std::pair<double, std::pair<int, int>>> GRASP::getDeliveryRestrictedCandidateList() {
	std::vector<std::pair<double, std::pair<int, int>>> ans;
	unsigned int numberOfCandidates = (int)(PERCENTAGE_OF_COMPONENTS * deliveryDistanceComponents.size());
	unsigned int countCandidates = 0;
	for (auto it = deliveryDistanceComponents.begin(); it != deliveryDistanceComponents.end(); it++) {
		ans.push_back(*it);
		countCandidates++;
		if (countCandidates >= numberOfCandidates)
			break;
	}
	
	return ans;
}

std::vector<std::pair<double, std::pair<int, int>>> GRASP::getPickupRestrictedCandidateList() {
	std::vector<std::pair<double, std::pair<int, int>>> ans;
	unsigned int numberOfCandidates = (int)(PERCENTAGE_OF_COMPONENTS * pickupDistanceComponents.size());
	unsigned int countCandidates = 0;
	for (auto it = pickupDistanceComponents.begin(); it != pickupDistanceComponents.end(); it++) {
		ans.push_back(*it);
		countCandidates++;
		if (countCandidates >= numberOfCandidates)
			break;
	}
	return ans;
}

std::pair<double, std::pair<int, int>> GRASP::selectDeliveryComponent(
	std::vector<std::pair<double, std::pair<int,int>>>& restrictedCandidateList) {
	std::pair<double, std::pair<int, int>> ans;
	int randomIndex = rand() % restrictedCandidateList.size();
	ans = restrictedCandidateList[randomIndex];
	return ans;
}

std::pair<double, std::pair<int, int>> GRASP::selectPickupComponent(
	const std::vector<std::pair<double, std::pair<int, int>>>& restrictedCandidateList) {
	std::pair<double, std::pair<int, int>> ans;
	int randomIndex = rand() % restrictedCandidateList.size();
	ans = restrictedCandidateList[randomIndex];
	return ans;
}



double GRASP::calculateRouteLength(const std::vector<int>& routeEvents){
	double ans = 0;
	for (unsigned int i = 0; i < routeEvents.size()-1; i++) {
		ans += (getDistance(routeEvents[i], routeEvents[i + 1]));
	}
	ans += (getDistance(routeEvents[routeEvents.size()-1], routeEvents[0]));
	return ans;
}

int GRASP::getStartPointIndex(const std::vector<int>& route) {
	for (int i = 0; i < route.size(); i++) {
		if (route[i] == -1) {
			return i;
		}
	}
}
int GRASP::findPickupEvent(const std::vector<int>& route, const int & pickupEventID) {
	for (int i = 0; i < route.size(); i++) {
		if (route[i] == pickupEventID) {
			return i;
		}
	}
}

// This first value in the pair returns whether the route is valid, whereas the second returns route load in specific events
std::pair<bool, std::vector<int>> GRASP::calculateRouteLoad(const std::vector<int>& routeEvents, const int& routePickupEvnet) {
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

Solution GRASP::construct() {
	Solution ans ;

	// Insert delivery events based on their demand in non-decreasing order
	for (auto it = primaryEvents.begin(); it != primaryEvents.end(); it++) {
		ans.routeEvents.push_back(*it);
	}
	
	// Randomly shuffle solution
	std::shuffle(ans.routeEvents.begin(), ans.routeEvents.end(), std::default_random_engine(SEED));

	// Insert start point at the begining and a pickup event (selected at random at the end of the route)
	// so that the route is guaranted to be feasible (in terms of vehicle capacity)
	ans.routeEvents.insert(ans.routeEvents.begin(),-1); // The start point is labeled with an index of -1
	int rndPickupIndex = rand() % pickupEvents.size();
	ans.routePickupEvent = pickupEvents[rndPickupIndex];
	ans.routeEvents.push_back(ans.routePickupEvent);

	// Mark events that are not in the route, but could be added
	for (auto it = substitutingEvents.begin(); it != substitutingEvents.end(); it++) {
		ans.nonRouteEvents.push_back(*it);
	}

	// Calculate route length, load and its feasiblity
	ans.routeLength = this->calculateRouteLength(ans.routeEvents);
	ans.routeLoad = this->calculateRouteLoad(ans.routeEvents, ans.routePickupEvent);
	return ans;
}

void GRASP::printRoute(std::vector<int> route) {
	for (int i = 0; i < route.size(); i++) {
		std::cout << route[i] << " ";
	}
	std::cout << std::endl;
}

void GRASP::printDistances(std::vector<int> route) {
	for (int i = 0; i < route.size() - 1; i++) {
		std::cout << getDistance(route[i], route[i + 1]) << " ";
	}
	std::cout << getDistance(route[route.size() - 1], route[0]);
	std::cout << std::endl;
}

Solution GRASP::_2OptMove(const Solution& s) {
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
	return ans;
}
void GRASP::reverseRouteSegement(std::vector<int>& routeEvents, const int& startIndex, const int& endIndex) {
	for (int i = startIndex; i < endIndex; i++) {
		int temp = routeEvents[i];
		routeEvents[i] = routeEvents[endIndex - (i - startIndex)];
		routeEvents[endIndex - (i - startIndex)] = temp;
	}
}

Solution GRASP::solve() {
	Solution current = this->construct();
	Solution best(current);
	for (int currentIteration = 1; currentIteration <= HILL_CLIMBING_LENGTH; currentIteration++) {
		int replaceEventSelector =1+rand() % 100;
		if (replaceEventSelector <= REPLACE_EVENT_PROBABLITY) {
			// Replace delivery event
			int rndIndex = rand() % substitutingPairs.size();
			for (int i = rndIndex; i < substitutingPairs.size() + rndIndex; i++) {
				std::pair<int, int> currentPair = substitutingPairs[rndIndex % substitutingPairs.size()];
				bool eventReplaced = false;
				for (int j = 0; j < current.routeEvents.size(); j++) {
					if (current.routeEvents[j] == currentPair.first) {
						auto it = std::find(current.routeEvents.begin(),
							current.routeEvents.end(), currentPair.second);
						if (it == current.routeEvents.end()) {
							int temp = current.routeEvents[j];
							current.routeEvents[j] = currentPair.second;
							currentPair.first = currentPair.second;
							currentPair.second = temp;
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
		else {
			// Replace pickup event
			int pickupEventIndex = this->findPickupEvent(current.routeEvents, current.routePickupEvent);
			int rndIndex = rand() % pickupEvents.size();
			current.routePickupEvent = (pickupEvents[rndIndex] != current.routePickupEvent) ? (pickupEvents[rndIndex]) :
				(pickupEvents[(rndIndex + 1) % pickupEvents.size()]);
			current.routeEvents[pickupEventIndex] = current.routePickupEvent;
		}
		// Apply 2OptMove by using the best improvming version
		this->printRoute(current.routeEvents);
		current = _2OptMove(current);
		if (current.routeLoad.first && current.routeLength < best.routeLength) {
			best = Solution(current);
			std::cout << current.routeLength << std::endl;
		}
	}
	std::cout << "Number of delivery events: " << best.routeEvents.size()-2 << std::endl;
	std::cout << "Best route length: " << best.routeLength << std::endl;
	return best;
}

