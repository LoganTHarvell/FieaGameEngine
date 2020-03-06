#pragma region Includes
// Pre-compiled Header
#include "pch.h"

// Header
#include "JsonScopeParseHelper.h"

// First Party
#include "Utility.h"
#pragma endregion Includes

namespace Library
{
#pragma region Constants
	const HashMap<std::string, Scope::Types> JsonScopeParseHelper::TypeStringMap =
	{
		{ "integer", Scope::Types::Integer },
		{ "float", Scope::Types::Float },
		{ "vector", Scope::Types::Vector },
		{ "matrix", Scope::Types::Matrix },
		{ "string", Scope::Types::String },
		{ "scope", Scope::Types::Scope },
	};
#pragma endregion Constants

#pragma region Shared Data Special Members
	JsonScopeParseHelper::SharedData::SharedData(Scope& scope)
	{
		mRootScope = &scope;
	}

	JsonScopeParseHelper::SharedData::~SharedData()
	{
		if (mOwnsScope)
		{
			delete mRootScope;
			mRootScope = nullptr;
			mOwnsScope = false;
		}

		mStack.Clear();
	}

	JsonScopeParseHelper::SharedData::SharedData(SharedData&& rhs) noexcept : 
		mRootScope(rhs.mRootScope), mOwnsScope(rhs.mOwnsScope), mStack(std::move(rhs.mStack)), JsonParseMaster::SharedData(std::move(rhs))
	{
		rhs.mRootScope = nullptr;
		rhs.mOwnsScope = false;
	}

	JsonScopeParseHelper::SharedData& JsonScopeParseHelper::SharedData::operator=(SharedData&& rhs) noexcept
	{
		mRootScope = rhs.mRootScope;
		mOwnsScope = rhs.mOwnsScope;
		mStack = std::move(rhs.mStack);

		JsonParseMaster::SharedData::operator=(std::move(rhs));

		rhs.mRootScope = nullptr;
		rhs.mOwnsScope = false;

		return *this;
	}
#pragma endregion Shared Data Special Members

#pragma region Parser Methods
	void JsonScopeParseHelper::SharedData::Initialize()
	{
		JsonParseMaster::SharedData::Initialize();
		
		if (mRootScope == nullptr)
		{
			mRootScope = new Scope();
			mOwnsScope = true;
			assert(mRootScope);
		}

		mStack.Clear();
	}
#pragma endregion Parser Methods

#pragma region Shared Data Accessors
	const Scope& JsonScopeParseHelper::SharedData::GetScope() const
	{
		return *mRootScope;
	}

	void JsonScopeParseHelper::SharedData::SetScope(Scope& scope)
	{
		if (mOwnsScope)
		{
			delete mRootScope;
			mOwnsScope = false;
		}

		mRootScope = &scope;
	}

	gsl::owner<Scope*> JsonScopeParseHelper::SharedData::TransferScope()
	{
		if (!mOwnsScope) return nullptr;
		
		gsl::owner<Scope*> disownedScope = mRootScope;
		
		mRootScope = nullptr;
		mOwnsScope = false;

		return disownedScope;
	}
#pragma endregion Shared Data Accessors

#pragma region Handlers
	bool JsonScopeParseHelper::StartHandler(JsonParseMaster::SharedData& data, const std::string& key, const Json::Value& value, bool)
	{
		SharedData* testHelperData = data.As<SharedData>();
		if (testHelperData == nullptr) return false;

		std::string formattedKey = String::ToLower(key);

		StackFrame* stackFrame = nullptr;
		if (!testHelperData->mStack.IsEmpty()) stackFrame = &testHelperData->mStack.Top();

		bool handled = false;

		if (formattedKey == "type" && value.isString())
		{
			auto it = TypeStringMap.Find(value.asString());

			if (it != TypeStringMap.end())
			{
				assert(stackFrame);

				stackFrame->Type = it->second;
				handled = true;
			}
		}
		else if (formattedKey == "value")
		{
			assert(stackFrame);

			if (value.isString() || value.isObject() || value.isInt() || value.isDouble())
			{
				stackFrame->Value = &value;

				if (value.isObject())
				{
					stackFrame->Context.AppendScope(stackFrame->Key);
				}

				handled = true;
			}
			else if (value.isArray() && value.size() > 0)
			{
				handled = true;

				for (const auto& v1 : value)
				{
					if (!v1.isString() && !v1.isObject() && !v1.isInt() && !v1.isDouble())
					{
						handled = false;
						break;
					}

					for (const auto& v2 : value)
					{
						if (v1.type() != v2.type())
						{
							handled = false;
							break;
						}
					}
				}

				if (handled)
				{
					stackFrame->Value = &value;
				}
			}
			else
			{
				stackFrame->Type = Scope::Types::Unknown;
			}
		}
		else if (value.isObject())
		{
			Scope* scope;

			if (stackFrame)
			{
				Scope::DataType& scopeData = *stackFrame->Context.Find(stackFrame->Key);
				scope = &scopeData[scopeData.Size() - 1];
			}
			else
			{
				scope = testHelperData->mRootScope;
			}

			testHelperData->mStack.Push({ key, nullptr, Scope::Types::Unknown, *scope });
			handled = true;
		}

		return handled;
	}

	bool JsonScopeParseHelper::EndHandler(JsonParseMaster::SharedData& data, const std::string& key)
	{
		assert(data.Is(SharedData::TypeIdClass()));

		SharedData* testHelperData = data.As<SharedData>();
		assert(!testHelperData->mStack.IsEmpty());

		std::string formattedKey = String::ToLower(key);
		if (formattedKey == "type" || formattedKey == "value") return true;

		const StackFrame& stackFrame = testHelperData->mStack.Top();
		
		bool handled = false;

		if (stackFrame.Type != Scope::Types::Unknown && stackFrame.Value != nullptr)
		{
			if (stackFrame.Value->isObject())
			{
				handled = true;
			}
			else if (stackFrame.Value->isArray())
			{
				for (const auto& v : *stackFrame.Value)
				{
					if (!v.isArray())
					{
						auto& scopeData = stackFrame.Context[stackFrame.Key];

						if (scopeData.Type() == Scope::Types::Unknown)
						{
							scopeData.SetType(stackFrame.Type);
						}

						scopeData.Resize(scopeData.Size() + 1);

						if (stackFrame.Type == Scope::Types::Integer)
						{
							scopeData.Set(v.asInt(), scopeData.Size() - 1);
						}
						else if (stackFrame.Type == Scope::Types::Float)
						{
							scopeData.Set(static_cast<float>(v.asDouble()), scopeData.Size() - 1);
						}
						else
						{
							assert(v.isString());
							scopeData.SetFromString(v.asString(), scopeData.Size() - 1);
						}
						
						handled = true;
					}
				}
			}
			else
			{
				auto& scopeData = stackFrame.Context[stackFrame.Key];

				if (scopeData.Type() == Scope::Types::Unknown)
				{
					scopeData.SetType(stackFrame.Type);
				}

				scopeData.Resize(scopeData.Size() + 1);

				if (stackFrame.Type == Scope::Types::Integer)
				{
					scopeData.Set(stackFrame.Value->asInt(), scopeData.Size() - 1);
				}
				else if (stackFrame.Type == Scope::Types::Float)
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
