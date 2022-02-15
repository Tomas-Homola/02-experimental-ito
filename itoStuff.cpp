#include "itoStuff.h"

ItoProcess::ItoProcess(double timeEndValue, unsigned int timeAxisTicks, unsigned int trajectoriesCount)
{
	//std::cout << "ito constructor\n";
	this->timeAxisTicks = timeAxisTicks; // delenie casovej osi
	this->trajectoriesCount = trajectoriesCount; // pocet trajektorii

	this->timeAxis = linspace(0.0, timeEndValue, timeAxisTicks); // vytvorenie casovej osi
	this->trajectories = new double* [trajectoriesCount]; // alokacia pamati pre trajektorie

	for (size_t i = 0; i < trajectoriesCount; i++)
	{
		//this->trajectories[i] = new double[timeAxisTicks]; // alokacia pamati pre jednotlive trajektorie
		this->trajectories[i] = (double*)calloc(timeAxisTicks, sizeof(double));
	}
}

ItoProcess::~ItoProcess()
{
	//std::cout << "wiener destructor\n";
	delete[] this->timeAxis;
	for (size_t i = 0; i < this->trajectoriesCount; i++)
	{
		delete[] this->trajectories[i];
	}
	delete[] trajectories;
}

void ItoProcess::computeTrajectoriesTransform(double(*transform)(double, double))
{
	double t = 0.0, W = 0.0;

	for (size_t i = 0; i < trajectoriesCount; i++)
	{
		// Vypocitanie trajektorie wienerovho procesu
		trajectories[i][0] = normalDistribution(0.0, timeAxis[0] - 0.0);
		for (size_t j = 1; j < timeAxisTicks; j++)
		{
			trajectories[i][j] = trajectories[i][j - 1] + normalDistribution(0.0, timeAxis[j] - timeAxis[j - 1]);
		}

		// Transformacia na Itoov proces
		for (size_t j = 0; j < timeAxisTicks; j++)
		{
			t = timeAxis[j];
			W = trajectories[i][j];

			trajectories[i][j] = transform(t, W);
		}
		//std::cout << i << "done\n";
	}
	std::cout << "computing trajectories done\n";
}

void ItoProcess::computeTrajectoriesDefinition(double(*drift)(double, double), double(*volatility)(double, double))
{
	double riemann = 0.0, ito = 0.0;
	
	std::cout << "computing trajectories ... ";
	for (size_t i = 0; i < trajectoriesCount; i++)
	{
		// Vypocitanie trajektorie wienerovho procesu
		trajectories[i][0] = normalDistribution(0.0, timeAxis[0] - 0.0);
		for (size_t j = 1; j < timeAxisTicks; j++)
		{
			trajectories[i][j] = trajectories[i][j - 1] + normalDistribution(0.0, timeAxis[j] - timeAxis[j - 1]);
		}

		// Transformacia na Itoov proces
		trajectories[i][0] = PI / 4;
		for (size_t j = 1; j < timeAxisTicks; j++)
		{
			riemann = NIntegrate_Riemann(0, j, drift, this, i);
			ito = NIntegrate_Ito(0, j, volatility, this, i);
			trajectories[i][j] = trajectories[i][0] + riemann + ito;
		}
	}
	std::cout << " done\n";
}

void ItoProcess::reset(double timeEndValue, unsigned int timeAxisTicks, unsigned int trajectoriesCount)
{
	// vymazanie starych udajov
	delete[] this->timeAxis; // vymazanie udajov starej casovej osi
	for (size_t i = 0; i < this->trajectoriesCount; i++) // vymazanie starych udajov jednotlivych trajektorii
	{
		delete[] this->trajectories[i];
	}
	delete[] trajectories; // vymazanie starych trajektorii

	// definovanie novych udajov
	this->timeAxisTicks = timeAxisTicks; // nove delenie casovej osi
	this->trajectoriesCount = trajectoriesCount; // novy pocet trajektorii

	this->timeAxis = linspace(0.0, timeEndValue, timeAxisTicks); // vytvorenie novej casovej osi
	this->trajectories = new double* [trajectoriesCount]; // alokacia pamati pre nove trajektorie

	for (size_t i = 0; i < trajectoriesCount; i++)
	{
		this->trajectories[i] = (double*)calloc(timeAxisTicks, sizeof(double)); // alokacia pamati pre udaje jednotlivych trajektorii
	}
}
bool ItoProcess::exportData(std::string fileName)
{
	std::cout << "Exporting ...";
	fileName += ".csv";
	std::ofstream exportFile(fileName);
	if (!exportFile.is_open())
		return false;

	for (size_t i = 0; i < timeAxisTicks; i++)
	{
		exportFile << timeAxis[i] << ";";
	}

	exportFile << "\n";

	for (size_t i = 0; i < trajectoriesCount; i++)
	{
		for (size_t j = 0; j < timeAxisTicks; j++)
		{
			exportFile << trajectories[i][j] << ";";
		}
		exportFile << "\n";
	}

	exportFile.close();
	std::cout << " export done\n";
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
			std::cout << trajectories[i][j] << ",";
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
		tempLeft = deltaT * (function(ip->timeAxis[i], ip->trajectories[trajectory][i]));
		tempRight = deltaT * (function(ip->timeAxis[i + 1], ip->trajectories[trajectory][i + 1]));

		result += 0.5 * (tempLeft + tempRight);
	}
	
	return result;
}
double NIntegrate_Ito(int a, int b, double(*function)(double, double), ItoProcess* ip, int trajectory)
{
	double result = 0.0;
	double deltaW = 0.0;

	for (size_t i = a; i < b; i++)
	{
		deltaW = ip->trajectories[trajectory][i + 1] - ip->trajectories[trajectory][i];
		result += function(ip->timeAxis[i], ip->trajectories[trajectory][i]) * deltaW;
	}

	return result;
}