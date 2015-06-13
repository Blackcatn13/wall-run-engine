#include <Windows.h>
#include "Core/Engine.h"
#include "ParticleProcess.h"
#include "Core/Core.h"
#include "GraphicsManager.h"
#include "Utils/Logger.h"
#include "Utils/Exception.h"
#include "Core\EngineDefs.h"
#include "Core_Utils\MemLeaks.h"

#define APPLICATION_NAME	"PARTICLE CREATOR"

CEngine	*m_Engine = 0;

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
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

void ShowErrorMessage (const std::string &message) {
  bool logSaved = LOGGER->SaveLogsInFile();
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

int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow) {

#ifdef _DEBUG
  {
    MemLeaks::MemoryBegin();
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
  }
#endif
  // Register the window class
  WNDCLASSEX wc = {	sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };
  RegisterClassEx( &wc );
  try {
    // Create the application's window
    m_Engine = new CEngine();
    m_Engine->ParseConfFile(".\\Data\\ConfigParticle.xml");
    CONFIG_INFO l_Conf = m_Engine->getConfig();

    DWORD dwStyle = WS_BORDER;//WS_POPUPWINDOW;
    HWND hWnd = CreateWindow(APPLICATION_NAME, APPLICATION_NAME, dwStyle/*WS_POPUPWINDOW*/, l_Conf.Win_posX, l_Conf.Win_posY, l_Conf.Screen_Width, l_Conf.Screen_Heigth, NULL, NULL, wc.hInstance, NULL );

    // Añadir aquí el Init de la applicacioón
    CProcess *proc = new CParticleProcess();
    m_Engine->Init(proc, hWnd);
    proc->Init();
    // Añadir en el while la condición de salida del programa de la aplicación
    ShowWindow(hWnd, _nCmdShow);
    UpdateWindow( hWnd );
    MSG msg;
    ZeroMemory( &msg, sizeof(msg) );
    while ( msg.message != WM_QUIT && !proc->getexitGame() ) {
      if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) ) {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
      } else {
        m_Engine->Update();
        m_Engine->Render();
      }
    }
  } catch (CException &e) {
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