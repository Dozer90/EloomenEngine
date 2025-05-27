#pragma once

#if !defined(ELOO_IMGUI_IS_ENABLED)
#define IMGUI_MUTABLE constexpr
#else
#define IMGUI_MUTABLE

#define IMGUI_DEFINE_MATH_OPERATORS

#define IM_VEC2_CLASS_EXTRA \
    ImVec2(const eloo::float2::values& f) { x = f.x(); y = f.y(); } \
    operator eloo::float2::values() const { return eloo::float2::values(x, y); }

#define IM_VEC4_CLASS_EXTRA \
    ImVec4(const eloo::float4::values& f) { x = f.x(); y = f.y(); z = f.z(); w = f.w(); } \
    operator eloo::float4::values() const { return eloo::float4::values(x, y, z, w); }

#include "datatypes/float2.h"
#include "datatypes/float4.h"

#include <imgui.h>

#include <EASTL/array.h>
#include <EASTL/stack.h>
#include <EASTL/string.h>
#include <EASTL/unordered_map.h>


//-----------------------------------------------------------------------------
// [SECTION] Forward declarations and basic types
//-----------------------------------------------------------------------------

// Scalar data types

// Forward declarations
struct ImGuiExCol;

typedef int ImGuiExColor;               // -> enum ImGuiCol_             // Enum: A color identifier for styling


enum ImGuiExCol_ {
    ImGuiExCol_TooltipNotification,
    ImGuiExCol_CanvasBackground,
    ImGuiExCol_CanvasBorder,

    ImGuiExCol_COUNT,
    ImGuiExCol_TOTAL_COUNT = ImGuiCol_COUNT + ImGuiExCol_COUNT
};

struct ImGuiExStyle {
    ImGuiStyle  mImGuiStyle;

    ImVec4      Colors[ImGuiExCol_COUNT];
};


namespace ImGuiEx {

// Main
IMGUI_API ImGuiExStyle& GetStyle();                                                     // access the Style structure (colors, sizes). Always use PushStyleColor(), PushStyleVar() to modify style mid-frame!

IMGUI_API void          PushStyleColor(ImGuiExCol idx, ImU32 col);                      // modify a style color. always use this if you modify the style after NewFrame().
IMGUI_API void          PushStyleColor(ImGuiExCol idx, const ImVec4& col);
IMGUI_API void          PushStyleColor(ImGuiCol idx, ImU32 col);                        // modify a style color. always use this if you modify the style after NewFrame().
IMGUI_API void          PushStyleColor(ImGuiCol idx, const ImVec4& col);

void TextWithTooltip(const char* label, const char* fmt, ...);
void Tooltip(const char* fmt, ...);
void DisplayTooltip(const char* fmt, ...);

void DrawTooltipNotification();


/// <summary>
/// Helper class designed to prevent duplicated IDs when working with ImGui elements. The ID
/// consists of sub-ids that last until the end of the scope where CompoundingScopedID::Push
/// is called.
/// </summary>
class CompoundingScopedID {
private:
    explicit CompoundingScopedID(eastl::string&& newID, bool isCounted);

    CompoundingScopedID() = delete;
    CompoundingScopedID(const CompoundingScopedID&) = delete;
    CompoundingScopedID(CompoundingScopedID&&) = delete;

public:
    ~CompoundingScopedID();

    /// <summary>
    /// Push a scoped sub-id
    /// </summary>
    /// <param name="id">The sub-id. Can be empty/null</param>
    /// <param name="isCounted">If true, an index will be appended to the ID based on the number of times it has
    /// been used. When the compounded ID is empty, the counter resets to 0.</param>
    /// <returns>A CompoundingScopedID for scope management</returns>
    static CompoundingScopedID Push(const char* id = nullptr, bool isCounted = false);

    static const eastl::string& GetID();

private:
    static eastl::stack<eastl::string> sIDs;
    static eastl::unordered_map<eastl::string, uint32_t> sCounts;
};

bool SliderPlusMinusFloat(float* v, float v_min, float v_max, float smallChange = 0.1f, float bigChange = 0.0f, float width = 0.0f);

void Superscript(const char* string);
void Subscript(const char* string);

ImU32 ColorLerp(ImU32 color1, ImU32 color2, float delta);

namespace Shapes {
struct Style {
    ImU32 mFillColour = 0U;
    ImU32 mBorderColour = 0U;
    float mBorderThickness = 0.f;
};

namespace Piviots {
static constexpr float2_v TopLeft       { 0.0f, 0.0f };
static constexpr float2_v Top           { 0.5f, 0.0f };
static constexpr float2_v TopRight      { 1.0f, 0.0f };
static constexpr float2_v Left          { 0.0f, 0.5f };
static constexpr float2_v Middle        { 0.5f, 0.5f };
static constexpr float2_v Right         { 1.0f, 0.5f };
static constexpr float2_v BottomLeft    { 0.0f, 1.0f };
static constexpr float2_v Bottom        { 0.5f, 1.0f };
static constexpr float2_v BottomRight   { 1.0f, 1.0f };
}

struct Transform {
    float2_v mPiviot = Piviots::TopLeft;
    float2_v mPosition = ImVec2(0.0f, 0.0f);
    float2_v mScale = ImVec2(1.0f, 1.0f);
    float mRotation = 0.0f;
    float mShear = 0.0f;
};

enum class Type : unsigned int {
    Line,
    Path,
    Triangle,
    Rectangle,
    Ellipse,
    NGon,
    Concave,
    Bezier
};

class Canvas;

class Shape {
public:
    virtual Type GetType() const = 0;

    virtual void CheckIntersection(const float2_v& point, bool inclusive) const = 0;

protected:
    virtual void Update(const float deltaTime) {}
    virtual void Draw(Canvas& canvas) const = 0;

protected:
    Style mStyle;
    Transform mTransform;
};

// ==================================================
//  Canvas
// ==================================================
class Canvas {
public:
    explicit Canvas(ImVec2 size) : mPos(ImGui::GetCursorScreenPos()), mSize(size) {}

    inline ImDrawList* GetDrawList() const { return ImGui::GetWindowDrawList(); }
    inline const ImVec2& GetPosition() const { return mPos; }

    void Update(const float deltaTime);
    void Draw() const;

private:
    ImVec2 mPos, mSize;
    eastl::vector<Shape> mShapes;
};

// ==================================================
//  Line
// ==================================================
class Line : public Shape {
public:
    Line() : mP1(0.0f, 0.0f), mP2(0.0f, 0.0f) {}
    Line(const ImVec2& p1, const ImVec2& p2) : mP1(p1), mP2(p2) {}

    void SetPoints(const ImVec2& p1, const ImVec2& p2);
    inline void SetPoint1(const ImVec2& p) { mP1 = p; }
    inline void SetPoint2(const ImVec2& p) { mP2 = p; }

public:
    inline virtual Type GetType() const override { return Type::Line; }

    //virtual void CheckIntersection(const ImVec2& /*point*/, bool /*inclusive*/) const override {}

protected:
    virtual void Update(const float deltaTime) override;
    virtual void Draw(Canvas& canvas) const override;

private:
    ImVec2 mP1, mP2;
};

// ==================================================
//  Path
// ==================================================
class Path : public Shape {
public:
    Path() = default;
    inline Path(eastl_size_t reserveSize) { mPoints.reserve(reserveSize); }
    inline Path(eastl::vector<ImVec2>&& points) : mPoints(eastl::move(points)) {}

    inline eastl_size_t AddPoint(ImVec2&& p) { mPoints.push_back(eastl::move(p)); }

    inline ImVec2& GetPoint(eastl_size_t index) { return mPoints[index]; }
    inline const ImVec2& GetPoint(eastl_size_t index) const { return mPoints[index]; }

    inline eastl_size_t Length() const { return mPoints.size(); }

public:
    inline virtual Type GetType() const override { return Type::Path; }

    //virtual void CheckIntersection(const ImVec2& /*point*/, bool /*inclusive*/) const override {}

protected:
    virtual void Update(const float deltaTime) override;
    virtual void Draw(Canvas& canvas) const override;

private:
    eastl::vector<ImVec2> mPoints;
};

// ==================================================
//  Triangle
// ==================================================
class Triangle : public Shape {
public:
    inline Triangle() : mPoints({ImVec2{ 0.0f, 0.0f }}) {}
    inline Triangle(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3) : mPoints({p1, p2, p3}){}

    inline void SetPoints(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3) { mPoints = { p1, p2, p3 }; }
    inline void SetPoint1(const ImVec2& p) { mPoints[0] = p; }
    inline void SetPoint2(const ImVec2& p) { mPoints[1] = p; }
    inline void SetPoint3(const ImVec2& p) { mPoints[2] = p; }

public:
    inline virtual Type GetType() const override { return Type::Line; }

    //virtual void CheckIntersection(const ImVec2& /*point*/, bool /*inclusive*/) const override {}

protected:
    virtual void Update(const float deltaTime) override;
    virtual void Draw(Canvas& canvas) const override;

private:
    eastl::array<ImVec2, 3> mPoints;
};

// ==================================================
//  Rectangle
// ==================================================
class Rectangle : public Shape {
public:
    Rectangle() = default;
    inline Rectangle(const ImVec2& min, const ImVec2& max) : mMin(min), mMax(max) {}

    inline ImVec2& GetMin() { return mMin; }
    inline ImVec2& GetMax() { return mMax; }
    inline const ImVec2& GetMin() const { return mMin; }
    inline const ImVec2& GetMax() const { return mMax; }

public:
    inline virtual Type GetType() const override { return Type::Rectangle; }

    //virtual void CheckIntersection(const ImVec2& /*point*/, bool /*inclusive*/) const override {}

protected:
    virtual void Update(const float deltaTime) override;
    virtual void Draw(Canvas& canvas) const override;

private:
    ImVec2 mMin = ImVec2(0.0f, 0.0f);
    ImVec2 mMax = ImVec2(0.0f, 0.0f);
};

// ==================================================
//  Ellipse
// ==================================================
class Ellipse : public Shape {
public:
    Ellipse() = default;
    Ellipse(float radius, const ImVec2& origin) : mSize(radius, radius), mOrigin(origin) {}
    Ellipse(const ImVec2& size, const ImVec2& origin) : mSize(size), mOrigin(origin) {}

public:
    inline virtual Type GetType() const override { return Type::Ellipse; }

    //virtual void CheckIntersection(const ImVec2& /*point*/, bool /*inclusive*/) const override {}

protected:
    virtual void Update(const float deltaTime) override;
    virtual void Draw(Canvas& canvas) const override;

private:
    ImVec2 mSize = ImVec2(0.0f, 0.0f);
    ImVec2 mOrigin = ImVec2(0.0f, 0.0f);
};

#pragma region Old code
#if 0
/*
All shapes will need to derive from ShapeBase. It holds a basic set of functions for defining a
boundary box, setting/getting the size, scaling, and checking if a point is within it. There
are two virtual functions that need to be defined to draw the correct shape, and Contains()
can also be overridden to allow more specific shape-checking maths to be performed.
*/
class ShapeBase {
public:
    /// <summary>
    /// Set the region of the shapes bounding box where [minX, minY] are the top-left coords, and
    /// [maxX, maxY] are the bottom-right coords.
    /// </summary>
    /// <param name="minX">Left-most position of the bounding box</param>
    /// <param name="minY">Top-most position of the bounding box</param>
    /// <param name="maxX">Right-most position of the bounding box</param>
    /// <param name="maxY">Bottom-most position of the bounding box</param>
    void SetBounds(float minX, float minY, float maxX, float maxY);


    /// <summary>
    /// Get the shapes origin.
    /// </summary>
    /// <returns>The origin coordinates. 0 = top/left, 1 = bottom/right</returns>
    const ImVec2& GetOrigin() const { return mOrigin; }

    /// <summary>
    /// Set the shapes origin.
    /// </summary>
    /// <param name="origin">The origin coordinates. 0 = top/left, 1 = bottom/right</param>
    void SetOrigin(const ImVec2& origin) { mOrigin = origin; }


    /// <summary>
    /// Get the current position of the shape at its origin.
    /// </summary>
    /// <returns>Location at origin</returns>
    ImVec2 GetPosition() const { return GetPosition(mOrigin); }

    /// <summary>
    /// Get the current position of the shape at the provided origin.
    /// </summary>
    /// <param name="atOrigin">Origin coords to get position</param>
    /// <returns>Location at origin</returns>
    ImVec2 GetPosition(const ImVec2& atOrigin) const;

    /// <summary>
    /// Set the position of the shape without changing the dimensions.
    /// </summary>
    /// <param name="position">Location to position the element</param>
    /// <param name="origin">Origin to use when positioning (overrides shapes origin)</param>
    void SetPosition(const ImVec2& position, const ImVec2& origin);

    /// <summary>
    /// Set the position of the shape without changing the dimensions.
    /// </summary>
    /// <param name="position">Location to position the element</param>
    void SetPosition(const ImVec2& position) { SetPosition(position, mOrigin); }


    /// <summary>
    /// Get the width of the bounding box.
    /// </summary>
    /// <returns>Width of the bounding box</returns>
    float GetWidth() const { return mMax.x - mMin.x; }

    /// <summary>
    /// Get the height of the bounding box.
    /// </summary>
    /// <returns>Height of the bounding box</returns>
    float GetHeight() const { return mMax.y - mMin.y; }

    /// <summary>
    /// Get the width and height of the bounding box.
    /// </summary>
    /// <returns>An ImVec2 with the width (x value) and height (y value) of the bounding box</returns>
    ImVec2 GetSize() const { return { GetWidth(), GetHeight() }; }

    /// <summary>
    /// Set the width of the shape relative to its origin.
    /// </summary>
    /// <param name="width">Width of shape</param>
    void SetWidth(float width) { SetSize(width, GetHeight()); }

    /// <summary>
    /// Set the height of the shape relative to its origin.
    /// </summary>
    /// <param name="height">Height of shape</param>
    void SetHeight(float height) { SetSize(GetWidth(), height); }

    /// <summary>
    /// Set the size of the shape relative to its origin.
    /// </summary>
    /// <param name="size">Width and height of shape</param>
    void SetSize(const ImVec2& size);

    /// <summary>
    /// Set the size of the shape relative to its origin.
    /// </summary>
    /// <param name="width">Width of shape</param>
    /// <param name="height">Height of shape</param>
    void SetSize(float width, float height) { SetSize({ width, height }); }


    /// <summary>
    /// Get the top-left position of the bounding box.
    /// </summary>
    /// <returns>The top-left coord of the bounding box</returns>
    const ImVec2& GetMin() const { return mMin; }

    /// <summary>
    /// Get the bottom-right position of the bounding box.
    /// </summary>
    /// <returns>The bottom-right coord of the bounding box</returns>
    const ImVec2& GetMax() const { return mMax; }


    /// <summary>
    /// Scale the bounding box by a given scalar around the given pivot point.
    /// </summary>
    /// <param name="scalar">The scalar to apply to the bounding boxes size</param>
    /// <param name="pivot">The location to apply the scale around, eg: expand from center using [0.5, 0.5]. Defaults to top-left [0,0]</param>
    void Scale(float scalar, const ImVec2& pivot = { 0.f, 0.f }) { Scale(scalar, scalar, pivot); }

    /// <summary>
    /// Scale the bounding box by a given scalar around the given pivot point.
    /// </summary>
    /// <param name="xScale">The scalar to apply to the bounding boxes width</param>
    /// <param name="yScale">The scalar to apply to the bounding boxes height</param>
    /// <param name="pivot">The location to apply the scale around, eg: expand from center using [0.5, 0.5]. Defaults to top-left [0,0]</param>
    void Scale(float xScale, float yScale, const ImVec2& pivot = { 0.f, 0.f });


    /// <summary>
    /// Check if a given point is within the bounding box. Can be overridden by a derived shape class
    /// to perform more specific checks for itself.
    /// </summary>
    /// <param name="point">The point to check</param>
    /// <param name="inclusiveCheck">When true, a point on the boundary is considered as contained</param>
    /// <returns>True if the point is within the shape OR, if inclusiveCheck is true, returns true if the point is on the shape boundary</returns>
    virtual bool Contains(const ImVec2& point, bool inclusiveCheck = true) const;


    /// <summary>
    /// Draw the shape to the given ImDrawList.
    /// </summary>
    /// <param name="pDrawList">Pointer to the desired ImDrawList the shape will be drawn to</param>
    void Draw(ImDrawList* pDrawList);

protected:

    /// <summary>
    /// Should be orridden to call the desired ImDrawList::AddXFilled() for pDrawList. If the visual data for the
    /// current mouse state has a different x/y scalar, those values will be given as visualMin and visualMax.
    /// </summary>
    /// <param name="pDrawList">Pointer to the desired ImDrawList the shape will be drawn to</param>
    /// <param name="visualMin">The visual left-top position of the bounding rectangle</param>
    /// <param name="visualMax">The visual bottom-right position of the bounding rectangle</param>
    virtual void DrawFilledImpl(ImDrawList* pDrawList, const ImVec2& visualMin, const ImVec2& visualMax) const = 0;

    /// <summary>
    /// Should be orridden to call the desired ImDrawList::AddX() for pDrawList. If the visual data for the
    /// current mouse state has a different x/y scalar, those values will be given as visualMin and visualMax.
    /// </summary>
    /// <param name="pDrawList">Pointer to the desired ImDrawList the shape will be drawn to</param>
    /// <param name="visualMin">The visual left-top position of the bounding rectangle</param>
    /// <param name="visualMax">The visual bottom-right position of the bounding rectangle</param>
    virtual void DrawBorderImpl(ImDrawList* pDrawList, const ImVec2& visualMin, const ImVec2& visualMax) const = 0;

    /// <summary>
    /// Returns the scaled values of the bounding box that would be applied when calling Scale(xScale, yScale, pivot).
    /// </summary>
    /// <param name="xScale">The scalar to apply to the bounding boxes width</param>
    /// <param name="yScale">The scalar to apply to the bounding boxes height</param>
    /// <param name="pivot">The location to apply the scale around, eg: expand from center using [0.5, 0.5]. Defaults to top-left [0,0]</param>
    /// <returns>The result of scaling the bounding box with the given parameters</returns>
    ImVec4 GetScaledBounds(float xScale, float yScale, const ImVec2& pivot = { 0.5f, 0.5f }) const;

public:
    VisualData mVisualData;

protected:
    ImVec2 mMin;
    ImVec2 mMax;
    ImVec2 mOrigin;
};


/*
If you want to create a shaped button, ensure your shape derives from Interactable. This interface
will manage button states based on mouse positions and button presses and perform callbacks for
MouseEnter, MouseExit, ButtonDown, and ButtonUp
*/
class Interactable {
public:
    enum class State {
        Idle,
        Hover,
        Down,
        Disabled,
    };

    enum class ButtonID {
        Unknown = -1,

        Left,
        Right,
        Middle,
        Other1,
        Other2,

        Count
    };

    // Callback types
    using OnMouseEnterCallback = eastl::function<void(Interactable*)>;
    using OnMouseExitCallback = eastl::function<void(Interactable*)>;
    using OnMouseButtonDownCallback = eastl::function<void(Interactable*, ButtonID)>;
    using OnMouseButtonUpCallback = eastl::function<void(Interactable*, ButtonID)>;

public:
    /// <summary>
    /// Enable a button index we want to react to.
    /// </summary>
    /// <param name="buttonID">The button ID to listen to (0 = left, 1 = right, 2 = middle, 3-4 = additional buttons)</param>
    /// <returns>True if successful</returns>
    bool ListenToButton(ButtonID buttonID);

    /// <summary>
    /// Stop listening to the given button.
    /// </summary>
    /// <param name="buttonID">The button ID to stop listening to (0 = left, 1 = right, 2 = middle, 3-4 = additional buttons)</param>
    /// <returns>True if successful</returns>
    bool RemoveButton(ButtonID buttonID);


    /// <summary>
    /// Get the visual data for the current button state.
    /// </summary>
    /// <returns>The visual data of the current state</returns>
    const VisualData& GetCurrentStateVisualData() const { return mStateVisualData[static_cast<int>(mState)]; }

    /// <summary>
    /// Get a reference to the idle state's visual data.
    /// </summary>
    /// <returns>Reference of the idle state's visual data struct</returns>
    VisualData& GetIdleVisualData() { return mStateVisualData[static_cast<int>(State::Idle)]; }

    /// <summary>
    /// Get a reference to the hover state's visual data.
    /// </summary>
    /// <returns>Reference of the hover state's visual data struct</returns>
    VisualData& GetHoverVisualData() { return mStateVisualData[static_cast<int>(State::Hover)]; }

    /// <summary>
    /// Get a reference to the down state's visual data.
    /// </summary>
    /// <returns>Reference of the down state's visual data struct</returns>
    VisualData& GetDownVisualData() { return mStateVisualData[static_cast<int>(State::Down)]; }

    /// <summary>
    /// Get a reference to the disabled state's visual data.
    /// </summary>
    /// <returns>Reference of the disabled state's visual data struct</returns>
    VisualData& GetDisabledVisualData() { return mStateVisualData[static_cast<int>(State::Disabled)]; }


    /// <summary>
    /// Set the callback for when the mouse enters the buttons bounds.
    /// </summary>
    /// <param name="callback">Callback to call for the OnMouseEnter event</param>
    void SetOnMouseEnterCallback(OnMouseEnterCallback callback) { mOnMouseEnterCallback = callback; }

    /// <summary>
    /// Set the callback for when the mouse leaves the buttons bounds.
    /// </summary>
    /// <param name="callback">Callback to call for the OnMouseExit event</param>
    void SetOnMouseExitCallback(OnMouseExitCallback callback) { mOnMouseExitCallback = callback; }

    /// <summary>
    /// Set the callback for when the mouse position is within the shape, and a button we
    /// are listening to is pressed.
    /// </summary>
    /// <param name="callback">Callback to call for the OnMouseButtonDown event</param>
    void SetOnMouseButtonDownCallback(OnMouseButtonDownCallback callback) { mOnMouseButtonDownCallback = callback; }

    /// <summary>
    /// Set the callback for when the mouse position is within the shape, and a button we
    /// are listening to is released.
    /// </summary>
    /// <param name="callback">Callback to call for the OnMouseButtonUp event</param>
    void SetOnMouseButtonUpCallback(OnMouseButtonUpCallback callback) { mOnMouseButtonUpCallback = callback; }


    /// <summary>
    /// Disable the button.
    /// </summary>
    /// <param name="disable">Should the button be disabled? Defaults to True</param>
    void Disable(bool disable = true) { SetState(disable ? State::Disabled : State::Idle); }

    /// <summary>
    /// Checks to see if the button is disabled.
    /// </summary>
    /// <returns>True if the button is disabled</returns>
    bool IsDisabled() const { return mState == State::Disabled; }


    /// <summary>
    /// Sets whether or not the button can be interacted with. This will lock the button into
    /// whichever state it is currently in.
    /// </summary>
    /// <param name="interactable">Should the button be interactable? Defaults to True</param>
    void SetInteractable(bool interactable = true) { mIsInteractable = interactable; }

    /// <summary>
    /// Checks to see if the button is interactable.
    /// </summary>
    /// <returns>True if the button is interactable</returns>
    bool IsInteractable() const { return mIsInteractable; }


    /// <summary>
    /// Should apply the current states visual data to the shape's visual data.
    /// </summary>
    virtual void RefreshVisuals() = 0;


    /// <summary>
    /// Update the button states and keep the position correct (windows can have scroll bars, be moved, etc.).
    /// </summary>
    /// <param name="currentMin">The top-left coords of the boundary box</param>
    void Update(const ImVec2& currentMin);

protected:
    /// <summary>
    /// Should call the shape's Contains function.
    /// </summary>
    /// <param name="point">The point to check</param>
    /// <param name="inclusiveCheck">When true, a point on the boundary is considered as contained</param>
    /// <returns>True if the point is within the shape OR, if inclusiveCheck is true, returns true if the point is on the shape boundary</returns>
    virtual bool Contains(const ImVec2& point, bool inclusiveCheck = true) const = 0;

    /// <summary>
    /// Should set the position of the shape.
    /// </summary>
    /// <param name="currentMin">The top-left coords of the boundary box</param>
    virtual void UpdatePosition(const ImVec2& currentMin) = 0;

private:
    void SetState(State newState);

private:
    State mState = State::Idle;

    bool mIsInteractable = true;
    ButtonID mButtonIDDown = ButtonID::Unknown;

    eastl::array<bool, static_cast<int>(ButtonID::Count)> mActiveButtonIDs{ false };
    eastl::array<VisualData, 4> mStateVisualData;

    OnMouseEnterCallback mOnMouseEnterCallback = nullptr;
    OnMouseExitCallback mOnMouseExitCallback = nullptr;
    OnMouseButtonDownCallback mOnMouseButtonDownCallback = nullptr;
    OnMouseButtonUpCallback mOnMouseButtonUpCallback = nullptr;
};


//////////////////////////////////////////////////////////////////////
// Rectangle

class Rectangle : public ShapeBase {
public:
    Rectangle() = default;
    Rectangle(const ImVec4& points);
    Rectangle(const ImVec2& topLeft, const ImVec2& size);
    Rectangle(float minX, float minY, float maxX, float maxY);

protected:
    virtual void DrawFilledImpl(ImDrawList* pDrawList, const ImVec2& visualMin, const ImVec2& visualMax) const override;
    virtual void DrawBorderImpl(ImDrawList* pDrawList, const ImVec2& visualMin, const ImVec2& visualMax) const override;
};


//////////////////////////////////////////////////////////////////////
// Ellipse

class Ellipse : public ShapeBase {
public:
    Ellipse();
    Ellipse(const ImVec2& position, float radiusX, float radiusY);
    Ellipse(const ImVec2& position, float radius);

    float GetRadiusX() const { return GetWidth() * 0.5f; }
    float GetRadiusY() const { return GetHeight() * 0.5f; }

    virtual bool Contains(const ImVec2& point, bool inclusiveCheck = true) const override;

protected:
    virtual void DrawFilledImpl(ImDrawList* pDrawList, const ImVec2& visualMin, const ImVec2& visualMax) const override;
    virtual void DrawBorderImpl(ImDrawList* pDrawList, const ImVec2& visualMin, const ImVec2& visualMax) const override;
};


//////////////////////////////////////////////////////////////////////
// Ellipse Button

class InteractableEllipse : public Ellipse, public Interactable {
public:
    InteractableEllipse(const ImVec2& position, float radiusX, float radiusY) : Ellipse(position, radiusX, radiusY) {}
    InteractableEllipse(const ImVec2& position, float radius) : Ellipse(position, radius) {}

    virtual bool Contains(const ImVec2& point, bool inclusiveCheck = true) const override { return Ellipse::Contains(point, inclusiveCheck); }

    virtual void UpdatePosition(const ImVec2& newPosition) { SetPosition(newPosition); }

    virtual void RefreshVisuals() override { mVisualData = GetCurrentStateVisualData(); }

protected:
    virtual void DrawFilledImpl(ImDrawList* pDrawList, const ImVec2& visualMin, const ImVec2& visualMax) const override {
        Ellipse::DrawFilledImpl(pDrawList, visualMin, visualMax);
    }
    virtual void DrawBorderImpl(ImDrawList* pDrawList, const ImVec2& visualMin, const ImVec2& visualMax) const override {
        Ellipse::DrawBorderImpl(pDrawList, visualMin, visualMax);
    }
};
#endif
#pragma endregion
}
}
#endif