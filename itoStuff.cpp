#include "itoStuff.h"

ItoProcess::ItoProcess(double timeEndValue, unsigned int timeAxisTicks, unsigned int trajectoriesCount)
{
	//std::cout << "ito constructor\n";
	this->timeAxisTicks = timeAxisTicks; // delenie casovej osi
	this->trajectoriesCount = trajectoriesCount; // pocet trajektorii

	this->timeAxis = linspace(0.0, timeEndValue, timeAxisTicks); // vytvorenie casovej osi
	this->wienerSample = new double* [trajectoriesCount]; // alokacia pamati pre vzorku wienerovho procesu
	this->itoTrajectories = new double* [trajectoriesCount]; // alokacia pamati pre trajektorie

	for (size_t i = 0; i < trajectoriesCount; i++)
	{
		this->wienerSample[i] = (double*)calloc(timeAxisTicks, sizeof(double));
		this->itoTrajectories[i] = (double*)calloc(timeAxisTicks, sizeof(double));
	}

	computeWienerSample(); // vypocitanie vzorky trajektorii wienerovho procesu
}
ItoProcess::~ItoProcess()
{
	//std::cout << "wiener destructor\n";
	delete[] this->timeAxis;
	for (size_t i = 0; i < this->trajectoriesCount; i++)
	{
		delete[] this->wienerSample[i];
		delete[] this->itoTrajectories[i];
	}
	delete[] wienerSample;
	delete[] itoTrajectories;
}

void ItoProcess::computeWienerSample()
{
	// Vypocitanie vzorky trajektorii wienerovho procesu
	std::cout << "computing wiener sample ... ";
	for (size_t i = 0; i < this->trajectoriesCount; i++)
	{
		wienerSample[i][0] = 0.0;
		for (size_t j = 1; j < timeAxisTicks; j++)
		{
			wienerSample[i][j] = wienerSample[i][j - 1] + normalDistribution(0.0, timeAxis[j] - timeAxis[j - 1]);
		}
	}
	std::cout << "done\n";
}
void ItoProcess::computeTrajectoriesTransform(double(*transform)(double, double))
{
	double t = 0.0, W = 0.0;

	std::cout << "Computing trajectories by transform ... ";
	for (size_t i = 0; i < trajectoriesCount; i++)
	{
		// Transformacia na Itoov proces
		for (size_t j = 0; j < timeAxisTicks; j++)
		{
			t = timeAxis[j]; // t
			W = wienerSample[i][j]; // W(ω, t)

			itoTrajectories[i][j] = transform(t, W); // g(t, W(ω,t))
		}
	}
	std::cout << "done\n";
}
void ItoProcess::computeTrajectoriesDefinition(double(*drift)(double, double), double(*volatility)(double, double))
{
	double riemann = 0.0, ito = 0.0;
	
	std::cout << "computing itoTrajectories by definition ... ";
	for (size_t i = 0; i < trajectoriesCount; i++)
	{
		// Transformacia na Itoov proces
		itoTrajectories[i][0] = PI / 4;
		for (size_t j = 1; j < timeAxisTicks; j++)
		{
			riemann = NIntegrate_Riemann(j - 1, j, drift, this, i);
			ito = NIntegrate_Ito(j - 1, j, volatility, this, i);

			itoTrajectories[i][j] = itoTrajectories[i][j - 1] + riemann + ito;
			//itoTrajectories[i][0] +
		}
	}
	std::cout << "done\n";
}

void ItoProcess::reset(double timeEndValue, unsigned int timeAxisTicks, unsigned int trajectoriesCount)
{
	// vymazanie starych udajov
	delete[] this->timeAxis; // vymazanie udajov starej casovej osi
	for (size_t i = 0; i < this->trajectoriesCount; i++) // vymazanie starych udajov jednotlivych trajektorii
	{
		delete[] this->wienerSample[i];
		delete[] this->itoTrajectories[i];
	}
	delete[] wienerSample;
	delete[] itoTrajectories; // vymazanie starych trajektorii

	// definovanie novych udajov
	this->timeAxisTicks = timeAxisTicks; // nove delenie casovej osi
	this->trajectoriesCount = trajectoriesCount; // novy pocet trajektorii

	this->timeAxis = linspace(0.0, timeEndValue, timeAxisTicks); // vytvorenie novej casovej osi
	this->wienerSample = new double* [trajectoriesCount];
	this->itoTrajectories = new double* [trajectoriesCount]; // alokacia pamati pre nove trajektorie

	for (size_t i = 0; i < trajectoriesCount; i++)
	{
		this->wienerSample[i] = (double*)calloc(timeAxisTicks, sizeof(double));
		this->itoTrajectories[i] = (double*)calloc(timeAxisTicks, sizeof(double)); // alokacia pamati pre udaje jednotlivych trajektorii
	}

	computeWienerSample(); // vypocitanie novej vzorky trajektorii wienerovho procesu
}
void ItoProcess::clearItoTrajectories()
{
	// vycistenie starych hodnot trajektorii itoovho procesu
	std::cout << "Clearing trajectories ... ";
	for (size_t i = 0; i < trajectoriesCount; i++)
	{
		for (size_t j = 0; j < timeAxisTicks; j++)
		{
			itoTrajectories[i][j] = 0.0;
		}
	}
	std::cout << "done\n";
}

bool ItoProcess::exportData(std::string fileName)
{
	fileName += ".csv";
	std::ofstream exportFile(fileName);
	if (!exportFile.is_open())
		return false;

	std::cout << "Exporting \"" << fileName << "\" ... ";
	for (size_t i = 0; i < timeAxisTicks; i++)
	{
		exportFile << timeAxis[i] << ";";
	}

	exportFile << "\n";

	for (size_t i = 0; i < trajectoriesCount; i++)
	{
		for (size_t j = 0; j < timeAxisTicks; j++)
		{
			exportFile << itoTrajectories[i][j] << ";";
		}
		exportFile << "\n";
	}

	exportFile.close();
	std::cout << "done\n";
	return true;
}
bool ItoProcess::exportWiener(std::string fileName)
{
	fileName += ".csv";
	std::ofstream exportFile(fileName);
	if (!exportFile.is_open())
		return false;

	std::cout << "Exporting \"" << fileName << "\" ... ";
	for (size_t i = 0; i < timeAxisTicks; i++)
	{
		exportFile << timeAxis[i] << ";";
	}

	exportFile << "\n";

	for (size_t i = 0; i < trajectoriesCount; i++)
	{
		for (size_t j = 0; j < timeAxisTicks; j++)
		{
			exportFile << wienerSample[i][j] << ";";
		}
		exportFile << "\n";
	}

	exportFile.close();
	std::cout << "done\n";
	return true;
}
void ItoProcess::printData()
{
	for (size_t i = 0; i < timeAxisTicks; i++)
	{
		std::cout << timeAxis[i] << ",";
	}

	std::cout << "\n";

	for (size_t i = 0; i < trajectoriesCount; i++)
	{
		for (size_t j = 0; j < timeAxisTicks; j++)
		{
			std::cout << itoTrajectories[i][j] << ",";
		}
		std::cout << "\n";
	}
}

double* linspace(double startValue, double endValue, int ticks)
{
	double* temp = new double[ticks];
	double deltaX = (endValue - startValue) / (ticks - 1);

	for (size_t i = 0; i < ticks; i++)
	{
		temp[i] = i * deltaX;
	}

	return temp;
}
double normalDistribution(double mean, double dispersion)
{
	// dve nahodne cisla rovnomerne rozdelene z intervalu [0,1]
	double U1 = (double)rand() / RAND_MAX;
	double U2 = (double)rand() / RAND_MAX;

	if (U1 < EPSILON) // kontrola kvoli definicnemu oboru prirodzeneho logaritmu
		return 0.0;
	else
		return (mean + sqrt(-2.0 * log(U1) * dispersion) * cos(2 * PI * U2));
}

double NIntegrate_Riemann(int a, int b, double(*function)(double, double), ItoProcess* ip, int trajectory)
{
	double result = 0.0;
	double tempLeft = 0.0, tempRight = 0.0;
	double deltaT = 0.0;

	for (size_t i = a; i < b; i++)
	{
		deltaT = ip->timeAxis[i + 1] - ip->timeAxis[i];
		tempLeft = deltaT * (function(0.5*(ip->timeAxis[i + 1] + ip->timeAxis[i]), 0.5*(ip->wienerSample[trajectory][i] + ip->wienerSample[trajectory][i + 1])));
		//tempLeft = deltaT * (function(ip->timeAxis[i], ip->wienerSample[trajectory][i]));
		//tempRight = deltaT * (function(ip->timeAxis[i + 1], ip->wienerSample[trajectory][i + 1]));

		//result += 0.5 * (tempLeft + tempRight);
		result += tempLeft;
		//result += tempRight;
	}
	
	return result;
}
double NIntegrate_Ito(int a, int b, double(*function)(double, double), ItoProcess* ip, int trajectory)
{
	double result = 0.0;
	double deltaW = 0.0;

	for (size_t i = a; i < b; i++)
	{
		deltaW = ip->wienerSample[trajectory][i + 1] - ip->wienerSample[trajectory][i];
		result += function(ip->timeAxis[i], ip->wienerSample[trajectory][i]) * deltaW;
	}

	return result;
}