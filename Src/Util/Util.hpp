#pragma once
#include <assert.h>
#include <string>
#include <glm/glm.hpp>
#include <Foundation/Foundation.hpp>
#include "Renderer/DiligentRenderer.hpp"

class FUtil
{
public:
	template <typename T>
	static void CopyDataToBuffer(Diligent::RefCntAutoPtr<Diligent::IDeviceContext> Context, 
		T* SrcData,
		Diligent::RefCntAutoPtr<Diligent::IBuffer> TargetBuffer)
	{
		void* pMappedData;
		Context->MapBuffer(TargetBuffer,
			Diligent::MAP_TYPE::MAP_WRITE,
			Diligent::MAP_FLAGS::MAP_FLAG_DISCARD,
			pMappedData);
		memcpy(pMappedData, SrcData, sizeof(T));
		Context->UnmapBuffer(TargetBuffer, Diligent::MAP_TYPE::MAP_WRITE);
	}

	static std::array<glm::vec2, 4> ScreenQuad() noexcept;

	template <typename T>
	static T Back(std::vector<T> Values)
	{
		assert(Values.empty() == false);
		for (size_t i = 0; i < Values.size(); i++)
		{
			if (Values[i])
			{
				return Values[i];
			}
		}
		return Values[0];
	}
};