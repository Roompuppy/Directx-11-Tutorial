# Directx-11-Tutorial
Making framework using c++ and understanding it.

Class Structure
    
    SystemClass ⊃ GraphicsClass
                ⊃ InputClass

Main cpp

    Function WinMain : Set window
    Make System Object
    Use Function : Initialize(), Run(), Shutdown()
    Delete dynamic allocation

SystemClass

    Including InputClass and GraphicsClass
    Copy Constructor : SystemClass(const SystemClass&)
    Public Functions : Initialize(), Shutdown(), Run()
      + CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM)
    Private Functions : Frame(), InitializeWindows(int&, int&), ShutdownWindows()
    Private Values : LPCWSTR m_applicationName, HINSTANCE m_hinstance, HWND m_hwnd, InputClass* m_Input, GraphicsClass* m_Graphics

GraphicsClass

    Constants : FULL_SCREEN = false, VSYNC_ENABLED = true, SCREEN_DEPTH = 1000.0f, SCREEN_NEAR = 0.1f
    Copy Constructor : GraphicsClass(const GraphicsClass&)
    Public Function : Intialize(int, int, HWND), Shutdown(), Frame()
    Private Function : Render()

Input Class

    Copy Constructor : InputClass(const InputClass&)
    Public Function : Initialize(), KeyDown(unsigned int), KeyUp(unsigned int), IskeyDown(unsigned int)
    Private Values : bool m_keys[256]
