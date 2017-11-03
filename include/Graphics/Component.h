#pragma once

namespace gpx
{

	class Component
	{
	public:
		Component() {}
		virtual ~Component(void){}
		virtual void Bind() const {}
		virtual void UnBind() const {}
		virtual bool IsBound() const { return false; }
		virtual void Dispose() {}
	private:
		virtual Component& operator=(const Component& o) = delete;
		Component(const Component& o) = delete;
	};
}