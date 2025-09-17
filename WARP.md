# WARP.md

This file provides guidance to WARP (warp.dev) when working with code in this repository.

## Common Development Commands

### Project Generation and Compilation
This project uses a custom C#-based build system with PowerShell scripts for code generation and compilation.

#### Generate Project Files
```powershell
pwsh ProjectTools/Scripts/GenCodeProject.ps1 -Project <PROJECT> -Generator <GENERATOR> -Platform <PLATFORM> -Configuration <CONFIG>
```
- **Projects**: `Core`, `AppleFrameworks`, `Networking`, `TestSuit`
- **Generators**: `Clang`, `VisualStudio`
- **Platforms**: `iOS`, `tvOS`, `visionOS`, `macOS`, `Windows`, `Android`
- **Configurations**: `Debug`, `Release`

#### Compile Projects
```powershell
pwsh ProjectTools/Scripts/Compile.ps1 -Project <PROJECT> -Platform <PLATFORM> -Configuration <CONFIG>
```

#### Clean Build
```powershell
pwsh ProjectTools/Scripts/Compile.ps1 -Project <PROJECT> -Platform <PLATFORM> -Configuration <CONFIG> -Clean
```

#### Recompile (Force Rebuild)
```powershell
pwsh ProjectTools/Scripts/Compile.ps1 -Project <PROJECT> -Platform <PLATFORM> -Configuration <CONFIG> -Recompile
```

#### Compile Specific Test Modules
For Core tests:
```powershell
# macOS
pwsh ProjectTools/Scripts/Compile.ps1 -Project Core -Platform macOS -Configuration Debug -Modules CoreTests

# Windows
pwsh ProjectTools/Scripts/Compile.ps1 -Project Core -Platform Windows -Configuration Debug -Modules CoreTests
```

For Networking tests:
```powershell
# macOS
pwsh ProjectTools/Scripts/Compile.ps1 -Project Networking -Platform macOS -Configuration Debug -Modules NetworkingTests

# Windows
pwsh ProjectTools/Scripts/Compile.ps1 -Project Networking -Platform Windows -Configuration Debug -Modules NetworkingTests
```

### Build System Tools
```powershell
# Compile only the ProjectTools (C# build system)
pwsh ProjectTools/Scripts/Compile.ps1 -ProjectToolsOnly
```

## Project Architecture

### High-Level Structure
This is a **cross-platform C++ mobile framework** with the following key characteristics:

1. **Module-based Architecture**: The project is organized into modules, each with their own `.Module.cs` definition files that configure platform-specific dependencies, compilation flags, and linking requirements.

2. **Project Hierarchy**: 
   - **Core**: Foundation module providing core functionality, depends on platform-specific frameworks
   - **AppleFrameworks**: Apple-specific wrappers (Foundation, Metal, MetalFX, AppKit, QuartzCore)
   - **Networking**: HTTP networking module that depends on Core
   - **TestSuit**: Test framework library

3. **Custom Build System**: Uses a C# .NET-based build tool (`ProjectTools`) that:
   - Generates platform-specific project files (Clang/VisualStudio)
   - Handles cross-platform source file filtering
   - Manages module dependencies and platform-specific configurations
   - Supports multiple target platforms simultaneously

### Module System
Each project contains multiple modules defined by `.Module.cs` files:
- Modules inherit from `AModuleDefinition`
- Support platform-specific dependencies via `ETargetPlatform` enumeration
- Can specify different binary types: `DynamicLibrary`, `StaticLibrary`, `Executable`, `ShaderLibrary`
- Platform-specific code is automatically filtered based on directory paths containing platform names

### Apple Frameworks Integration
The `AppleFrameworks` project provides **metal-cpp** integration:
- Header-only C++ interface for Metal graphics API
- Direct mapping of Objective-C Metal classes to C++ namespace `MTL::`
- Memory management following Cocoa/CoreFoundation patterns
- Support for Foundation, Metal, MetalFX, AppKit, and QuartzCore frameworks
- Requires C++17 for `constexpr` usage in `NS::Object`

### Platform Support
- **Apple**: iOS, tvOS, visionOS, macOS (uses Clang generator)
- **Microsoft**: Windows (uses VisualStudio generator)  
- **Google**: Android (uses Clang generator)

### Source Code Organization
- Platform-specific code is organized in subdirectories named after platforms/platform groups
- Build system automatically excludes sources not matching the target platform
- Headers use extensions: `.h`, `.hh`, `.hpp`, `.hxx`
- Sources use extensions: `.c`, `.cpp`, `.cc`, `.cxx`, `.c++`, `.i`, `.ii`

### Dependencies and Linking
- Modules declare dependencies through `.Module.cs` configuration
- Platform-specific linking (e.g., `shell32.lib`, `ole32.lib` for Windows)
- Apple frameworks linked automatically (e.g., `Foundation` framework for macOS)

## Key Files and Directories

- `ProjectTools/` - Git submodule containing the C# build system
- `ProjectTools/Scripts/` - PowerShell build scripts
- `*.Project.cs` - Project definition files
- `Modules/*/` - Module definition files and source code
- `.vscode/tasks.json` - VSCode build tasks configuration

## Development Workflow

1. **Setup**: Ensure ProjectTools submodule is initialized and up-to-date
2. **Generate**: Run `GenCodeProject.ps1` to generate platform-specific build files
3. **Compile**: Use `Compile.ps1` to build projects or specific modules
4. **Test**: Compile and run specific test modules using the `-Modules` parameter
5. **Clean**: Use `-Clean` flag to remove build artifacts when needed

## Platform-Specific Notes

### Apple Platforms
- Uses metal-cpp for graphics programming
- Memory management follows manual reference counting (no ARC in C++)
- Foundation framework integration for core platform services
- Automatic framework linking handled by build system

### Windows
- Links against system libraries (`shell32.lib`, `ole32.lib`)
- Uses VisualStudio generator for project files

### Multi-Platform Development
- Source code filtering happens automatically based on directory structure
- Platform-specific dependencies configured per-module
- Single codebase supports all target platforms through conditional compilation
