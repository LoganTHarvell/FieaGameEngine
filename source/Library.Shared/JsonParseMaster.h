#pragma once

#pragma region Includes
// Standard
#include <stdint.h>

// Third Party
#pragma warning(disable : 26812)
#include <json/json.h>
#pragma warning(default : 26812)

#include <gsl/gsl>

// First Party
#include "RTTI.h"
#include "Vector.h"
#pragma endregion Includes

namespace Library
{
	class IJsonParseHelper;
	
	/// <summary>
	/// JSON parser master class for managing IJsonParserHelper handles and parsing JSON data.
	/// </summary>
	class JsonParseMaster final
	{
#pragma region Shared Data
public:
		class SharedData : public RTTI
		{
			RTTI_DECLARATIONS(SharedData, RTTI)

			friend JsonParseMaster;

#pragma region Special Member Functions
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
			SharedData(const SharedData&) = delete;

			/// <summary>
			/// Copy assignment operator.
			/// </summary>
			SharedData& operator=(const SharedData&) = delete;

			/// <summary>
			/// Move constructor.
			/// </summary>
			SharedData(SharedData&&) = delete;

			/// <summary>
			/// Move assignment operator.
			/// </summary>
			SharedData& operator=(SharedData&&) = delete;
#pragma endregion Special Member Functions

#pragma region Virtual Constructor
		public:
			/// <summary>
			/// Virtual constructor.
			/// Creates a heap allocated instance of SharedData.
			/// </summary>
			/// <returns>Pointer to the new SharedData instance.</returns>
			virtual gsl::owner<SharedData*> Create() const = 0;
#pragma endregion Virtual Constructor
			
#pragma region Accessors
			JsonParseMaster* GetJsonParseMaster();
			const JsonParseMaster* GetJsonParseMaster() const;
			std::uint16_t Depth() const;
#pragma endregion Accessors

#pragma region Modifiers
		public:
			void SetJsonParseMaster(JsonParseMaster* master);
			virtual void Initialize() {};
			void IncrementDepth();
			void DecrementDepth();
#pragma endregion Modifiers
					
#pragma region Data Members
		private:
			JsonParseMaster* mMaster{ nullptr };
			std::uint16_t mDepth{ 0 };
#pragma endregion Data Members
		};
#pragma endregion Shared Data

#pragma region Special Member Functions
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="sharedData">Pointer to an instance of a SharedData subclass.</param>
		JsonParseMaster(SharedData* sharedData=nullptr);

		/// <summary>
		/// Default destructor.
		/// </summary>
		~JsonParseMaster();

		/// <summary>
		/// Copy constructor.
		/// </summary>
		JsonParseMaster(const JsonParseMaster&) = delete;

		/// <summary>
		/// Copy assignment operator.
		/// </summary>
		JsonParseMaster& operator=(const JsonParseMaster&) = delete;

		/// <summary>
		/// Move constructor.
		/// </summary>
		/// <param name="rhs">JsonParseMaster to be moved.</param>
		JsonParseMaster(JsonParseMaster&& rhs) = default;

		/// <summary>
		/// Move assignment operator.
		/// </summary>
		/// <param name="rhs">JsonParseMaster to be moved.</param>
		JsonParseMaster& operator=( JsonParseMaster&& rhs) = default;
#pragma endregion Special Member Functions

#pragma region Virtual Copy Constructor
	public:
		/// <summary>
		/// Virtual copy constructor.
		/// Creates a JsonParseMaster instance.
		/// Creates new heap allocated SharedData and Helper instances from the original instances.
		/// </summary>
		/// <returns>Pointer to the new JsonParseMaster instance.</returns>
		gsl::owner<JsonParseMaster*> Clone() const;
#pragma endregion Virtual Copy Constructor

#pragma region Accessors
		/// <summary>
		/// Gets the filename of the last file parsed.
		/// </summary>
		/// <returns>Reference to the filename std::string instance.</returns>
		const std::string& GetFilename() const;

		/// <summary>
		/// Gets the SharedData instance associated with the JsonParseMaster.
		/// </summary>
		/// <returns>Pointer to the SharedData instance.</returns>
		SharedData* GetSharedData();

		/// <summary>
		/// Gets the SharedData instance associated with the JsonParseMaster.
		/// </summary>
		/// <returns>Pointer to the SharedData instance.</returns>
		const SharedData* GetSharedData() const;
#pragma endregion Accessors

#pragma region Modifiers
	public:
		/// <summary>
		/// Gets the SharedData instance associated with the JsonParseMaster.
		/// </summary>
		/// <param name="sharedData">Reference to a SharedData instance.</param>
		void SetSharedData(SharedData& sharedData);

		/// <summary>
		/// Adds a parse helper to the JsonParseMaster.
		/// </summary>
		/// <param name="helper">Reference to the IJsonParseHelper subclass.</param>
		void AddHelper(IJsonParseHelper& helper);

		/// <summary>
		/// Removes a parse helper to the JsonParseMaster.
		/// </summary>
		/// <param name="helper">Reference to the IJsonParseHelper subclass.</param>
		bool RemoveHelper(IJsonParseHelper& helper);
#pragma endregion Modifiers
		
#pragma region Parse Methods
	public:
		/// <summary>
		/// Parses a JSON string.
		/// </summary>
		/// <param name="string">JSON string to be parsed.</param>
		void Parse(const std::string& string);

		/// <summary>
		/// Parses an input stream containing JSON data.
		/// </summary>
		/// <param name="inputStream">A std::istream instance containing JSON data.</param>
		void Parse(std::istream& inputStream);

		/// <summary>
		/// Parses a JSON file.
		/// </summary>
		/// <param name="filename">Filename of the JSON file to be parsed.</param>
		void ParseFromFile(const std::string& filename);
#pragma endregion Parse Methods

#pragma region Parse Helper Methods
	private:
		/// <summary>
		/// Iterates through the members of JSON value object and parses the members.
		/// </summary>
		/// <param name="value">Reference to a JSON value object to be parsed.</param>
		void ParseMembers(const Json::Value& value);

		/// <summary>
		/// Parses a JSON value.
		/// </summary>
		/// <param name="key">Key associated with the JSON value.</param>
		/// <param name="value">Reference to the JSON value.</param>
		/// <param name="isArray">Boolean representing if the JSON value is an array.</param>
		void Parse(const std::string& key, const Json::Value& value, bool isArray);
#pragma endregion Parse Helper Methods

#pragma region Data Members
	private:
		/// <summary>
		/// Pointer to a SharedData subclass instance to be filled with parsed data.
		/// </summary>
		SharedData* mSharedData;

		/// <summary>
		/// List of pointers to parse helpers subclasses.
		/// </summary>
		Vector<IJsonParseHelper*> mHelpers;

		/// <summary>
		/// Filename of the last file parsed.
		/// </summary>
		const std::string* mFilename{ nullptr };

		/// <summary>
		/// Describes if the SharedData is owned by the JsonParseMaster due to cloning.
		/// </summary>
		bool mOwnsSharedData{ false };

		/// <summary>
		/// List of indices in the list of helpers that contain owned Helper instances.
		/// </summary>
		Vector<std::size_t> mOwnedHelperIndices;
#pragma endregion Data Members
	};
}

#include "JsonParseMaster.inl"
