#include "PrecompiledHeader/Pch.h"

#include "JsonSerialization.h"
#include "Physics/State/RigidBodyState.h"
#include "Physics/Potentials/ConstantGravityPotential.h"
#include "Physics/Potentials/SpringPotential.h"

namespace HamiltonEngine::Serialization
{
    using namespace entt;

	void Save(cereal::JSONOutputArchive& Record, const HamiltonEngine::Serialization::JsonEntityWrapper& Wrapper, const std::uint32_t Version)
	{
		Record(cereal::make_nvp("Count", Wrapper.Count));
		Record(cereal::make_nvp("Values", Wrapper.Values));
	}

	void Load(cereal::JSONInputArchive& Record, HamiltonEngine::Serialization::JsonEntityWrapper& Wrapper, const std::uint32_t Version)
	{
		Record(Wrapper.Count);
		Record(Wrapper.Values);
	}


	/**TODO:
	* 1 remove using statement for entt
	* 2 Can we do better versioning for template wraper code?
	* 3 Avoid all the copying
	**/
    
    template<typename Registry>
    class CerealJsonSnapshot 
    {
        static_assert(!std::is_const_v<Registry>, "Non-const registry type required");
        using TraitsType = entt_traits<typename Registry::entity_type>;

    public:
        using RegistryType = Registry;

        using EntityType = typename RegistryType::entity_type;

        CerealJsonSnapshot(const RegistryType& Source) noexcept
            : Reg{ &Source } {
        }

        CerealJsonSnapshot(const CerealJsonSnapshot&) = delete;

        CerealJsonSnapshot(CerealJsonSnapshot&&) noexcept = default;

        ~CerealJsonSnapshot() = default;

        CerealJsonSnapshot& operator=(const CerealJsonSnapshot&) = delete;

        CerealJsonSnapshot& operator=(CerealJsonSnapshot&&) noexcept = default;

        template<typename Type>
        const CerealJsonSnapshot& Get(cereal::JSONOutputArchive& Archive, const char* Key, const id_type Id = type_hash<Type>::value()) const {
            if (const auto* Storage = Reg->template storage<Type>(Id); Storage) 
			{
                const typename RegistryType::common_type& base = *Storage;
                //Entity Id
				if constexpr (std::is_same_v<Type, EntityType>)
				{
					JsonEntityWrapper Wrapper;
					//Wrapper.Count = static_cast<typename TraitsType::EntityType>(Storage->size());
					Wrapper.Count = Storage->size(); //TODO do the thing above
					Wrapper.Values.reserve(Wrapper.Count);
					
					Archive(cereal::make_nvp("FreeList", static_cast<typename TraitsType::entity_type>(Storage->free_list())));
					for (auto first = base.rbegin(), last = base.rend(); first != last; ++first)
					{
						Wrapper.Values.push_back(*first);
					}

					Archive(cereal::make_nvp(Key, Wrapper));
                }
				// Components
                else if constexpr (RegistryType::template storage_for_type<Type>::storage_policy == deletion_policy::in_place)
				{
                    // THIS BRANCH OF THE CODE IS UNTESTED
					JsonComponentWrapper<Type> Wrapper;
					//Wrapper.Count = static_cast<typename TraitsType::EntityType>(Storage->size());
					Wrapper.Count = Storage->size(); //TODO do the thing above
					Wrapper.Values.reserve(Wrapper.Count);
					
					for (auto it = base.rbegin(), last = base.rend(); it != last; ++it)
					{
                        if (const auto entt = *it; entt == tombstone) 
						{
                            Archive(cereal::make_nvp("Tombstone",static_cast<EntityType>(null)));
                        }
                        else 
						{
							const auto& [Entity, Component] = Storage->get_as_tuple(entt);
							Wrapper.Values.push_back(Component);
						}
                    }
					Archive(cereal::make_nvp(Key, Wrapper));
                }
                else 
				{
					JsonComponentWrapper<Type> Wrapper;
					//Wrapper.Count = static_cast<typename TraitsType::EntityType>(Storage->size());
					Wrapper.Count = Storage->size(); //TODO do the thing above
					Wrapper.Values.reserve(Wrapper.Count);
					
					for (const auto& [Entity, Component] : Storage->reach())
					{
						EntityComponentPair Pair{ Entity, Component };
						Wrapper.Values.push_back(Pair);
					}
					Archive(cereal::make_nvp(Key, Wrapper));
                }
            }
            else
			{
                Archive(typename TraitsType::entity_type{});
            }

            return *this;
        }

    private:
        const RegistryType* Reg;
    };
	
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
			using namespace entt::literals;
			
			cereal::JSONOutputArchive output{ Filestream };

			//TODO maybe someday we automatically detect the things that go into this list
            CerealJsonSnapshot{ Registry }
				.Get<entt::entity>(output, "EntityId")
				.Get<Physics::RigidBodyStateComponent>(output, "RigidBodyStateComponent")
				.Get<Physics::RigidBodyGravityComponent>(output, "RigidBodyGravityComponent")
				.Get<Physics::SpringPotentialComponent>(output, "SpringComponent");
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
				//.get<Physics::RigidBodyGravityComponent>(input)
				//.get<Physics::SpringPotentialComponent>(input);
		}

		Filestream.close();
		
		return false;
	}
}