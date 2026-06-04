#pragma once
/**
 * @file BufferResource.h
 * @brief バッファリソースのクラス
 * @author 茂木翼
 */

#include <d3d12.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;

namespace Elysia {

	ComPtr<ID3D12Resource> CreateBufferResource(const size_t& sizeInBytes);

	
};