#include <stdio.h>
#include <windows.h>
#include <stdbool.h>

#define HOTKEY_ID 1
#define MODIFIER_KEY MOD_CONTROL // 使用Ctrl键作为修饰键
#define HOTKEY_KEY VK_F2

const char *toolFolderName = "tools";
bool isPaused = false; // 用于跟踪暂停/启动状态

bool GetExecutableDirectory(char *path, size_t size)
{
	char executablePath[MAX_PATH];
	if (GetModuleFileName(NULL, executablePath, MAX_PATH) == 0)
	{
		return false;
	}
	char *lastSlash = strrchr(executablePath, '\\');
	if (lastSlash != NULL)
	{
		*lastSlash = '\0';
	}
	snprintf(path, size, "%s", executablePath);
	return true;
}

bool ExecutePssuspend(const char *toolFolder, const char *processName)
{
	char scriptPath[MAX_PATH];
	snprintf(scriptPath, MAX_PATH, "%s\\pssuspend.exe %s", toolFolder, processName);
	int result = system(scriptPath);
	return result == 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_HOTKEY)
	{
		if (wParam == HOTKEY_ID)
		{
			char toolFolder[MAX_PATH];
			if (GetExecutableDirectory(toolFolder, MAX_PATH))
			{
				snprintf(toolFolder, MAX_PATH, "%s\\%s", toolFolder, toolFolderName);
				if (GetFileAttributesA(toolFolder) == INVALID_FILE_ATTRIBUTES)
				{
					printf("工具文件夹不存在\n");
					printf("确保根目录下存在tools文件夹\n");
					// 播放提示音
					Beep(750, 600); // 频率和持续时间
				}
				else
				{
					char pssuspendPath[MAX_PATH];
					snprintf(pssuspendPath, MAX_PATH, "%s\\pssuspend.exe", toolFolder);
					if (GetFileAttributesA(pssuspendPath) != INVALID_FILE_ATTRIBUTES)
					{
						const char *processName = "MonsterHunterWorld.exe";
						if (isPaused)
						{
							// 启动操作
							if (ExecutePssuspend(toolFolder, "-r MonsterHunterWorld.exe"))
							{
								printf("pssuspend 恢复执行成功\n");
								// 播放提示音
								Beep(500, 600);   // 频率和持续时间
								isPaused = false; // 成功后切换状态
							}
							else
							{
								printf("pssuspend 恢复执行失败\n");
								// 播放提示音
								Beep(250, 600); // 频率和持续时间
							}
						}
						else
						{
							// 暂停操作
							if (ExecutePssuspend(toolFolder, "MonsterHunterWorld.exe"))
							{
								printf("pssuspend 执行成功\n");
								// 播放提示音
								Beep(500, 600);  // 频率和持续时间
								isPaused = true; // 成功后切换状态
							}
							else
							{
								printf("pssuspend 执行失败\n");
								// 播放提示音
								Beep(250, 600); // 频率和持续时间
							}
						}
					}
					else
					{
						printf("找不到 pssuspend.exe 文件\n");
						printf("确保根目录下的tools文件夹内存在pssuspend.exe 文件\n");
						// 播放提示音
						Beep(250, 600); // 频率和持续时间
					}
				}
			}
			else
			{
				printf("无法获取当前可执行文件的目录\n");

				// 播放提示音
				Beep(250, 600); // 频率和持续时间
			}
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main()
{
	WNDCLASSA wc = {0}; // 使用 WNDCLASSA，确保ANSI字符集
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = "HotkeyDemo"; // 使用 ANSI 字符串
	RegisterClassA(&wc);             // 使用 ANSI 版本的 RegisterClass
	
	printf("lil candy makes it for free\r\n23.10.10\r\n");
	printf("WIN11/WIN10\r\nMHW VES 15.11.01\r\n");
	
	
	HWND hwnd = CreateWindowA("HotkeyDemo", "HotkeyDemo", 0, 0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL); // 使用 ANSI 字符串
	
	if (RegisterHotKey(hwnd, HOTKEY_ID, MODIFIER_KEY, HOTKEY_KEY))
	{
		printf("F2 + Ctrl 热键注册成功\n");
	}
	else
	{
		printf("F2 + Ctrl 热键注册失败\n");
		return 1;
	}
	
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	UnregisterHotKey(hwnd, HOTKEY_ID);
	
	return 0;
}

