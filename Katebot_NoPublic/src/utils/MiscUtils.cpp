#include "..\main.h"

#define BUFSIZE 1024
#define MD5LEN  16


CMiscUtils* miscUtils = new CMiscUtils();

#define clamp(val, a, b) { if(val < a) val = a; if(val > b) val = b; }

std::string CMiscUtils::GenerateRandomString(int length)
{

	static const char alphanum[] =
		"0123456789"
		"!@#$%^&*"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	std::string str;
	srand(time(0));
	for (int i = 0; i < length; i++) {
		str += alphanum[rand() % sizeof(alphanum) - 1];
	}

	return str;
}

void CMiscUtils::GenerateRandomWindowTitle(int stringLength)
{
	SetConsoleTitle(GenerateRandomString(stringLength).c_str());
}

void CMiscUtils::DeleteSelf(char* ProgramPath)
{
	// Windows keeps .exe files open during execution...
	char* batPath = new char[strlen(ProgramPath) + 5];
	strcpy_s(batPath, strlen(ProgramPath) + 5, ProgramPath);
	strcat_s(batPath, strlen(ProgramPath) + 5, xorstr(".bat").crypt_get());

	const char* batFormat =
		":Repeat\n"
		"del \"%s\"\n"
		"if exist \"%s\" goto Repeat\n"
		"del \"%s\"\n";

	char* batFile = new char[strlen(batPath) + strlen(batFormat) + strlen(ProgramPath) * 2];
	sprintf_s(batFile, strlen(batPath) + strlen(batFormat) + strlen(ProgramPath) * 2,
			  batFormat, ProgramPath, ProgramPath, batPath);

	FILE* f;
	fopen_s(&f, batPath, xorstr("w").crypt_get());
	if (f != NULL)
	{
		fwrite(batFile, strlen(batFile), 1, f);
		fclose(f);
	}

	STARTUPINFOA startupInfo;
	PROCESS_INFORMATION procInfo;
	memset(&startupInfo, 0, sizeof(startupInfo));

	startupInfo.cb = sizeof(startupInfo);

	CreateProcess(batPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL,
				  &startupInfo, &procInfo);

	delete[] batFile;
	delete[] batPath;
}

void CMiscUtils::AllowDebugging()
{
	HANDLE _HandleProcess = GetCurrentProcess();
	HANDLE _HandleToken;
	TOKEN_PRIVILEGES tkPrivileges;
	LUID _LUID;

	OpenProcessToken(_HandleProcess, TOKEN_ADJUST_PRIVILEGES, &_HandleToken);
	LookupPrivilegeValue(0, xorstr("seDebugPrivilege").crypt_get(), &_LUID);

	tkPrivileges.PrivilegeCount = 1;
	tkPrivileges.Privileges[0].Luid = _LUID;
	tkPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(_HandleToken, false, &tkPrivileges, 0, 0, 0);
	CloseHandle(_HandleToken);
	CloseHandle(_HandleProcess);
}

void CMiscUtils::PrintLine(std::string s)
{
	printf(xorstr("%s\n").crypt_get(), s.c_str());
}

int CMiscUtils::GetKeyFromString(std::string str)
{
	char data = str.c_str()[0];

	if ((data >= '0' && data <= '9') || (data >= 'A' && data <= 'Z'))
		return (int)data;
	
	if (str == xorstr("mouse1").crypt_get()) return VK_LBUTTON;
	else if (str == xorstr("mouse2").crypt_get()) return VK_RBUTTON;
	else if (str == xorstr("mouse3").crypt_get()) return VK_MBUTTON;
	else if (str == xorstr("mouse4").crypt_get()) return VK_XBUTTON1;
	else if (str == xorstr("mouse5").crypt_get()) return VK_XBUTTON2;
	else if (str == xorstr("backspace").crypt_get()) return VK_BACK;
	else if (str == xorstr("tab").crypt_get()) return VK_TAB;
	else if (str == xorstr("enter").crypt_get()) return VK_RETURN;
	else if (str == xorstr("shift").crypt_get()) return VK_SHIFT;
	else if (str == xorstr("ctrl").crypt_get()) return VK_CONTROL;
	else if (str == xorstr("alt").crypt_get()) return VK_MENU;
	else if (str == xorstr("capslock").crypt_get()) return VK_CAPITAL;
	else if (str == xorstr("escape").crypt_get()) return VK_ESCAPE;
	else if (str == xorstr("space").crypt_get()) return VK_SPACE;
	else if (str == xorstr("pgup").crypt_get()) return VK_PRIOR;
	else if (str == xorstr("pgdn").crypt_get()) return VK_NEXT;
	else if (str == xorstr("end").crypt_get()) return VK_END;
	else if (str == xorstr("home").crypt_get()) return VK_HOME;
	else if (str == xorstr("leftarrow").crypt_get()) return VK_LEFT;
	else if (str == xorstr("rightarrow").crypt_get()) return VK_RIGHT;
	else if (str == xorstr("uparrow").crypt_get()) return VK_UP;
	else if (str == xorstr("downarrow").crypt_get()) return VK_DOWN;
	else if (str == xorstr("ins").crypt_get()) return VK_INSERT;
	else if (str == xorstr("del").crypt_get()) return VK_DELETE;
	else if (str == xorstr("numpad_0").crypt_get()) return VK_NUMPAD0;
	else if (str == xorstr("numpad_1").crypt_get()) return VK_NUMPAD1;
	else if (str == xorstr("numpad_2").crypt_get()) return VK_NUMPAD2;
	else if (str == xorstr("numpad_3").crypt_get()) return VK_NUMPAD3;
	else if (str == xorstr("numpad_4").crypt_get()) return VK_NUMPAD4;
	else if (str == xorstr("numpad_5").crypt_get()) return VK_NUMPAD5;
	else if (str == xorstr("numpad_6").crypt_get()) return VK_NUMPAD6;
	else if (str == xorstr("numpad_7").crypt_get()) return VK_NUMPAD7;
	else if (str == xorstr("numpad_8").crypt_get()) return VK_NUMPAD8;
	else if (str == xorstr("numpad_9").crypt_get()) return VK_NUMPAD9;
	else if (str == xorstr("kp_multiply").crypt_get()) return VK_MULTIPLY;
	else if (str == xorstr("kp_plus").crypt_get()) return VK_ADD;
	else if (str == xorstr("kp_minus").crypt_get()) return VK_SUBTRACT;
	else if (str == xorstr("kp_slash").crypt_get()) return VK_DIVIDE;
	else if (str == xorstr("f1").crypt_get()) return VK_F1;
	else if (str == xorstr("f2").crypt_get()) return VK_F2;
	else if (str == xorstr("f3").crypt_get()) return VK_F3;
	else if (str == xorstr("f4").crypt_get()) return VK_F4;
	else if (str == xorstr("f5").crypt_get()) return VK_F5;
	else if (str == xorstr("f6").crypt_get()) return VK_F6;
	else if (str == xorstr("f7").crypt_get()) return VK_F7;
	else if (str == xorstr("f8").crypt_get()) return VK_F8;
	else if (str == xorstr("f9").crypt_get()) return VK_F9;
	else if (str == xorstr("f10").crypt_get()) return VK_F10;
	else if (str == xorstr("f11").crypt_get()) return VK_F11;
	else if (str == xorstr("f12").crypt_get()) return VK_F12;
	else if (str == xorstr(";").crypt_get()) return VK_OEM_1;
	else if (str == xorstr("+").crypt_get()) return VK_OEM_PLUS;
	else if (str == xorstr("-").crypt_get()) return VK_OEM_MINUS;
	else if (str == xorstr(",").crypt_get()) return VK_OEM_COMMA;
	else if (str == xorstr(".").crypt_get()) return VK_OEM_PERIOD;
	else if (str == xorstr("/").crypt_get()) return VK_OEM_2;
	else if (str == xorstr("~").crypt_get()) return VK_OEM_3;
	else if (str == xorstr("[").crypt_get()) return VK_OEM_4;
	else if (str == std::to_string(char(0x5C))) return VK_OEM_5;
	else if (str == xorstr("]").crypt_get()) return VK_OEM_6;
	else if (str == std::to_string(char(0x22))) return VK_OEM_7;
	else return -1;

}


std::string CMiscUtils::GetStringFromKey(int key)
{
	char c[2] = { 0 };

	if ((key >= '0' && key <= '9') || (key >= 'A' && key <= 'Z'))
	{
		c[0] = (char)key;
		std::string sweg(c);
		std::transform(sweg.begin(), sweg.end(), sweg.begin(), ::tolower);
		return sweg;
	}
	
	switch (key)
	{
		case VK_LBUTTON: return std::string("mouse1");
		case VK_RBUTTON: return std::string("mouse2");
		case VK_MBUTTON: return std::string("mouse3");
		case VK_XBUTTON1: return std::string("mouse4");
		case VK_XBUTTON2: return std::string("mouse5");
		case VK_BACK: return std::string("backspace");
		case VK_TAB: return std::string("tab");
		case VK_RETURN: return std::string("enter");
		case VK_SHIFT: return std::string("shift");
		case VK_CONTROL: return std::string("ctrl");
		case VK_MENU: return std::string("alt");
		case VK_CAPITAL: return std::string("capslock");
		case VK_ESCAPE: return std::string("escape");
		case VK_SPACE: return std::string("space");
		case VK_PRIOR: return std::string("pgup");
		case VK_NEXT: return std::string("pgdn");
		case VK_END: return std::string("end");
		case VK_HOME: return std::string("home");
		case VK_LEFT: return std::string("leftarrow");
		case VK_UP: return std::string("uparrow");
		case VK_DOWN: return std::string("downarrow");
		case VK_RIGHT: return std::string("rightarrow");
		case VK_INSERT: return std::string("ins");
		case VK_DELETE: return std::string("del");
		case VK_NUMPAD0: return std::string("numpad_0");
		case VK_NUMPAD1: return std::string("numpad_1");
		case VK_NUMPAD2: return std::string("numpad_2");
		case VK_NUMPAD3: return std::string("numpad_3");
		case VK_NUMPAD4: return std::string("numpad_4");
		case VK_NUMPAD5: return std::string("numpad_5");
		case VK_NUMPAD6: return std::string("numpad_6");
		case VK_NUMPAD7: return std::string("numpad_7");
		case VK_NUMPAD8: return std::string("numpad_8");
		case VK_NUMPAD9: return std::string("numpad_9");
		case VK_MULTIPLY: return std::string("kp_multiply");
		case VK_ADD: return std::string("kp_plus");
		case VK_SUBTRACT: return std::string("kp_minus");
		case VK_DIVIDE: return std::string("kp_slash");
		case VK_F1: return std::string("f1");
		case VK_F2: return std::string("f2");
		case VK_F3: return std::string("f3");
		case VK_F4: return std::string("f4");
		case VK_F5: return std::string("f5");
		case VK_F6: return std::string("f6");
		case VK_F7: return std::string("f7");
		case VK_F8: return std::string("f8");
		case VK_F9: return std::string("f9");
		case VK_F10: return std::string("f10");
		case VK_F11: return std::string("f11");
		case VK_F12: return std::string("f12");
		case VK_OEM_1: return std::string(";");
		case VK_OEM_PLUS: return std::string("+");
		case VK_OEM_MINUS: return std::string("-");
		case VK_OEM_COMMA: return std::string(",");
		case VK_OEM_PERIOD: return std::string(".");
		case VK_OEM_2: return std::string("/");
		case VK_OEM_3: return std::string("~");
		case VK_OEM_4: return std::string("[");
		case VK_OEM_5: return std::to_string(char(0x5C));
		case VK_OEM_6: return std::string("]");
		case VK_OEM_7: return std::to_string(char(0x22));
		default: return std::string(xorstr("unknown key").crypt_get());
	}
}

void CMiscUtils::ParseColor(std::string str, Vector& clr)
{
	LPTSTR pStop;
	int color = _tcstol(str.c_str(), &pStop, 16);
	clr.x = ((color & 0xFF0000) >> 16) / 255.0f;
	clr.y = ((color & 0xFF00) >> 8) / 255.0f;
	clr.z = (color & 0xFF) / 255.0f;
}

std::string CMiscUtils::GetStringFromColor(Vector color)
{
	color *= 255.0f;
	int r = (((int)color.x & 0xFF) << 16);
	int g = (((int)color.y & 0xFF) << 8);
	int b = (((int)color.z & 0xFF));
	std::stringstream s;
	s << std::hex << std::uppercase << (r | g | b);
	return s.str();
}

bool CMiscUtils::IsActiveWindow()
{
	char windowTitle[256];
	char consoleTitle[256];
	HWND hwnd = GetForegroundWindow();
	GetWindowText(hwnd, windowTitle, sizeof(windowTitle));
	GetConsoleTitle(consoleTitle, sizeof(consoleTitle));

	return (!strcmp(windowTitle, consoleTitle));
}

bool CMiscUtils::IsCSGOActiveWindow()
{
	return csgoActiveWindow;
}

bool CMiscUtils::CheckCSGOWindowState()
{
	char windowTitle[256];
	HWND hwnd = GetForegroundWindow();
	GetWindowText(hwnd, windowTitle, sizeof(windowTitle));
	
	csgoActiveWindow = !strcmp(windowTitle, xorstr("Counter-Strike: Global Offensive").crypt_get());
	return csgoActiveWindow;
}

bool CMiscUtils::DoesCSGOExist()
{
	HWND hwnd = FindWindow(NULL, "Counter-Strike: Global Offensive");
	return (hwnd != NULL);
}

HWND CMiscUtils::GetGameWindow()
{
	return FindWindow(NULL, "Counter-Strike: Global Offensive");
}

void CMiscUtils::DisableQuickEdit()
{
	HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
	DWORD consoleMode;
	GetConsoleMode(hConsole, &consoleMode);
	consoleMode &= ~ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(hConsole, consoleMode);
}

std::string CMiscUtils::GetHash(std::string fileName)
{
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	HCRYPTPROV hProv;
	HCRYPTHASH hHash;

	DWORD cbHash = MD5LEN;
	BYTE rgbHash[MD5LEN];
	
	CHAR rgbDigits[] = "0123456789abcdef";

	if (hFile == INVALID_HANDLE_VALUE)
	{
		int dwStatus = GetLastError();
		CMiscUtils::PrintLine(std::string(xorstr("Error reading in .ini").crypt_get()));
		CloseHandle(hFile);
		return std::string(xorstr("ERROR").crypt_get());
	}

	// Get handle to the crypto provider
	if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
	{
		int dwStatus = GetLastError();
		CMiscUtils::PrintLine(std::string(xorstr("Error acquiring CryptAcquireContext").crypt_get()));
		CloseHandle(hFile);
		return std::string("ERROR");
	}

	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
	{
		int dwStatus = GetLastError();
		CMiscUtils::PrintLine(std::string(xorstr("Error creating hash").crypt_get()));
		CloseHandle(hFile);
		CryptReleaseContext(hProv, 0);
		return std::string("ERROR");
	}

	BOOL bResult = FALSE;
	DWORD cbRead;
	BYTE rgbFile[BUFSIZE];

	while (bResult = ReadFile(hFile, rgbFile, BUFSIZE, &cbRead, NULL))
	{
		if (!cbRead)
		{
			break;
		}

		if (!CryptHashData(hHash, rgbFile, cbRead, 0))
		{
			int dwStatus = GetLastError();
			CMiscUtils::PrintLine(std::string(xorstr("Error with CryptHashData").crypt_get()));
			CryptReleaseContext(hProv, 0);
			CryptDestroyHash(hHash);
			CloseHandle(hFile);
			return std::string("ERROR");
		}
	}

	if (!bResult)
	{
		int dwStatus = GetLastError();
		CMiscUtils::PrintLine(std::string(xorstr("ReadFile failed").crypt_get()));
		CryptReleaseContext(hProv, 0);
		CryptDestroyHash(hHash);
		CloseHandle(hFile);
		return std::string("ERROR");
	}

	std::string hashOutput;

	if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
	{
		//printf(xorstr("MD5 hash of file %s is: ").crypt_get(), fileName.c_str());

		for (DWORD i = 0; i < cbHash; i++) {
			hashOutput += std::to_string(rgbDigits[rgbHash[i] >> 4]);
			hashOutput += std::to_string(rgbDigits[rgbHash[i] & 0xf]);

			//printf(xorstr("%c%c").crypt_get(), rgbDigits[rgbHash[i] >> 4], rgbDigits[rgbHash[i] & 0xf]);
		}
		//printf("\n");
	}


	CryptReleaseContext(hProv, 0);
	CryptDestroyHash(hHash);
	CloseHandle(hFile);

	return hashOutput;
}
