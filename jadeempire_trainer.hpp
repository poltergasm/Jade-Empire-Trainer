#define ERROR_MEMORY_READ "Cannot read from memory"
#define ERROR_MEMORY_WRITE "Failed to write to memory"

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <TlHelp32.h>
#include <tchar.h>

const char *winStr = "Jade Empire";
const char *exeStr = "JadeEmpire.exe";

// offsets
DWORD focusOffsets[] = {0xA0, 0x0, 0x0, 0x68, 0x48C};
DWORD chiOffsets[]   = {0xA0, 0x0, 0x0, 0x528, 0x478};
DWORD pointsOffsets[] = {0xA0, 0x0, 0x0, 0x68, 0x83C};
DWORD alignOffsets[]  = {0xA0, 0x0, 0x0, 0x68, 0x4E8};

// function prototypes
void exit_with_error(char *err, HANDLE *hProcess);
DWORD findDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress);
DWORD_PTR getModuleBaseAddress(DWORD dwProcID, TCHAR *szModuleName);
