#pragma once
#include <vector>
#include "DirectXMath.h"

class VertexLayout
{
public:
	enum ElementType
	{
		Position3D,
		Texture2D,
		Normal
	};

	struct Element
	{
		ElementType type;
		size_t offset;

		Element(ElementType type, size_t offset)
			: type(type), offset(offset)
		{
		}

		size_t getByteSize() const
		{
			switch (type)
			{
			case Position3D: case Normal:
				return sizeof(DirectX::XMFLOAT3);

			case Texture2D:
				return sizeof(DirectX::XMFLOAT2);
			}
		}
	};

	template<ElementType type>
	const Element* findElementOfType() const noexcept
	{
		auto retIt = std::find_if(elements.begin(), elements.end(),
			[](const Element& e) -> bool
			{
				return e.type == type;
			});

		if (retIt != elements.end()) return &(*retIt);

		return nullptr;
	}

	template<ElementType type>
	VertexLayout& append() noexcept
	{
		size_t newOffset = elements.empty() ? 0u : elements.back().offset + elements.back().getByteSize();
		elements.emplace_back(type, newOffset);

		return *this;
	}

	size_t getByteSize() const noexcept
	{
		const auto& lastElement = elements.back();
		return lastElement.offset + lastElement.getByteSize();
	}

	const Element& getElementAt(int index) const noexcept
	{
		assert(0 <= index && index < elements.size() && "Vertex Element at this index does not exist");

		return elements[index];
	}

private:
	std::vector<Element> elements;
};

class Vertex
{
public:
	Vertex(char* dataPtr, const VertexLayout& layout)
		: data(dataPtr), layout(layout)
	{
	}

	template<VertexLayout::ElementType type>
	auto* getElementAsXMFLOAT() noexcept
	{
		auto element = layout.findElementOfType<type>();
		char* elementData = nullptr;

		if (element)
			elementData = data + element->offset;

		if constexpr (type == VertexLayout::Position3D || type == VertexLayout::Normal)
			return reinterpret_cast<DirectX::XMFLOAT3*>(elementData);

		if constexpr (type == VertexLayout::Texture2D)
			return reinterpret_cast<DirectX::XMFLOAT2*>(elementData);
	}

	template<typename To, typename From>
	static void setElementAs(char* elementPtr, From&& newData) noexcept
	{
		// if you look inside setElementAs, this function should be compiled
		// for every case of To and From, for example, To = XMFloat3 and From = XMFloat2.
		// If you don't do compile time branch here, the assignment below doesn't have
		// overloads for such cases as above. So we make functions for each branch;
		// a function for when the assignment can happen, and another function (else)
		// for when the overload cannot be found.
		if constexpr (std::is_assignable<To, From>::value)
			*reinterpret_cast<To*>(elementPtr) = std::forward<From>(newData);
		else
			assert(0 && "type mismatch");
	}

	template<typename From>
	void setElementAt(int index, From&& newData) noexcept
	{
		auto& element = layout.getElementAt(index);
		auto elementPtr = data + element.offset;

		switch (element.type)
		{
		case VertexLayout::Position3D:
		case VertexLayout::Normal:
			setElementAs<DirectX::XMFLOAT3>(elementPtr, std::forward<From>(newData));
			break;

		case VertexLayout::Texture2D:
			setElementAs<DirectX::XMFLOAT2>(elementPtr, std::forward<From>(newData));
			break;

		default:
			break;
		}
	}

	template<typename First, typename ...Rest>
	void setElementAt(int index, First&& first, Rest&& ...rest) noexcept
	{
		setElementAt(index, std::forward<First>(first));
		setElementAt(index + 1, std::forward<Rest>(rest)...);
	}

private:
	char* data;
	const VertexLayout& layout;
};

class VertexArray
{
public:
	std::vector<char> buffer;
	VertexLayout layout;

	VertexArray(VertexLayout layout)
		: layout(layout)
	{
	}

	template<typename ...Params>
	void emplace_back(Params&& ...params) noexcept
	{
		// layout.getByteSize is the byte size of the vertex structure.
		// For example, Vertex { pos3, tex2 } is 20 bytes in size.
		buffer.resize(buffer.size() + layout.getByteSize());

		// we don't know what the layout looks like so each parameter
		// can be one of XMFLOAT3, XMFLOAT4, etc.
		// Use template recursion to figure out what element each parameter
		// corresponds to.
		back().setElementAt(0, std::forward<Params>(params)...);
	}

	Vertex back() noexcept
	{
		assert(!buffer.empty() && "VertexBuffer is empty and you tried to access the last vertex");

		int backIndex = buffer.size() - layout.getByteSize();
		char* backPtr = &buffer[backIndex];

		return { backPtr, layout };
	}
};