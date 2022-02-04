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
	for (size_t i = 0; i < trajectoriesCount; i++)
	{
		trajectories[i][0] = normalDistribution(0.0, timeAxis[0] - 0.0);
		for (size_t j = 1; j < timeAxisTicks; j++)
		{
			trajectories[i][j] = trajectories[i][j - 1] + normalDistribution(0.0, timeAxis[j] - timeAxis[j - 1]);
		}
		//std::cout << i << "done\n";
	}
	std::cout << "computing trajectories done\n";
}

void ItoProcess::computeTrajectoriesDefinition(double(*drift)(double, double), double(*volatility)(double, double))
{

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

double NIntegrate_Riemann(double a, double b, double(*function)(double), int n)
{
	double result = 0.0;
	int temp = n - 1;
	double deltaX = (b - a) / n;

	result += function(a); // f(x_0)
	for (size_t i = 1; i <= temp; i++)
	{
		result += 2 * function(a + (i * deltaX)); // f(x_1), ..., f(x_{n-1}
	}
	result += function(b); // f(x_n)
	result = (deltaX / 2) * result;
	
	return result;
}

double NIntegrate_Ito(double a, double b, double(*function)(double, double), ItoProcess* ip)
{
	double result = 0.0;

	return result;
}
