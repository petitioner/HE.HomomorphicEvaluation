#include <NTL/BasicThreadPool.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <algorithm>

#include "MyMethods.h"
#include "MyTools.h"

using namespace std;
using namespace NTL;


int main(int argc, char **argv) {

	SetNumThreads(8);

		//MyMethods::testSine();

		//MyMethods::Sigmoid28();

		/////////////////MyMethods::Sigmoid100();//failed 

		MyMethods::NNover30();

		//MyMethods::NNover50();

		//MyMethods::NNover70();

	
	

	cout << endl << "END OF THE PROGRAMM" << endl;
	return 0;
}
