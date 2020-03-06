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
	const HashMap<std::string, Scope::DataType::Types> JsonScopeParseHelper::TypeStringMap =
	{
		{ "integer", Scope::DataType::Types::Integer },
		{ "float", Scope::DataType::Types::Float },
		{ "vector", Scope::DataType::Types::Vector },
		{ "matrix", Scope::DataType::Types::Matrix },
		{ "string", Scope::DataType::Types::String },
		{ "scope", Scope::DataType::Types::Scope },
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
		mRootScope(rhs.mRootScope), mOwnsScope(rhs.mOwnsScope), mStack(std::move(rhs.mStack))
	{
		rhs.mRootScope = nullptr;
		rhs.mOwnsScope = false;
	}

	JsonScopeParseHelper::SharedData& JsonScopeParseHelper::SharedData::operator=(SharedData&& rhs) noexcept
	{
		mRootScope = rhs.mRootScope;
		mOwnsScope = rhs.mOwnsScope;
		mStack = std::move(rhs.mStack);

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
		
		mRootScope = nullptr;
		mOwnsScope = false;
		return mRootScope;
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
		else if (formattedKey == "value" && (value.isString() || value.isObject() || value.isInt() || value.isDouble() || value.isArray()))
		{
			assert(stackFrame);

			stackFrame->Value = &value;
			handled = true;
		}
		else if (value.isObject())
		{
			Scope& scope = stackFrame ? stackFrame->Context.AppendScope(key) : *testHelperData->mRootScope;
			testHelperData->mStack.Push({ key, &value, Scope::DataType::Types::Unknown, scope });
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

 		if (stackFrame.Value->isObject())
 		{
			handled = true;
 		}
 		if (stackFrame.Value->isArray())
 		{
 			for (const auto& v : *stackFrame.Value)
 			{
 				if (v.isObject())
 				{
					handled = true;
 				}
				else if (!v.isArray())
				{
					if (v.isInt()) stackFrame.Context[stackFrame.Key].PushBack(v.asInt());
					else if (v.isDouble()) stackFrame.Context[stackFrame.Key].PushBack(static_cast<float>(v.asDouble()));
					else if (v.isString())
					{
						auto uninitialized = stackFrame.Context[stackFrame.Key];
						uninitialized.SetType(stackFrame.Type);
						uninitialized.SetFromString(v.asString(), uninitialized.Size()-1);
					}
					else
					{
						break;
					}
					
					handled = true;
				}
 			}
 		}
		else if (stackFrame.Type != Scope::DataType::Types::Unknown)
		{
			auto& scopeData = stackFrame.Context[stackFrame.Key];
			
			if (scopeData.Type() == Scope::DataType::Types::Unknown)
			{
				scopeData.SetType(stackFrame.Type);
			}

			scopeData.Resize(scopeData.Size() + 1);

			if (stackFrame.Type == Scope::DataType::Types::Integer)
			{
				scopeData.Set(stackFrame.Value->asInt(), scopeData.Size() - 1);
				handled = true;
			}
			else if (stackFrame.Type == Scope::DataType::Types::Float)
			{
				scopeData.Set(static_cast<float>(stackFrame.Value->asDouble()), scopeData.Size() - 1);
				handled = true;
			}
			else if (stackFrame.Value->isString())
			{
				scopeData.SetFromString(stackFrame.Value->asString(), scopeData.Size() - 1);
				handled = true;
			}
		}
 		testHelperData->mStack.Pop();

		return handled;
	}
#pragma endregion Handlers
}
