#include "itoStuff.h"
#include <iomanip>

using std::cout;
using std::endl;

double f(double x)
{
	return (cos(x) * sin(x));
}

int main()
{
	//srand(time(0));
	//double endTime = 3.0;
	//int n = 100, division = 601;

	double result = NIntegrate_Riemann(0.0, PI/2, &f, 50);
	cout << "result: " << std::setprecision(15) << result << endl;;
	return 0;
}