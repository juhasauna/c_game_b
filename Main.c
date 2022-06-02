#include <stdio.h>

#pragma warning(push, 3)
#include <windows.h> // Triggers warnings with pedantic compiler settings.
#pragma warning(pop, 0)
#include "Main.h" // Make sure to define this AFTER windows.h
BOOL gGameIsRunning;
HWND gGameWindow;

GAMEBITMAP gDrawingSurface;
// __stdcall is for x64. Aliases WINAPI, CALLBACK.
int __stdcall WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CommandLine, INT CmdShow)
{
    UNREFERENCED_PARAMETER(Instance);
    UNREFERENCED_PARAMETER(PrevInstance);
    if (GameIsAlreadyRUnning())
    {
        MessageBoxA(NULL, "Another instance of this program is already running!", "Error!", MB_ICONEXCLAMATION | MB_OK);
    }
    if (CreateMainGameWindow() != ERROR_SUCCESS)
    {
        goto Exit;
    }
    gDrawingSurface.BitmapInfo.bmiHeader.biSize = sizeof(gDrawingSurface.BitmapInfo.bmiHeader);
    gDrawingSurface.BitmapInfo.bmiHeader.biWidth = GAME_RES_WIDTH;
    gDrawingSurface.BitmapInfo.bmiHeader.biHeight = GAME_RES_HEIGHT;
    gDrawingSurface.BitmapInfo.bmiHeader.biBitCount = GAME_BPP;
    gDrawingSurface.BitmapInfo.bmiHeader.biCompression = BI_RGB;
    gDrawingSurface.BitmapInfo.bmiHeader.biPlanes = 1;
    gDrawingSurface.Memory = VirtualAlloc(NULL, GAME_DRAWING_AREAA_MEMORY_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (gDrawingSurface.Memory == NULL)
    {
        MessageBoxA(NULL, "Failed to allocate memory fro drawing surface!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    MSG Message = {0};
    gGameIsRunning = TRUE;
    while (gGameIsRunning)
    {
        while (PeekMessageA(&Message, gGameWindow, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&Message);
        }
        ProcessPlayerInput();
        RenderFrameGraphics();
        Sleep(1); // Pause & yield for 1 millisecond.
    }
    while (GetMessageA(&Message, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Message);
        DispatchMessageA(&Message);
    }
Exit:
    return 0;
}

LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    LRESULT Result = 0;
    char buf[12] = {0};
    _itoa_s(Message, buf, _countof(buf), 10);

    OutputDebugStringA(buf);
    OutputDebugStringA("\n");
    switch (Message)
    {

    case WM_CLOSE:
    {
        gGameIsRunning = FALSE;
        PostQuitMessage(0);
        break;
    }

    default:
    {
        Result = DefWindowProcA(WindowHandle, Message, wParam, lParam);
    }
    }
    return Result;
}

DWORD CreateMainGameWindow(void)
{
    DWORD Result = ERROR_SUCCESS;
    WNDCLASSEXA WindowClass = {0};

    WindowClass.cbSize = sizeof(WNDCLASSEXA);
    WindowClass.style = 0;
    WindowClass.lpfnWndProc = MainWindowProc;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = GetModuleHandleA(NULL); // Same as WindowClass.hInstance = Instance;

    WindowClass.hIcon = LoadIconA(NULL, IDI_APPLICATION);
    WindowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
    WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    WindowClass.lpszMenuName = NULL;
    WindowClass.lpszClassName = "GAME_NAME_WINDOWCLASS";
    // WindowClass.lpszClassName = GAME_NAME "_WINDOWCLASS";
    WindowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (RegisterClassEx(&WindowClass) == 0)
    {
        Result = GetLastError();

        MessageBoxA(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    gGameWindow = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        WindowClass.lpszClassName,
        "The title of my window",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 666, 333,
        NULL, NULL, GetModuleHandleA(NULL), NULL);

    if (gGameWindow == NULL)
    {
        Result = GetLastError();

        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }
Exit:
    return Result;
}

BOOL GameIsAlreadyRUnning(void)
{
    HANDLE Mutex = NULL; // Mutual exclusion
    Mutex = CreateMutexA(NULL, NULL, "GAME_NAME_GameMutex");
    // Mutex = CreateMutexA(NULL, GAME_NAME "_GameMutex");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void ProcessPlayerInput()
{
    short EscpapeKeyIsDown = GetAsyncKeyState(VK_ESCAPE);
    if (EscpapeKeyIsDown)
    {
        SendMessageA(gGameWindow, WM_CLOSE, 0, 0);
    }
}

void RenderFrameGraphics()
{
}