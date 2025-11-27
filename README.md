# imgui-monitor

Cross-platform playground for Bazel + Zig hermetic toolchains with GLFW windowing. The project is split into simple Bazel targets that prove we can build native binaries (Linux/macOS/Windows) without touching the host toolchain.

## Prerequisites
- Bazel 8+ with Bzlmod enabled (default)
- Python 3 (for Bazel's hermetic_cc_toolchain repo rule)
- Network access for fetching the Zig SDK, GLFW, and the llvm-mingw SDK used for Windows import libraries

## Getting Started
```bash
# Build the console hello world target
bazel build //test:hello

# Build and run the GLFW smoke test on the local platform
bazel run //test:window
```
The window target opens a 640x480 GLFW surface for a couple of seconds, swaps buffers, then exits.

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
- `test/` – demo applications (`hello.cpp`, `window.cpp`)

As more milestones land (imgui, monitoring widgets, etc.), extend the Bazel packages alongside the existing hermetic toolchain configuration.
