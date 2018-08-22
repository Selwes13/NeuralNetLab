#include "stdafx.h"
#include "Layer.h"
#include <stdlib.h> //rand()
#include <iostream>//cout

//Layer Class
Layer::Layer() {
	inputs = 0;
	outputs = 0;

}

//construct a new layer from scratch
Layer::Layer(int numberOfInputs, int numberOfOutputs) {
	inputs = numberOfInputs;
	outputs = numberOfOutputs;

	//initialises the bias array
	biases = new double[outputs];

	//creates weight array of length outputs (then points to inputs)
	weights = new double * [outputs];

	//fills the weights array with random values between 0 and 1
	for (int out = 0; out < outputs; out++) {
		//declairs this row
		weights[out] = new double[inputs];
		
		//sets the bias value to 0
		biases[out] = 0;

		for (int in = 0; in < inputs; in++) {
			weights[out][in] = (double)(rand()%100 + 1)/100;
		}
	}

}

//construct a layer with given weights
Layer::Layer(int numberOfInputs, int numberOfOutputs, double** weightVals) {
	inputs = numberOfInputs;
	outputs = numberOfOutputs;

	//initialises the bias array
	biases = new double[outputs];

	//creates weight array of length outputs (then points to inputs)
	weights = new double *[outputs];

	//fills the given weights
	for (int out = 0; out < outputs; out++) {
		//declairs this row
		weights[out] = new double[inputs];

		//sets the bias value to 0
		biases[out] = 0;

		for (int in = 0; in < inputs; in++) {
			weights[out][in] = weightVals[out][in];
		}
	}

}

//construct a layer with given weights and biases
Layer::Layer(int numberOfInputs, int numberOfOutputs, double** weightVals, double * biasVals) {
	inputs = numberOfInputs;
	outputs = numberOfOutputs;

	//initialises the bias array
	biases = new double[outputs];

	//creates weight array of length outputs (then points to inputs)
	weights = new double *[outputs];

	//fills the given weights
	for (int out = 0; out < outputs; out++) {
		//declairs this row
		weights[out] = new double[inputs];

		//sets the bias value to 0
		biases[out] = biasVals[out];

		for (int in = 0; in < inputs; in++) {
			weights[out][in] = weightVals[out][in];
		}
	}

}

//prints all the weights in the layer to the console
void Layer::print() {
	for (int out = 0; out < outputs; out++) {
		std::cout << "w,";
		for (int in = 0; in < inputs; in++) {
			std::cout << "(" << in << "," << out << ")" << weights[out][in] << ",";
		}
		std::cout << std::endl;
	}

	std::cout << "b,";
	for (int out = 0; out < outputs; out++) {
		std::cout << "(" << out << ")"<< biases[out] << ", ";
	}
	std::cout << std::endl;

}

double** Layer::getWeights() { return &weights[0]; }

double Layer::getWeight(int in, int out) { return weights[out][in]; }

void Layer::modifyWeight(int in, int out, double val) { weights[out][in] += val; }


double * Layer::getBiases() { return biases; }

double Layer::getBias(int out) { return biases[out]; }

void Layer::modifyBias(int out, double val) { biases[out] += val; }


int Layer::getNumberInputs() { return inputs; }

int Layer::getNumberOutputs() { return outputs; }

double Layer::getWeights(int in, int out) {
	return weights[out][in];
}

//calculates the values of the output neurons based on given inputs (inputVals)
double* Layer::forwardPass(double * inputVals) {

	double * out = new double[outputs];

	//gets the result of each of the outputs based on the inputs
	for (int outP = 0; outP < outputs; outP++) {

		//sumes the values of each input times its weight to the current output
		out[outP] = 0;
		for (int inP = 0; inP < inputs; inP++) {
			out[outP] += weights[outP][inP] * inputVals[inP];
		}

		//adds the bias
		out[outP] += biases[outP];
	}

	return out;
}

/*calculates the values of the output neurons based on given inputs (inputVals)
but it also passes the output values through the sigmoid function*/
double* Layer::sigmoidForwardPass(double * inputVals) {
	double * out = new double[outputs];

	//gets the result of each of the outputs based on the inputs
	for (int outP = 0; outP < outputs; outP++) {

		//sumes the values of each input times its weight to the current output
		out[outP] = 0;
		for (int inP = 0; inP < inputs; inP++) {
			out[outP] += weights[outP][inP] * inputVals[inP];
		}
		//adds the bias
		out[outP] += biases[outP];

		out[outP] = sigmoid(out[outP]);
	}

	return out;
}

//passes x through the sigmoid function
double Layer::sigmoid(double x) {
	// -> 1/(1+e^-x)
	//return 1 / (1 + exp(x * -1));
	//double ex = exp(x);
	return 1/(1 + exp(x * -1));
}

//derivative of sigmoid function 
double Layer::sigmoidDeriv(double x) {
	double sig = sigmoid(x);
	return (sig - 1)/sig;
}
