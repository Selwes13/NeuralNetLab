#pragma once
#include "stdafx.h"
#include "Layer.h"

class NeuralNet {
private:
	//holds neuron values in a jagged array
	double ** neurons;//[layer][neuron]

	//holds the layers that connect each set of neurons
	Layer * layers;

	//neurons in each layer
	int * neuronsPerLayer;

	//number of layers of neurons in the network
	int numberOfLayers;

	//learning coefficient
	double alpha;

	//error from the last back propegation
	double errorSignal;


public:
	NeuralNet(int Layers, int * hiddenSizes, double leaningCoeff);
	NeuralNet(int numberOfInputs, int numberOfOutputs, double leaningCoeff);

	//forward pass of the network
	void runNetwork(double * inputs);

	//get output neuron values
	double* getOutputs();
	int getNumberOfOutputs();

	//calculates the input value for the given neuron
	double Z(int L, int j, int k);

	//get the error signal of the lact back propagation
	double getErrorSignal();
	
	/*Finds the effect of each weight in the network on the output 
	and changes the weight by this value multiplied by the learning coefficient
	*/
	void backPropagate(double * groundTruth);

	//save and load networks to a file
	bool save(std::string filePath);
	static NeuralNet load(std::string filePath);

	//finds the index of the largest value in outs
	static int getStrongestClass(double * outs, int len);

	void setLearningRate(double a);
	double getLearningRate();

	//print network info
	void print();
	//print network info and network map
	void printFull();
	//print only the weights in each layer
	void printLayers();

	//get pointer to layer array 
	Layer* getLayers();

private:
	//calculate the effect the specificed weight has on the output recursivly
	double backPropagate(int layer, int in, int out, double * errors, double * groundTruth);
	double backPropagateBias(int layer, int out, double * errors);
};