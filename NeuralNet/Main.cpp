#include "Program.h"
#include <iostream>

#pragma comment( lib, "libmx.lib" )
#pragma comment( lib, "libmex.lib" )
#pragma comment( lib, "libeng.lib" )


int main()
{
	// Initialize MATLAB session.
	MatlabHelper::open();

#ifdef DEBUG
	// Run test cases.
	test_JobManager();
	std::cout<< "\nJobManager test case clear.\n";
	test_MatlabHelper();
	std::cout<< "\nMatlabHelper test case clear.\n";
#endif

	system("pause");
	// Close Matlab session.
	MatlabHelper::close();
	return 0;
}
