#include"../header/input.h"
#include"../header/parameters.h"
#include"../header//grasp.h"

#include<iostream>
#include<string>

const std::string instanceName = "VRP solver/Vehicle-Routing-Problem/input/Toy example.txt";

int main() {
	Input input = getInputFromFile(instanceName);
	int pickupEventID = 0;
	Preprocess p(input, pickupEventID);
	return 0;
}