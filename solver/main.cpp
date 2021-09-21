#include"../header/input.h"
#include"../header/parameters.h"
#include"../header/two_phase_algorithm.h"

#include<iostream>
#include<string>

const std::string instanceName = "VRP solver/Vehicle-Routing-Problem/input/VRP1000.txt";

int main() {
	Input input = getInputFromFile(instanceName);
	Preprocess prepInput(input);
	TwoPhaseAlgorithm grasp(input);
	grasp.solve();
	return 0;
}