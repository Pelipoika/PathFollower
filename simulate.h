#ifndef _INCLUDE_SIMULATE_H_
#define _INCLUDE_SIMULATE_H_


#include "extension.h"
#include "sp_callbacks.h"
#include "context.h"


extern std::map<int, PFContext> bots;


void SimulateFrame(bool simulating);

void RemoveBot(int idx);


#endif // _INCLUDE_SIMULATE_H_
