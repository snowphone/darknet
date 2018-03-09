#include "Frame.h"


Frame::Frame()
{
}


Frame::~Frame()
{
}

Frame& Frame::Append(const Object::Ptr obj)
{
	mList.push_back(obj);
	return *this;
}

bool Frame::IsEmpty() const
{
	return mList.empty();
}

size_t Frame::Size() const
{
	return mList.size();
}

vector<Object::Ptr>& Frame::ObjectPtrList() 
{
	return mList;
}

const vector<Object::Ptr>& Frame::ObjectPtrList() const
{
	return mList;
}

ostream& operator<<(ostream& os, const Frame& frame)
{
	if (frame.IsEmpty())
	{
		os << endl;
		return os;
	}
	for(const shared_ptr<Object> objPtr : frame.mList)
	{
		os << *objPtr << endl;
	}
	return os;
}
