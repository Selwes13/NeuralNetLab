#include "stdafx.h"
#include "Perceptron.h"
#include <stdlib.h> //rand()
#include <iostream>//cout

//constructors
Perceptron::Perceptron() {
	numberOfInputs = 2;
	output = 0;
	bias = 0;
	alpha = 0.1;
	weights = new double[numberOfInputs];

	for (int i = 0; i < numberOfInputs; i++)
		weights[i] = (double)(rand() % 100 + 1) / 100;
}

Perceptron::Perceptron(int inputs, double learningRate) {
	numberOfInputs = inputs;
	output = 0;
	bias = 0;
	alpha = learningRate;
	weights = new double[numberOfInputs];

	for (int i = 0; i < numberOfInputs; i++)
		weights[i] = (double)(rand() % 100 + 1) / 100;
}

void Perceptron::printWeights() {
	std::cout << "---Weights---" << std::endl;
	for (int i = 0; i < numberOfInputs; i++)
		std::cout << "w" << i << " = " << weights[i] << std::endl;
	std::cout << "Bias = " << bias << std::endl;
}

int Perceptron::getNumberOfINputs() { return numberOfInputs; }

//forward pass of network
void Perceptron::run(double * inputs) {
	//resets the stored output and calculates the new output
	output = 0;

	//sum of the inputs times their respective weights
	for (int i = 0; i < numberOfInputs; i++) {
		output += weights[i] * inputs[i];
	}

	output += bias;

}

void Perceptron::learn(double * inputs, int targetOutput) {
	error = targetOutput - this->getOutput();
	
	//perfoems the adjustment to each weight based on the input and output provided 
	for (int i = 0; i < numberOfInputs; i++) 
		//w(t+1) = w(t) + (desiredOutput - output) * input
		weights[i] += error * inputs[i] * alpha;

	bias += error * alpha;

	
}

int Perceptron::getOutput() {
	if (output > 0)
		return 1;
	else
		return 0;
}

double Perceptron::getOutputNatural() {
	return output;
}

double Perceptron::getError() {
	return error;
}