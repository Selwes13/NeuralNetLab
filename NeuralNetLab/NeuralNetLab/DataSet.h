#pragma once
#include "stdafx.h"

class DataSet {

private:
	double ** inputs;//example input
	double ** outputs;//ground truth for example
	std::string lables;//example lable

	std::string classes;//classes in dataset

	//pointers to the split data: inputs[setA_pointer[i]]
	int * setA_pointers;
	int * setB_pointers;

	bool dataIsSplit;


public:
	int numberOfInputs;
	int numberOfOutputs;
	int numberOfClasses;

	int numberOfExamples;

	int numberOfExamples_A;
	int numberOfExamples_B;


public:
	//constructor
	DataSet();
	DataSet(int numIns, int numOuts);
	DataSet(int numIns, int numOuts, int numClasses);

	//statics
	static int getFileSize(std::string filePath);
	static int getLenghOfLine(std::string s, char delim);

	//gets
	double * getInput(int n);
	double * getOutput(int n);
	std::string * getLable(int n);
	std::string getClasses();

	//sets
	void setInputs(double ** data);
	void setOutputs(double ** data);
	void setLables(std::string * data);
	void setSetClasses(std::string * data);

	//prints
	void print();
	void printInput(int n);
	void printOutput(int n);
	void printLable(int n);
	void printClass(int n);

	//load seperat
	void loadInputs(std::string filePath);
	void loadOutputs(std::string filePath);
	void loadLables(std::string filePath);
	void loadClasses(std::string filePath);

	//split dataset
	void splitData();//splits items 50/50 after shuffling

	double * getInput_A(int n);
	double * getOutput_A(int n);
	void print_A();

	double * getInput_B(int n);
	double * getOutput_B(int n);
	void print_B();

	//load full set
	void loadDataSet(std::string filePath);

};