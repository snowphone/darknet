#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#ifdef _DEBUG
#include <iostream>
#endif

#include "Classifier.h"

using std::remove_if;	using std::endl;		
using std::min_element;	using std::min;
using std::max;			using std::ifstream;	
using std::ofstream;	using std::copy_if;		
using std::shared_ptr;	using std::back_inserter;
using std::find_if;		using std::count_if;

Classifier::Classifier()
{
}

Classifier::Classifier(const string& path)
{
	Read(path);
}

Classifier& Classifier::Classify(const size_t backthrough, const size_t threshold)
{
	//extract frames if each frames have at least a player object.
	vector<Frame::Ptr> validFramePtrList; 
	copy_if(mFramePtrList.begin(), mFramePtrList.end(), back_inserter(validFramePtrList), [](Frame::Ptr f) {
		return !f->IsEmpty() &&
			count_if(f->ObjectPtrList().begin(), f->ObjectPtrList().end(),
				[](Object::Ptr obj) { return obj->GetType() == Object::Type::player; });
	});

	mMaxUsedID = backthrough + 1;

	//fPtrIt is framePtrIterator
	for(auto fPtrIt = ++validFramePtrList.begin(); fPtrIt != validFramePtrList.end(); ++fPtrIt)
	{
		Frame::Ptr framePtr = *fPtrIt;
		for (Object::Ptr objToClassify : framePtr->ObjectPtrList())
		{
			if (objToClassify->GetType() != Object::Type::player)
				continue;

			auto beg = (fPtrIt - validFramePtrList.begin() < backthrough) ? validFramePtrList.begin() : fPtrIt - backthrough;
			const Object::Ptr closestObj = findTheClosestObject(objToClassify, beg, fPtrIt, threshold);
			objToClassify->SetID(closestObj->mID);
			mMaxUsedID = max(objToClassify->mID, closestObj->mID);
		}
	}
	return *this;
}

Classifier& Classifier::Read(const string& path)
{
	mFramePtrList.clear();


	ifstream is(path);
	string info;
	while (getline(is, info))
	{
		Object::Ptr obj(new Object(info));
		if (obj->GetType() == Object::Type::header)
		{
			Frame::Ptr f(new Frame());
			mFramePtrList.push_back(f);
		}
		else
		{
			mFramePtrList.back()->Append(obj);
		}
	}
	return *this;
}

Classifier& Classifier::Write(const string& name)
{
	ofstream os(name);
	for (const Frame::Ptr framePtr : mFramePtrList)
	{
		os << *framePtr << endl;
	}
	return *this;
}

const Object::Ptr Classifier::findTheClosestObject(
	Object::Ptr objToClassify, const_iterator FramePtrBeg, const_iterator FramePtrEnd, const size_t threshold)
{
	vector<Object::Ptr> objects = Chain(FramePtrBeg, FramePtrEnd);

	const Object::Ptr nearest = *min_element(objects.begin(), objects.end(), [objToClassify](Object::Ptr l, Object::Ptr r) {
		return Distance(*l, *objToClassify) < Distance(*r, *objToClassify);
	});

	if (Distance(*nearest, *objToClassify) < threshold)
	{
		return nearest;
	}
	else
	{
#ifdef _DEBUG
		std::cout << "------------------" << std::endl
			<< "nearset: " << *nearest << std::endl
			<< "object: " << *objToClassify << std::endl
			<< "distance: " << Distance(*nearest, *objToClassify) << std::endl;
#endif
		//reidentificate
		objToClassify->SetID(mMaxUsedID + 1);
		return objToClassify;
	}
}

/* Chain function extracts objects from several frames if the frame has objects and the object's type is 'player'.
 * This function returns the vector of reference_wrapper<Object or const Object> to save memory.
 */
vector<Object::Ptr> Chain(Classifier::const_iterator begin, Classifier::const_iterator end)
{
	const static auto isPlayer = [](const Object::Ptr obj) { return obj->GetType() == Object::Type::player; };

	vector<Object::Ptr> ret;
	for (auto it = begin; it != end; ++it)
	{
		Frame::Ptr ptr = *it;
		copy_if(ptr->ObjectPtrList().begin(), ptr->ObjectPtrList().end(), back_inserter(ret), isPlayer);
	}
	return ret;
}
