//
// Reusable class for setting up a DirectX11 Application (particularly, a game!)
// Justin Bonczek
//

#include "Game.h"
#include "Timer.h"
//
// Global Callback Function
//
Game* game = 0;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return game->MsgProc(hwnd, msg, wParam, lParam);
}

Game::Game(HINSTANCE hInstance) :
hInstance(hInstance),
windowWidth(1280),
windowHeight(720),
windowTitle(L"Default Title"),
hWnd(NULL),
dev(NULL),
devCon(NULL),
swapChain(0),
depthStencilBuffer(0),
renderTargetView(0),
depthStencilView(0),
deltaTime(0)
{
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	game = this;
}

Game::~Game(void)
{
	ReleaseMacro(depthStencilView);
	ReleaseMacro(depthStencilBuffer);
	ReleaseMacro(renderTargetView);
	ReleaseMacro(swapChain);

	if (devCon)
		devCon->ClearState();
	ReleaseMacro(devCon);
	ReleaseMacro(dev);
}

// Ensures everything is properly initialized
bool Game::Initialize(void)
{
	if (!InitializeWindow())
		return false;
	if (!InitializeDirect3D())
		return false;

	return true;
}

//
// Windows Programming bits: Create and show a window
//
bool Game::InitializeWindow(void)
{
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(WNDCLASS));
	wc.style		 = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WindowProc;
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon		 = LoadIcon(0, IDI_APPLICATION);
	wc.lpszMenuName	 = NULL;
	wc.lpszClassName = L"WindowClassMain";
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.cbWndExtra	 = NULL;
	wc.cbClsExtra	 = NULL;

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, L"RegisterClass failed.", NULL, NULL);
		return false;
	}

	RECT wr = { 0, 0, windowWidth, windowHeight };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindow(
		L"WindowClassMain",
		windowTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
	{
		MessageBox(NULL, L"CreateWindow Failed.", NULL, NULL);
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
}

bool Game::InitializeDirect3D()
{
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	scd.BufferCount		  = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width  = windowWidth;
	scd.BufferDesc.Height = windowHeight;
	scd.BufferUsage		  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow	  = hWnd;
	scd.Windowed		  = TRUE;
	scd.SwapEffect		  = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags			  = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.SampleDesc.Count  = 4;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	featureLevel = D3D_FEATURE_LEVEL_9_1;
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapChain,
		&dev,
		&featureLevel,
		&devCon);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"D3D11CreateDeviceAndSwapChain failed.", NULL, NULL);
		return false;
	}

	//swapChain->SetFullscreenState(TRUE, NULL);
	OnResize();
	return true;
}

float Game::AspectRatio() const
{
	return (float)windowWidth / windowHeight;
}

void Game::OnResize()
{
	ReleaseMacro(renderTargetView);
	ReleaseMacro(depthStencilView);
	ReleaseMacro(depthStencilBuffer);

	swapChain->ResizeBuffers(
		1,
		windowWidth,
		windowHeight,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		NULL);
	ID3D11Texture2D* backBuffer;
	swapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	dev->CreateRenderTargetView(backBuffer, 0, &renderTargetView);
	ReleaseMacro(backBuffer);

	D3D11_TEXTURE2D_DESC dsd;
	ZeroMemory(&dsd, sizeof(D3D11_TEXTURE2D_DESC));
	dsd.Width			 = windowWidth;
	dsd.Height			 = windowHeight;
	dsd.MipLevels		 = 1;
	dsd.ArraySize		 = 1;
	dsd.Format			 = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsd.Usage			 = D3D11_USAGE_DEFAULT;
	dsd.BindFlags		 = D3D11_BIND_DEPTH_STENCIL;
	dsd.CPUAccessFlags	 = NULL;
	dsd.MiscFlags		 = NULL;
	dsd.SampleDesc.Count = 4;

	dev->CreateTexture2D(&dsd, NULL, &depthStencilBuffer);
	dev->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);
	devCon->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)windowWidth;
	viewport.Height = (float)windowHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	devCon->RSSetViewports(1, &viewport);
}

int Game::Run()
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		Timer::StartFrame();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update(Timer::GetFrameTime());
			Draw();
		}

		Timer::StopFrame();
		deltaTime = Timer::GetFrameTime();
	}

	return (int)msg.wParam;
}

LRESULT Game::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		windowWidth = LOWORD(lParam);
		windowHeight = HIWORD(lParam);
		if (dev)
		{
			if (wParam = SIZE_MINIMIZED){}
			else
			{
				OnResize();
			}
		}
		return 0;
	case WM_ENTERSIZEMOVE:
		return 0;
	case WM_EXITSIZEMOVE:
		OnResize();
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_MENUCHAR:
		return MAKELRESULT(0, MNC_CLOSE);
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}