# Repository Guidelines

## Project Structure & Module Organization
- Core LED matrix engine lives in `lib/`; public headers mirror in `include/`.
- Demo binaries and sample apps reside in `examples-api-use/`; utilities and viewers under `utils/`.
- Language bindings: `bindings/python` (wheel build scripts, samples) and `bindings/c#` (NuGet metadata, samples).
- Support assets: `adapter/` for hardware mappers, `fonts/` for bitmap fonts, `img/` for reference imagery.
- Hardware notes or experiment artifacts belong in `doc/` or per-feature subdirectories; avoid scattering configs.

## Build, Test, and Development Commands
- `make` — builds `lib/librgbmatrix.a` and refreshes demos in `examples-api-use/`.
- `make clean` — purges objects and binaries across lib, utils, and demos.
- `make -C utils all` — compiles optional viewers; requires GraphicsMagick/FFmpeg for rich targets.
- `make build-python` / `make install-python` — build or install the Python wheel (use a venv first).
- `make build-csharp` — generates the C# NuGet package and sample binaries.
- `sudo examples-api-use/demo -D 0` — run hardware demo against panel 0; adjust `--led-*` flags per setup.

## Coding Style & Naming Conventions
- C++: two-space indent, no tabs; methods use PascalCase, member fields end with `_` (see `lib/led-matrix.cc`).
- Maintain include ordering, GPL headers, and concise explanatory comments where logic is dense.
- Python: PEP 8 modules in snake_case, classes in PascalCase; keep docstrings factual.
- C#: respect existing namespaces, PascalCase members, camelCase locals.

## Testing Guidelines
- No automated suite; validate on hardware after builds.
- Run `sudo examples-api-use/demo -D 1 runtext.ppm` and note panel type plus `--led-*` flags in PRs.
- Exercise new visualization code via `utils/led-image-viewer` or `text-scroller`; document observed behavior.
- Python updates: execute samples in `bindings/python/samples`; provide dry-run logs if hardware is unavailable.

## Commit & Pull Request Guidelines
- Commits use imperative summaries (e.g., `Add FM6126A mapper`) and group related areas (`lib`, `utils`, etc.).
- PR descriptions should list hardware tested, commands executed, relevant flags, and linked issues.
- Include screenshots or short clips for visual changes; call out breaking config or flag updates explicitly.
- Keep diffs focused; avoid drive-by formatting unless it clarifies touched code.
