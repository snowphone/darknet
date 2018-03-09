#pragma once
#include <algorithm>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "Frame.h"

class Object;

using std::string;		using std::vector;
using std::copy_if;		using std::shared_ptr;
using std::back_inserter; 


class Classifier
{
public:
	using iterator = vector<Frame::Ptr>::iterator;
	using const_iterator = vector<Frame::Ptr>::const_iterator;
	Classifier();
	Classifier(const string& path);

	Classifier& Classify(const size_t backthrough = 3, const size_t threshold = 150ull);
	Classifier& Read(const string& path);
	Classifier& Write(const string& name);
private:
	const Object::Ptr findTheClosestObject(Object::Ptr  objToClassify, const_iterator pFrameBeg, const_iterator pFrameEnd, const size_t threshold);
	string path;
	vector<Frame::Ptr> mFramePtrList;
	int mMaxUsedID = 1;
};

vector<Object::Ptr> Chain(Classifier::const_iterator begin, Classifier::const_iterator end);
