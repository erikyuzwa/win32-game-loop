
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

const WCHAR* className = L"WIN32GAME";
const WCHAR* appTitle = L"My win32 gameloop - ESC to exit";

extern int game_startup();
extern void game_update();
extern void game_shutdown();

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void center_window(HWND hWnd);

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{

    WNDCLASSEX winclass;
    HWND       hWnd;
    MSG        msg;

    // setup the class instance of our Win32 application
    winclass.cbSize = sizeof(WNDCLASSEX);
    winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    winclass.lpfnWndProc = WindowProc;
    winclass.cbClsExtra = 0;
    winclass.cbWndExtra = 0;
    winclass.hInstance = hInstance;
    winclass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    winclass.lpszMenuName = NULL;
    winclass.lpszClassName = className;
    winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    // attempt to register the win32 application class
    if (!RegisterClassEx(&winclass)) {
        return -1;
    }

    // create the actual visual window handle - 640x480
    if (!(hWnd = CreateWindowEx(NULL,
        className,
        appTitle,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        640,
        480,
        NULL,
        NULL,
        hInstance,
        NULL)))
        return -2;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    SetFocus(hWnd);

    center_window(hWnd);

    // with the window created, run your game initialization
    if (game_startup() < 0) {
        // depending on which startup initialization failed
        // goto our shutdown pathway to ensure resource cleanup
        goto SHUTDOWN_AND_EXIT;
    }

    // enter the main event loop
    while (TRUE) {
        // test if there is a message in queue, if so get it
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            //
            if (msg.message == WM_QUIT)
                break;

            // translate any accelerator keys
            TranslateMessage(&msg);

            // send the message to the window proc
            DispatchMessage(&msg);
        }

        // run your game frame processing here
        game_update();

    } // end while

SHUTDOWN_AND_EXIT:
    // shutdown and cleanup any resource
    game_shutdown();

    return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam) {

    PAINTSTRUCT        ps;
    HDC                hdc;

    // what is the message
    switch (uMsg) {
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_ESCAPE:
            PostMessage(hWnd, WM_CLOSE, 0, 0);
            break;
        default:
            break;
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void center_window(HWND hWnd) {

    DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    RECT client_rect;
    GetClientRect(hWnd, &client_rect);
    AdjustWindowRectEx(&client_rect, style, FALSE, 0);

    int client_width = client_rect.right - client_rect.left;
    int client_height = client_rect.bottom - client_rect.top;

    SetWindowPos(
        hWnd,
        NULL,
        screen_width / 2 - client_width / 2,
        screen_height / 2 - client_height / 2,
        client_width,
        client_height,
        0);

}

