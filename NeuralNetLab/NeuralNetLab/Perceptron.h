#pragma once
#include "stdafx.h"

class Perceptron {

private:
	//1D array of perceptron weights
	double * weights;

	double bias;

	int numberOfInputs;

	//learning rate
	double alpha;
	
	//output from the last forward pass
	double output;

	//error from the last backward pass
	double error;

public:
	//constructors
	Perceptron();
	Perceptron(int inputs, double learningRate);
	
	void printWeights();

	int getNumberOfINputs();

	//forward pass of network
	void run(double * inputs);
	void learn(double * inputs, int targetOutput);

	int getOutput();
	double getOutputNatural();
	double getError();


};
