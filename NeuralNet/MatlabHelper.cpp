/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
* MatlabHelper.cpp
* Implementation of class MatlabHelper.
*/
#include "Program.h"
#include <string>
#include <cassert>
#include <memory>
#include <engine.h>
using namespace std;

DataSet::DataSet(const char* x_name, const char *y_name)
{
	MatlabHelper& mh = MatlabHelper::open();
	
	size = mh.size(x_name,1);
	assert( size == mh.size(y_name,1) );	// input-output come in pairs

	x_dim = mh.size(x_name,2);
	y_dim = mh.size(y_name,2);

	X = new double[ size * x_dim ];
	Y = new double[ size * y_dim ];

	mh.cpyFromMat(X,x_name);
	mh.cpyFromMat(Y,y_name);
}

DataSet::~DataSet()
{
	delete [] X;
	delete [] Y;
}


void MatlabHelper::cpyToMat(const double *src, const char *name, uint m, uint n)
{
	mxArray *mx = mxCreateDoubleMatrix(n,m,mxREAL);
	memcpy(mxGetPr(mx), src, sizeof(double) * m * n );
	engPutVariable(pEng, name, mx);
	transpose(name);
}

void MatlabHelper::cpyFromMat( double *des, const char *name)
{
	transpose(name);
	mxArray *mx = engGetVariable(pEng, name);
	uint noe = mxGetNumberOfElements(mx);
	uint sz = mxGetElementSize(mx);
	memcpy( des, mxGetPr(mx), mxGetElementSize(mx)*mxGetNumberOfElements(mx) );
	transpose(name);
	mxDestroyArray(mx);
}


void MatlabHelper::clear(const char *names)
{
	if( names == 0 )
		cmd("close all, clear");
	else
	{
		string expr("close all, clear ");
		expr = expr + names;
		cmd(expr.c_str());
	}
}


void MatlabHelper::cmd(const char* s)
{
	engEvalString(pEng,s);
}


void MatlabHelper::load(const char *path)
{
	string expr("load('");
	expr = expr + path + "')";
	cmd(expr.c_str());
}


void MatlabHelper::save(const char *path, const char *names)
{
	string expr("save('");
	if( names == 0 )
		expr = expr + path + + "')";
	else
		expr = expr + path + "'," + names + ")";
	cmd(expr.c_str());
}

void MatlabHelper::transpose(const char* name)
{
	string expr(name);
	expr = expr + '=' + expr + '\'';
	cmd(expr.c_str());
}

uint MatlabHelper::size(const char *name, uint dim)
{
	char v1[] = "var_0";
	char d = '0' + dim;
	string expr(v1);
	expr = expr + "=size(" + name + "," + d + ")";
	cmd(expr.c_str());

	mxArray *mx = engGetVariable(pEng, v1);
	uint sz = (uint)*mxGetPr(mx);
	mxDestroyArray(mx);
	return sz;
}


MatlabHelper& MatlabHelper::open()
{
	if( !mh )
		mh = new MatlabHelper();
	return *mh;
}


void MatlabHelper::close()
{
	if( mh )
	{
		delete mh;
		mh = 0;
	}
}


MatlabHelper* MatlabHelper::mh = 0;
MatlabHelper::MatlabHelper()
{
	pEng = engOpen(0);
	assert(pEng);
}

MatlabHelper::~MatlabHelper()
{
	if(pEng)
		engClose(pEng);
	pEng = 0;
}

#ifdef DEBUG
void test_MatlabHelper()
{
	MatlabHelper& mh = MatlabHelper::open();
	mh.cmd("m_a = 0.5:0.5:4");
	mh.cmd("m_a = vec2mat(m_a,2)");

	// Test size() function
	uint m = mh.size("m_a", 1);
	uint n = mh.size("m_a", 2);
	assert( m == 4 );
	assert( n == 2 );

	double a[8];
	double b[8] = {0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4};
	mh.cpyFromMat(a,"m_a");

	// Check values copied from MATLAB.
	for(uint i = 0; i < 8; ++i)
		assert( a[i] == b[i] );

	// Initialize m_b with local values.
	mh.cpyToMat(b, "m_b", 4, 2);
	// Check size of m_b
	assert( mh.size("m_b", 1) == 4 );
	assert( mh.size("m_b", 2) == 2 );

	mh.cmd("m_a = -m_a");
	mh.cmd("m_c = m_a + m_b");
	double c[8];
	mh.cpyFromMat(c,"m_c");
	// Check results computed with m_b.
	for(uint i = 0; i < 8; ++i)
		assert(c[i] == 0);
	
	mh.save("d:\\m_b","'m_b'");
	mh.clear();
	mh.load("d:\\m_b");
	mh.cmd("delete('d:\\m_b.mat')");
	mh.cpyFromMat(a,"m_b");
	// Check values read from .mat file.
	for(uint i = 0; i < 8; ++i)
		assert(a[i] == b[i]);

	mh.clear();
	mh.cmd( "X = (1:10)'" );
	mh.cmd( "X(:,2) = X(:,1).^2" );
	mh.cmd( "Y = sum(X')'" );
	DataSet ds("X","Y");
	// Check data set.
	assert( ds.size == 10 );
	assert( ds.x_dim == 2 );
	assert( ds.y_dim == 1 );
	for(uint i = 0; i < ds.size; ++i)
		assert( (ds.Y[i] - ds.X[2*i] - ds.X[2*i+1]) == 0 ); 

	// Ploting test.
	mh.cmd("plot(sin(0:0.1:2*pi))");
}
#endif