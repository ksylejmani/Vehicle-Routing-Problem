#include"../header/input.h"
#include"../header/parameters.h"
#include"../header//grasp.h"

#include<iostream>
#include<string>

const std::string instanceName = "VRP solver/Vehicle-Routing-Problem/input/Events1000.txt";

int main() {
	Input input = getInputFromFile(instanceName);
	Preprocess prepInput(input);
	GRASP grasp(input);
	grasp.solve();
	return 0;
}