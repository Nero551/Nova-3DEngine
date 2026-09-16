#include "VertexArray.hpp"

#include "ArrayBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Math/Vector/Vector3.hpp"
#include "Math/Vector/Vector4.hpp"

namespace N
{
VertexArray& VertexArray::operator=(VertexArray&& Other) noexcept {}
void VertexArray::Generate()
{
    if (IsGenerated())
    {
        return;
    }
    glCreateVertexArrays(1, &m_Id);
}

bool VertexArray::IsGenerated()
{
    return m_Id != 0;
}

void VertexArray::Delete()
{
    glDeleteVertexArrays(1, &m_Id);
    m_Id = 0;
}

VertexArray::~VertexArray()
{
    Delete();
}

void VertexArray::Bind()
{
    Generate();
    glBindVertexArray(m_Id);
}

void VertexArray::Unbind()
{
    glBindVertexArray(0);
}

void VertexArray::SetVertexBuffer(const ArrayBuffer& vbo, int bindingIndex, int stride, int offset)
{
    Generate();
    glVertexArrayVertexBuffer(m_Id, bindingIndex, vbo.GetId(), offset, stride);
}

void VertexArray::SetIndexBuffer(const IndexBuffer& ebo)
{
    Generate();
    glVertexArrayElementBuffer(m_Id, ebo.GetId());
}

void VertexArray::SetAttribPointer(
    int index, int size, DataType type, int bindingIndex, size_t offset, bool normalized)
{
    Generate();
    glEnableVertexArrayAttrib(m_Id, index);
    glVertexArrayAttribFormat(m_Id, index, size, static_cast<GLenum>(type), normalized, offset);
    glVertexArrayAttribBinding(m_Id, index, bindingIndex);
}

void VertexArray::SetMatrix3AttribPointer(int startIndex, int offset, int bindingIndex)
{
    SetAttribPointer(startIndex, 3, DataType::Float, bindingIndex, 0 + offset);
    SetAttribPointer(startIndex + 1, 3, DataType::Float, bindingIndex, sizeof(M::Vector3) + offset);
    SetAttribPointer(startIndex + 2, 3, DataType::Float, bindingIndex, 2 * sizeof(M::Vector3) + offset);
}

void VertexArray::SetMatrix4AttribPointer(int startIndex, int offset, int bindingIndex)
{
    SetAttribPointer(startIndex, 4, DataType::Float, bindingIndex, 0 + offset);
    SetAttribPointer(startIndex + 1, 4, DataType::Float, bindingIndex, sizeof(M::Vector4) + offset);
    SetAttribPointer(startIndex + 2, 4, DataType::Float, bindingIndex, 2 * sizeof(M::Vector4) + offset);
    SetAttribPointer(startIndex + 3, 4, DataType::Float, bindingIndex, 3 * sizeof(M::Vector4) + offset);
}

void VertexArray::SetAttribDivisor(int bindingIndex, int divisor)
{
    Generate();
    glVertexArrayBindingDivisor(m_Id, bindingIndex, divisor);
}

unsigned int VertexArray::GetId() const
{
    return m_Id;
}
} // namespace N
