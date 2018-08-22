#pragma once
#include "stdafx.h"

class Layer {

private:
	//2D array containing connection weights for this layer
	double ** weights;
	//number of inputs
	int inputs;
	//number of outputs
	int outputs;
	//stores the bias for each neuron bias (none for inputs)
	double * biases;



public:
	//--Functions--
	//constructor
	Layer();
	Layer(int numberOfInputs, int numberOfOutputs);
	Layer(int numberOfInputs, int numberOfOutputs, double** weightVals);
	Layer(int numberOfInputs, int numberOfOutputs, double** weightVals, double * biasVals);

	//displays the layer on the console
	void print();
	double ** getWeights();
	double getWeight(int in, int out);
	void modifyWeight(int in, int out, double val);

	double * getBiases();
	double getBias(int out);
	void modifyBias(int out, double val);

	int getNumberInputs();
	int getNumberOutputs();
	double getWeights(int in, int out);

	double* forwardPass(double * inputVals);
	double* sigmoidForwardPass(double * inputVals);

	static double sigmoid(double x);
	static double sigmoidDeriv(double x);

};