#include "stdafx.h"
#include <iostream>
#include <fstream>//reading/writing  file
#include <sstream> //string stream
#include "DataSet.h"

//DataSet Class 
DataSet::DataSet() {
	numberOfExamples = -1;
	numberOfInputs = -1;
	numberOfOutputs = -1;
	numberOfClasses = -1;

	numberOfExamples_A = 0;
	numberOfExamples_B = 0;
	dataIsSplit = false;
}

DataSet::DataSet(int numIns, int numOuts) {
	numberOfExamples = -1;
	numberOfInputs = numIns;
	numberOfOutputs = numOuts;
	numberOfClasses = -1;

	numberOfExamples_A = 0;
	numberOfExamples_B = 0;
	dataIsSplit = false;
}

DataSet::DataSet(int numIns, int numOuts, int numExamples) {
	numberOfExamples = numExamples;
	numberOfInputs = numIns;
	numberOfOutputs = numOuts;
	numberOfClasses = -1;

	numberOfExamples_A = 0;
	numberOfExamples_B = 0;
	dataIsSplit = false;
}


int DataSet::getFileSize(std::string filePath) {
	std::ifstream inF;
	inF.open(filePath);

	int count = 0;
	std::string s;

	while (std::getline(inF, s))
		count++;

	return count;

}

int DataSet::getLenghOfLine(std::string s, char delim) {

	int count = 0;

	std::stringstream line;
	line.str(s);
	std::string temp;

	while (getline(line, temp, delim))
		count++;

	return count;

}

double * DataSet::getInput(int n) {
	
	if (n < numberOfExamples) {
		double * out = new double[numberOfInputs];

		for (int i = 0; i < numberOfInputs; i++)
			out[i] = inputs[n][i];

		return out;
	}
	return new double [1] { 0 };
}

double * DataSet::getOutput(int n) {
	if (n < numberOfExamples) {
		double * out = new double[numberOfOutputs];

		for (int i = 0; i < numberOfOutputs; i++)
			out[i] = outputs[n][i];

		return out;
	}
	return new double [1] { 0 };
}

void DataSet::loadInputs(std::string filePath) {
	std::ifstream inF;
	inF.open(filePath);

	if (inF.is_open()) {
		std::cout << "loadInputs -> Reading from '" << filePath << "'" << std::endl;
		//checks if the number of examples has been recorded or not
		if (numberOfExamples == -1)
			numberOfExamples = getFileSize(filePath);//counts number of examples

		//finds the number of inputs if it's not already assigned
		std::string lineStr;
		inF >> lineStr;

		if (numberOfInputs == -1)
			numberOfInputs = getLenghOfLine(lineStr, ',');

		//creates the storage array
		inputs = new double*[numberOfExamples];
		std::cout << "Reading " << numberOfExamples << " examples" << std::endl;

		//keeps going through the list untill it runs out of items
		int rowPtr = 0;
		do {
			inputs[rowPtr] = new double[numberOfInputs];

			std::stringstream line;
			line.str(lineStr);
			std::string item;

			//for each item in the line
			int colPtr = 0;
			while (std::getline(line, item, ',')) {
				//converts item string to double
				double x = strtof(item.substr(0).c_str(), 0);
				//stores item
				inputs[rowPtr][colPtr] = x;
				colPtr++;
			}
				
			rowPtr++;
			inF >> lineStr;
			//checks if there is data to be stored
		} while (rowPtr < numberOfExamples);

	}
	else {
		std::cout << "Failed to open '" << filePath << "'" << std::endl;
	}
}

void DataSet::loadOutputs(std::string filePath) {
	std::ifstream inF;
	inF.open(filePath);

	if (inF.is_open()) {
		std::cout << "loadOutputs -> Reading from '" << filePath << "'" << std::endl;
		//checks if the number of examples has been recorded or not
		if (numberOfExamples == -1)
			numberOfExamples = getFileSize(filePath);//counts number of examples

		//finds the number of outputs if it's not already assigned
		std::string lineStr;
		inF >> lineStr;

		if (numberOfOutputs == -1)
			numberOfOutputs = getLenghOfLine(lineStr, ',');

		std::cout << "loading data with " << numberOfOutputs << " outputs" << std::endl;

		//creates the storage array
		outputs = new double*[numberOfExamples];
		std::cout << "Reading " << numberOfExamples << " examples" << std::endl;

		//keeps going through the list untill it runs out of items
		int rowPtr = 0;
		do {
			outputs[rowPtr] = new double[numberOfOutputs];

			std::stringstream line;
			line.str(lineStr);
			std::string item;

			//for each item in the line
			int colPtr = 0;
			while (std::getline(line, item, ',')) {
				//converts item string to double
				double x = strtof(item.substr(0).c_str(), 0);
				//stores item
				outputs[rowPtr][colPtr] = x;
				colPtr++;
			}

			rowPtr++;
			inF >> lineStr;
			//checks if there is data to be stored
		} while (rowPtr < numberOfExamples);

	}
	else {
		std::cout << "Failed to open '" << filePath << "'" << std::endl;
	}
}


void DataSet::loadDataSet(std::string filePath, int startLine, int numInputs, int * inputCols, int numOutputs, int * outputCols) {
	std::ifstream inF;
	inF.open(filePath);

	if (inF.is_open()) {
		std::cout << "loadOutputs -> Reading from '" << filePath << "'" << std::endl;
		//checks if the number of examples has been recorded or not
		if (numberOfExamples == -1)
			numberOfExamples = getFileSize(filePath);//counts number of examples
		int numberOfLines = numberOfExamples;

		//adjusts the storage array to compensate with the start
		numberOfExamples -= startLine;
		
		//finds the number of outputs if it's not already assigned
		std::string lineStr;
		inF >> lineStr;

		numberOfInputs = numInputs;
		numberOfOutputs = numOutputs;
		std::cout << "loading data with " << numberOfInputs << " inputs and " << numberOfOutputs << " outputs" << std::endl;
		

		//creates the storage array
		outputs = new double*[numberOfExamples];
		inputs = new double*[numberOfExamples];
		std::cout << "Reading " << numberOfExamples << " examples" << std::endl;

		//keeps going through the list untill it runs out of items
		int rowPtr = 0;
		int linePtr = 0;
		do {
			//if the pointer is at or past the start line, begin processing
			if (linePtr >= startLine) {
				outputs[rowPtr] = new double[numberOfOutputs];
				inputs[rowPtr] = new double[numberOfInputs];

				std::stringstream line;
				line.str(lineStr);
				std::string item;

				//for each item in the line
				//finds the number of commas in the line, line has c + 1 items
				size_t c = std::count(lineStr.begin(), lineStr.end(), ',');
				//array to store each item in the line
				double * lineArr = new double[c + 1];

				//gets each item in the line and stores them in an array
				int colPtr = 0;
				while (std::getline(line, item, ',')) {
					//converts item string to double
					double x = strtof(item.substr(0).c_str(), 0);
					//stores item
					lineArr[colPtr] = x;
					colPtr++;
				}


				//for each of the input columns
				colPtr = 0;
				for (; colPtr < numberOfInputs; colPtr++) {
					//stores the relevant item from the line in the input array
					inputs[rowPtr][colPtr] = lineArr[inputCols[colPtr]];
				}

				//for each of the output columns
				colPtr = 0;
				for (; colPtr < numberOfOutputs; colPtr++) {
					//stores the relevant item from the line in the input array
					outputs[rowPtr][colPtr] = lineArr[outputCols[colPtr]];
				}

				lineArr = NULL;
				delete lineArr;

				rowPtr++; 
			}

			linePtr++;
			inF >> lineStr;
			//checks if there is data to be stored

		} while (linePtr < numberOfLines);

	}
	else {
		std::cout << "Failed to open '" << filePath << "'" << std::endl;
	}
}

void DataSet::splitData() {
	//makes and array containing a 'pointer' to each entry of the data set
	int * pointers = new int[numberOfExamples];

	for (int i = 0; i < numberOfExamples; i++)
		pointers[i] = i;

	//the pointers are then shuffled
	for (int i = numberOfExamples - 1; i >= 0; i--) {
		//picks a random index to swap with between 0 and i inclusive
		int s = rand() % (i+1);

		//swaps index i and s
		int temp = pointers[i];
		pointers[i] = pointers[s];
		pointers[s] = temp;
	}

	//the pointers are then split into 2 sets, 50/50 split
	numberOfExamples_A = floor(numberOfExamples / 2);
	numberOfExamples_B = ceil(numberOfExamples / 2);

	setA_pointers = new int[numberOfExamples_A];
	setB_pointers = new int[numberOfExamples_B];

	int i = 0;//pointer array pointer
	int p = 0;//set array pointer
	//fills A array
	for (; i < numberOfExamples_A; i++) {
		setA_pointers[p] = pointers[i];
		p++;
	}

	//fills B array
	p = 0;
	for (; i < numberOfExamples; i++) {
		setB_pointers[p] = pointers[i];
		p++;
	}

	dataIsSplit = true;

	//delete temporary array
	pointers = NULL;
	delete pointers;

}

double * DataSet::getInput_A(int n) { return inputs[setA_pointers[n]]; }

double * DataSet::getOutput_A(int n) { return outputs[setA_pointers[n]]; }

double * DataSet::getInput_B(int n) { return inputs[setB_pointers[n]]; }

double * DataSet::getOutput_B(int n) { return outputs[setB_pointers[n]]; }

void DataSet::loadLables(std::string filePath) {

}

void DataSet::loadClasses(std::string filePath) {

}


void DataSet::print() {
	std::cout << "--Data set--" << std::endl;
	std::cout << numberOfExamples << " examples" << std::endl;

	for (int e = 0; e < numberOfExamples; e++) {
		std::cout << "[" << e << "] ";
		
		//prints inputs
		for (int i = 0; i < numberOfInputs; i++) {
			std::cout << inputs[e][i];

			if (i < numberOfInputs)
				std::cout << ",";

		}

		std::cout << " -> ";

		//prints outputs
		for (int i = 0; i < numberOfOutputs; i++) {
			std::cout << outputs[e][i];

			if (i < numberOfOutputs)
				std::cout << ",";

		}

		std::cout << std::endl;
	}

}

void DataSet::print_A() {
	std::cout << "--Data set A--" << std::endl;
	std::cout << numberOfExamples_A << " examples" << std::endl;

	for (int e = 0; e < numberOfExamples_A; e++) {
		std::cout << "[" << e << "] ";
		int p = setA_pointers[e];

		//prints inputs
		for (int i = 0; i < numberOfInputs; i++) {
			std::cout << inputs[p][i];

			if (i < numberOfInputs)
				std::cout << ",";

		}

		std::cout << " -> ";

		//prints outputs
		for (int i = 0; i < numberOfOutputs; i++) {
			std::cout << outputs[p][i];

			if (i < numberOfOutputs)
				std::cout << ",";

		}

		std::cout << std::endl;
	}
}

void DataSet::print_B() {
	std::cout << "--Data set B--" << std::endl;
	std::cout << numberOfExamples_B << " examples" << std::endl;

	for (int e = 0; e < numberOfExamples_B; e++) {
		std::cout << "[" << e << "] ";
		int p = setB_pointers[e];

		//prints inputs
		for (int i = 0; i < numberOfInputs; i++) {
			std::cout << inputs[p][i];

			if (i < numberOfInputs)
				std::cout << ",";

		}

		std::cout << " -> ";

		//prints outputs
		for (int i = 0; i < numberOfOutputs; i++) {
			std::cout << outputs[p][i];

			if (i < numberOfOutputs)
				std::cout << ",";

		}

		std::cout << std::endl;
	}
}
