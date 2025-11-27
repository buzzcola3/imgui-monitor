# DebugGlass

DebugGlass is a drop-in Dear ImGui overlay that boots in its own thread so you can instrument any native app quickly. The repository also doubles as a cross-platform Bazel + Zig toolchain playground, demonstrating hermetic builds for Linux/macOS/Windows without relying on host compilers.

## Prerequisites
- Bazel 8+ with Bzlmod enabled (default)
- Python 3 (for Bazel's hermetic_cc_toolchain repo rule)
- Network access for fetching the Zig SDK, GLFW, and the llvm-mingw SDK used for Windows import libraries

## Getting Started
```bash
# Run the DebugGlass demo locally (opens a window for ~10 seconds)
bazel run //examples:hello_debugglass
```
The example spins up the DebugGlass overlay, prints "Hello World" to stdout, and exits after a short delay. Use it as a template for wiring the library into your own project.

## Cross-compiling with Zig
We register all Zig toolchains from `hermetic_cc_toolchain`, so you can target any supported platform:
```bash
# Example: build the GLFW sample for Windows from Linux/macOS
bazel build --platforms=@zig_sdk//platform:windows_amd64 //test:window
```
MinGW import libraries (user32, gdi32, shell32, opengl32) come from the bundled `llvm_mingw_sdk` archive and are wired in through `third_party/windows_sdk` so cross-linking succeeds without a local Windows SDK.

## Project Layout
- `MODULE.bazel` – Bzlmod dependencies (hermetic Zig toolchain, GLFW, llvm-mingw SDK)
- `third_party/` – wrappers for GLFW and platform SDK bits
- `debugglass/` – library sources (`debugglass.h/.cpp`)
- `examples/` – runnable samples (currently `hello_debugglass`)

Add more DebugGlass-powered examples under `examples/` as new milestones land (custom widgets, telemetry panels, etc.).
