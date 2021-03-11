#pragma once
#include "Drawable.h"
#include "../Bindables/IndexBuffer.h"

namespace UniChili 
{
	/// <summary>
	/// In this class are the static portion of bindings.
	/// Add instance-specific bindings using Drawable member functions.
	/// </summary>
	template<class From>
	class DrawableBase : public Drawable
	{
	protected:
		static bool isStaticInit() noexcept
		{
			return !staticBindables.empty();
		}

		static void addStaticBindable(std::unique_ptr<Bindable> bindable) noexcept
		{
			staticBindables.push_back(std::move(bindable));
		}
		
		static void setStaticIndex(IndexBuffer* indexBuffer) noexcept
		{
			pStaticIndexBuffer = indexBuffer;
		}

		void setIndexFromStatic() noexcept
		{
			pIndexBuffer = pStaticIndexBuffer;
		}

	private:
		const std::vector<std::unique_ptr<Bindable>> &getStaticBindables() const noexcept override
		{
			return staticBindables;
		}

	private:
		static std::vector<std::unique_ptr<Bindable>> staticBindables;
		static IndexBuffer* pStaticIndexBuffer;
	};

	template<class From>
	std::vector<std::unique_ptr<Bindable>> DrawableBase<From>::staticBindables;

	template<class From>
	IndexBuffer* DrawableBase<From>::pStaticIndexBuffer;
}


