#include "stdafx.h"
#include <iostream>
#include <fstream>//reading/writing  file
#include <sstream> //string stream
#include "NeuralNet.h"

//NeuralNet class
//constructor with hidden layers
NeuralNet::NeuralNet(int Layers, int * layerSize, double leaningCoeff) {

	//number of layers of neurons
	numberOfLayers = Layers;

	//in, out and hidden layers
	neuronsPerLayer = new int[numberOfLayers];
	//learning coefficient
	alpha = leaningCoeff;


	//holds neuron values in a jagged array
	neurons = new double*[numberOfLayers];

	//adds the correct number of neurons to each layer of the network
	//index 0 is the input layer and index n is the output
	for (int l = 0; l < numberOfLayers; l++) {
		neurons[l] = new double[layerSize[l]];
		neuronsPerLayer[l] = layerSize[l];
	}

	//creates all the layers to connect the neurons
	layers = new Layer[numberOfLayers - 1];
	for (int l = 0; l < numberOfLayers - 1; l++)
		layers[l] = Layer(neuronsPerLayer[l], neuronsPerLayer[l + 1]);

}

//constructor without hidden layers
NeuralNet::NeuralNet(int numberOfInputs, int numberOfOutputs, double leaningCoeff) {
	//number of layers of neurons
	numberOfLayers = 2;

	//in, out and hidden layers
	neuronsPerLayer = new int[numberOfLayers];
	//learning coefficient
	alpha = leaningCoeff;


	//holds neuron values in a jagged array
	neurons = new double*[numberOfLayers];

	//adds the correct number of neurons to each layer of the network
	//input layer
	neurons[0] = new double[numberOfInputs];
	neuronsPerLayer[0] = numberOfInputs;

	//output layer
	neurons[1] = new double[numberOfOutputs];
	neuronsPerLayer[1] = numberOfOutputs;

	//adds a layer between inputs and outputs
	layers = new Layer[1];
	layers[0] = Layer(numberOfInputs, numberOfOutputs);

}

//takes the inputs and passes them through the network
void NeuralNet::runNetwork(double * inputs) {

	//puts the intput values into the first set of neurons
	for (int in = 0; in < neuronsPerLayer[0]; in++) {
		neurons[0][in] = inputs[in];
	}

	//for each layer, provides the input neurons and stores the outputs
	for (int l = 0; l < numberOfLayers - 1; l++) {

		//passes the data through the layer
		double * act = layers[l].sigmoidForwardPass(neurons[l]);

		//stores the output from the layer in the next layer of neurons
		for (int n = 0; n < neuronsPerLayer[l + 1]; n++) {
			neurons[l+1][n] = act[n];
		}

	}
}

double*  NeuralNet::getOutputs() { return neurons[numberOfLayers - 1]; }

int NeuralNet::getNumberOfOutputs() { return neuronsPerLayer[numberOfLayers - 1]; }

double NeuralNet::getErrorSignal() { return errorSignal; }

double NeuralNet::Z(int L, int j, int k) {
	//if this is the input layer, return the input activation
	if (L == 0) {
		return neurons[0][j];
	}
	else { //if it's not the input layer
		//gets the sum of the activation from the previous layer times theior weight
		//Z = a(previous layer)k * w(L)jk
		return neurons[L][k] * layers[L].getWeight(k, j);
	}
}

void NeuralNet::backPropagate(double * groundTruth) {

	int endLayer = numberOfLayers - 1;
	int numberOutput = neuronsPerLayer[endLayer];

	double * predicted = this->getOutputs();
	double * errors = new double[numberOutput];

	errorSignal = 0;
	std::cout << "\n";
	//calculates the full error signal
	for (int e = 0; e < numberOutput; e++) {
		errors[e] = (predicted[e] - groundTruth[e]);
		errorSignal += pow(errors[e], 2);

	}


	//for each layer of connections from output to input 
	for (int l = numberOfLayers - 2; l >= 0; l--) {
		//for each weight in this layer
		int ins = layers[l].getNumberInputs();
		int outs = layers[l].getNumberOutputs();

		//for each of the connections in this layer
		for (int in = 0; in < ins; in++) {
			for (int out = 0; out < outs; out++) {
				//calculates the effec of this connection on the output
				double effect = backPropagate(l, in, out, errors, groundTruth);
				//updates the weight proportionally to the learning rate
				layers[l].modifyWeight(in, out, effect*alpha);
			}
		}

		//for each bias in the layer
		for (int out = 0; out < outs; out++) {
			//finds the effect of the given bias on the output
			double effect = backPropagateBias(l, out, errors);
			//updates the bias
			layers[l].modifyBias(out, effect*alpha);
		}
	}


}

double NeuralNet::backPropagate(int layer, int in, int out, double * errors, double * groundTruth) {
	//std::cout << "back prop on layer " << layer << " : " << in << " to " << out << std::endl;

	//checks if this is the output layer
	if (layer == (numberOfLayers - 2)) {
		//output layer
		double z = 0;

		//sum of the inputs from the previous layer without sigmoid 
		for (int i = 0; i < neuronsPerLayer[layer - 1]; i++)
			z += neurons[layer][in] * layers[layer].getWeight(i, out);

		//dC0/dw
		z = 2 * (errorSignal)* Layer::sigmoidDeriv(z) * neurons[layer][in];

		//overall effect
		//(yj - aj) * a(L-1) * dC0/dw
		return (groundTruth[out] - errors[out]) * neurons[layer][in] * z;
	
	}
	else {
		//any other layers

		double sum = 0;
		//gets the sum of the effect of each item connected down stream to this connection
		int lenIn = neuronsPerLayer[layer+1];
		for (int i = 0; i < lenIn; i++) {

		}

		return sum;
	}

}

double NeuralNet::backPropagateBias(int layer, int out, double * errors) {

	//checks if this is the output layer
	if (layer == (numberOfLayers - 2)) {
		//std::cout << "update bias " << errors[out] << "(" << out << ")" << std::endl;
		return (errors[out]);
	}
	else {
		return 0;
	}
}

bool NeuralNet::save(std::string filePath) {

	std::ofstream outF;
	outF.open(filePath);

	if (outF.is_open()) {
		std::cout << "saving network to '" << filePath << "'" << std::endl;
		//writes the number of items in each layer
		std::stringstream setupLine;
		for (int l = 0; l < numberOfLayers; l++) {
			setupLine << neuronsPerLayer[l] << ",";
		}
		outF << setupLine.str();

		//writes learning coefficient
		outF << "\na," << alpha;


		//for each layer
		for (int l = 0; l < numberOfLayers - 1; l++) {
			
			//declare new layer in file
			outF << "\n";

			//write the weights 
			Layer lay = layers[l];
			int ins = lay.getNumberInputs();
			int outs = lay.getNumberOutputs();

			//writes each weight to the file
			for (int out = 0; out < outs; out++) {
				std::stringstream line;
				line << "\nw,";

				for (int in = 0; in < ins; in++) {
					line << lay.getWeights(in, out) << ",";
				}
				outF << line.str();
			}
			//writes the biases 
			std::stringstream line;
			line << "\nb,";

			double * biases = lay.getBiases();

			for (int out = 0; out < outs; out++)
				line << biases[out] << ",";
			outF << line.str();

		}
		outF.close();
		return true;
	} 
	std::cout << "failed to save to file '" << filePath << "'" << std::endl;
	return false;

}

//static function
NeuralNet NeuralNet::load(std::string filePath) {

	std::ifstream inF;
	inF.open(filePath);

	if (inF.is_open()) {
		std:: cout << "Reading from '" << filePath << "'" << std::endl;
		
		std::string x;
		std::stringstream line;

		//reads the structure of the network from the first line
		inF >> x;
		line << x;

		std::cout << "Structure Line -> " << line.str() << std::endl;

		//creates the structure array for the constructor
		int netSize = x.length() / 2;
		int * netStructure = new int[netSize];
		int count = 0;
		//adds each item to the structure array
		while (std::getline(line, x, ',')) {
			//puts the value in the array after converitng to int
			netStructure[count] = stoi(x);
			count++;
		}

		//reads the learning coefficient
		inF >> x;
		x = x.substr(2);//removes the first two character "a,"
		
		//creates the initial network (x is converted to a double as alpha)
		NeuralNet outputNet = NeuralNet(netSize, netStructure, strtof(x.substr(0).c_str(), 0));

		//for each connection layer
		for (int l = 0; l < netSize - 1; l++) {
			//creates storage array for the connection weights
			int ins = netStructure[l];
			int outs = netStructure[l + 1];

			double ** weights = new double*[outs];
			for (int i = 0; i < outs; i++)
				weights[i] = new double[ins];

			//fills out the array with the weights values from the file
			for (int row = 0; row < outs; row++) {
				//read line
				inF >> x;
				//removes "w," from the line and adds to string stream
				x = x.substr(2);
				
				//std::string item;
				//goes through each item in the line
				for (int col = 0; col < ins; col++) {
					//finds the item and extracts it
					std::string s;
					s = x.substr(0, x.find(','));
					//removes the item after it's been read
					x.erase(0, x.find(',') + 1);

					weights[row][col] = strtof(s.substr(0).c_str(), 0);
				}
			}

			double * biases = new double[outs];
			//reads the line
			inF >> x;
			//removes "b," from the line and adds to string stream
			x = x.substr(2);

			//fills out the array with all the bias values
			for (int out = 0; out < outs; out++) {
				//finds the item and extracts it
				std::string s;
				s = x.substr(0, x.find(','));

				//removes the item after it's been read
				x.erase(0, x.find(',') + 1);

				//adds the read value to the bias array
				biases[out] = strtof(s.substr(0).c_str(), 0);
			}
			
			//sets the weights in the current layer to the ones extracted form the file
			outputNet.layers[l] = Layer(ins, outs, weights, biases);


		}

		
		
		return outputNet;
	}
	else {
		std::cout << "Cannot open '" << filePath << "'" << std::endl;
		return NeuralNet(1, 1, 1.0f);
	}

}

int NeuralNet::getStrongestClass(double * outs, int len) {
	int maxIndex = -1;
	double maxVal = -1000000;

	for (int i = 0; i < len; i++) {
		if (outs[i] > maxVal) {
			maxIndex = i;
			maxVal = outs[i];
		}
	}

	return maxIndex;
}

void NeuralNet::setLearningRate(double a) { alpha = a; }

double NeuralNet::getLearningRate() { return alpha; }

void NeuralNet::print() {
	//displays completed network info
	
	std::cout << "Network of size " << numberOfLayers << "\n[";
	
	for (int l = 0; l < numberOfLayers - 1; l++)
		std::cout << neuronsPerLayer[l] << ", ";
	
	std::cout << neuronsPerLayer[numberOfLayers - 1] << "]" << std::endl;

	std::cout << "Alpha = " << alpha << std::endl;
}

void NeuralNet::printFull() {
	std::cout << "-------Network Info--------\nNetwork of size " << numberOfLayers << "\n[";

	for (int l = 0; l < numberOfLayers - 1; l++)
		std::cout << neuronsPerLayer[l] << ", ";

	std::cout << neuronsPerLayer[numberOfLayers - 1] << "]" << std::endl;

	std::cout << "Alpha = " << alpha << "\n-------Neuron Activations--------" << std::endl;

	for (int l = 0; l < numberOfLayers; l++) {
		std::cout << "Layer " << l << ": ";

		for (int i = 0; i < neuronsPerLayer[l]; i++) {
			std::cout << neurons[l][i] << ", ";
		}
		std::cout << std::endl;
	}
	std::cout << "-------End--------" << std::endl;
}

void NeuralNet::printLayers() {

	for (int l = 0; l < numberOfLayers - 1; l++) {
		std::cout << "-----Layer " << l << "-----" << std::endl;
		layers[l].print();
	}
	std::cout << "---------" << std::endl;

}

Layer* NeuralNet::getLayers() {
	return layers;
}
