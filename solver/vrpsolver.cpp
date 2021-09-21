#include"../header/input.h"
#include"../header/parameters.h"
#include"../header/two_phase_algorithm.h"

#include<iostream>
#include<string>
#include<algorithm>
#include<vector>

int main(const int argc, const char** argv) {

	if (argc == 1) {
		std::cout << "Input instance was not supplied! Please run again the solver and supplied the instance name (e.g. vrp100).\n";
		return 0;
	}
	else if (argc == 2) {
		std::string fileName = argv[1];
		std::transform(fileName.begin(), fileName.end(), fileName.begin(), std::tolower);
		std::vector<std::string> insanceList{ "toyexample","vrp100","vrp200","vrp400","vrp600","vrp800","vrp1000" };
		auto it = std::find(insanceList.begin(), insanceList.end(), fileName);
		if (it != insanceList.end()) {
			std::string instancePath = "";
			std::string instanceName = instancePath + fileName + ".txt";
			Input input = getInputFromFile(instanceName);
			TwoPhaseAlgorithm alg(input);
			std::cout << "Running... (Note that for large instances, the running time might go up to 2 minutes)" << std::endl;
			Solution s = alg.solve();
			alg.printSolution(s);
			return 0;
		}
		else {
			std::cout << "The name of the instance is not correct! Please run the solver with the correct instance name.\n";
			return 0;
		}
	}
	else {
		std::cout << "Arguments were not supplied correctly! Run the solver by typing the solver name, followed by the instance name.\n";
	}
	return 0;
}
