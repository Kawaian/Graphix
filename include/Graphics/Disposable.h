#pragma once

namespace gpx
{


	class Disposable
	{
	public:
		virtual ~Disposable() {}
		virtual void Dispose() = 0;
	};
}