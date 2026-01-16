#pragma once
#include <windows.h>
#include <cstdint>
#include <vector>
#include <string>

struct Vector3 {
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
};

class HarryPotterSDK {
private:
    HANDLE hProcess;
    uintptr_t baseModule;
    
    uintptr_t cameraInstructionAddr; 
    uintptr_t harryPointerAddr;      
    uintptr_t crestsStaticAddr;      

    uintptr_t dynamicCameraBase;
    uintptr_t dynamicHarryBase;

    // Freecam
    bool freecamActive;
    Vector3 freecamPos;
    float freecamSpeed;
    Vector3 savedPosition;
    bool hasSavedPosition;

    uintptr_t FindPattern(const char* pattern, const char* mask);

public:
    HarryPotterSDK();
    ~HarryPotterSDK();

    bool Init();
    void UpdatePointers();
    void PatchMemory(LPVOID addr, BYTE* data, int size);

    // Gameplay
    Vector3 GetHarryPos();
    bool SetScale(float scale);
    float GetScale();
    int32_t GetMiniCrests();
    void SetMiniCrests(int32_t count);

    void ToggleFreecam();
    void UpdateFreecam();
    void SavePosition();
    void LoadPosition();
    
    bool IsFreecamActive() { return freecamActive; }
    float GetFreecamSpeed() { return freecamSpeed; }
    uintptr_t GetCameraBase() { return dynamicCameraBase; }
    uintptr_t GetHarryBase() { return dynamicHarryBase; }
};