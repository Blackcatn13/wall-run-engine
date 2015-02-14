#include <Windows.h>
#include "Core/Engine.h"
#include "VideoGame_Process.h"
#include "Test_Process.h"
//#include "Test_Space.h"
#include "TestCommands.h"
#include "Core/Core.h"
#include "GraphicsManager.h"
#include "Utils/Logger.h"
#include "Utils/Exception.h"
#include "cal3d\global.h"
#include "cal3d\coremodel.h"
#include "cal3d\model.h"
#include "Core_Utils\MemLeaks.h"

#define APPLICATION_NAME	"VIDEOGAME"

CEngine	*m_Engine = 0;

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch ( msg ) {
    case WM_DESTROY: {
            PostQuitMessage( 0 );
            return 0;
        }
        break;
    case WM_KEYDOWN: {
            switch ( wParam ) {
            case VK_ESCAPE:
                //Cleanup();
                PostQuitMessage( 0 );
                return 0;
                break;
            }
        }
        break;
    case WM_SIZE: {
            if (m_Engine != 0) {
                GRAPHM->GetWindowRect(hWnd);
            }
        }
        break;
    }//end switch( msg )
    return DefWindowProc( hWnd, msg, wParam, lParam );
}

void ShowErrorMessage (const std::string& message)
{
    bool logSaved = false;
    logSaved = LOGGER->SaveLogsInFile();
    std::string end_message = "";
    if (logSaved) {
        end_message += "Sorry, Application failed. Logs saved\n";
    } else {
        end_message += "Sorry, Application failed. Logs could not be saved\n";
    }
    end_message += message;
    MessageBox(0, end_message.c_str(), "FlostiProject Report", MB_OK | MB_ICONERROR);
}

//-----------------------------------------------------------------------
// WinMain
//-----------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
{
#ifdef _DEBUG
    {
        MemLeaks::MemoryBegin();
    }
#endif
    // Register the window class
    WNDCLASSEX wc = {	sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };
    RegisterClassEx( &wc );
    try {
        // Create the application's window
        HWND hWnd = CreateWindow(	APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, wc.hInstance, NULL );
        // Añadir aquí el Init de la applicacioón
        ShowWindow( hWnd, SW_SHOWDEFAULT );
        UpdateWindow( hWnd );
        MSG msg;
        ZeroMemory( &msg, sizeof(msg) );
        m_Engine = new CEngine();
        //new CVideoGame_Process()
        //new CTest_Process()
        CProcess *proc = new CTestCommands();
        m_Engine->Init(proc, ".\\Data\\Config.xml", hWnd);
        proc->Init();
        // Añadir en el while la condición de salida del programa de la aplicación
        while ( msg.message != WM_QUIT ) {
            if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) ) {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            } else {
                m_Engine->Update();
                m_Engine->Render();
                // Main loop: Añadir aquí el Update y Render de la aplicación principal
            }
        }
    } catch (CException& e) {
        ShowErrorMessage(e.GetDescription());
    } catch (...) {
        ShowErrorMessage("Exception Occured");
    }
    UnregisterClass( APPLICATION_NAME, wc.hInstance );
    delete m_Engine;
#ifdef _DEBUG
    {
        MemLeaks::MemoryEnd();
    }
#endif
    //  MemLeaks::MemoryEnd();
    // Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos
    return 0;
}