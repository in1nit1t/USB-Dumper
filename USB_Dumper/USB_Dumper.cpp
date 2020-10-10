// USB_Dumper_Plus.cpp : 定义应用程序的入口点
//

#include "framework.h"
#include "USB_Dumper.h"

#define MAX_LOADSTRING 100

// 全局变量
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
WCHAR szNote[MAX_LOADSTRING];

// 此代码模块中包含的函数的前向声明
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_USBDUMPER, szWindowClass, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_NOTE, szNote, MAX_LOADSTRING);

    // 注册主窗口
    WNDCLASSEXW wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = 0;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = NULL;
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = NULL;
    if (!RegisterClassExW(&wcex))
        return 0;

    // 实例化主窗口
    HWND hWndMain = CreateWindowW(
        szWindowClass, szTitle,
        WS_POPUP,
        0, 0, 0, 0,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    if (!hWndMain)
        return FALSE;
    ShowWindow(hWndMain, SW_HIDE);

    // 主消息循环
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息
//
//  WM_CREATE        - 窗口创建完毕
//  WM_DEVICECHANGE  - 设备变更
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    CHAR                           diskLabel;
    PDEV_BROADCAST_DEVICEINTERFACE devInterface;
    PDEV_BROADCAST_VOLUME          devVolume;

    switch (message)
    {
        case WM_CREATE:
            // 主窗口创建完毕后显示关于界面
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case WM_DEVICECHANGE:
            switch (wParam)
            {
                // 设备接入
                case DBT_DEVICEARRIVAL:
                    devInterface = (PDEV_BROADCAST_DEVICEINTERFACE)lParam;
                    if (devInterface->dbcc_devicetype == DBT_DEVTYP_VOLUME) {
                        devVolume = (PDEV_BROADCAST_VOLUME)lParam;
                        // 获取盘符
                        diskLabel = GetDiskLabel(devVolume->dbcv_unitmask);
                        // 拷贝文件
                        CopyFiles(diskLabel);
                    }
                    break;
                // 设备移除
                case DBT_DEVICEREMOVECOMPLETE:
                    // TODO
                    break;
            }
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
    }
    return (INT_PTR)FALSE;
}
