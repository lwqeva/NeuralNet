/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
* JobManager.cpp
* Implementation of class JobManager.
*/
#include "Program.h"
#include <cassert>
#include <iostream>
#include <ctime>
using namespace std;


bool JobManager::append(void (*job)(void*), void *param)
{
	if( job_in_queue == MAX_JOBS )
		return false;
	jobs[job_in_queue] = job;
	params[job_in_queue++] = param;
	return true;
}


void JobManager::launch()
{
	system("cls");

	time_t rawtime;
	time(&rawtime);
	struct tm *timeinfo = localtime(&rawtime);

	cout<< "JobManager lauched :" << asctime(timeinfo) <<endl;
	for(uint i = 0; i < job_in_queue; ++i)
	{
		cout<<"Job "<< i << " running...";

		(*jobs[i])(params[i]);

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		cout<<'\r'<<"Job " << i << " completed! :"<< asctime(timeinfo);
	}
	cout<< "\nAll jobs are completed!"<<endl;
	job_in_queue = 0;   // Clear job list.
}

JobManager::JobManager()
{
	job_in_queue = 0;
}


JobManager* JobManager::jm = 0;
JobManager& JobManager::getReference()
{
	if( !jm )
		jm = new JobManager;
	return *jm;
}



#ifdef DEBUG
uint gList[10];
void test_assign_value(void *param)
{
	uint i = *(uint*)param;
	gList[i] = 9 - i;
}

void test_JobManager()
{
	JobManager& jm = JobManager::getReference();
	uint params[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	for(uint i = 0; i < 10; ++i)
		jm.append(test_assign_value,params+i);

	jm.launch();

	for(uint i = 0; i < 10; ++i)
		assert( gList[i] == params[9-i] );
}
#endif