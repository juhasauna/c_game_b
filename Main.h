#pragma once
#define GAME_NAME "Game_b"
#define GAME_RES_WIDTH 348
#define GAME_RES_HEIGHT 216
#define GAME_BPP 32 // Bits per pixel
#define GAME_DRAWING_AREAA_MEMORY_SIZE (GAME_RES_WIDTH * GAME_RES_HEIGHT * (GAME_BPP / 8))

typedef struct GAMEBITMAP
{
    BITMAPINFO BitmapInfo;
    void *Memory;
} GAMEBITMAP;
LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM wParam, _In_ LPARAM lParam);
DWORD CreateMainGameWindow(void);
BOOL GameIsAlreadyRunning(void);
void ProcessPlayerInput();
void renkderFrameGraphics(void);
