#pragma once

#include "IJsonParseHelper.h"
#include "Stack.h"

namespace UnitTests
{
	
	class JsonParseTestHelper : public Library::IJsonParseHelper
	{ 
	
	public:
		class SharedData : public Library::JsonParseMaster::SharedData
		{
			RTTI_DECLARATIONS(SharedData, Library::JsonParseMaster::SharedData)

			friend JsonParseTestHelper;

#pragma region Shared Data Special Member Functions
		public:
			/// <summary>
			/// Default constructor.
			/// </summary>
			SharedData() = default;

			/// <summary>
			/// Default destructor.
			/// </summary>
			virtual ~SharedData() = default;

			/// <summary>
			/// Copy constructor.
			/// </summary>
			SharedData(const SharedData & rhs) = default;

			/// <summary>
			/// Copy assignment operator.
			/// </summary>
			SharedData& operator=(const SharedData & rhs) = default;

			/// <summary>
			/// Move constructor.
			/// </summary>
			/// <param name="rhs">Shared to be moved.</param>
			/// <remarks>Sets SharedData for moved instance's master to this instance, if it exists.</remarks>
			SharedData(SharedData&& rhs);

			/// <summary>
			/// Move assignment operator.
			/// </summary>
			/// <param name="rhs">Shared to be moved.</param>
			/// <remarks>Sets SharedData for moved instance's master to this instance, if it exists.</remarks>
			SharedData& operator=(SharedData&& rhs);
#pragma endregion Shared Data Special Member Functions

		public:
			virtual gsl::owner<Library::JsonParseMaster::SharedData*> Create() const override;
			virtual void Initialize() override;

		public:
			int GetInt() const;
			float GetFloat() const;
			const std::string& GetString() const;
			const gsl::span<const int> GetArray() const;

		private:
			int mInt{ 0 };
			float mFloat{ 0 };
			std::string mString;
			int mArray[3]{ 0 };

			std::size_t arrayFillCount{ 0 };
		};

	public:
		virtual gsl::owner<IJsonParseHelper*> Create() const override;
		virtual bool StartHandler(Library::JsonParseMaster::SharedData& data, const std::string& key, const Json::Value& value, bool isArray) override;
		virtual bool EndHandler(Library::JsonParseMaster::SharedData& data, const std::string& key) override;

	private:
		Library::Stack<std::pair<const std::string&, const Json::Value&>> mStack;
	};
}

