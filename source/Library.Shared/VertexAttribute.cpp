// Pre-compiled Header
#include "pch.h"

// Header
#include "VertexAttribute.h"

namespace Library
{
    VertexAttribute::VertexAttribute(
        const std::string&      name,
        const Library::Format   format,
        std::uint32_t           location,
        std::uint32_t           instanceDivisor,
        const ShaderSystemValue systemValue)
        :
        Name(name),
        FormatValue(format),
        Location(location),
        SystemValue(systemValue),
        InstanceDivisor(instanceDivisor)
    {
    }

    VertexAttribute::VertexAttribute(
        const std::string&      semanticName,
        std::uint32_t           semanticIndex,
        const Library::Format   format,
        std::uint32_t           location,
        std::uint32_t           instanceDivisor)
        :
        Name(semanticName),
        FormatValue(format),
        Location(location),
        SemanticIndex(semanticIndex),
        InstanceDivisor(instanceDivisor)
    {
    }

    VertexAttribute::VertexAttribute(
        const std::string&      name,
        const Library::Format   format,
        const std::uint32_t     location,
        const std::uint32_t     offset,
        const std::uint32_t     stride,
        const std::uint32_t     slot,
        const std::uint32_t     instanceDivisor)
        :
        VertexAttribute(name, 0, format, location, offset, stride, slot, instanceDivisor)
    {
    }

    VertexAttribute::VertexAttribute(
        const std::string&      semanticName,
        const std::uint32_t     semanticIndex,
        const Library::Format   format,
        const std::uint32_t     location,
        const std::uint32_t     offset,
        const std::uint32_t     stride,
        const std::uint32_t     slot,
        const std::uint32_t     instanceDivisor)
        :
        Name(semanticName),
        FormatValue(format),
        Location(location),
        SemanticIndex(semanticIndex),
        Slot(slot),
        Offset(offset),
        Stride(stride),
        InstanceDivisor(instanceDivisor)
    {
    }

    std::uint32_t VertexAttribute::GetSize() const
    {
        const auto& formatDesc = FormatDescMap[FormatValue];
        return ((formatDesc.Flags & FormatFlags::SupportsVertex) != 0) ? (formatDesc.BitSize / 8) : 0;
    }


    bool VertexAttribute::operator==(const VertexAttribute& rhs) const noexcept
    {
        return  Name == rhs.Name
    		&&  FormatValue == rhs.FormatValue
    		&&  Location == rhs.Location
    		&&  SemanticIndex == rhs.SemanticIndex
			&&  SystemValue == rhs.SystemValue
            &&  Slot == rhs.Slot
            &&  Offset == rhs.Offset
            &&  Stride == rhs.Stride
            &&  InstanceDivisor == rhs.InstanceDivisor;
    }

    bool VertexAttribute::operator!=(const VertexAttribute& rhs) const noexcept
    {
        return !operator==(rhs);
    }
}
