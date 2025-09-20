# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is the **rpi-rgb-led-matrix** library by Henner Zeller - a C++ library for controlling RGB LED matrix panels (16x32, 32x32, 64x64, etc.) using Raspberry Pi GPIO. The library supports PWM up to 11-bit per channel, true 24bpp color with CIE1931 profile, and can drive multiple chained panels at high refresh rates.

## Build Commands

### Core Library
```bash
# Build the main library
make

# Build library only
make -C lib

# Clean all builds
make clean
```

### Examples and Demos
```bash
# Build API examples
make -C examples-api-use

# Run a simple demo (requires sudo for GPIO access)
sudo examples-api-use/demo -D0

# Test with a specific panel configuration (example for 64x32 panel)
sudo examples-api-use/demo --led-rows=32 --led-cols=64 -D0
```

### Utilities
```bash
# Build utilities (image viewer, video player, text scroller)
make -C utils

# Example: Display an image
sudo utils/led-image-viewer image.jpg --led-rows=32 --led-cols=64

# Example: Show scrolling text
sudo utils/text-scroller --led-rows=32 --led-cols=64 -f fonts/7x13.bdf "Hello World"
```

### Language Bindings
```bash
# Python bindings
make build-python
make install-python

# C# bindings
make build-csharp
```

## Code Architecture

### Core Components

- **`include/led-matrix.h`**: Main API - `RGBMatrix` class and configuration options
- **`include/canvas.h`**: Canvas interface for drawing operations
- **`include/graphics.h`**: Graphics primitives (fonts, shapes, text rendering)
- **`include/pixel-mapper.h`**: Coordinate transformation for panel arrangements
- **`lib/`**: Core implementation (GPIO control, framebuffer, threading)

### Key Classes

- **`RGBMatrix`**: Main class implementing the Canvas interface
- **`RGBMatrix::Options`**: Hardware configuration (rows, cols, chain length, etc.)
- **`RuntimeOptions`**: Runtime behavior (drop privileges, daemon mode, etc.)
- **`FrameCanvas`**: For double-buffering and smooth animations
- **`Font`**: BDF font loading and text rendering

### Hardware Abstraction

The library abstracts different hardware configurations through:
- **GPIO mappings**: `regular`, `adafruit-hat`, `adafruit-hat-pwm`, `compute-module`
- **Pixel mappers**: Handle panel arrangements (U-shape, rotation, mirroring)
- **Multiplexing options**: Support for different panel types (outdoor, shift-register)

## Panel Configuration

Critical configuration flags that vary by hardware:
- `--led-rows=<N>`: Panel height (8, 16, 32, 64)
- `--led-cols=<N>`: Panel width (32, 64, 128)
- `--led-chain=<N>`: Number of daisy-chained panels
- `--led-parallel=<N>`: Number of parallel chains (1-3 on Pi, up to 6 on Compute Module)
- `--led-gpio-mapping=<type>`: Hardware variant (`regular`, `adafruit-hat`, etc.)
- `--led-multiplexing=<0-17>`: Panel multiplexing type for outdoor/special panels
- `--led-row-addr-type=<0-5>`: Addressing scheme (affects 64x64 and ABC panels)

## Performance Optimization

Key performance-related options:
- `--led-slowdown-gpio=<0-4>`: GPIO timing (higher for faster Pi/slower panels)
- `--led-pwm-bits=<1-11>`: Color depth vs refresh rate tradeoff
- `--led-pwm-lsb-nanoseconds=<N>`: Base timing unit (affects quality/speed)
- `--led-pwm-dither-bits=<0-2>`: Time dithering for higher refresh rates
- `--led-show-refresh`: Display current refresh rate for tuning

## Development Patterns

### Basic Usage Example
```cpp
#include "led-matrix.h"
using rgb_matrix::RGBMatrix;

RGBMatrix::Options defaults;
defaults.rows = 32;
defaults.cols = 64;
defaults.chain_length = 1;

RGBMatrix *matrix = RGBMatrix::CreateFromOptions(defaults, runtime);
// Draw on matrix canvas
delete matrix;
```

### Animation with Double Buffering
```cpp
FrameCanvas *offscreen = matrix->CreateFrameCanvas();
// Draw on offscreen canvas
matrix->SwapOnVSync(offscreen);
```

### Common Issues to Address

1. **Root Privileges**: All programs need `sudo` for GPIO access
2. **Sound Conflicts**: Disable onboard audio (`dtparam=audio=off` in `/boot/config.txt`)
3. **Panel Detection**: Many panels need specific `--led-multiplexing` or `--led-row-addr-type` values
4. **Timing Issues**: Faster Pis often need `--led-slowdown-gpio=2` or higher
5. **Flicker**: May need PWM hardware mod on Adafruit HATs or timing adjustments

## File Structure Reference

- **`examples-api-use/`**: Programming examples showing library usage
- **`utils/`**: Ready-to-use utilities (image viewer, video player, text scroller)
- **`bindings/`**: Language bindings (Python, C#)
- **`fonts/`**: BDF fonts for text rendering
- **`adapter/`**: PCB designs for level shifters and connectors
- **`lib/`**: Core library implementation
- **`include/`**: Public API headers

## Testing Hardware

Always test with the demo first:
```bash
sudo examples-api-use/demo --led-rows=<your_rows> --led-cols=<your_cols> -D0
```

If no output appears, try different `--led-multiplexing` values (0-17) and `--led-row-addr-type` values (0-5).