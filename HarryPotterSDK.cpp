#include "HarryPotterSDK.h"
#include <psapi.h>
#include <tlhelp32.h>
#include <cstdio>
#include <vector>

HarryPotterSDK::HarryPotterSDK() : hProcess(NULL), baseModule(0), cameraInstructionAddr(0), 
    harryPointerAddr(0), crestsStaticAddr(0), dynamicCameraBase(0), dynamicHarryBase(0),
    freecamActive(false), freecamSpeed(2.0f), hasSavedPosition(false) {}

HarryPotterSDK::~HarryPotterSDK() { if (hProcess) CloseHandle(hProcess); }

uintptr_t HarryPotterSDK::FindPattern(const char* pattern, const char* mask) {
    MODULEINFO mInfo = { 0 };
    if (!K32GetModuleInformation(hProcess, (HMODULE)baseModule, &mInfo, sizeof(MODULEINFO))) return 0;
    
    std::vector<BYTE> remoteMem(mInfo.SizeOfImage);
    if (!ReadProcessMemory(hProcess, (LPCVOID)baseModule, remoteMem.data(), mInfo.SizeOfImage, NULL)) return 0;

    size_t patternLength = strlen(mask);
    for (uintptr_t i = 0; i < mInfo.SizeOfImage - patternLength; i++) {
        bool found = true;
        for (size_t j = 0; j < patternLength; j++) {
            if (mask[j] != '?' && (BYTE)pattern[j] != remoteMem[i + j]) {
                found = false;
                break;
            }
        }
        if (found) return baseModule + i;
    }
    return 0;
}

bool HarryPotterSDK::Init() {
    PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) };
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) return false;
    
    bool foundProc = false;
    if (Process32First(hSnap, &pe32)) {
        do {
            if (_stricmp(pe32.szExeFile, "hp6.exe") == 0) {
                hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
                foundProc = true;
                break;
            }
        } while (Process32Next(hSnap, &pe32));
    }
    CloseHandle(hSnap);
    if (!foundProc) return false;

    HMODULE hMods[1024]; DWORD cbNeeded;
    if (K32EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) baseModule = (uintptr_t)hMods[0];

    harryPointerAddr = baseModule + 0x935CE8; 
    crestsStaticAddr = baseModule + 0x8EF624;

    cameraInstructionAddr = FindPattern("\x0F\x29\x86\x10\x03\x00\x00", "xxxxxxx");
    if (!cameraInstructionAddr) cameraInstructionAddr = 0x5F5912; 

    UpdatePointers();
    
    printf("\n[SDK] --- Sync Successful ---\n");
    printf("[SDK] Base: 0x%X | HarryPtr: 0x%X\n", baseModule, harryPointerAddr);
    printf("[SDK] Crests: 0x%X | CamInstr: 0x%X\n", crestsStaticAddr, cameraInstructionAddr);
    printf("[SDK] Dynamic Cam Base: 0x%X\n", dynamicCameraBase);

    return true; 
}

void HarryPotterSDK::UpdatePointers() {
    ReadProcessMemory(hProcess, (LPCVOID)harryPointerAddr, &dynamicHarryBase, 4, NULL);

    ReadProcessMemory(hProcess, (LPCVOID)(baseModule + 0x971240), &dynamicCameraBase, 4, NULL);
}

void HarryPotterSDK::ToggleFreecam() {
    if (!cameraInstructionAddr || !dynamicCameraBase) return;

    freecamActive = !freecamActive;
    if (freecamActive) {
        BYTE nops[7] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
        PatchMemory((LPVOID)cameraInstructionAddr, nops, 7);
        
        ReadProcessMemory(hProcess, (LPCVOID)(dynamicCameraBase + 0x310), &freecamPos, sizeof(Vector3), NULL);
        printf("\n[SDK] Freecam activated (Keys I,K,J,L, PageUp/Down)");
    } else {
        BYTE orig[7] = { 0x0F, 0x29, 0x86, 0x10, 0x03, 0x00, 0x00 };
        PatchMemory((LPVOID)cameraInstructionAddr, orig, 7);
        printf("\n[SDK] Freecam deactivated");
    }
}

void HarryPotterSDK::UpdateFreecam() {
    if (!freecamActive || !dynamicCameraBase) return;

    if (GetAsyncKeyState(VK_ADD) & 0x8000) freecamSpeed += 0.2f;
    if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000) freecamSpeed -= 0.2f;
    if (freecamSpeed < 0.1f) freecamSpeed = 0.1f;
    
    float s = (GetAsyncKeyState(VK_SHIFT) & 0x8000) ? freecamSpeed * 5 : freecamSpeed;

    Vector3 fw;
    ReadProcessMemory(hProcess, (LPCVOID)(dynamicCameraBase + 0x340), &fw, sizeof(Vector3), NULL);
    Vector3 rt(fw.y, -fw.x, 0); 

    if (GetAsyncKeyState('I') & 0x8000) { freecamPos.x += fw.x * s; freecamPos.y += fw.y * s; freecamPos.z += fw.z * s; }
    if (GetAsyncKeyState('K') & 0x8000) { freecamPos.x -= fw.x * s; freecamPos.y -= fw.y * s; freecamPos.z -= fw.z * s; }
    if (GetAsyncKeyState('J') & 0x8000) { freecamPos.x -= rt.x * s; freecamPos.y -= rt.y * s; }
    if (GetAsyncKeyState('L') & 0x8000) { freecamPos.x += rt.x * s; freecamPos.y += rt.y * s; }
    if (GetAsyncKeyState(VK_PRIOR) & 0x8000) freecamPos.z += s; 
    if (GetAsyncKeyState(VK_NEXT) & 0x8000)  freecamPos.z -= s; 

    WriteProcessMemory(hProcess, (LPVOID)(dynamicCameraBase + 0x310), &freecamPos, sizeof(Vector3), NULL);
}

int32_t HarryPotterSDK::GetMiniCrests() {
    int32_t c = 0;
    if (crestsStaticAddr) ReadProcessMemory(hProcess, (LPCVOID)crestsStaticAddr, &c, 4, NULL);
    return c;
}

void HarryPotterSDK::SetMiniCrests(int32_t count) {
    if (crestsStaticAddr) WriteProcessMemory(hProcess, (LPVOID)crestsStaticAddr, &count, 4, NULL);
}

Vector3 HarryPotterSDK::GetHarryPos() {
    Vector3 p = {0,0,0};
    if (dynamicHarryBase) ReadProcessMemory(hProcess, (LPCVOID)(dynamicHarryBase + 0x40), &p, sizeof(Vector3), NULL);
    return p;
}

float HarryPotterSDK::GetScale() {
    float s = 1.0f;
    if (dynamicHarryBase) ReadProcessMemory(hProcess, (LPCVOID)(dynamicHarryBase + 0xA4), &s, 4, NULL);
    return s;
}

bool HarryPotterSDK::SetScale(float scale) {
    if (!dynamicHarryBase) return false;
    return WriteProcessMemory(hProcess, (LPVOID)(dynamicHarryBase + 0xA4), &scale, 4, NULL);
}

void HarryPotterSDK::PatchMemory(LPVOID addr, BYTE* data, int size) {
    DWORD old;
    VirtualProtectEx(hProcess, addr, size, PAGE_EXECUTE_READWRITE, &old);
    WriteProcessMemory(hProcess, addr, data, size, NULL);
    VirtualProtectEx(hProcess, addr, size, old, &old);
}

void HarryPotterSDK::SavePosition() { savedPosition = GetHarryPos(); hasSavedPosition = true; }
void HarryPotterSDK::LoadPosition() { 
    if (hasSavedPosition && dynamicHarryBase) 
        WriteProcessMemory(hProcess, (LPVOID)(dynamicHarryBase + 0x40), &savedPosition, sizeof(Vector3), NULL); 
}