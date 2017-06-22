#pragma once

#define UNICODE
#define _UNICODE

#include <d3dx9.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

class GraphicsDevice
{
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 device;


public:
	GraphicsDevice();
	~GraphicsDevice();

	bool Initialize(HWND hWnd, bool windowed);
	void Begin() { device->BeginScene(); }
	void End() { device->EndScene(); }
	void Present() { device->Present(NULL, NULL, NULL, NULL); }
	void Clear(D3DCOLOR color = D3DCOLOR_XRGB(0, 0, 0)) { device->Clear(0, NULL, D3DCLEAR_TARGET, color, 1.0f, 0); } //todo zbuffer??

	LPDIRECT3DDEVICE9 GetDevice() { return device; }

};

