#include <cassert>
#include <iostream>
#include <string>
#include "Classifier.h"
#define notTEST

using namespace std;

int main(int argc, char* argv[]) {
#ifdef TEST 
	string path = "../no.txt";
#else
	if (!(argc > 1))
	{
		cout << "You must pass the file as an argument!" << endl;
		return 1;
	}
	string path = argv[1];
#endif

	Classifier classifier;
	classifier.Read(path);
	classifier.Classify();
	string output = path.substr(0, path.rfind(".")) + "_cppclassified.txt";
	classifier.Write(output);
};