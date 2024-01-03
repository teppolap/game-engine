/**
 * ============================================================================
 *  Name        : IApplication.h
 *  Part of     : Simple OpenGL graphics engine framework
 *  Description : Abstract application layer
 * ============================================================================
**/

#pragma once

#if defined (_WINDOWS)
// include minimum set of win32 stuff
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined (_LINUX)
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#endif


// we use std::string and std::vector
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iterator>
#include <string_view>

// include timer component and renderer interface
#include "Timer.h"
#include "IRenderer.h"

// define some common keycodes
#if defined (_WINDOWS)
#define KEY_ESC		VK_ESCAPE
#define KEY_LEFT	VK_LEFT
#define KEY_RIGHT	VK_RIGHT
#define KEY_UP		VK_UP
#define KEY_DOWN	VK_DOWN
#define KEY_SPACE	VK_SPACE

#elif defined (_LINUX)
#define KEY_ESC		XK_Escape
#define KEY_LEFT	XK_Left
#define KEY_RIGHT	XK_Right
#define KEY_UP		XK_Up
#define KEY_DOWN	XK_Down
#define KEY_SPACE	XK_Space

#endif


class IApplication
{
public:
	IApplication();
	~IApplication();

	/**
	 * Create
	 * initialize the application layer
	 * @param resX horizontal resolution of the screen in pixels
	 * @param resY vertical resolution of the screen in pixels
	 * @param title window title text
	 * @return true if successful, false otherwise
	 */
	bool Create(int32_t resX, int32_t resY, const std::string& title);

	/**
	 * Run
	 * enter into app main loop. This function will not return until app has been terminated.
	 */
	void Run();

	/**
	 * OnCreate
	 * pure virtual app initializer
	 * @return true if successful, false otherwise. If implementation returns false, app startup is canceled and app is closed.
	 */
	virtual bool OnCreate() = 0;

	/**
	 * OnDestroy
	 * pure virtual app destroyer, called by the application when app is about to get destroyed
	 */
	virtual void OnDestroy() = 0;

	/**
	 * OnUpdate
	 * pure virtual app update loop
	 * @param frametime time since previous update, in seconds
	 */
	virtual void OnUpdate(float frametime) = 0;

	/**
	 * OnDraw
	 * pure virtual app drawing method
	 * @param renderer renderer object
	 */
	virtual void OnDraw(IRenderer& renderer) = 0;

	/**
	 * IsActive
	 * @return true if app is active, false otherwise
	 */
	inline bool IsActive() const { return m_bActive; }

	/**
	 * SetActive
	 * @param true to set app active, false otherwise
	 */
	void SetActive(bool set);

	/**
	 * Close
	 * close the app
	 */
	void Close();

	/**
	 * GetFrameTime
	 * @return time since previous update, in seconds, this value is essentially the same as
	 * frametime parameter for OnUpdate
	 */
	inline float GetFrameTime() const { return m_Timer.GetElapsedSeconds(); }

	/**
	 * GetRenderer
	 * @return pointer to current renderer
	 */
	inline IRenderer* GetRenderer() { return m_pRenderer.get(); }

	/**
	 * RandSeed
	 * initialize random seed with tick count
	 */
	static void RandSeed();

	/**
	 * GetWidth
	 * @return screen width in pixels
	 */
	inline int32_t GetWidth() const { return m_iWidth; }

	/**
	 * GetHeight
	 * @return screen height in pixels
	 */
	inline int32_t GetHeight() const { return m_iHeight; }

	/**
	 * GetAspect
	 * @return screen aspect ratio
	 */
	inline float GetAspect() const { return (float)m_iWidth / (float)m_iHeight; }

	/**
	 * IsKeyDown
	 * @param osKeyCode os-dependent key code
	 * @return true if key is down
	 */
	static bool IsKeyDown(uint32_t osKeyCode);

	/**
	 * GetWindow
	 * @return window handle
	 */
#if defined (_WINDOWS)
	inline HWND GetWindow() { return m_Window; }
#endif
#if defined (_LINUX)
	inline Display* GetDisplay() { return m_pDisplay; }
	inline Window GetWindow() { return m_Window; }
#endif


	/**
	 * GetApp
	 * static app pointer accessor
	 * @return pointer to active app, or nullptr if app has been terminated
	 */
	static IApplication* GetApp() { return m_pApp; }

	/**
	 * Debug
	 * @param msg message to print to debug stream
	 */
	static void Debug(const wchar_t* msg);
	static void Debug(const std::string& msg);
	static void Debug(const char* msg);

protected:
	/**
	 * OnScreenSizeChanged
	 * event when screen size changes
	 * @param widthPixels, heightPixels new pixel size of the screen
	 */
	virtual void OnScreenSizeChanged(uint32_t widthPixels, uint32_t heightPixels) {}

	/**
	 * OnMouseBegin
	 * event when mouse button is pressed down
	 * @param buttonIndex index of the mouse button (0 based)
	 * @param point event coordinate in pixels
	 * @return true if event was handled by the receiver
	 */
	virtual bool OnMouseBegin(int32_t buttonIndex, const glm::vec2& point) { return false; }

	/**
	 * OnMouseDrag
	 * event when mouse is dragged
	 * @param buttonIndex index of the mouse button (0 based)
	 * @param point event coordinate in pixels
	 * @return true if event was handled by the receiver
	 */
	virtual bool OnMouseDrag(int32_t buttonIndex, const glm::vec2& point) { return false; }

	/**
	 * OnMouseEnd
	 * event when mouse button is lifted up
	 * @param buttonIndex index of the mouse button (0 based)
	 * @param point event coordinate in pixels
	 * @return true if event was handled by the receiver
	 */
	virtual bool OnMouseEnd(int32_t buttonIndex, const glm::vec2& point) { return false; }

	/**
	 * OnKeyDown
	 * key down event
	 * @param keyCode key code
	 * @return true if event was handled by the receiver
	 */
	virtual bool OnKeyDown(uint32_t keyCode) { return false; }

#if defined (_WINDOWS)
	/**
	 * OnEvent
	 * Allows capturing all win32 events before they are handled by default handler.
	 * @param message message id
	 * @param wParam win32 wparam
	 * @param lParam win32 lparam
	 * @return true if event was handled by the receiver, if receiver returns false, event is passed to default handler.
	 */
	virtual bool OnEvent(UINT message, WPARAM wParam, LPARAM lParam);
#endif

private:
#if defined (_WINDOWS)
	static HWND MakeWindow(int32_t width, int32_t height, const std::string& title);
	static long WINAPI WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	HWND							m_Window;
#endif

#if defined (_LINUX)
	static Window MakeWindow(int32_t width, int32_t height, const char* title);
    static bool ProcessEvents(XEvent* evnt);
    static KeySym PreprocessKeyEvent(XEvent* evnt);

    static Display*					m_pDisplay;
    static int32_t					m_iScreen;
    static Window					m_Window;
    static XSetWindowAttributes		m_WindowAttributes;
    static Atom						m_CloseAtom;
    static uint8_t                  m_uKeyboard[65536];
#endif

	static IApplication*			m_pApp;
	bool							m_bActive;
	Timer							m_Timer;

	int32_t							m_iWidth;
	int32_t							m_iHeight;

	std::unique_ptr<IRenderer>		m_pRenderer;
};

