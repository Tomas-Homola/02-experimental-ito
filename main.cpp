#include "itoStuff.h"
#include <iomanip>

#define SPEED 10.0 // k samotne ulohe je default hodnota 1.0, ale je zaujimave skusit aj nejake vyssie rychlosti a sledovat, ako na uhol s vacsiou rychlostou menej vplyva proces W

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

int main()
{
	srand(time(0));
	double endTime = 500.0;
	int division = 5000;
	
	ItoProcess ip(endTime, division, 100);
	ip.computeTrajectoriesTransform(&g_deg);
	ip.exportData("data100");	
	ip.reset(endTime, division, 1000);

	ip.computeTrajectoriesTransform(&g_deg);
	ip.exportData("data1000");
	
	return 0;
}