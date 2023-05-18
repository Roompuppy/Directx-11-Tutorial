////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_Grass = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_camX = 0.0f;
	m_camZ = -50.0f;
	m_colorR = 1.0f;
	m_colorG = 1.0f;
	m_colorB = 1.0f;
	m_lightA = 1.0f;
	m_lightD = 1.0f;
	m_lightS = 1.0f;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -50.0f);
		
	// Create the model object.
	m_Model = new ModelClass;
	if(!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(m_D3D->GetDevice(), L"./data/aircraft.obj", L"./data/aircraft_d.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Grass = new ModelClass;
	if (!m_Grass)
	{
		return false;
	}
	result = m_Grass->Initialize(m_D3D->GetDevice(), L"./data/aircraft.obj", L"./data/aircraft_d.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}


	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 0.0f);

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the model object.
	if(m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
	
	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += XM_PI * 0.003f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	m_Camera->SetRotation(0.0f, 0.0f, 0.0f);
	m_Camera->SetPosition(m_camX, 0.0f, m_camZ);

	m_Light->SetAmbientColor(m_colorR*m_lightA*0.1f, m_colorG* m_lightA * 0.1f, m_colorB* m_lightA * 0.1f, 1.0f);
	m_Light->SetDiffuseColor(m_colorR*m_lightD, m_colorG * m_lightD, m_colorB * m_lightD, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 0.0f);

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	worldMatrix = XMMatrixRotationY(rotation);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetVertexCount(), m_Model->GetInstanceCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());
	
	if(!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

void GraphicsClass::MoveCam(int type)
{
	float speed = 0.1f;

	switch (type)
	{
	case 0: // w
		m_camZ += speed;
		break;
	case 1: // a
		m_camX -= speed;
		break;
	case 2: // s
		m_camZ -= speed;
		break;
	case 3: // d
		m_camX += speed;
		break;
	default:
		break;
	}
}

void GraphicsClass::ChageColor(int type)
{
	switch (type)
	{
	case 0:
		m_colorR = 1.0f;
		m_colorG = 0.0f;
		m_colorB = 0.0f;
		break;
	case 1:
		m_colorR = 0.0f;
		m_colorG = 1.0f;
		m_colorB = 0.0f;
		break;
	case 2:
		m_colorR = 0.0f;
		m_colorG = 0.0f;
		m_colorB = 1.0f;
		break;
	default:
		break;
	}
}

void GraphicsClass::TurnLight(int type)
{
	switch (type)
	{
	case 0:
		if (m_lightA == 1.0f)
		{
			m_lightA = 0.0f;
		}
		else
		{
			m_lightA = 1.0f;
		}
		break;
	case 1:
		if (m_lightD == 1.0f)
		{
			m_lightD = 0.0f;
		}
		else
		{
			m_lightD = 1.0f;
		}
		break;
	case 2:
		if (m_lightS == 1.0f)
		{
			m_lightS = 0.0f;
		}
		else
		{
			m_lightS = 1.0f;
		}
		break;
	default:
		break;
	}
}