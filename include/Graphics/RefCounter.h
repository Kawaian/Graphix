#pragma once

namespace gpx
{


	class ReferenceCounter
	{
	public:
		ReferenceCounter(){ _refs = 1; }
		inline int getRefCount(){ return _refs; };
		inline void addRef(){ _refs++; }
		inline bool removeRef(){ _refs--; return _refs == 0; }
	private:
		int _refs;
	};

}