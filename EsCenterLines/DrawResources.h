#pragma once
#pragma comment(lib, "d2d1")
#include <Windows.h>
#include <d2d1.h>

struct DrawResources
{
	D2D1_RENDER_TARGET_PROPERTIES rt_properties = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		D2D1_ALPHA_MODE_IGNORE),
		0,
		0,
		D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE,
		D2D1_FEATURE_LEVEL_DEFAULT
	);

};