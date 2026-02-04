#pragma once

namespace HamiltonEngine 
{
	template<class T>
	class Singleton 
	{
		//friend T;
	public:
		template<class... Params>
		static void Initialize(Params&&... params) 
		{
			Storage.InitializeImpl(params...);
		}

		static const T& Get() 
		{
			return Storage;
		}

		static T& GetMutable()
		{
			return Storage;
		}

	private:
		static T Storage;
	};

}
