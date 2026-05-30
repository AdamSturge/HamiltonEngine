#include "PrecompiledHeader/Pch.h"

#include "JsonSerialization.h"
#include "Physics/State/RigidBodyState.h"
#include "Physics/Potentials/ConstantGravityPotential.h"
#include "Physics/Potentials/SpringPotential.h"

namespace HamiltonEngine::Serialization
{

	/**TODO:
	* 1 Remove using statement for entt
	* 2 Default Constructors
	**/

    using namespace entt;

	// Wrapper used for serializing a vector of entities to json
	struct JsonEntityWrapper
	{
		SERIALIZATION_VERSION(JsonEntityWrapper, 1)

		size_t Count = 0;
		std::vector<entt::entity> Values;
	};

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

	//Pair used for serializing an entity and a component as a single json object
	// The Cereal docs warn about serialzing raw pointers since you may serialize the 
	// same object twice. But we should be good in this case
	template<typename T>
	struct EntityComponentPair
	{
		SERIALIZATION_VERSION(EntityComponentPair<T>, 1)

		entt::entity Entity;
		//Although using pointers works for saving we need cereal to create an instance that
		// we can move into the entt registry when we load this thing
		T Component;
	};

	template<class T>
	void Save(cereal::JSONOutputArchive& Record, const HamiltonEngine::Serialization::EntityComponentPair<T>& Pair, const std::uint32_t Version)
	{
		Record(cereal::make_nvp("EntityId", Pair.Entity));
		Record(cereal::make_nvp("Component", Pair.Component));
	}

	template<class T>
	void Load(cereal::JSONInputArchive& Record, HamiltonEngine::Serialization::EntityComponentPair<T>& Pair, const std::uint32_t Version)
	{
		Record(Pair.Entity);
		Record(Pair.Component);
	}


	//Wrapper for serializing an array of entity-component pairs to json
	template<class T>
	struct JsonEntityComponentWrapper
	{
		SERIALIZATION_VERSION(JsonEntityComponentWrapper<T>, 1)

		size_t Count = 0;
		std::vector<EntityComponentPair<T>> Values;
	};

	template<class T>
	void Save(cereal::JSONOutputArchive& Record, const HamiltonEngine::Serialization::JsonEntityComponentWrapper<T>& Wrapper, const std::uint32_t Version)
	{
		Record(cereal::make_nvp("Count", Wrapper.Count));
		Record(cereal::make_nvp("Components", Wrapper.Values));
	}

	template<class T>
	void Load(cereal::JSONInputArchive& Record, HamiltonEngine::Serialization::JsonEntityComponentWrapper<T>& Wrapper, const std::uint32_t Version)
	{
		Record(Wrapper.Count);
		Record(Wrapper.Values);
	}

	//Saves out an EnTT registry to Json using a custom format
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
					Wrapper.Count = Storage->size(); 
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
					JsonEntityComponentWrapper<Type> Wrapper;
					Wrapper.Count = Storage->size();
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
					JsonEntityComponentWrapper<Type> Wrapper;
					Wrapper.Count = Storage->size(); 
					Wrapper.Values.reserve(Wrapper.Count);
					
					for (const auto& [Entity, Component] : Storage->reach())
					{
						Wrapper.Values.push_back({ Entity, Component });
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

	// Loads an Entt registry from our custom json format
	template<typename Registry>
	class CerealJsonSnapshotLoader
	{
		static_assert(!std::is_const_v<Registry>, "Non-const registry type required");
		using traits_type = entt_traits<typename Registry::entity_type>;

	public:
		/*! Basic registry type. */
		using RegistryType = Registry;
		/*! @brief Underlying entity identifier. */
		using EntityType = typename RegistryType::entity_type;

		CerealJsonSnapshotLoader(RegistryType& source) noexcept
			: Reg{ &source } 
		{
			// restoring a snapshot as a whole requires a clean registry
			ENTT_ASSERT(Reg->template storage<EntityType>().free_list() == 0u, "Registry must be empty");
		}

		CerealJsonSnapshotLoader(const CerealJsonSnapshotLoader&) = delete;

		CerealJsonSnapshotLoader(CerealJsonSnapshotLoader&&) noexcept = default;

		~CerealJsonSnapshotLoader() = default;

		CerealJsonSnapshotLoader& operator=(const CerealJsonSnapshotLoader&) = delete;

		CerealJsonSnapshotLoader& operator=(CerealJsonSnapshotLoader&&) noexcept = default;

		template<typename Type>
		CerealJsonSnapshotLoader& Get(cereal::JSONInputArchive& Archive, const char* Key, const id_type Id = type_hash<Type>::value())
		{
			auto& Storage = Reg->template storage<Type>(Id);

			if constexpr (std::is_same_v<Type, EntityType>)
			{
				typename traits_type::entity_type FreeList;
				Archive(cereal::make_nvp("FreeList", FreeList));
				
				JsonEntityWrapper Entities;
				Archive(cereal::make_nvp(Key, Entities));
				
				EntityType Placeholder{};

				for (EntityType Entity : Entities.Values)
				{
					Storage.generate(Entity);
					Placeholder = (Entity > Placeholder) ? Entity : Placeholder;
				}

				Storage.start_from(traits_type::next(Placeholder));
				Storage.free_list(FreeList);
			}
			else
			{
				auto& Other = Reg->template storage<EntityType>();
				EntityType Entt{ null };

				JsonEntityComponentWrapper<Type> Components;
				Archive(cereal::make_nvp(Key, Components));

				for (const EntityComponentPair<Type>& Pair : Components.Values)
				{
					const auto Entity = Other.contains(Pair.Entity) ? Pair.Entity : Other.generate(Pair.Entity);

					if constexpr (std::tuple_size_v<decltype(Storage.get_as_tuple({})) > == 0u)
					{
						// To be honest I don't know what this contexpr branch is about. Copied it form the basic_loader
						Storage.emplace(Entity);
					}
					else
					{
						Storage.emplace(Entity, std::move(Pair.Component));
					}
				}

			}
			
			return *this;
		}

	private:
		RegistryType* Reg;
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
			
			cereal::JSONOutputArchive Output{ Filestream };

			//TODO maybe someday we automatically detect the things that go into this list
            CerealJsonSnapshot{ Registry }
				.Get<entt::entity>(Output, "EntityId")
				.Get<Physics::RigidBodyStateComponent>(Output, "RigidBodyStateComponent")
				.Get<Physics::RigidBodyGravityComponent>(Output, "RigidBodyGravityComponent")
				.Get<Physics::SpringPotentialComponent>(Output, "SpringComponent");
		}

		Filestream.close();

		return true;
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
			cereal::JSONInputArchive Input{ Filestream };
		
			CerealJsonSnapshotLoader{ Registry }
				.Get<entt::entity>(Input, "EntityId")
				.Get<Physics::RigidBodyStateComponent>(Input, "RigidBodyStateComponent")
				.Get<Physics::RigidBodyGravityComponent>(Input, "RigidBodyGravityComponent")
				.Get<Physics::SpringPotentialComponent>(Input, "SpringComponent");
		}

		Filestream.close();
		
		return true;
	}
}