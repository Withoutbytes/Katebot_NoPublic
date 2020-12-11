#include "..\main.h"

Mem* mem = new Mem();
Module* client = nullptr;
Module* engine = nullptr;
Module* csgobase = nullptr;

#define IS_WOW64_ADDRESS(address) (address <= 0xffffffffUL)

Mem::Mem()
{

}

Mem::~Mem()
{
	Detach();
}

bool Mem::Attach(const std::string& ExeName)
{
	Detach();

	if (ExeName.empty()) return false;

	m_dwProcessId = GetProcessIdByName(ExeName);

	if (!m_dwProcessId) return false;

	m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_dwProcessId);

	if (!m_hProcess) return false;

	return DumpModList();
}

void Mem::Detach()
{
	if (m_hProcess) CloseHandle(m_hProcess);

	m_hProcess = NULL;
	m_dwProcessId = NULL;

	SAFE_DELETE_VECTOR(m_pModList);
}

bool Mem::Read(DWORD_PTR dwAddress, LPVOID lpBuffer, DWORD_PTR dwSize)
{
	SIZE_T Out = NULL;

	return (ReadProcessMemory(m_hProcess, (LPCVOID)dwAddress, lpBuffer, dwSize, &Out) == TRUE);
}

bool Mem::Write(DWORD_PTR dwAddress, LPCVOID lpBuffer, DWORD_PTR dwSize)
{
	SIZE_T Out = NULL;

	return (WriteProcessMemory(m_hProcess, (LPVOID)dwAddress, lpBuffer, dwSize, &Out) == TRUE);
}

Module* Mem::GetModule(const std::string& ImageName)
{
	if (m_pModList.empty())
	{
		return nullptr;
	}

	for (auto& pMod : m_pModList)
	{
		if (ImageName.compare(pMod->GetName()) == 0)
		{
			return pMod;
		}
	}

	return nullptr;
}

uintptr_t Mem::GetModulePtr(const std::string& ImageName)
{
	return GetModule(ImageName)->GetImage();
}

void Mem::SetWindow(HWND window)
{
	m_hWindow = window;
}

HWND Mem::GetWindow()
{
	return m_hWindow;
}

HMODULE Mem::LoadRemote(const std::string& ImageName)
{
	if (m_pModList.empty())
	{
		return NULL;
	}

	for (auto& pMod : m_pModList)
	{
		if (ImageName.compare(pMod->GetName()) == 0)
		{
			return LoadLibrary(pMod->GetPath().c_str());
		}
	}

	return NULL;
}

bool DataCompare(const BYTE* pData, const BYTE* pMask, const char* pszMask)
{
	for (; *pszMask; ++pszMask, ++pData, ++pMask)
	{
		if (*pszMask == 'x' && *pData != *pMask)
		{
			return false;
		}
	}

	return (*pszMask == NULL);
}

DWORD_PTR Mem::Scan(DWORD_PTR dwStart, DWORD_PTR dwSize, const char* pSignature, const char* pMask)
{
	BYTE* pRemote = new BYTE[dwSize];	// Forgot to delete this after we're finshed with it, resulting in a memory leak

	if (!Read(dwStart, pRemote, dwSize))
	{
		delete[] pRemote;
		return NULL;
	}

	for (DWORD_PTR dwIndex = 0; dwIndex < dwSize; dwIndex++) {
		if (DataCompare((const BYTE*)(pRemote + dwIndex), (const BYTE*)pSignature, pMask))
		{
			delete[] pRemote;
			return (dwStart + dwIndex);
		}
	}
	delete[] pRemote;

	return NULL;
}

DWORD_PTR Mem::FindPattern(Module* mod, const char* pattern)
{
	if (!mod->GetImage())
		return 0;

	static auto compare_bytes = [](const byte* bytes, const char* pattern) -> bool
	{
		for (; *pattern; *pattern != ' ' ? ++bytes : bytes, ++pattern) {
			if (*pattern == ' ' || *pattern == '?')
				continue;
			if (*bytes != getByte(pattern))
				return false;
			++pattern;
		}
		return true;
	};

	auto get_text_section = [&](uintptr_t& start, size_t& size)
	{
		auto header = (byte*)malloc(0x1000);
		Read(mod->GetImage(), header, 0x1000);

		auto pDosHdr = (const IMAGE_DOS_HEADER*)(header);
		if (pDosHdr->e_magic != IMAGE_DOS_SIGNATURE)
		{
			::free(header);
			return false;
		}
		const IMAGE_NT_HEADERS* pImageHdr = (const IMAGE_NT_HEADERS*)((uint8_t*)pDosHdr + pDosHdr->e_lfanew);

		if (pImageHdr->Signature != IMAGE_NT_SIGNATURE)
		{
			::free(header);
			return false;
		}

		auto pSection = (const IMAGE_SECTION_HEADER*)((uint8_t*)pImageHdr + sizeof(IMAGE_NT_HEADERS));
		for (int i = 0; i < pImageHdr->FileHeader.NumberOfSections; ++i, pSection++)
		{
			if (_stricmp((LPCSTR)pSection->Name, xorstr(".text").crypt_get()) == 0)
			{
				start = (uintptr_t)(mod->GetImage() + pSection->VirtualAddress);
				size = pSection->Misc.VirtualSize;
				::free(header);
				return true;
			}
		}

		::free(header);
		return false;
	};

	uintptr_t base; size_t size;
	if (!get_text_section(base, size))
	{
		base = mod->GetImage();
		size = mod->GetSize();
	}

	auto pb = (byte*)malloc(size);
	auto max = size;
	Read(base, pb, size);

	uintptr_t offset = 0;
	for (auto off = 0UL; off < max; ++off) {
		if (compare_bytes(pb + off, pattern)) {
			offset = base + off;
			break;
		}
	}

	return offset;
}

bool Mem::DumpModList()
{
	m_pModList.clear();

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_dwProcessId);

	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	MODULEENTRY32 Entry = { NULL };

	Entry.dwSize = sizeof(MODULEENTRY32);

	if (!Module32First(hSnapshot, &Entry))
	{
		CloseHandle(hSnapshot);
		return false;
	}

	Module* pMod = nullptr;

	do
	{
		char szPath[MAX_PATH] = { NULL };

		GetModuleFileNameEx(m_hProcess, Entry.hModule, szPath, MAX_PATH);

		pMod = new Module((DWORD_PTR)Entry.hModule, (DWORD_PTR)Entry.modBaseSize, Entry.szModule, szPath);
		m_pModList.push_back(pMod);
	} while (Module32Next(hSnapshot, &Entry));

	CloseHandle(hSnapshot);

	return !m_pModList.empty();
}
uintptr_t Mem::FindExport(uintptr_t module, const char* name)
{
	uintptr_t a0;
	uint32_t  a1[4], a2[30];

	a0 = module + this->Read<uint16_t>(module + 0x3C);
	a0 = module + this->Read<uint32_t>(a0 + 0x88 - (bool)(IS_WOW64_ADDRESS(module)) * 16);
	this->Read(a0 + 0x18, &a1, sizeof(a1));
	while (a1[0]--) {
		a0 = this->Read<uint32_t>(module + a1[2] + (a1[0] * 4));
		this->Read(module + a0, &a2, sizeof(a2));
		if (!_stricmp((const char*)a2, name)) {
			return (module + this->Read<uint32_t>(module + a1[1] + (this->Read<uint16_t>(module + a1[3] + (a1[0] * 2)) * 4)));
		}
	}
	return 0;
}

DWORD Mem::AllocMem(size_t size)
{
	return (DWORD)VirtualAllocEx(mem->m_hProcess, nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
}

bool Mem::Free(void* _address, size_t _size)
{
	return VirtualFreeEx(this->m_hProcess, _address, _size, MEM_RELEASE);
}

void Mem::CRT(void* shellcode, void* parameter) {
	HANDLE Thread = CreateRemoteThread(this->m_hProcess, nullptr, NULL, static_cast<LPTHREAD_START_ROUTINE>(shellcode), parameter, NULL, nullptr);
	WaitForSingleObject(Thread, INFINITE);
	CloseHandle(Thread);
}

DWORD Mem::GetProcessIdByName(const std::string& name)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	PROCESSENTRY32 Entry = { NULL };

	Entry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnapshot, &Entry))
	{
		CloseHandle(hSnapshot);
		return NULL;
	}

	do
	{
		if (name.compare(Entry.szExeFile) == 0)
		{
			break;
		}
	} while (Process32Next(hSnapshot, &Entry));


	CloseHandle(hSnapshot);

	return Entry.th32ProcessID;
}