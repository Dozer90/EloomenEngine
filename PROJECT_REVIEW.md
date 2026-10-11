# Shine Game Engine — resumption review

Reviewed 10 October 2026. This is a snapshot of the existing working tree, not a record of fixes.

**P0 follow-up, 11 October 2026:** see [P0_BUILD_GUIDE.md](P0_BUILD_GUIDE.md) for expanded steps and current status. EABase has been re-enabled, many namespace imports corrected, the duplicate backend directory removed, and `operator ?` replaced with `has_bitflag`. The original findings below describe the 10 October snapshot; the follow-up distinguishes remaining work from those fixes and records the newer EABase version-mismatch failure.

## Where you left off

You were building a C++23 engine with data-oriented storage, a static engine library, a separate demo executable, and backend interfaces. The immediate rendering goal was a Windows window displaying a hard-coded DX12 triangle. There is substantial foundation code, but the current checkout is not a working demo or a complete engine.

The last **committed** work is `2200785`, dated **3 July 2025**, “Completed: Auto-generated error code headers from json files.” Before that:

| Date | Commit | Milestone described in history |
| --- | --- | --- |
| 29 April 2025 | `ab57107` | Base vector, matrix, and quaternion math described as complete. Current inspection still finds defects. |
| 30 April 2025 | `4603657`, `03551f7` | Base ray/sphere casts implemented and a spherecast fix applied. |
| 10 May 2025 | `5123096` | Core backend interfaces and DX12 renderer/window added. |
| 28 May 2025 | `108d8b0` | Engine converted into a library; main moved into DemoProject; compile/warning fixes. |
| 28 May 2025 | `c2f79d0` | Suppress warnings from dependency headers. |
| 3 July 2025 | `2200785` | JSON-to-error-header CMake automation added. |

The **uncommitted** work is later and appears to be an unfinished rename/build cleanup:

- Most public headers now use `sge`, and macros/CMake variables use `SGE_`; many implementations and the demo still use `shine`. No namespace alias bridges them.
- Engine/library names and README were renamed from Eloom to Shine. The demo window title still says Eloom.
- EASTL was bumped from `3.21.23` to `3.27.01`; dependency fetching and submodule handling were being adjusted. A CMake comment explicitly records earlier Windows dependency failures.
- The error-code generator, its template, and all four JSON inputs are deleted locally, despite generated headers still directing you to edit those inputs.
- The shader-generator scaffold is deleted locally and its root CMake inclusion is commented out.
- An untracked `engine/backends/dx12/` tree exists alongside the tracked `engine/backends/win32/dx12/` tree. The latter is selected by current CMake; the former is not wired in.
- `filesystem_interface.h` is a new, empty placeholder.

At review start there were **89 modified/deleted tracked paths** plus untracked backend/interface files. Most large diffs are renaming, not newly implemented systems. Git cannot tell us exactly when the local edits happened or which you intended to keep. The existing build cache has activity dated 10 October 2026, so commit date is not the date of your last local work.

**Best-supported interpretation:** your latest local activity was getting the renamed project/dependencies building again, while carrying forward an unfinished DX12 prototype and utility-system implementations. Finish that recovery before expanding the engine.

## What is actually present

| Area | Current state |
| --- | --- |
| Build/library/demo | CMake structure exists; current Debug build fails. |
| Vectors/matrices/quaternions | Value containers, operators, ID-backed storage, and extensive math helpers exist; correctness and API/linkage gaps remain. |
| Memory storage | Reusable IDs and sequential blocks exist; nontrivial object lifetime and ID reuse need correction. |
| Windows/DX12 | Window/message loop and a hard-coded triangle pipeline are partly written; compile errors and missing setup prevent a usable demo. |
| Meshes | Metadata, CPU index storage, and intended GPU buffer registration exist; backend support and data retrieval are unfinished. |
| Textures/materials/camera | Texture metadata only; material implementation empty; camera implementation commented out. |
| Events/input | Event payloads and observer API exist; input is only enums; subscription/storage/integration need work. |
| Serialization/streaming | Binary/string formats and buffer streams exist; compile errors, missing methods, and boundary bugs remain. |
| Filesystem | Broad public API, partial management implementation, mostly unfinished registration/search/watch/query functionality. |
| Logging/assertions | Public logger API and partial implementation; file stream dependency unfinished; runtime assertion function has no implementation in project files. |
| ImGui | Partial shape/canvas rewrite plus large disabled old implementation; no functioning GUI integration established. |
| Localization/audio/networking | Localization namespace and error placeholder; audio/networking interfaces empty. |
| ECS/scenes/assets/physics integration | No implementation found. Entity IDs are forward-declared for rendering; Box2D is fetched but not used by project code. |
| Tests/CI | No project test suite or CI workflow found. |

## Build verification

Ran the existing configuration without changing source:

```powershell
cmake --build out/build/x64-Debug --parallel 2
```

It failed during dependency scanning of both DX12 source files:

```text
fatal error C1083: Cannot open include file: 'EABase/eabase.h'
fatal error C1083: Cannot open include file: 'EABase/nullptr.h'
ninja: build stopped: subcommand failed.
```

The backend target gets EASTL's include path manually but does not get EABase's includes through that arrangement. EABase's explicit fetch/include setup in `engine/CMakeLists.txt` is commented out. EASTL's fetched CMake does fetch/link EABase internally, but those usage requirements are not propagated to the separate backend through a direct EASTL target dependency.

This is the **first observed build failure**, not the only one. The additional findings below are source inspection, not compiler/runtime-verified failures. No fresh network configure, Release build, executable launch, or test suite was run. Existing generated output and third-party dependency trees were excluded from the project-file inventory; cached dependency CMake and DXC archive layout were inspected where relevant.

## Prioritized to-do list

### P0 — recover a coherent build

- [ ] **Choose the canonical namespace and complete the rename.** `sge` is the dominant public API. Update implementation definitions, `using namespace sge`, collision namespaces, interpolation references, ImGui references, and `demo_project/src/main.cpp`. Update the demo title too. Validate public headers and implementations agree.
- [ ] **Choose one DX12 directory/layout.** Current CMake uses `backends/win32/dx12`. Compare and retain useful work from the untracked alternative before retiring either tree. Its pre/post scripts are not currently included.
- [ ] **Repair EABase/EASTL include propagation.** Follow the local warning about Windows dependency fetching. Make every consuming target receive the required headers. Success criterion: dependency scanning proceeds past EABase in a fresh supported configuration.
- [ ] **Create a real ImGui library target.** The cached ImGui source has no `CMakeLists.txt`; `FetchContent_MakeAvailable(ImGui)` does not itself create the `ImGui` target being linked. Compile its required `.cpp` files and later the Win32/DX12 integration sources. Ensure ImVec customization macros are consistent across translation units.
- [ ] **Correct DX12 build dependencies.** Explicitly link needed Windows graphics/compiler libraries. DXC's cached archive puts files under `lib/x64/` and `bin/x64/`, while the imported target points at `lib/` and `bin/`. Copy runtime DLLs to DemoProject's runtime directory, not the static backend library's directory. Pin DirectX-Headers to a tested revision instead of `main`.
- [ ] **Fix common-header compile blockers.** `bitflag_helpers.h:57` tries to define `operator ?`, which C++ cannot overload. Fix stale error enum references and undefined traits/operators before working through the remaining compile/link diagnostics. Correct the reversed shared error-check macro as described below.
- [ ] **Resolve missing declarations/definitions and inconsistent signatures.** Implement `runtime_assert`; the datatype `is_valid` methods; declared unary-plus/float overloads where missing; matrix enum-index overloads; align `constexpr` operator declarations with definitions and namespaces; align filesystem `resolve` exception specifications.
- [ ] **Repair logging/file-stream compilation.** Logger code refers to `mFile`, but the member is `mFilestream`; it attempts to construct abstract `filesystem::ofstream`; `ifstream` constructs `streaming::istream` without its required buffer argument. Complete or deliberately exclude unfinished code from the initial build with an explicit follow-up issue.
- [ ] **Repair serialization/streaming compilation.** Replace `error_id::...` with the actual scoped `errc` type; replace undefined `has_wchar_data<T>` with the implemented wide-string concept; supply missing vector string-write support and `iostream` constructors; replace illegal static binary shift operator declarations with valid operators/functions.
- [ ] **Repair DX12 compile blockers.** See concrete renderer/window findings below. Build Debug and Release, then make a checkpoint commit of the coherent rename/backend/build work.

### P1 — make the first demo reliable

- [ ] **Finish renderer initialization and implement its interface.** The DX12 class lacks overrides for GPU buffer creation/destruction, mesh registration, and draw submission, so it is abstract. Its constructor takes `window_dx12*`, but the factory forwards a generic `window_interface*`. Resolve that conversion at the factory/backend boundary.
- [ ] **Fix DX12 API/type/name errors.** `FRAME_COUNT` remains in arrays after the constant was renamed to `BACK_BUFFER_COUNT`; `mRenderTargetDescriptorSize` is used but absent from the header; derived code accesses private base `mFrameIndex`; `DXGI_ADAPTER_DEsC` is misspelled; `DXGI_SWAP_CHAIN_DESC` is used with DESC1 fields/API; shader blobs and swap-chain descriptors are retrieved using incorrect output-parameter call forms. An undeclared `shutdown()` definition and trailing unmatched brace also need correction.
- [ ] **Create render targets before rendering.** `create_render_target_heap()` is written but never called during construction. Bind the target with `OMSetRenderTargets`, initialize descriptor stride, and align the PSO render-target format with the swap chain. Match packed `colour_t` vertex data to the shader/input layout instead of reading it as four floats.
- [ ] **Add the missing shader assets/build step.** The renderer requests `shaders/vertex.cso` and `shaders/fragment.cso`; neither sources nor compiled assets are in the project inventory. Compile/copy shaders with explicit dependencies and use a predictable runtime asset path. Shader reflection/hot reload can follow later.
- [ ] **Make GPU synchronization and shutdown correct.** Use monotonically increasing fence values for the shared fence, wait before allocator/resource reuse, close the fence event, flush on shutdown, and unsubscribe resize callbacks before renderer destruction. Handle initialization failure rather than returning a partially usable object.
- [ ] **Fix native window state and resize flow.** `Window::WindowProc`/`Window*` are leftover class names. `mTitle` is a raw `const wchar_t*`, but native code calls `.c_str()`. Window construction should own title storage. `WM_SIZE` currently calls the public resize method, which calls `SetWindowPos` again. Separate notification from resize requests; update position/focus/minimize/maximize state; do not render/resize zero-size minimized surfaces. Use client-space viewport coordinates.
- [ ] **Validate the first milestone:** launch DemoProject, display a triangle, resize, minimize/restore, move, close, and restart with the DX12 debug layer clean. Add initialization failure checks in the demo. This is the most useful next visible milestone.

### P2 — make foundations safe and complete

- [ ] **Correct shared error handling at the source.** Every generated header has `SGE_RETURN_ERRC_IF_FAILED(_e)` testing `!_e`; for `std::error_code`, that returns failure on success. Restore/update the JSON/template generator or explicitly retire generation and centralize the helpers. Also decide whether error enum value zero should represent success; it currently names a real error even though `std::error_code` treats numeric zero as success.
- [ ] **Fix filesystem logic and complete a small usable subset.** `create` rejects overwrite when the flag is true and can truncate existing files when false; `destroy` rejects existing paths; `clear_directory` rejects an existing directory; `canonicalize_path` does not return the successful path. Stop inferring file/directory intent only from an extension. Complete file registration/deregistration and keep ID paths synchronized after rename/move/delete. Then implement queries/search and finally monitoring. Header-defined non-template search/list functions need inline linkage or source definitions.
- [ ] **Finish file streams and logging.** Implement file-handle RAII, read/write/flush, failure reporting, and destructors. Implement the logger's missing constructor/destructor, returning filename generator, console output, level filtering (`can_log` is currently unused), newline formatting, and formatting-failure behavior.
- [ ] **Repair managed storage.** Vectors already construct elements during `resize`; calling `uninitialized_fill` over them creates objects over live objects. Restrict raw `memcpy` to appropriate types; clarify `push(T)` versus `push(T&&)` overloads; handle minimal-capacity growth; validate removed IDs in accessors and sequential element offsets. Independent per-component free-ID sets do not guarantee vectors/quaternions reuse the same ID after multiple removals: use a shared allocation decision. Decide whether generational IDs are needed to reject stale handles. `count()` currently represents the high-water mark, not live objects.
- [ ] **Fix observer/event infrastructure.** Header-defined global event observers need one-definition-safe storage (`inline` or extern/source definitions). ID keys require a suitable hash/equality policy. `unsubscribe` should erase the map entry by key rather than use `eastl::remove` on map pairs. Define mutation-during-broadcast behavior and manage subscriber lifetimes. Renderer resize callbacks must accept the actual `window_resized_data` payload or use an adapter.
- [ ] **Repair serialization bounds and ownership.** The offset string decoder starts at the original buffer and increases the advertised size (`bufferSize + bufferOffset`), rather than advancing the pointer and reducing remaining size. Validate offsets before subtraction/pointer arithmetic, guard length overflow and truncated hash data, define empty-string behavior, and return spans into the destination for `serialize_to_buffer`. Decide fixed-width length/hash/encoding/endianness before calling this a portable saved-file format. Move constexpr helper definitions into headers if callers need them.
- [ ] **Repair streaming boundaries/growth.** `OSTREAM_EXPANSION_FACTOR` is `size_t` initialized from `1.4f`, so its value is 1; expansion loops cannot grow the buffer. Exact-fit writes incorrectly fail on non-expanding streams, empty/small buffers can stall, typed writes bypass expansion, input seek rejects end position, and input sees allocated buffer size rather than logical written length. Implement and test read/write stream constructors and ownership rules.
- [ ] **Fix half conversion before trusting math constants.** `half(float)` initializes raw `mBits` from a returned `half`, causing a numeric conversion rather than extracting its bit pattern. For example, converting 1.0 through this path stores integer 1 rather than IEEE half bits `0x3c00`. Test normal/subnormal values, infinities, NaNs, rounding, and signed zero equality.
- [ ] **Correct core math.** Quaternion multiplication does not preserve the identity and `/=` currently subtracts. Matrix cofactors/adjugates are inconsistent; the 4x4 cofactor function supplies only four of sixteen elements. `matrix3x3`/`matrix4x4` mutable column access only accepts columns below 2. Floating `mod` does not compute the expected remainder; approximate log functions have questionable bit conversions/constants. Fast 2D normalization's equal-components shortcut fails for vectors such as `(2,2)`. Resolve zero-vector behavior, matrix/vector/coordinate conventions, perspective depth range, transform decomposition, and shortest-path/antipodal quaternion slerp.
- [ ] **Correct casts with regression examples.** Ray triangle distance uses `dot(rayDir,Q)` instead of `dot(edge2,Q)` and omits the `v`/`u+v` rejection. Quad casts ignore finite length in the plane call and measure from ray origin instead of the quad's computed center. Capsule cap tests ignore finite ray length, and the default hit distance is `FLT_MAX` while success is decided by `>= 0`, so a miss can be reported as a hit. Sphere triangle casts have the same sentinel/success mismatch. Define normalized-direction and hit-position conventions; test parallel, zero direction, inside starts, tangencies, misses, finite lengths, and degenerate shapes. Inflated AABB/ellipsoid sphere casts should be documented as approximations or replaced with exact tests.
- [ ] **Fix mesh ownership/indexing.** Mesh creation uses the submesh metadata offset as the starting index-data offset, which diverges with multiple submeshes/meshes. Released submesh/index allocations are never reclaimed; free-ID reuse can break assumed contiguous sections. Implement `try_get_data`, validate empty meshes/submesh indices, recover from GPU allocation failure, and make metadata accessible to the backend.
- [ ] **Add focused tests:** storage reuse and nontrivial lifetimes; vector/matrix/quaternion identities; half conversions; finite cast misses/hits; filesystem overwrite/error success; serialization malformed buffers and nonzero offsets; stream exact-fit/growth/EOF; observer unsubscribe and cross-translation-unit events. Also compile representative public headers independently.

### P3 — continue engine features after the demo/foundations

- [ ] Implement camera view/projection matrices, configuration, transform binding, and consistent radians/depth conventions.
- [ ] Implement GPU texture upload, data ownership, file loading, and resource release. Current texture APIs only store a borrowed CPU data pointer and dimensions.
- [ ] Implement material allocation/properties and connect materials/textures to submeshes, shader bindings, and draw submission.
- [ ] Connect Win32 keyboard/mouse messages to input state and typed engine events.
- [ ] Finish or narrow the ImGui rewrite: shape ownership, intersection/update/draw methods, style stack, correct rectangle/ellipse borders, and triangle type. Remove or migrate disabled old code after replacement works.
- [ ] Repair/narrow random helpers (`std::uniform_real` does not exist; integer distribution support and shared generator state are unfinished).
- [ ] Repair chrono helpers (`stdns` typo, missing closing namespace brace, optional calendar declaration ordering and ISO weekday encoding) or use standard chrono to reduce maintenance.
- [ ] Introduce the first entity/transform/scene storage and lifecycle, then integrate renderer submission. This is currently a planned connection, not an existing ECS.
- [ ] Integrate Box2D only when a concrete physics use case is ready; then decide priorities for asset loading, localization, audio, and networking.
- [ ] Write build/install instructions, required toolchain/SDK versions, demo controls, supported backends, and a repeatable clean build/CI path. Current platform selection checks UNIX before APPLE and advertises unimplemented backends/None; reject unsupported selections clearly.
- [ ] Preserve applicable license notices and clarify the inherited “CMake SFML Project” license text/project identity when preparing distribution.

## File-by-file inventory

All 94 current project-owned files discovered outside ignored build/dependency/Git directories are listed below. “Implemented” means code exists, not that it passed runtime tests. Common rename/build/storage findings above apply across the corresponding files.

### Root and demo — 7 files

| File | State / next action |
| --- | --- |
| `.gitignore` | Ignores build/output/editor directories; adequate starting point. |
| `CMakeLists.txt` | C++23 root, engine/demo wiring; shader tool disabled; policy minimum added. |
| `CMakeSettings.json` | Visual Studio Ninja x64 Debug only; document supported configs/toolchain and add Release verification. |
| `README.md` | Shine/DOD description; installation is only “Todo.” |
| `LICENSE.md` | Inherited dual-license text referencing CMake SFML Project; preserve notices and clarify identity. |
| `demo_project/CMakeLists.txt` | Minimal DemoProject linked to `sge`; needs shader/runtime asset setup. |
| `demo_project/src/main.cpp` | Intended 1920x1080 window/render loop; stale namespace/title; initialization failure handling absent. |

### Engine build and backend interfaces — 8 files

| File | State / next action |
| --- | --- |
| `engine/CMakeLists.txt` | Full source/dependency/backend setup; EABase propagation and real ImGui target are immediate tasks. |
| `engine/backends/core/interfaces/audio_interface.h` | Empty placeholder. |
| `engine/backends/core/interfaces/filesystem_interface.h` | Empty untracked placeholder. |
| `engine/backends/core/interfaces/input_interface.h` | Keyboard/mouse enums only; implement input API/platform mapping. |
| `engine/backends/core/interfaces/networking_interface.h` | Empty placeholder. |
| `engine/backends/core/interfaces/renderer_interface.h` | Metadata/buffer/mesh/draw contract exists; DX12 does not yet implement it. |
| `engine/backends/core/interfaces/window_interface.h` | Window API/state exists; title ownership, native event flow, and position updates need work. |
| `engine/backends/win32/CMakeLists.txt` | Windows-only DX12 subdirectory dispatcher. |

### Selected DX12 backend — 5 files

| File | State / next action |
| --- | --- |
| `engine/backends/win32/dx12/CMakeLists.txt` | Selected backend library; repair dependencies/DXC paths/runtime copy. |
| `engine/backends/win32/dx12/include/dx12/renderer_dx12.h` | Partial renderer refactor; undefined FRAME_COUNT, missing stride/interface methods. |
| `engine/backends/win32/dx12/include/dx12/window_dx12.h` | Native handle/message declarations; align source/class/title handling. |
| `engine/backends/win32/dx12/src/renderer_dx12.cpp` | Triangle prototype; compile and initialization/render/sync gaps detailed above. |
| `engine/backends/win32/dx12/src/window_dx12.cpp` | Native window/message prototype; stale Window names, title mismatch, resize feedback. |

### Alternative untracked DX12 backend — 6 files

| File | State / next action |
| --- | --- |
| `engine/backends/dx12/dx12_pre.cmake` | Proposed direct source/include/define injection; not included by active build. |
| `engine/backends/dx12/dx12_post.cmake` | Not wired; leftover SFML/OpenAL DLL copy references unavailable SFML source variable. |
| `engine/backends/dx12/include/renderer_dx12.h` | Older render-without-window signature incompatible with current base; missing interface implementation. |
| `engine/backends/dx12/include/window_dx12.h` | Duplicate native-window header; reconcile with selected tree. |
| `engine/backends/dx12/src/renderer_dx12.cpp` | Older prototype with stale include paths, undeclared members/width/height and incompatible signatures. |
| `engine/backends/dx12/src/window_dx12.cpp` | Mostly duplicate with changed includes/namespace; title constructor handling still inconsistent. |

### Datatype headers — 11 files

| File | State / next action |
| --- | --- |
| `engine/include/datatypes/float2.h` | 2D values/operators/ID API; complete declared methods and match implementation namespace. |
| `engine/include/datatypes/float3.h` | 3D equivalent with xy projection; same completion/storage checks. |
| `engine/include/datatypes/float4.h` | 4D equivalent with xy/xyz projections; same completion/storage checks. |
| `engine/include/datatypes/half.h` | Half conversion/operators/limits; constructor bit conversion and IEEE semantics need tests/fixes. |
| `engine/include/datatypes/int2.h` | Integer 2D API including mixed float operators; several declared methods lack definitions. |
| `engine/include/datatypes/int3.h` | Integer 3D API; missing validation/unary-plus definitions. |
| `engine/include/datatypes/int4.h` | Integer 4D API; missing validation/unary-plus definitions. |
| `engine/include/datatypes/matrix2x2.h` | Row-major values/index/operators; constexpr and enum-index declaration/definition alignment. |
| `engine/include/datatypes/matrix3x3.h` | Row-major 3x3 API; same alignment checks. |
| `engine/include/datatypes/matrix4x4.h` | Row-major 4x4 API and vector transforms; same alignment/convention checks. |
| `engine/include/datatypes/quaternion.h` | Value/ID API; defaulted create overloads overlap; define validation and fix arithmetic. |

### Datatype implementations — 10 files

| File | State / next action |
| --- | --- |
| `engine/src/datatypes/float2.cpp` | Component storage/operators; missing is_valid/unary-plus; source-only inline accessors need linkage review. |
| `engine/src/datatypes/float3.cpp` | Component storage/operators; missing is_valid/unary-plus; shared component-ID allocation needed. |
| `engine/src/datatypes/float4.cpp` | Component storage/operators; missing is_valid/unary-plus; shared component-ID allocation needed. |
| `engine/src/datatypes/int2.cpp` | Component storage/operators; missing is_valid/unary-plus and some float compound operators; inline linkage review. |
| `engine/src/datatypes/int3.cpp` | Component storage/operators; missing is_valid/unary-plus; shared allocation needed. |
| `engine/src/datatypes/int4.cpp` | Component storage/operators; missing is_valid/unary-plus; shared allocation needed. |
| `engine/src/datatypes/matrix2x2.cpp` | Storage and arithmetic exist; free operators are defined globally rather than matching friend namespace declarations. |
| `engine/src/datatypes/matrix3x3.cpp` | Same operator/storage concerns; mutable column bound incorrectly uses 2. |
| `engine/src/datatypes/matrix4x4.cpp` | Same operator/storage concerns; mutable column bound incorrectly uses 2. |
| `engine/src/datatypes/quaternion.cpp` | Storage/Euler conversion/operators; missing is_valid, wrong product and subtracting /=. |

### Math — 5 files

| File | State / next action |
| --- | --- |
| `engine/include/maths/constants.h` | f16/f32/f64 constants; depends on corrected half conversion. |
| `engine/include/maths/interpolation.h` | Easing enums/options/public scalar entry point; validate defaults and endpoints. |
| `engine/include/maths/math.h` | Extensive scalar/vector/matrix/quaternion helpers; cofactor/inverse, normalization, approximation, transform/projection and slerp fixes. |
| `engine/include/maths/random.h` | Older naming and per-translation-unit generator; invalid uniform_real API and arithmetic-type constraints. |
| `engine/src/maths/interpolation.cpp` | Many easing functions implemented; stale namespace; bounce/elastic endpoint/formula checks and zero step validation. |

### Rendering API and implementation — 12 files

| File | State / next action |
| --- | --- |
| `engine/include/rendering/camera.h` | View/projection methods declared; no usable implementation/configuration. |
| `engine/include/rendering/material.h` | Material properties/ID API designed; implementations absent; initialize scalar properties deliberately. |
| `engine/include/rendering/mesh.h` | Vertex/submesh/data types and lifetime API; material integration disabled; try_get_data unfinished. |
| `engine/include/rendering/renderer.h` | Abstract renderer ownership/factory alias; align factory with concrete backend. |
| `engine/include/rendering/texture.h` | Texture properties/lifetime API; file loading absent; borrowed data pointer ownership unresolved. |
| `engine/include/rendering/window.h` | Window ownership/factory API. |
| `engine/src/rendering/camera.cpp` | Includes plus entirely commented-out legacy implementation. |
| `engine/src/rendering/material.cpp` | Empty. |
| `engine/src/rendering/mesh.cpp` | GPU-buffer requests and metadata; wrong index origin, retained allocations, missing retrieval/material signature mismatch. |
| `engine/src/rendering/renderer.cpp` | DX12 factory; stale namespace and constructor type mismatch. |
| `engine/src/rendering/texture.cpp` | Metadata storage/create/release/get only; no GPU resources or file loader. |
| `engine/src/rendering/window.cpp` | Factory/state wrappers/events; stale namespace; move does not update mPosition; resize/event semantics need correction. |

### Events, errors, filesystem, streams and logging — 17 files

| File | State / next action |
| --- | --- |
| `engine/include/events/engine/data.h` | Window/input/filesystem payloads; define safe path payload lifetime. |
| `engine/include/events/engine/events.h` | Engine observers defined directly in header; fix global linkage and callback integration. |
| `engine/include/events/game/events.h` | Macro-configurable game events; same observer/global linkage requirements. |
| `engine/include/error_codes/filesystem_errc.h` | Generated filesystem errors; missing generator source and reversed shared error check. |
| `engine/include/error_codes/localization_errc.h` | Generated single temp error; generator/shared semantics need correction. |
| `engine/include/error_codes/serialization_errc.h` | Generated serialization errors; actual enum is errc, not implementation's error_id. |
| `engine/include/error_codes/streaming_errc.h` | Generated stream errors; same enum/helper correction. |
| `engine/include/filesystem/filesystem.h` | Large API; queries, file registration, watching, search unfinished; inline wrapper linkage and resolve signatures need alignment. |
| `engine/include/filesystem/filestream.h` | File-stream shells; output streams abstract; read/write API and RAII incomplete. |
| `engine/include/serialization/serialization.h` | Primitive/string serialization/deserialization; undefined wide trait, missing vector support, bounds/constexpr fixes. |
| `engine/include/streaming/streaming.h` | Buffer-stream APIs; invalid static binary operators; growth scalar truncation and missing combined-stream constructors. |
| `engine/include/logging.h` | Logger API; undefined member in log(), filtering unused, file sink mismatched. |
| `engine/src/utility/filesystem/filesystem.cpp` | Partial operations through register_directory; overwrite/existence/error checks, missing return and broad missing implementation. |
| `engine/src/utility/filesystem/filestream.cpp` | Nine-line input constructor only; wrong stream constructor, missing destructors/output/bidirectional methods. |
| `engine/src/utility/serialization.cpp` | String format/hash implementation; stale enum/namespace, incorrect offset decode. |
| `engine/src/utility/streaming.cpp` | Input/output buffers; stale enum/namespace, growth/exact-fit/EOF and logical size problems. |
| `engine/src/logging.cpp` | Partial logger construction/tag/levels; create_filename has no return; missing overload/destructor. |

### Other utilities — 13 files

| File | State / next action |
| --- | --- |
| `engine/include/utility/bitflag_helpers.h` | Bitwise enum operators plus illegal operator ?; replace with named predicate. |
| `engine/include/utility/chrono_helpers.h` | Large optional EASTL calendar port and conversion helpers; typo, brace/order/encoding defects; narrow scope or repair. |
| `engine/include/utility/colour.h` | Packed colors, conversions and named constants; clamp inputs and validate Kelvin formula/units and GPU/ImGui packing. |
| `engine/include/utility/defines.h` | Assertion/compiler/ID macros; runtime_assert absent; ID hash/equality/generation policy incomplete. |
| `engine/include/utility/imgui_ext.h` | New shapes/canvas declarations and disabled old design; abstract value storage/missing methods/type inconsistency. |
| `engine/include/utility/localization/localization.h` | Empty namespace; wrong error-header include path. |
| `engine/include/utility/managed_memory_block.h` | Central ID pools; object lifetime, free-ID coordination, bounds/growth/raw-copy issues. |
| `engine/include/utility/observer.h` | Subscription/broadcast API; incorrect map unsubscribe, ID hashing and lifetime/mutation policy. |
| `engine/include/utility/raycast.h` | Shape cast API and FLT_MAX miss sentinel; define shared direction/distance semantics. |
| `engine/include/utility/spherecast.h` | Sweep API aliasing raycast result; inherited sentinel convention and approximation scope. |
| `engine/src/utility/imgui_ext.cpp` | Only new shape Draw methods active; rest under #if 0; wrong filled-border calls and missing new implementations. |
| `engine/src/utility/raycast.cpp` | Plane/quad/triangle/AABB/sphere/ellipsoid/capsule implementations; concrete finite-length/math/miss defects. |
| `engine/src/utility/spherecast.cpp` | Inflated geometry/triangle sweep code; sentinel false positives, plane side/quad frame and approximation checks. |

### Deleted tracked files — 9 additional paths reviewed through Git

| File | Previous purpose / decision required |
| --- | --- |
| `cmake/automations/errc_header_gen/automate.cmake` | Generate error headers from JSON; restore and rename variables if generation remains the design. |
| `cmake/automations/errc_header_gen/errc.h.in` | Header template; fix shared error helpers here when restoring. |
| `cmake/automations/errc_header_gen/data/filesystem.json` | Filesystem error definitions/messages; generated header still names this as source. |
| `cmake/automations/errc_header_gen/data/localization.json` | Placeholder localization error data. |
| `cmake/automations/errc_header_gen/data/serialization.json` | Serialization error data. |
| `cmake/automations/errc_header_gen/data/streaming.json` | Stream error data. |
| `tools/shader_generator/CMakeLists.txt` | Shader-tool scaffold; decide build-time shader compiler scope. |
| `tools/shader_generator/include/reflection_generator.h` | Empty scaffold at HEAD. |
| `tools/shader_generator/src/reflection_generator.cpp` | Empty scaffold at HEAD. |

## Suggested next work session

1. Settle `sge` and one backend layout; preserve the alternative backend until compared.
2. Fix EABase/ImGui/DXC wiring and shared compile errors until the engine and demo link.
3. Complete DX12 initialization plus shader assets until the triangle appears and resizes safely.
4. Commit that runnable baseline, document the build, and add focused foundation regression tests.
5. Connect a real mesh and camera, then textures/materials and input. Build broader ECS/scene/physics systems from that working slice.

The report is the only project document added by this review. Existing source edits/deletions were preserved. The attempted build can update ignored build metadata. This review identifies the resumption state and known defects; it does not certify every algorithm or replace compiler/runtime testing after repair.
