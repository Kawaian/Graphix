#pragma once

namespace gpx
{

	class Bindable
	{
	public:
		virtual ~Bindable() {}
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
	};
}