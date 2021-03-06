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

	return (upper1 / (lower1 * lower1)) + (upper2 / lower2);
}

double volatility(double t, double W)
{
	return (t + 1.0) / (2.0 + 3.0 * t + t * t + 2.0 * sqrt(t + 1.0) * W + W * W);
}

double g(double t, double W)
{
	return (t * sin(W));
}
double d(double t, double W)
{
	return (1.0 - 0.5 * t) * sin(W);
}
double v(double t, double W)
{
	return (t* cos(W));
}

double g1(double t, double W)
{
	return atan(1 + W);
}

double d1(double t, double W)
{
	return -(1 + W) / ((1 + (1 + W) * (1 + W)) * (1 + (1 + W) * (1 + W)));
}

double v1(double t, double W)
{
	return 1.0 / (1.0 + (1 + W) * (1 + W));
}

int main()
{
	srand(time(0));
	double endTime = 1000.0;
	int division = 10001;
	
	ItoProcess ip(endTime, division, 6); // vytvorenie objektu pre itoov proces
	
	//#################### 100 trajektorii ####################//
	ip.computeTrajectoriesTransform(&g_rad); // vypocet cez transformaciu wienerovho procesu
	ip.exportData("data100_transf"); // export udajov
	//ip.clearItoTrajectories(); // vycistenie trajektorii, aby sa dali vypocitat nove, mozno ani netreba?
	
	//ip.computeTrajectoriesDefinition(&drift, &volatility); // vypocet cez definiciu itoovho procesu
	ip.computeTrajectoriesDefinition(&drift, &volatility); // vypocet cez definiciu itoovho procesu
	ip.exportData("data100_def");	// export udajov
	ip.exportWiener("data_Wiener");
	//ip.reset(endTime, division, 1000); // zresetovanie objektu pre itoov proces
	//
	////#################### 1000 trajektorii ####################//
	//ip.computeTrajectoriesTransform(&g_rad); // vypocet cez transformaciu wienerovho procesu
	//ip.exportData("data1000_transf"); // export udajov
	//
	//ip.computeTrajectoriesDefinition(&drift, &volatility); // vypocet cez definiciu itoovho procesu
	//ip.exportData("data1000_def"); // export udajov
	
	return 0;
}