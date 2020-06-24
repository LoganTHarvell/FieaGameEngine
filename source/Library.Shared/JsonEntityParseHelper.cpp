#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Standard
#include <string>

// Header
#include "JsonEntityParseHelper.h"

// First Party
#include "Utility.h"
#include "Factory.h"
#pragma endregion Includes


using namespace std::string_literals;


namespace Library
{
#pragma region Constants
	const HashMap<std::string, Entity::Types> JsonEntityParseHelper::TypeStringMap =
	{
		{ "integer", Entity::Types::Integer },
		{ "float", Entity::Types::Float },
		{ "vector", Entity::Types::Vector },
		{ "matrix", Entity::Types::Matrix },
		{ "string", Entity::Types::String },
	};
#pragma endregion Constants

#pragma region Shared Data Special Members
	JsonEntityParseHelper::SharedData::SharedData(Entity& scope)
	{
		mRootEntity = &scope;
	}

	JsonEntityParseHelper::SharedData::~SharedData()
	{
		if (mOwnsEntity)
		{
			delete mRootEntity;
			mRootEntity = nullptr;
			mOwnsEntity = false;
		}

		mStack.Clear();
	}

	JsonEntityParseHelper::SharedData::SharedData(SharedData&& rhs) noexcept : JsonParseMaster::SharedData(std::move(rhs)),
		mRootEntity(rhs.mRootEntity), mOwnsEntity(rhs.mOwnsEntity), mStack(std::move(rhs.mStack))
	{
		rhs.mRootEntity = nullptr;
		rhs.mOwnsEntity = false;
	}

	JsonEntityParseHelper::SharedData& JsonEntityParseHelper::SharedData::operator=(SharedData&& rhs) noexcept
	{
		mRootEntity = rhs.mRootEntity;
		mOwnsEntity = rhs.mOwnsEntity;
		mStack = std::move(rhs.mStack);

		rhs.mRootEntity = nullptr;
		rhs.mOwnsEntity = false;

		JsonParseMaster::SharedData::operator=(std::move(rhs));

		return *this;
	}
#pragma endregion Shared Data Special Members

#pragma region Parser Methods
	void JsonEntityParseHelper::SharedData::PreParse()
	{
		JsonParseMaster::SharedData::PreParse();
		
		if (mRootEntity == nullptr)
		{
			mRootEntity = new Entity();
			mOwnsEntity = true;
			assert(mRootEntity);
		}

		mStack.Clear();
	}
#pragma endregion Parser Methods

#pragma region Shared Data Accessors
	const Entity& JsonEntityParseHelper::SharedData::GetEntity() const
	{
		return *mRootEntity;
	}

	void JsonEntityParseHelper::SharedData::SetEntity(Entity& scope)
	{
		if (mOwnsEntity)
		{
			delete mRootEntity;
			mOwnsEntity = false;
		}

		mRootEntity = &scope;
	}

	gsl::owner<Entity*> JsonEntityParseHelper::SharedData::TransferEntity()
	{
		if (!mOwnsEntity) return nullptr;
		
		const gsl::owner<Entity*> disownedEntity = mRootEntity;
		
		mRootEntity = nullptr;
		mOwnsEntity = false;

		return disownedEntity;
	}
#pragma endregion Shared Data Accessors

#pragma region Handlers
	bool JsonEntityParseHelper::StartHandler(JsonParseMaster::SharedData& data, const std::string& key, const Json::Value& value)
	{
		SharedData* helperData = data.As<SharedData>();
		if (helperData == nullptr) return false;

		if (helperData->mStack.IsEmpty())
		{
			if (!value.isObject() || !value.isMember("type"s) || !value.isMember("value"s)) return false;
			
			helperData->mStack.Push({ key, Entity::Types::Unknown, "Entity"s, nullptr, *helperData->mRootEntity });
			return true;
		}
		
		bool handled = false;
		StackFrame& stackFrame = helperData->mStack.Top();
		const std::string formattedKey = String::ToLower(key);
		
		if (formattedKey == "type"s && value.isString())
		{
			const std::string& valueStr = value.asString();
			const auto it = TypeStringMap.Find(valueStr);

			if (it != TypeStringMap.end())
			{
				stackFrame.Type = it->second;
				handled = true;
			}
			else if (Factory<Entity>::IsRegistered(valueStr))
			{
				stackFrame.ClassName = valueStr;
				stackFrame.Type = Entity::Types::Scope;
				handled = true;
			}
			else
			{
				throw std::runtime_error("\""s + valueStr + "\" is not a valid type."s);
			}
		}
		else if (formattedKey == "value"s)
		{
			if (value.isString() || value.isObject() || value.isInt() || value.isDouble())
			{
				stackFrame.Value = &value;

				if (value.isObject())
				{
					const Scope::Data* scopeData = stackFrame.Context.Find(stackFrame.Key);
					
					if (!scopeData || stackFrame.ArrayIndex >= scopeData->Size())
					{
						stackFrame.Context.CreateChild(stackFrame.ClassName, stackFrame.Key);
					}

					++stackFrame.ArrayIndex;
				}

				handled = true;
			}
			else if (value.isArray() && !value.empty())
			{
				for (const auto& v1 : value)
				{
					if (!v1.isString() && !v1.isObject() && !v1.isInt() && !v1.isDouble())
					{
						throw std::runtime_error("Invalid array value type."s);
					}

					for (const auto& v2 : value)
					{
						if (v1.type() != v2.type())
						{
							throw std::runtime_error("Mismatched array value types."s);
						}
					}
				}

				stackFrame.Value = &value;
				handled = true;
			}
			else
			{
				throw std::runtime_error("Invalid value type."s);
			}
		}
		else if (value.isObject())
		{			
			Entity::Data& entityData = *stackFrame.Context.Find(stackFrame.Key);

			assert(entityData.Type() == Entity::Types::Scope);
			assert(entityData[stackFrame.ArrayIndex-1].Is(Entity::TypeIdClass()));
			
			Entity* entity = static_cast<Entity*>(entityData.Get<Scope*>(entityData.Size() - 1));
			
			helperData->mStack.Push({ key, Entity::Types::Unknown, "Entity"s, nullptr, *entity });
			handled = true;
		}

		return handled;
	}

	bool JsonEntityParseHelper::EndHandler(JsonParseMaster::SharedData& data, const std::string& key)
	{
		assert(data.Is(SharedData::TypeIdClass()));

		SharedData* testHelperData = data.As<SharedData>();
		assert(!testHelperData->mStack.IsEmpty());

		const std::string formattedKey = String::ToLower(key);
		if (formattedKey == "type"s || formattedKey == "value"s) return true;

		const StackFrame& stackFrame = testHelperData->mStack.Top();
		
		bool handled = false;

		if (stackFrame.Type != Entity::Types::Unknown)
		{
			if (stackFrame.Value == nullptr)
			{
				auto& scopeData = stackFrame.Context[stackFrame.Key];

				if (scopeData.Type() == Entity::Types::Unknown)
				{
					scopeData.SetType(stackFrame.Type);
				}

				handled = true;
			}
			else if (stackFrame.Value->isObject())
			{
				handled = true;
			}
			else if (stackFrame.Value->isArray())
			{
				auto& scopeData = stackFrame.Context[stackFrame.Key];

				if (scopeData.Type() == Entity::Types::Unknown)
				{
					scopeData.SetType(stackFrame.Type);
				}

				if (scopeData.HasInternalStorage())
				{
					scopeData.Resize(stackFrame.Value->size());
				}
				else if (scopeData.Size() < stackFrame.Value->size())
				{
					throw std::runtime_error("\""s + stackFrame.Key + "\" array has too many elements."s);
				}
				
				for (Json::Value::ArrayIndex i = 0; i < stackFrame.Value->size(); ++i)
				{
					const auto& v = stackFrame.Value->get(i, 0);

					if (stackFrame.Type == Entity::Types::Integer)
					{
						scopeData.Set(v.asInt(), i);
					}
					else if (stackFrame.Type == Entity::Types::Float)
					{
						scopeData.Set(static_cast<float>(v.asDouble()), i);
					}
					else
					{
						assert(v.isString());
						scopeData.SetFromString(v.asString(), i);
					}
				}
				
				handled = true;
			}
			else
			{
				auto& scopeData = stackFrame.Context[stackFrame.Key];

				if (scopeData.Type() == Entity::Types::Unknown)
				{
					scopeData.SetType(stackFrame.Type);
				}

				if (scopeData.HasInternalStorage())
				{
					scopeData.Resize(1);
				}

				if (stackFrame.Type == Entity::Types::Integer)
				{
					scopeData.Set(stackFrame.Value->asInt(), scopeData.Size() - 1);
				}
				else if (stackFrame.Type == Entity::Types::Float)
				{
					scopeData.Set(static_cast<float>(stackFrame.Value->asDouble()), scopeData.Size() - 1);
				}
				else
				{
					assert(stackFrame.Value->isString());
					scopeData.SetFromString(stackFrame.Value->asString(), scopeData.Size() - 1);
				}

				handled = true;
			}
		}

 		testHelperData->mStack.Pop();
		return handled;
	}
#pragma endregion Handlers
}
