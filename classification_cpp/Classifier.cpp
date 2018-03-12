#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>
#include <set>

#include "Classifier.h"
#include "Point.h"

using std::remove_if;	using std::endl;		
using std::min_element;	using std::min;
using std::max;			using std::ifstream;	
using std::ofstream;	using std::copy_if;		
using std::shared_ptr;	using std::back_inserter;
using std::find_if;		using std::count_if;
using std::vector;		using std::find;
using std::set;			using std::pair;
using std::tie;

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
	vector<Frame::Ptr> validFramePtrList = frameContainingPlayers();

	mMaxUsedID = backthrough + 1;

	//fPtrIt is framePtrIterator
	for(auto fPtrIt = ++validFramePtrList.begin(); fPtrIt != validFramePtrList.end(); ++fPtrIt)
	{
		Frame::Ptr framePtr = *fPtrIt;
		vector<Object::Ptr> used;
		for (Object::Ptr objToClassify : framePtr->ObjectPtrList())
		{
			if (objToClassify->GetType() != Object::Type::player)
				continue;

			auto beg = (fPtrIt - validFramePtrList.begin() < backthrough) ? validFramePtrList.begin() : fPtrIt - backthrough;
			const Object::Ptr approx = getApproximation(objToClassify, beg, fPtrIt, threshold);

			objToClassify->SetID(approx->mID);
			mMaxUsedID = max(objToClassify->mID, approx->mID);
			used.push_back(approx);
		}
	}
	return *this;
}

Classifier& Classifier::Classify_v2(const size_t threshold)
{
	vector<Frame::Ptr> frames = this->frameContainingPlayers();
	size_t MAXPLAYER = 10;

	vector<Point> playerLocations;
	
	for (auto frame : frames)
	{
		vector<Object::Ptr> objects;
		copy_if(frame->ObjectPtrList().begin(), frame->ObjectPtrList().end(), back_inserter(objects), 
			[](Object::Ptr p) {return p->GetType() == Object::Type::player; });
		//matching
		for (size_t i = 0; i < playerLocations.size(); i++)
		{
			if (objects.empty())
				continue;
			Object::Ptr approx = *min_element(objects.begin(), objects.end(), [ pnt=playerLocations[i] ](Object::Ptr l, Object::Ptr r) {
				return Distance(l->Center(), pnt) < Distance(r->Center(), pnt);
			});


			approx->SetID(i + 1);
			playerLocations[i] = approx->Center();
			objects.erase(find(objects.begin(), objects.end(), approx));
		}
		for (Object::Ptr obj : objects)
		{
			if (playerLocations.size() > MAXPLAYER)
				break;
			playerLocations.push_back(obj->Center());
			obj->SetID(playerLocations.size());
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

//비어있거나 공만 있는 프레임을 뺀 프레임 벡터를 반환한다.
vector<Frame::Ptr> Classifier::frameContainingPlayers() const
{
	//extract frames if each frames have at least a player object.
	vector<Frame::Ptr> validFramePtrList; 
	copy_if(mFramePtrList.begin(), mFramePtrList.end(), back_inserter(validFramePtrList), [](Frame::Ptr f) {
		return !f->IsEmpty() &&
			count_if(f->ObjectPtrList().begin(), f->ObjectPtrList().end(),
				[](Object::Ptr obj) { return obj->GetType() == Object::Type::player; });
	});
	return validFramePtrList;
}

const Object::Ptr Classifier::getApproximation(
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
