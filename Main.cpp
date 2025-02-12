

#include "Main.h"
#include "Manager.h"
#include <thread>
#include "Input.h"
#include "Time.h"
#include "Resource.h"

//#define _CRTDBG_MAP_ALLOC
//#include <cstdlib>
//#include <crtdbg.h>
//#define new  ::new( _NORMAL_BLOCK, __FILE__, __LINE__ )
//#define malloc(p1)  _malloc_dbg(p1,_NORMAL_BLOCK,__FILE__,__LINE__)

const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "Spin Dive";

#ifdef _DEBUG
float		g_CountFPS;				// FPSカウンタ
char	g_DebugStr[2048] = "";		// デバッグ文字表示用

#endif

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


HWND g_Window;

HWND GetWindow() {
	return g_Window;
}

void ExitApplication() {
	Time::GamePause();
	int result = MessageBox(g_Window, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2);
	if (result == IDOK) {
		DestroyWindow(g_Window);
	}
	else if (result == IDCANCEL) {
		Time::GameResume();
		return;
	}
}

void DispErrorMessageBox(const short& num) {
	// エラーメッセージをフォーマット
	char errorMessage[256];
	snprintf(errorMessage, sizeof(errorMessage), "Error Code: %03d\nThe application is closed.", num);

	if (MessageBoxA(g_Window, errorMessage, "Error", MB_OK | MB_ICONERROR) == IDOK) {
		ExitApplication();
	}
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	DWORD dwFPSLastTime;

	WNDCLASSEX wcex;
	{
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = 0;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = (HICON)LoadImage(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 0);
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = nullptr;
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = CLASS_NAME;
		wcex.hIconSm = nullptr;

		RegisterClassEx(&wcex);


		RECT rc = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		g_Window = CreateWindowEx(0, CLASS_NAME, WINDOW_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
	}

	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);


	Manager::Init();
	Time::Init();


	ShowWindow(g_Window, nCmdShow);
	UpdateWindow(g_Window);




	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	timeBeginPeriod(1);
	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = 0;



	MSG msg;
	while (1) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000) {	// 1秒ごとに実行
#ifdef _DEBUG
				g_CountFPS = Time::GetFrameRate();
#endif
				dwFPSLastTime = dwCurrentTime;				// FPSを測定した時刻を保存
			}

			//if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60)) {

#ifdef _DEBUG	// デバッグ版の時だけFPSを表示する
				sprintf_s(g_DebugStr, " FPS:%.0f", g_CountFPS);
				SetWindowText(g_Window, g_DebugStr);
#endif

				//float fps = 1000.0f / (dwCurrentTime - dwExecLastTime);
				//std::string title;
				//title = "Game" + std::to_string(fps) + "fps";
				//SetWindowText(g_Window, title.c_str());

				dwExecLastTime = dwCurrentTime;

				Time::Update();
				Manager::Update();
				Manager::Draw();
			//}
		}
	}

	timeEndPeriod(1);

	UnregisterClass(CLASS_NAME, wcex.hInstance);

	Manager::Uninit();

	CoUninitialize();

	return (int)msg.wParam;
}




LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			ExitApplication();
			break;
		}
		break;

	case WM_CLOSE://ウィンドウを閉じる
		ExitApplication();
		return 0;

	case WM_MOUSEWHEEL:
		Input::OnMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
		break;

	default:
		break;
	}
	Input::SetWindowHandle(hWnd);

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

