#pragma once

#include <iostream>
#include <map>
#include <sstream>
#include <cmath>
#include <cassert>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <NTL/RR.h>

using namespace NTL;

static long ENCRYPTION = 0;
static long MULTIPLICATION  = 1;
static long CONJUGATION = 2;


static RR const Pi = ComputePi_RR();


