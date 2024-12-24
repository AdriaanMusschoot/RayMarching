#pragma once

namespace sdf
{

	template <typename T>
	class Singleton
	{
	public:
		static T& GetInstance()
		{
			static T instance{};
			return instance;
		}

		virtual ~Singleton() = default;

		Singleton(Singleton const&) = delete;
		Singleton(Singleton&&) = delete;
		Singleton& operator=(Singleton const&) = delete;
		Singleton& operator=(Singleton&&) = delete;

	protected:
		Singleton() = default;
	};

}