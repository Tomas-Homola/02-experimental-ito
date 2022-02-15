#include "itoStuff.h"
#include <iomanip>

#define SPEED 1.0 // k samotne ulohe je default hodnota 1.0, ale je zaujimave skusit aj nejake vyssie rychlosti a sledovat, ako na uhol s vacsiou rychlostou menej vplyva proces W

using std::cout;
using std::endl;

double g_rad(double t, double W)
{
	return atan((sqrt(SPEED * t + 1) + W) / (SPEED * t + 1));
}

double g_deg(double t, double W)
{
	return (atan((sqrt(SPEED * t + 1) + W) / (SPEED * t + 1)) / PI) * 180.0;
}

double drift(double t, double W)
{
	double upper1 = -((1.0 + t) * (sqrt(1.0 + t) + W));
	double lower1 = (2.0 + 3.0 * t + t * t + 2.0 * sqrt(1.0 + t) * W + W * W);
	double upper2 = -(1.0 + t + 2.0 * sqrt(1.0 + t) * W);
	double lower2 = (2.0 * sqrt(1.0 + t) * (2.0 + 3.0 * t + t * t + 2.0 * sqrt(1.0 + t) * W + W * W));

	return (upper1 / (lower1 * lower1)) + 0.5 * (upper2 / lower2);
}

double volatility(double t, double W)
{
	return (t + 1.0) / (2.0 + 3.0 * t + t * t + 2.0 * sqrt(t + 1.0) * W + W * W);
}

int main()
{
	srand(time(0));
	double endTime = 1000.0;
	int division = 1001;
	
	ItoProcess ip(endTime, division, 100);
	//ip.computeTrajectoriesTransform(&g_deg);
	ip.computeTrajectoriesDefinition(&drift, &volatility);
	ip.exportData("data100_def");	
	ip.reset(endTime, division, 1000);
	
	//ip.computeTrajectoriesTransform(&g_deg);
	ip.computeTrajectoriesDefinition(&drift, &volatility);
	ip.exportData("data1000_def");
	
	return 0;
}