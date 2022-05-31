#pragma once
#include GAME_NAME "Game_b"

LRESULT CALLBACK MainWindowProc(_In_  HWND WindowHandle,_In_  UINT Message,_In_  WPARAM wParam,_In_  LPARAM lParam);
DWORD CreateMainGameWindow(void);

BOOL GameIsAlreadyRunning(void);