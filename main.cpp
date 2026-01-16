#include "HarryPotterSDK.h"
#include <conio.h>
#include <cstdint>
#include <cstdio>

int main() {
    // SetConsoleTitleA("Harry Potter 6 Ultimate SDK");

    printf("==========================================================\n");
    printf("=== Harry Potter SDK - Ultimate Version with Freecam ===\n");
    printf("==========================================================\n");
    printf("Controls:\n");
    printf("- F1: Save position | F2: Load position\n");
    printf("- F3: Toggle Freecam (Fly!)\n");
    printf("- G : Giant Mode (5x)      | T : Tiny Mode (0.2x)\n");
    printf("- R : Reset Scale (1x)     | M : +100 mini-crests\n");
    printf("- X : Quit SDK\n\n");
    printf("Freecam Controls (if active):\n");
    printf("- I/K : Forward/Backward | J/L : Left/Right (horizontal)\n");
    printf("- PageUp/PageDown     : Up/Down\n");
    printf("- SHIFT (Hold)   : Speed boost x4\n");
    printf("- Numpad +/- : Adjust freecam speed\n");
    printf("==========================================================\n\n");
    
    HarryPotterSDK sdk;

    if (!sdk.Init()) {
        printf("Error: Unable to connect to game!\n");
        printf("Pattern Scanning failed. Check if game is up to date.\n");
        _getch();
        return 1;
    }
    
    printf("SDK successfully connected via Pattern Scanning.\n\n");

    Vector3 lastHarryPos = {0, 0, 0};
    float lastScale = 0;
    int32_t lastCrests = -1;
    bool lastFreecamState = false;

    while (true) {
        sdk.UpdatePointers();

        Vector3 harryPos = sdk.GetHarryPos();
        float scale      = sdk.GetScale();
        int32_t crests   = sdk.GetMiniCrests();
        bool isFreecam   = sdk.IsFreecamActive();
        
        if (harryPos.x != lastHarryPos.x || scale != lastScale || crests != lastCrests || isFreecam != lastFreecamState) {
            
            const char* fcStatus = isFreecam ? "[ON]" : "OFF";
            printf("\rHarry[X:%.1f Z:%.1f] | Scale:%.1fx | Crests:%d | Freecam:%s Speed:%.1f    ", 
                   harryPos.x, harryPos.z, scale, crests, fcStatus, sdk.GetFreecamSpeed());
            
            lastHarryPos = harryPos;
            lastScale = scale;
            lastCrests = crests;
            lastFreecamState = isFreecam;
        }
        
        if (GetAsyncKeyState(VK_F1) & 0x8000) { sdk.SavePosition(); Sleep(200); }
        if (GetAsyncKeyState(VK_F2) & 0x8000) { sdk.LoadPosition(); Sleep(200); }
        
        if (GetAsyncKeyState(VK_F3) & 1) { sdk.ToggleFreecam(); }
        
        if (GetAsyncKeyState('G') & 0x8000) { sdk.SetScale(5.0f);  Sleep(150); }
        if (GetAsyncKeyState('T') & 0x8000) { sdk.SetScale(0.2f);  Sleep(150); }
        if (GetAsyncKeyState('R') & 0x8000) { sdk.SetScale(1.0f);  Sleep(150); }
        
        if (GetAsyncKeyState('M') & 0x8000) { 
            sdk.SetMiniCrests(sdk.GetMiniCrests() + 100); 
            Sleep(150); 
        }
        
        if (GetAsyncKeyState('X') & 0x8000) {
            if (sdk.IsFreecamActive()) sdk.ToggleFreecam(); 
            break;
        }

        sdk.UpdateFreecam();
        
        Sleep(10); 
    }
    
    printf("\n\nSDK stopped. Cleanup done.\n");
    return 0;
}