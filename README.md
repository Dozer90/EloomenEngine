# EloomenEngine

A modern C++ game engine built from the ground up as a learning exercise to understand low-level engine architecture, data-oriented design principles, and custom rendering backends.

## 🎯 Project Vision

EloomenEngine is a **learning-focused game engine** designed to explore:
- **Data-Oriented Design (DOD)** for cache-friendly memory layouts and optimal performance
- **Custom rendering backends** with platform-specific optimizations (currently DX12)
- **Modern C++20** features and best practices
- **Minimal dependencies** with full control over the engine's internals

This project prioritizes understanding and experimentation over production-readiness. It's a playground for learning how AAA game engines work under the hood.

### 🤖 Development Approach

This project is being developed **with AI assistance** (Claude by Anthropic) to help with:
- Code reviews and catching potential bugs early
- Architectural guidance and best practices
- Staying organized as the codebase grows
- Learning resources and explanations of complex concepts

**Important:** The AI serves as a **mentor and code reviewer**, not a replacement for learning. All architectural decisions, implementations, and understanding come from hands-on work. The goal is to learn engine development deeply, with AI helping to navigate the complexity and maintain code quality - similar to having an experienced developer available for questions and reviews.

---

## 🚀 Current Status

**Phase:** Getting to First Window & Triangle

✅ **Completed:**
- Core math library (vectors, matrices, quaternions, interpolation)
- ID-based handle system for memory management
- Backend abstraction layer for renderers, windows, input
- DX12 backend infrastructure
- Event system architecture
- CMake build system
- Code compiles successfully on Windows

🚧 **In Progress:**
- Getting the demo application to open a window
- Rendering the first triangle
- Finalizing DX12 pipeline setup

---

## 🏗️ Architecture Overview

### Core Systems

**Math Library** (`engine/include/maths/`)
- Comprehensive vector (float2/3/4, int2/3/4) and matrix (2x2, 3x3, 4x4) types
- Quaternion support for rotations
- ~1700 lines of optimized math functions
- Safe normalize operations with zero-vector handling

**Memory Management** (`engine/include/utility/managed_memory_block.h`)
- ID-based handle system to prevent dangling references
- Pooled memory allocation for datatypes
- Component-style storage (separate X, Y, Z arrays)

**Backend Abstraction** (`engine/backends/core/interfaces/`)
- Clean interface layer for platform-specific implementations
- Renderer, Window, Input, Audio, Filesystem, Networking interfaces
- Currently supports DX12 on Windows

**Event System** (`engine/include/events/`)
- Observer pattern for engine and game events
- Decoupled communication between systems

### Dependencies

- **EASTL** - EA Standard Template Library for cross-platform STL
- **Box2D** - Physics engine
- **ImGui** - Immediate mode GUI for debug tools
- **DirectX 12** - Rendering API (Windows only for now)

---

## 🛠️ Building the Project

### Prerequisites

**Windows 10/11:**
- Visual Studio 2022 (Community Edition or higher)
- Windows SDK 10.0.22621.0 or later
- CMake 3.15+

### Build Instructions

1. **Clone the repository:**
   ```bash
   git clone https://github.com/Dozer90/EloomenEngine.git
   cd EloomenEngine
   ```

2. **Generate Visual Studio solution:**
   ```bash
   mkdir build
   cd build
   cmake .. -G "Visual Studio 17 2022" -A x64 -DELOO_RENDERER_BACKEND=DX12
   ```

3. **Build the engine:**
   ```bash
   cmake --build . --config Debug
   # Or open build/EloomenEngine.sln in Visual Studio
   ```

4. **Run the demo:**
   ```bash
   cd bin/Debug
   ./EloomDemo.exe
   ```

### Configuration Options

- `ELOO_RENDERER_BACKEND` - Choose rendering backend
  - `DX12` (default on Windows) - DirectX 12
  - `Vulkan` (planned) - Cross-platform Vulkan
  - `Metal` (planned) - macOS/iOS
  - `OpenGL` (planned) - Legacy fallback

- `CMAKE_BUILD_TYPE` - Build configuration
  - `Debug` - Full debug symbols, no optimization
  - `Release` - Full optimization, minimal debug info
  - `RelWithDebInfo` - Optimized with debug symbols

---

## 🎮 Planned Capabilities

### Short-Term Goals (Getting to Usable)

**Phase 1: First Window (Current)**
- ✅ Engine compiles without linker errors
- ✅ Math library is functional and safe
- 🚧 Open a blank window on Windows
- 🚧 DX12 device and swapchain initialization
- 🚧 Basic message pump and event handling

**Phase 2: First Triangle**
- Minimal DX12 rendering pipeline
- Vertex and index buffers
- Simple vertex/pixel shaders
- Clear screen and draw a colored triangle
- Basic input handling (keyboard/mouse)

**Phase 3: Basic 3D Scene**
- Camera system (perspective projection, FPS controls)
- Model loading (simple mesh formats)
- Basic lighting (Phong/Blinn-Phong)
- Texture loading and sampling
- Transform hierarchy

### Medium-Term Goals (Refactoring to DOD)

**Phase 4: Data-Oriented Refactor**
- Entity Component System (ECS) architecture
- Cache-friendly component storage
- SIMD optimization for vector operations
- Batch rendering system
- Profiling and performance metrics

**Phase 5: Core Engine Features**
- Scene graph and spatial partitioning
- Material system
- Asset pipeline and resource management
- ImGui integration for debug tools
- Multi-threaded job system

### Long-Term Goals (Production-Ready)

**Phase 6: Advanced Rendering**
- Physically-Based Rendering (PBR)
- Deferred rendering pipeline
- Shadow mapping (cascaded, omnidirectional)
- Post-processing stack (bloom, SSAO, tone mapping)
- Particle systems

**Phase 7: Cross-Platform Support**
- Vulkan backend for Linux/Windows
- Metal backend for macOS
- Unified shader compilation pipeline

**Phase 8: Game Systems**
- Full physics integration (3D with Box2D successor or custom)
- Audio system
- Animation system (skeletal, blend trees)
- Scripting layer (Lua or custom)
- Networking foundation

---

## 📋 Development Roadmap

### Current Priorities

1. **Get the window showing** - Verify DX12 device creation and swapchain
2. **Render first triangle** - Validate the rendering pipeline
3. **Implement camera controls** - FPS-style movement and look
4. **Load a simple model** - OBJ or custom format
5. **Add basic lighting** - Directional light with diffuse/specular

### Known Issues

- [ ] Friend operator warnings (style issue, not functional)
- [ ] Parameter name shadowing in datatype assignments
- [ ] Linux build fails due to DX12-only backend
- [ ] No unit tests yet
- [ ] Documentation is sparse

### Contributing

This is primarily a personal learning project, but feedback and suggestions are welcome! If you spot bugs or have ideas for improvements, feel free to open an issue.

---

## 🧠 Learning Resources

This project was built by studying:
- **Data-Oriented Design** - Mike Acton's talks and writings
- **Game Engine Architecture** - Jason Gregory
- **Real-Time Rendering** - Tomas Akenine-Möller et al.
- **DirectX 12 Programming Guide** - Microsoft documentation
- **Handmade Hero** - Casey Muratori (for low-level understanding)

---

## 📝 Recent Changes

### Latest Updates
- ✅ Fixed all critical linker errors (missing operator implementations)
- ✅ Added safety checks to normalize functions (prevent NaN from zero vectors)
- ✅ Comprehensive code review with inline comments
- ✅ Build system configured and tested

See [CODE_REVIEW_SUMMARY.md](CODE_REVIEW_SUMMARY.md) for detailed code quality findings and recommendations.

---

## 📜 License

This project is released under the MIT License - feel free to learn from it, use it, or break it!

---

## 🙏 Acknowledgments

- **EASTL** - Electronic Arts for their excellent STL implementation
- **ImGui** - Omar Cornut for the best debug UI library
- **DirectX-Headers** - Microsoft for open-sourcing DX12 headers
- **Box2D** - Erin Catto for the fantastic physics engine

---

**Happy Engine Building!** 🎮✨

_Last Updated: 2025-11-17_
