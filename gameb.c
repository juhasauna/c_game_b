#include <stdio.h>

#pragma warning(push, 3)
#include <windows.h> // Triggers warnings with pedantic compiler settings.
#pragma warning(pop, 0)
#include "Main.h" // Make sure to define this AFTER windows.h

int WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CommandLine, INT CmdShow)
{
    UNREFERENCED_PARAMETER(Instance);
    UNREFERENCED_PARAMETER(PrevInstance);
    if (GameIsAlreadyRUnning() == TRUE)
    {
        MessageBoxA(NULL, "Another instance of this program is already running!", "Error!", MB_ICONEXCLAMATION | MB_OK);
    }
    if (CreateMainGameWindow() != ERROR_SUCCESS)
    {
        goto Exit;
    }
    MSG Message = {0};
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
    switch (Message)
    {
        OutputDebugStringA(&Message);
        OutputDebugStringA("\n");

    case WM_CLOSE:
    {
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
    HWND WindowHandle = 0;

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
    WindowClass.lpszClassName = GAME_NAME "_WINDOWCLASS";
    WindowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (RegisterClassEx(&WindowClass) == 0)
    {
        Result = GetLastError();

        MessageBoxA(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    WindowHandle = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        WindowClass.lpszClassName,
        "The title of my window",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, GetModuleHandleA(NULL), NULL);

    if (WindowHandle == NULL)
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
    Mutex = CreateMutexA(NULL, GAME_NAME "_GameMutex");
    if (GetLAstError() == ERROR_ALREADY_EXISTS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}