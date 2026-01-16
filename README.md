# Harry Potter 6 SDK

A comprehensive SDK (Software Development Kit) for Harry Potter and the Half-Blood Prince game with advanced features including freecam, teleportation, and memory manipulation.

## Features

- **Freecam Mode**: Detach camera from Harry and fly freely around the game world
- **Teleportation**: Save and load positions instantly
- **Scale Manipulation**: Change Harry's size (Giant/Tiny modes)
- **Memory Management**: Structured access to game memory
- **Real-time Display**: Live position and statistics updates
- **Pattern Scanning**: Automatic memory address detection

## Controls

### Main Controls
- **F1**: Save position
- **F2**: Load position  
- **F3**: Toggle Freecam (Fly mode)
- **G**: Giant Mode (5x scale)
- **T**: Tiny Mode (0.2x scale)
- **R**: Reset Scale (1x)
- **M**: Add 100 mini-crests
- **X**: Quit SDK

### Freecam Controls (when active)
- **I/K**: Forward/Backward
- **J/L**: Left/Right (horizontal)
- **PageUp/PageDown**: Up/Down
- **SHIFT (Hold)**: Speed boost x4
- **Numpad +/-**: Adjust freecam speed

## Files Structure

```
├── HarryPotterSDK.h     # SDK class definitions
├── HarryPotterSDK.cpp   # SDK implementation
├── main.cpp             # Main application
├── build.bat            # Build script
├── README.md            # This file
├── LICENSE              # MIT License
└── .gitignore           # Git ignore file
```

## Building

### Quick Build
```batch
build.bat
```

### Manual Build
```batch
g++ -o HarryPotterSDK.exe main.cpp HarryPotterSDK.cpp -lpsapi
```

## Usage

1. Launch Harry Potter 6 game
2. Run `HarryPotterSDK.exe`
3. Use the controls to manipulate the game

## Architecture

### Core Classes

#### Vector3 Structure
```cpp
struct Vector3 {
    float x, y, z;
};
```

#### HarryPotterSDK Class
Main interface for game interaction:
- Memory reading/writing
- Process attachment
- Freecam control
- Position manipulation

### Memory Addresses

- **Harry Pointer**: `baseModule + 0x935CE8` (dynamic)
- **Mini Crests**: `baseModule + 0x8EF624` (static)
- **Camera Base**: `baseModule + 0x971240` (dynamic)
- **Camera Instruction**: Pattern scanning or `0x5F5912` (static)

### Key Offsets

- **Harry Position**: `dynamicHarryBase + 0x40`
- **Harry Scale**: `dynamicHarryBase + 0xA4`
- **Camera Position**: `dynamicCameraBase + 0x310`
- **Camera Look Vector**: `dynamicCameraBase + 0x340`

## Technical Details

### Pattern Scanning
The SDK uses pattern scanning to find critical memory addresses dynamically:
```cpp
cameraInstructionAddr = FindPattern("\x0F\x29\x86\x10\x03\x00\x00", "xxxxxxx");
```

### Memory Patching
For freecam functionality, the SDK patches camera instructions:
- **PATCH**: Replace with NOP instructions to detach camera
- **RESTORE**: Replace original MOVAPS instruction

### Process Management
- Automatic process detection (`hp6.exe`)
- Memory permission handling
- Safe memory operations

## Requirements

- Windows OS
- Harry Potter 6 game
- MinGW/g++ compiler
- psapi library

## Safety Notes

- Game must be running before using the SDK
- Designed for specific game version
- Use responsibly in single-player mode only

## Contributing

Feel free to extend the SDK with new features:
- Additional memory addresses
- New manipulation functions
- Enhanced UI/controls

## License

This project is for educational purposes only.
