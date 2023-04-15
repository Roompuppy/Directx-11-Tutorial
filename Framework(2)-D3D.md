# Directx-11-Tutorial
Making framework using c++ and understanding it.

Class Structure
    
    SystemClass ⊃ GraphicsClass ⊃ D3dClass
                ⊃ InputClass

Add in GraphicsClass
    Include D3dClass
    Set D3dClass in function : Initialize(...), Shutdown(), Render() 

D3dClass
    Using DirectX
    Public Function : Initialize(int, int, bool, HWND, bool, float, float), Shutdown(), BeginScene(float, float, float, float), EndScene(), GetProjectionMatrix(XMMATRIX&), GetWorldMatrix(XMMATRIX&), GetOrthoMatrix(XMMATRIX&), GetVideoCardInfo(char*, int&), ID3D11Device* GetDevice(), ID3D11DeviceContext* GetDeviceContext()
    Private Values : bool m_vsync_enabled, int m_videoCardMemory, char m_videoCardDescription[128], IDXGISwapChain* m_swapChain, ID3D11Device* m_device, ID3D11DeviceContext* m_deviceContext, ID3D11RenderTargetView* m_renderTargetView, ID3D11Texture2D* m_depthStencilBuffer, ID3D11DepthStencilState* m_depthStencilState, ID3D11DepthStencilView* m_depthStencilView, ID3D11RasterizerState* m_rasterState, XMMATRIX m_projectionMatrix, XMMATRIX m_worldMatrix, XMMATRIX m_orthoMatrix

Static Class AlignedAllocationPolicy
    operator new(size_t size)
	{
		return _aligned_malloc(size, T);
	}
    operator delete(void* memory)
	{
		_aligned_free(memory);
	}