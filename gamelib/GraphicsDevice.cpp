#include "GraphicsDevice.h"

GraphicsDevice::GraphicsDevice() : device(NULL)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
}

GraphicsDevice::~GraphicsDevice() //todo release refactoring .. template or macro 
{
	if (device)
	{
		device->Release();
		device = NULL;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = NULL;
	}
}

bool GraphicsDevice::Initialize(HWND hWnd, bool windowed)
{
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = windowed;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;

	if (!device) // when constructed, device is NULL 
		if (FAILED
			(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_MIXED_VERTEXPROCESSING, &d3dpp, &device))
			) return false;
	else
		if (FAILED
			(device->Reset(&d3dpp))
			) return false;

	return true;
}