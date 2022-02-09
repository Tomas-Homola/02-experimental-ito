#pragma once

#define EPSILON 0.000000001
#define PI 3.14159265358979323846

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

class ItoProcess
{
private:

public:
	unsigned int timeAxisTicks = 0; // pocet bodov na casovej osi
	double* timeAxis = nullptr; // pole na ukladanie hodnot pre jednotlive casy

	unsigned int trajectoriesCount = 0; // pocet trajektorii
	double** trajectories = nullptr; // pole, kde su ulozene jednotlive trajektorie

	ItoProcess(double timeEndValue, unsigned int timeAxisTicks, unsigned int trajectoriesCount);
	~ItoProcess();

	void computeTrajectoriesTransform(double (*transform)(double, double)); // vypocitanie trajektorii pomocou transformacie Wienerovho procesu
	void computeTrajectoriesDefinition(double (*drift)(double, double), double(*volatility)(double, double)); // vypocitanie trajektorii pomocou definicie s integralmi
	void reset(double timeEndValue, unsigned int timeAxisTicks, unsigned int trajectoriesCount);

	bool exportData(std::string fileName);
	void printData();
};

double* linspace(double startValue, double endValue, int ticks); // funkcia na vytvorenie delenia casovej osi, inspirovane funkciou "linspace" z Matlabu
double normalDistribution(double mean, double dispersion); // funkcia na generovanie cisel z normalneho rozdelenia N(mean, dispersion), pouzity Box-Muller Transform, zdroj: https://www.youtube.com/watch?v=EXsdT91XFAY&t=194s
double NIntegrate_Riemann(int a, int b, double (*function)(double, double), ItoProcess* ip, int trajectory);
double NIntegrate_Ito(int a, int b, double (*function)(double, double), ItoProcess* ip, int trajectory);