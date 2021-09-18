#include"../header/grasp.h"
#include"../header/parameters.h"
#include"../header/preprocess.h"

#include<math.h>
#include<vector>
#include<iostream>
#include<random>
#include<algorithm>


GRASP::GRASP(Input _input, double _percentageOfComponents, int _hillClimbingLength, int _pickupEventID):
	input(_input), percentageOfComponents(_percentageOfComponents), hillClimbingLength(_hillClimbingLength),
	pickupEventID(_pickupEventID){
	srand(seed); 

}

std::vector<std::pair<double, std::pair<int, int>>> GRASP::getDeliveryRestrictedCandidateList() {
	std::vector<std::pair<double, std::pair<int, int>>> ans;
	//unsigned int numberOfCandidates = (int)(percentageOfComponents * deliveryComponents.size());
	//unsigned int countCandidates = 0;
	//for (auto it = deliveryComponents.begin(); it != deliveryComponents.end(); it++) {
	//	ans.push_back(*it);
	//	countCandidates++;
	//	if (countCandidates >= numberOfCandidates)
	//		break;
	//}
	return ans;
}

std::vector<std::pair<double, int>> GRASP::getPickupRestrictedCandidateList() {
	std::vector<std::pair<double, int>> ans;
	//unsigned int numberOfCandidates = (int)(percentageOfComponents * pickupComponents.size());
	//unsigned int countCandidates = 0;
	//for (auto it = pickupComponents.begin(); it != pickupComponents.end(); it++) {
	//	ans.push_back(*it);
	//	countCandidates++;
	//	if (countCandidates >= numberOfCandidates)
	//		break;
	//}
	return ans;
}

std::pair<double, std::pair<int, int>> GRASP::selectDeliveryComponent(std::vector<std::pair<double, std::pair<int,
	int>>>& restrictedCandidateList) {
	std::pair<double, std::pair<int, int>> ans;
	int randomIndex = rand() % restrictedCandidateList.size();
	ans = restrictedCandidateList[randomIndex];
	return ans;
}

std::pair<double, int> GRASP::selectPickupComponent(std::vector<std::pair<double, int>>& restrictedCandidateList) {
	std::pair<double, int> ans;
	int randomIndex = rand() % restrictedCandidateList.size();
	ans = restrictedCandidateList[randomIndex];
	return ans;
}

double GRASP::calculateRouteLength(const std::vector<int>& routeEvents){
	double ans = 0;
	//ans += (this->startEndDistances[routeEvents[0]]);
	//for (int i = 1; i < routeEvents.size()-1; i++) {
	//	ans += (getDistance(routeEvents[i], routeEvents[i + 1]));
	//}
	//ans += (this->pickupDistances[routeEvents[routeEvents.size() - 1]]);
	//ans += (this->startEndDistances[encodedPickupEventID]);
	return ans;
}



Solution GRASP::construct() {
	Solution current ;
	std::vector<std::pair<double, std::pair<int, int>>> deliveryRestrictedCandidateList=this->getDeliveryRestrictedCandidateList();

	return current;
}
