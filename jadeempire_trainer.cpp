#include "jadeempire_trainer.hpp"

using namespace std;

void exit_with_error(char *err, HANDLE *hProcess)
{
	cout << "[!] " << err << endl;
	CloseHandle(hProcess);
	cin.get();
	exit(1);
}

using namespace std;

DWORD findDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress)
{ 
    DWORD pointer = BaseAddress; 
    DWORD pTemp; 
    DWORD pointerAddr; 
  
    for (int c = 0; c < PointerLevel; c++) { 
            if (c == 0) { 
                ReadProcessMemory(hProcHandle, (LPCVOID)pointer, &pTemp, sizeof(pTemp), NULL); 
            } 
            pointerAddr = pTemp + Offsets[c]; 
            ReadProcessMemory(hProcHandle, (LPCVOID)pointerAddr, &pTemp, sizeof(pTemp), NULL); 
    }

    return pointerAddr; 
}

DWORD_PTR getModuleBaseAddress(DWORD dwProcID, TCHAR *szModuleName)
{
    DWORD_PTR dwModuleBaseAddress = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcID);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 ModuleEntry32;
        ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
        if (Module32First(hSnapshot, &ModuleEntry32)) {
            do {
                if (_tcsicmp(ModuleEntry32.szModule, szModuleName) == 0) {
                    dwModuleBaseAddress = (DWORD_PTR)ModuleEntry32.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnapshot, &ModuleEntry32));
        }
        CloseHandle(hSnapshot);
    }
    return dwModuleBaseAddress;
}

#pragma comment(lib, "user32.lib")
int main()
{
	HWND dWin = FindWindow(0, _T(winStr));
	HANDLE hProcess;
	DWORD pid;
	DWORD BaseAddr;

	// save pid
	GetWindowThreadProcessId(dWin, &pid);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	if (!hProcess)
		exit_with_error("Problem opening process. Try running as admin", &hProcess);

	cout << ">> Jade Empire Trainer Loaded\n";
	int canRead;
	int canWrite;
	float focus;
	float chi;
	float addFocus = 2000.00;
	float addChi   = 2000.00;
	
	BaseAddr = getModuleBaseAddress(pid, (TCHAR*)exeStr);
	BaseAddr = (BaseAddr+0x36C924);

	DWORD focusAddr;
	DWORD chiAddr;
	focusAddr = findDmaAddy(5, hProcess, focusOffsets, BaseAddr);
	chiAddr   = findDmaAddy(5, hProcess, chiOffsets, BaseAddr);

	while(1) {
		canRead = ReadProcessMemory(hProcess, (LPCVOID)focusAddr, &focus, sizeof(focus), NULL);
		if (!canRead)
			exit_with_error(ERROR_MEMORY_READ, &hProcess);

		if (focus < 120.00) {
			canWrite = WriteProcessMemory(hProcess, (void*)focusAddr, &addFocus, sizeof(addFocus), NULL);
			if (!canWrite)
				exit_with_error(ERROR_MEMORY_WRITE, &hProcess);
		}

		canRead = ReadProcessMemory(hProcess, (LPCVOID)chiAddr, &chi, sizeof(chi), NULL);
		if (!canRead)
			exit_with_error(ERROR_MEMORY_READ, &hProcess);

		if (chi < 120.00) {
			canWrite = WriteProcessMemory(hProcess, (void*)chiAddr, &addChi, sizeof(addChi), NULL);
			if (!canWrite)
				exit_with_error(ERROR_MEMORY_WRITE, &hProcess);
		}

		Sleep(500);
	}

	CloseHandle(hProcess);
	return 0;
}
