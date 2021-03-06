// NeuralNetLab.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <string>
#include "Layer.h"
#include "NeuralNet.h"
#include "DataSet.h"
#include "Perceptron.h"
#include <fstream>//reading/writing  file

double mod(double x) {
	if (x < 0)
		return x * -1;
	return x;
}

int main()
{
	std::cout << "Hello" << std::endl;

	float ** w = new float*[2];
	w[0] = new float[3]{ 0.2f, 0.1f, 0.6f };
	w[1] = new float[3]{ 0.3f, 0.1f, 0.7f };

	NeuralNet n = NeuralNet(2, 1, 0.25);
	Perceptron p = Perceptron(2, 0.1);

	//n.runNetwork(new float[3]{5.0f, 1, 0.6f});
	//float * act = n.getOutputs();

	//int len = n.getNumberOfOutputs();
	//std::cout << len << std::endl;
	//for (int i = 0; i < len; i++)
	//	std::cout << act[i] << ", ";
	//std::cout << std::endl;

	
	//n2.printFull();
	//n2.printLayers();

	//training dataset
	DataSet ds = DataSet();
	ds.loadInputs("data/test_inputs.txt");
	ds.loadOutputs("data/test_outputs.txt");

	int exs = ds.numberOfExamples;
	int outs = ds.numberOfOutputs;

	std::string errOut = "";
	std::string valOut = "";

	ds.print();

	ds.splitData();

	ds.print_A();
	ds.print_B();

	system("pause");
	
	//training loop
	for (int r = 0; r < 25; r++) {//number of training pass throughs
		std::cout << "Round " << r << std::endl;


		float totalError = 0;
		for (int ex = 0; ex < exs; ex++) {//for each example in the training data
			//forward pass
			//n.runNetwork(ds.getInput(ex));
			p.run(ds.getInput(ex));

			//float a = n.getOutputs()[0];

			//backward pass
			n.backPropagate(ds.getOutput(ex));
			p.learn(ds.getInput(ex), ds.getOutput(ex)[0]);

			//a = n.getOutputs()[0] - a;

			//std::cout << "Predicted [" << n.getOutputs()[0] << "] ";
			//std::cout << "shoud have been [" << ds.getOutput(ex)[0] << "]" << std::endl;
			//std::cout << "err " << n.getErrorSignal() << std::endl;
			//std::cout << ds.getInput(ex)[0] << ", " << ds.getInput(ex)[1] << " -> " << n.getOutputs()[0] << " (" << ds.getOutput(ex)[0] << ")" << std::endl;
			//std::cout << ds.getInput(ex)[0] << ", " << ds.getInput(ex)[1] << " -> " << round(a * 100) / 100 << " (" << ds.getOutput(ex)[0] << ") error = " << n.getErrorSignal() << std::endl;
			
			totalError += mod(p.getError());//n.getErrorSignal();
			p.printWeights();
			std::cout << "error = " << p.getError() << std::endl;

			errOut += std::to_string(p.getError()) + ",";
			valOut += std::to_string(p.getOutput()) + ",";

			
		}
		std::cout << r << ": total error = " << totalError << std::endl;
		errOut += "\n";
		valOut += "\n";


		if (totalError < 0.1) {
			std::cout << "\n**Done training after " << r << " rounds**\n" << std::endl;
			break;
		}
		
	}

	//test
	std::cout << "***test***" << std::endl;
	int correct = 0;
	for (int ex = 0; ex < exs; ex++) {
		
		//n.runNetwork(ds.getInput(ex));
		p.run(ds.getInput(ex));

		std::cout << "predicted " << p.getOutput() << " (" << ds.getOutput(ex)[0] << ") -- [" << p.getOutputNatural() << "]" << std::endl;

	}
	p.printWeights();


	std::ofstream outF;
	outF.open("errorLog.txt");

	if (outF.is_open()) {
		outF << errOut;
	}
	outF.close();

	outF.open("errorValLog.txt");

	if (outF.is_open()) {
		outF << valOut;
	}
	outF.close();

	system("pause");

    return 0;
}



