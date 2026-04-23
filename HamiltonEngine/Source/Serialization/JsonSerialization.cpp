#include "PrecompiledHeader/Pch.h"

#include "JsonSerialization.h"
#include "Physics/State/RigidBodyState.h"

namespace HamiltonEngine::Serialization
{
	bool SerializeEnttRegistryAsJson(const entt::registry& Registry,
		const char* Filename,
		bool Overwrite)
	{
		std::ofstream Filestream;
		try
		{
			if (std::filesystem::exists(Filename))
			{
				if (!Overwrite) 
				{
					HAMILTON_LOG(Serialization,
						Warning,
						"Attempted to save %s but file already exists",
						Filename);
				
					return false;
				}
				
				//it exists so we erase the file contents
				Filestream.open(Filename, std::ofstream::out | std::ofstream::trunc);
			}
			else 
			{
				//Will create new file if it doesn't exist
				Filestream.open(Filename, std::ofstream::out);
			}

		}
		catch (std::filesystem::filesystem_error e)
		{
			HAMILTON_LOG(Serialization,
				Warning,
				"Attempting to open file %s but failed with exception: %s",
				Filename,
				e.what())

				return false;
		}
		
		{
			cereal::JSONOutputArchive output{ Filestream };

			//TODO maybe someday we automatically detect the things that go into this list
			entt::snapshot{ Registry }
				.get<entt::entity>(output)
				.get<Physics::RigidBodyStateComponent>(output);
		}

		Filestream.close();
	}

	bool DeserializeEnttRegistryFromJson(entt::registry& Registry, const char* Filename)
	{
		std::ifstream Filestream;
		try
		{
			if (std::filesystem::exists(Filename))
			{
				Filestream.open(Filename, std::ifstream::in);
			}
			else
			{
				HAMILTON_LOG(Serialization,
					Warning,
					"Attempting to open file but it does not exist %s",
					Filename)

					return false;
			}

		}
		catch (std::filesystem::filesystem_error e)
		{
			HAMILTON_LOG(Serialization,
				Warning,
				"Attempting to open file %s but failed with exception: %s",
				Filename,
				e.what())
				return false;
		}
		
		{
			cereal::JSONInputArchive input{ Filestream };
		
			entt::snapshot_loader{ Registry }
				.get<entt::entity>(input)
				.get<Physics::RigidBodyStateComponent>(input);
		}

		Filestream.close();
		
		return false;
	}
}
