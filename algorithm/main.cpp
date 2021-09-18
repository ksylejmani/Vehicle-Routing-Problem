#include"../header/input.h"
#include"../header/parameters.h"
#include"../header//grasp.h"

#include<iostream>
#include<string>

const std::string instanceName = "VRP solver/input/Toy example.txt";

int main() {
	Input input = getInputFromFile(instanceName);
	int pickupEventID = 0;
	GRASP grasp(input, percentageOfComponents, hillClimbingLength, pickupEventID);
	Solution s = grasp.construct();
	return 0;
}