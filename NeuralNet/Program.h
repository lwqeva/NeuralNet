/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
* Program.h
* Header file for framework modules of the program. Modules declared in this 
* header provide aiding methods for the program itself, which is independent of
* the implementation of specific algorithm.
*/
#pragma once
#include <engine.h>

#define DEBUG 1
typedef unsigned int uint;

/* JobManager
* Each job shall be registered with JobManager, and wait to be launched in 
* JobManager's job list.
*/
class JobManager
{
public:

	// Maximum number of jobs allowed in the list.
	static const uint MAX_JOBS = 64;

	// Append new job and its parameter pointer to the job list.
	bool append(void (*job)(void*), void *param=0);
	// Start to run jobs on the list.
	void launch();

	// Static method to provide reference to the Global JobManager.
	static JobManager& getReference();
	
private:
	JobManager();   // Only one global JobManager is allowed.
	static JobManager *jm;

	void (*jobs[MAX_JOBS])(void*);  // Job list.
	void *params[MAX_JOBS];			// Parameter list.
	uint job_in_queue;    // counter for jobs in the list.
};


/* DataSet
* DataSet object is used to store sample data for training. The data loading logic
* relies on MatlabHelper.
*/
class DataSet
{
public:
	// Constructor load data from MatlabHelper. x_name and y_name are identifiers
	// of variables in MATLAB.
	DataSet(const char* x_name, const char *y_name);
	// Destructor will release memory of X and Y.
	~DataSet();
	double *X;
	double *Y;
	uint size;		// Number of samples.
	uint x_dim;		// Length of input vector.
	uint y_dim;		// Length of output vector.
};


class MatlabHelper
{
public:
	// Copy the value a local array to MATLAB array.
	void cpyToMat(const double *src, const char *name, uint m, uint n=1);
	// Copy the value of MATLAB array to local array.
	// The destination memory shall be alocated in advance.
	void cpyFromMat(double *des, const char *name);
	// Clear variables and figures.
	void clear(const char *names=0);
	// Execute MATLAB command.
	void cmd(const char *s);
	// Load .mat data file.
	void load(const char *path);
	// Save MATLAB variables to target file.
	void save(const char *path, const char *names=0);
	// Transpose a matrix.
	void transpose(const char* name);
	// Get matrix size.
	uint size(const char *name, uint dim);
	// Get reference to MATLAB session.
	static MatlabHelper& open();
	// Close MATLAB session.
	static void close();
private:
	MatlabHelper();
	~MatlabHelper();
	static MatlabHelper* mh;  // pointer to MATLAB session.

	Engine *pEng;   // pointer to MATLAB engin.
};



/* Test cases for JobManager, MatlabHelper, DataSet
*/
#ifdef DEBUG
void test_JobManager();
void test_MatlabHelper();
#endif