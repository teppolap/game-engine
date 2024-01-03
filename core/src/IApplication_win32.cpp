/**
 * ============================================================================
 *  Name        : IApplication_win32.cpp
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : Abstract application layer (win32)
 * ============================================================================
**/

#include "../include/IApplication.h"

// include all your renderers
#include "../include/OpenGLRenderer.h"

#if defined (_WINDOWS)

IApplication* IApplication::m_pApp = nullptr;


IApplication::IApplication() :
	m_Window(nullptr),
	m_bActive(false),
	m_iWidth(0),
	m_iHeight(0)
{
	m_pApp = this;
}


IApplication::~IApplication()
{
	m_pApp = nullptr;
}


bool IApplication::Create(int32_t resX, int32_t resY, const std::string& title)
{
	m_Window = MakeWindow(resX, resY, title);
	if (m_Window)
	{
		m_iWidth = resX;
		m_iHeight = resY;

		// create the renderer
		m_pRenderer = std::make_unique<OpenGLRenderer>();
		if (!m_pRenderer->Create())
		{
			return false;
		}

		// call pure virtual OnCreate
		if (OnCreate())
		{
			SetActive(true);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


bool IApplication::OnEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
		{
			SetActive(false);
		}
		else if (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED)
		{
			RECT rect;
			::GetClientRect(GetWindow(), &rect);
			const int32_t windowWidth = rect.right - rect.left;
			const int32_t windowHeight = rect.bottom - rect.top;

			if (windowWidth != m_iWidth || windowHeight != m_iHeight)
			{
				m_iWidth = windowWidth;
				m_iHeight = windowHeight;
				m_pRenderer->SetViewport({ 0, 0, m_iWidth, m_iHeight });
				OnScreenSizeChanged(m_iWidth, m_iHeight);
			}

			SetActive(true);
		}
		break;

	case WM_KEYDOWN:
		OnKeyDown((uint32_t)wParam);
		break;

	case WM_LBUTTONDOWN:
		OnMouseBegin(0, glm::vec2(LOWORD(lParam), HIWORD(lParam)));
		break;
	case WM_MBUTTONDOWN:
		OnMouseBegin(1, glm::vec2(LOWORD(lParam), HIWORD(lParam)));
		break;
	case WM_RBUTTONDOWN:
		OnMouseBegin(2, glm::vec2(LOWORD(lParam), HIWORD(lParam)));
		break;
	case WM_LBUTTONUP:
		OnMouseEnd(0, glm::vec2(LOWORD(lParam), HIWORD(lParam)));
		break;
	case WM_MBUTTONUP:
		OnMouseEnd(1, glm::vec2(LOWORD(lParam), HIWORD(lParam)));
		break;
	case WM_RBUTTONUP:
		OnMouseEnd(2, glm::vec2(LOWORD(lParam), HIWORD(lParam)));
		break;

	case WM_MOUSEMOVE:
		int32_t pointerIndex = -1;
		if ((wParam & MK_LBUTTON) != 0) pointerIndex = 0;
		else if ((wParam & MK_MBUTTON) != 0) pointerIndex = 1;
		else if ((wParam & MK_RBUTTON) != 0) pointerIndex = 2;

		if (pointerIndex > -1)
		{
			OnMouseDrag(pointerIndex, glm::vec2(LOWORD(lParam), HIWORD(lParam)));
		}
		break;

	}
	return false;
}


void IApplication::SetActive(bool set)
{
	m_bActive = set;
	m_Timer.BeginTimer();
}


void IApplication::Close()
{
	::PostQuitMessage(0);
}


void IApplication::Run()
{
	// run the app
	MSG msg;
	BOOL gotMsg = false;

	// get the first message to initialize msg structure
	::PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE);

	while (msg.message != WM_QUIT)
	{
		if (IsActive())
		{
			// app is active, use PeekMessage not to block the execution
			// while receiving messages from Windows
			gotMsg = ::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
		}
		else
		{
			// app is not active, use GetMessage which blocks until
			// message is received. This allows OS to use power saving and
			// task switching more efficently when screen update is not needed.
			gotMsg = ::GetMessage(&msg, nullptr, 0, 0);
		}

		if (gotMsg)
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		if (msg.message != WM_QUIT)
		{
			m_Timer.EndTimer();
			m_Timer.BeginTimer();

			OnUpdate(m_Timer.GetElapsedSeconds());
			OnDraw(*m_pRenderer);
			m_pRenderer->Flip();
		}
	}

	OnDestroy();
	m_pRenderer = nullptr;
}


HWND IApplication::MakeWindow(int32_t width, int32_t height, const std::string& title)
{
	HINSTANCE hInst = ::GetModuleHandle(nullptr);
	DWORD windowStyle = 0;

	windowStyle = WS_OVERLAPPED |
		WS_CAPTION |
		WS_SYSMENU |
		WS_THICKFRAME |
		WS_MINIMIZEBOX |
		WS_MAXIMIZEBOX;

	WNDCLASS wc;
	memset(&wc, 0, sizeof(WNDCLASS));

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.hInstance = hInst;
	wc.hIcon = ::LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) ::GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = L"GRAPHICSENGINE_WNDCLASS";

	if (!::RegisterClass(&wc))
	{
		Debug("Failed to register window class, exiting...");
		return nullptr;
	}

	// create the window
	auto widestr = std::wstring(title.begin(), title.end());
	HWND window = ::CreateWindow(
		wc.lpszClassName,
		widestr.c_str(),
		windowStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		nullptr,
		nullptr,
		hInst,
		nullptr);

	if (!window)
	{
		Debug("Failed to create window, exiting...");
		return nullptr;
	}

	::SetWindowLong(window, GWL_STYLE, windowStyle);
	RECT clientArea = { 0, 0, width, height };
	::AdjustWindowRectEx(&clientArea, windowStyle, FALSE, 0);
	::SetWindowPos(
		window,
		nullptr,
		0,
		0,
		clientArea.right - clientArea.left,
		clientArea.bottom - clientArea.top,
		SWP_NOZORDER | SWP_NOMOVE | SWP_SHOWWINDOW);
	::UpdateWindow(window);
	::ShowWindow(window, SW_SHOWNORMAL);

	return window;
}


long WINAPI IApplication::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
		break;

	case WM_CREATE:
		::SetForegroundWindow(hwnd);
		break;

	case WM_CLOSE:
		::DestroyWindow(hwnd);
		return 0;
		break;

	default:
		break;
	}

	bool callDefWndProc = true;
	auto app = IApplication::GetApp();
	if (app)
	{
		// OnEvent returns true if it has handled everything and
		// call to DefWindowProc is not desired
		callDefWndProc = !app->OnEvent(message, wParam, lParam);
	}

	if (callDefWndProc)
	{
		return (long)::DefWindowProc(hwnd, message, wParam, lParam);
	}
	else
	{
		return 0;
	}
}


void IApplication::Debug(const wchar_t* msg)
{
	::OutputDebugStringW(msg);
}


void IApplication::Debug(const std::string& msg)
{
	::OutputDebugStringA(msg.c_str());
}


void IApplication::Debug(const char* msg)
{
	::OutputDebugStringA(msg);
}


void IApplication::RandSeed()
{
	srand((uint32_t)::GetTickCount64());
}


bool IApplication::IsKeyDown(uint32_t osKeyCode)
{
	return ::GetAsyncKeyState(osKeyCode);
}


#endif	// #if defined (_WINDOWS)

