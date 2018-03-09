#include <regex>
#include <string>

#include "Object.h"

using std::regex;	using std::regex_search;
using std::smatch;	using std::stoi;

int Object::identifier = 1;

Object::Object()
{
}

Object::Object(const string & info)
{
	regex pattern(R"(left=(\d+), right=(\d+), top=(\d+), bottom=(\d+), obj_id=(\d+).*)");
	smatch matchResults;

	if (!regex_search(info, matchResults, pattern))
	{
		mType = Type::header;
		return;
	}
	mLeftTop.first = stoi(matchResults[1].str()), mLeftTop.second = stoi(matchResults[3].str());
	mRightBottom.first = stoi(matchResults[2].str()), mRightBottom.second = stoi(matchResults[4].str());


	mType = static_cast<Type>(stoi(matchResults[5].str()));
	if (mType == Type::ball)
	{
		mID = 0;
	}
	else
	{
		mID = identifier++;
	}
	
	mCenter = { mLeftTop.first + Width() / 2 , mRightBottom.second };
}


Object::~Object()
{
}

//It represents its position.
//Center returns the middle point of two feet.
const Point& Object::Center() const
{
	return mCenter;
}

Object::Type Object::GetType() const
{
	return mType;
}

size_t Object::Height() const
{
	return mLeftTop.second - mRightBottom.second;
}

size_t Object::Width() const
{
	return mRightBottom.first - mLeftTop.first;
}


void Object::SetID(const int newID)
{
	mID = newID;
}

ostream& operator<<(ostream& os, const Object& obj)
{
	os << "left=" << obj.mLeftTop.first << ", right=" << obj.mRightBottom.first
		<< ", top=" << obj.mLeftTop.second << ", bottom=" << obj.mRightBottom.second
		<< ", obj_id=" << static_cast<int>(obj.mType)  << ", obj=" << obj.mID;
	return os;
}

double Distance(const Object & lhs, const Object & rhs)
{
	return Distance(lhs.Center(), rhs.Center());
}
