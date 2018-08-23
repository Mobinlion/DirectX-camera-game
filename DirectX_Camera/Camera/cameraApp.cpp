//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: cameraApp.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Demonstrates using the Camera class.
//         
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"
#include "camera.h"
#include "Windowsx.h"

bool flag = true;
HWND defaultwindow = NULL;
bool globel = false;
//
// Globals
//

IDirect3DDevice9* Device = 0; 

 int Width  = 1024;
 int Height = 768;
POINT pnt={0};
int defposx ;
int defposy ;

POINT pntlive={0};

Camera TheCamera(Camera::LANDOBJECT);

//
// Framework functions
//
bool Setup()
{
	//
	// Setup a basic scene.  The scene will be created the
	// first time this function is called.
	//

	d3d::DrawBasicScene(Device, 0.0f); 

	//
	// Set projection matrix.
	//

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
			&proj,
			D3DX_PI * 0.44f, // 45 - degree
			(float)Width / (float)Height,
			1.0f,
			1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

void Cleanup()
{
	// pass 0 for the first parameter to instruct cleanup.
	d3d::DrawBasicScene(0, 0.0f);
}

bool Display(float timeDelta)
{
	if( Device )
	{
		//
		// Update: Update the camera.
		//

		if( ::GetAsyncKeyState('W') & 0x8000f )
			TheCamera.walk(4.0f * timeDelta);

		if( ::GetAsyncKeyState('S') & 0x8000f )
			TheCamera.walk(-4.0f * timeDelta);

		if( ::GetAsyncKeyState('A') & 0x8000f )
			TheCamera.strafe(-4.0f * timeDelta);

		if( ::GetAsyncKeyState('D') & 0x8000f )
			TheCamera.strafe(4.0f * timeDelta);

		if( ::GetAsyncKeyState('R') & 0x8000f )
		{
			TheCamera.fly(4.0f * timeDelta);
			globel = true;
		}

		if( ::GetAsyncKeyState('F') & 0x8000f ){
			TheCamera.fly(-4.0f * timeDelta);
			globel = false;}

		if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
			TheCamera.pitch(1.0f * timeDelta);

		if(GetAsyncKeyState(VK_LBUTTON))
		{
			
			GetCursorPos(&pnt);
			
		}

		if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
			TheCamera.pitch(-1.0f * timeDelta);

		if( ::GetAsyncKeyState(VK_LEFT) & 0x8000f )
			TheCamera.yaw(-1.0f * timeDelta);
			
		if( ::GetAsyncKeyState(VK_RIGHT) & 0x8000f )
			TheCamera.yaw(1.0f * timeDelta);

		if( ::GetAsyncKeyState('N') & 0x8000f )
			TheCamera.roll(1.0f * timeDelta);

		if( ::GetAsyncKeyState('M') & 0x8000f )
			TheCamera.roll(-1.0f * timeDelta);

		static POINT tempoint={0};
		GetCursorPos(&tempoint);
		static HWND wnddef = 0;
		
		if((defposx-tempoint.x)!=0.0){
			//MessageBox(0, "yes", "Wtf", 1);
				float ang = (tempoint.x-defposx)*0.09f;
				TheCamera.yaw(ang * timeDelta);
				if( (wnddef = GetForegroundWindow())==defaultwindow)
				SetCursorPos(defposx, defposy);		
		}
			if((defposy-tempoint.y)!=0.0){
				float ang2 = (tempoint.y-defposy)*0.09f;
				TheCamera.pitch(ang2 * timeDelta);
				if( (wnddef = GetActiveWindow())==defaultwindow)
				SetCursorPos(defposx, defposy);		
				}
				

		// Update the view matrix representing the cameras 
        // new position/orientation.
		D3DXMATRIX V;
		TheCamera.getViewMatrix(&V);
		Device->SetTransform(D3DTS_VIEW, &V);

		//
		// Render
		//

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		Device->BeginScene();
		
		
		d3d::DrawBasicScene(Device, 1.0f);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);

		
	}
	return true;
}

//
// WndProc
//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
		
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			::DestroyWindow(hwnd);

		break;

	/*case WM_MOUSEMOVE:
		if(wParam == MK_LBUTTON)
		{
			POINT pnt2;
			GetCursorPos(&pnt2);
			if((pnt2.x-pnt.x)!=0.0){
				float ang = static_cast<float>(pnt2.x-pnt.x)*1.55f;
			TheCamera.yaw(ang * 0.001f);
			}
		
			if((pnt2.y-pnt.y)!=0.0){
				float ang2 = static_cast<float>(pnt2.y-pnt.y)*1.55f;
			TheCamera.pitch(ang2 * 0.001f);
			}
			
		}
		break;*/

	//case WM_MOUSEMOVE:
	//	{
	//		
	//		
	//	//if(((defposx-xpos)!=0.0)||((defposy-ypos)!=0.0)){
	//	int xpos = GET_X_LPARAM(lParam);
	//	int ypos = GET_Y_LPARAM(lParam);
	//	//POINT tempoint={0};
	//	//int xpos = (lParam)&0x00FF;
	//	//int ypos = (lParam)&0xFF00;
	//	
	//	//GetCursorPos(&tempoint);		
	//	if((defposx-xpos)!=0.0){
	//		//MessageBox(0, "yes", "Wtf", 1);
	//			float ang = (xpos-defposx)*0.025f;
	//			if(ang<0.0){
	//				TheCamera.yaw(-1.0f * 0.001);
	//				}
	//			else{
	//		TheCamera.yaw(1.0f * 0.001f);  //call downkey or sendinput downkey
	//		}
	//	}
	//	
	//		/*if((defposy-ypos)!=0.0){
	//			float ang2 = (ypos-defposy)*0.025f;
	//			TheCamera.pitch(ang2 * 0.001f);
	//		}*/

	//		//SetCursorPos(defposx, defposy);
	//		
	//	break;
	//	}
		
}

	

	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// WinMain
//
int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	RECT desktop;
	// Get a handle to the desktop window
	 const HWND hDesktop = GetDesktopWindow();
	  // Get the size of screen to the variable desktop
	 GetWindowRect(hDesktop, &desktop);
	Width = desktop.right;
	Height = desktop.bottom;
	ShowCursor(false);
	defposx = Width/2;
	defposy = Height/2;
	SetCursorPos(defposx, defposy);
	if(!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}
		
	if(!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}
	
	d3d::EnterMsgLoop( Display );

	Cleanup();

	Device->Release();

	return 0;
}
