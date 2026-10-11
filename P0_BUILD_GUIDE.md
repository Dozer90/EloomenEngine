# P0 — build fixes applied

Updated **12 October 2026**. The DX12 engine and DemoProject now compile and link in Debug and Release. Changes are uncommitted for review.

## Changes to review

- **DX12 backend:** consistent DirectX header include order, pinned fetched header revision, correct adapter/swap-chain descriptions and output parameters, shader blob loading, descriptor stride, render-target binding, matching vertex layout, monotonic fence values, and owned fence-event lifetime.
- **Renderer factory/interface:** validate the window backend; supply required GPU-buffer overrides using actual upload resources. General mesh registration/submission explicitly returns false because that pipeline is not implemented. The entity overload now accepts an incomplete entity ID by const reference.
- **Events/observers:** adapt resize event data to the renderer method, unsubscribe on destruction, subscribe only after successful initialization, use numeric map keys, erase by key, and make header-defined events inline.
- **File streams/logger:** owned input buffers loaded from files; concrete output writing/flushing and handle cleanup; logger uses the actual sink, filters messages, and defines its constructors/destructor. Bidirectional file streams explicitly report unsupported functionality.
- **Utilities/linkage:** implement missing vector/quaternion validity checks, expose source-defined vector functions at link time, add assertion implementation and EASTL allocation hooks, implement combined buffer-stream constructors, fix stream growth and typed-write capacity checks, and make filesystem header wrappers inline.
- **CMake:** link the actual `box2d` target and graphics libraries, propagate DirectX header includes to consumers, correct x64 DXC paths, and remove the unused DXC runtime-copy requirement from the static backend. Current shader loading uses d3dcompiler, not runtime DXC.
- **Regression checks:** optional `SGE_BUILD_TESTS` target covers primitive stream growth/readback, observer unsubscription, file output/loading construction, and logger construction/output.

## Validation

Debug and Release compile/link checks and the regression test passed. Logs are under `out/p0-build.log` and `out/p0-release-build.log`.

The local Debug check used embedded debug information (`/Z7`) to avoid an installed MSVC program-database-manager mismatch. This is a build-cache setting, not a source-level compiler workaround. Release was checked in `out/build/codex-Release`, reusing already fetched sources.

In Visual Studio, save and use **Project → Configure Cache**, then **Build → Build All** to pick up the CMake changes. No command-line workflow is required.

## Remaining limitations

- The demo has not been launched or validated on the GPU. Its precompiled shader files still need to exist at the expected runtime paths.
- General mesh submission and bidirectional file streams are explicitly unsupported; no successful placeholder handles/results are returned for those features.
- Input streams still need broader validation around written length versus allocated capacity, malformed input, and serialization offsets. Current regression coverage is deliberately limited.
- Assertion messages currently follow printf formatting; existing brace-style messages need alignment before relying on their formatted details.
- EASTL allocation hooks support ordinary alignment; requests requiring over-aligned storage or nonzero alignment offsets report allocation failure.
- Math correctness, wider public APIs, GPU resize/lifetime checks, and unused backend script cleanup remain under the later priorities in PROJECT_REVIEW.md.
