#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Object.h"

using std::vector;	using std::ostream;
using std::endl;	using std::shared_ptr;

class Frame
{
	friend ostream& operator<<(ostream& os, const Frame& frame);
public:
	using Ptr = shared_ptr<Frame>;
	Frame();
	~Frame();
	Frame& Append(const Object::Ptr obj);
	bool IsEmpty() const;
	size_t Size() const;
	vector<Object::Ptr>& ObjectPtrList();
	const vector<Object::Ptr>& ObjectPtrList() const;
private:
	vector<Object::Ptr> mList;
};

ostream& operator<<(ostream& os, const Frame& frame);
