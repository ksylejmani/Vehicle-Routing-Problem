#pragma once
#include<ctime>
// Algorithm parameters
const double PERCENTAGE_OF_COMPONENTS=0.7;
const int IMPROVEMENT_PHASE_LENGTH =10000;
const int TWO_OPT_OPERATOR_PROBABILITY = 50;
const int REPLACE_DELIVERY_EVENT_PROBABILITY = 10;
const int REPLACE_PICKUP_EVENT_PROBABILITY = 40;

// Code related parameters
const unsigned int SEED = time(0);