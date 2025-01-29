#include "Generic/ImGuiEx.h"

#if defined(IMGUI_IS_ENABLED)
#include "EASTL/vector.h"

namespace ImGuiEx {
namespace {
constexpr float TOOLTIP_TEXT_WRAP_PADDING = 10.f;

bool IsValidMouseButtonID(Shapes::Interactable::ButtonID buttonIndex) {
	return buttonIndex > Shapes::Interactable::ButtonID::Unknown &&
		buttonIndex < Shapes::Interactable::ButtonID::Count;
}

void TooltipInternal(bool forceVisible, const char* fmt, va_list args) {
	if (!forceVisible && !ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip)) {
		return;
	}

	const float width = ImGui::GetWindowWidth() - TOOLTIP_TEXT_WRAP_PADDING;
	if (!ImGui::BeginTooltip()) {
		return;
	}
	ImGui::PushTextWrapPos(width);
	ImGui::TextWrappedV(fmt, args);
	ImGui::PopTextWrapPos();
	ImGui::EndTooltip();
}
}

void DrawTooltipNotification() {
	constexpr ImU32 tooltipNotificationTextColor = IM_COL32(255, 255, 86, 120);
	ImGui::SameLine(0.0f, 0.0f);
	ImGui::PushStyleColor(ImGuiCol_Text, tooltipNotificationTextColor);
	Superscript("(?)");
	ImGui::PopStyleColor();
}

void LabelWithTooltip(const char* label, const char* fmt, ...) {

	ImGui::BeginGroup();
	ImGui::Text(label);
	DrawTooltipNotification();
	ImGui::EndGroup();

	va_list args;
	va_start(args, fmt);
	TooltipInternal(false, fmt, args);
	va_end(args);
}

void Tooltip(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	TooltipInternal(false, fmt, args);
	va_end(args);
}

void DisplayTooltip(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	TooltipInternal(true, fmt, args);
	va_end(args);
}

//=======================================================
// CompoundingScopedID

eastl::stack<eastl::string> CompoundingScopedID::sIDs;
eastl::unordered_map<eastl::string, uint32_t> CompoundingScopedID::sCounts;

CompoundingScopedID::CompoundingScopedID(eastl::string&& newID, bool isCounted) {
	if (isCounted) {
		uint32_t index = 0;
		const auto found = sCounts.find(newID);
		if (found == sCounts.end()) {
			sCounts[newID] = 0;
		} else {
			index = found->second;
		}
		sCounts[newID]++;

		newID.append_sprintf("-%u", index);
	}

	ImGui::PushID(newID.c_str());
	sIDs.push(eastl::move(newID));
}

CompoundingScopedID::~CompoundingScopedID() {
	sIDs.pop();
	ImGui::PopID();

	if (sIDs.empty()) {
		for (auto& [key, value] : sCounts) {
			value = 0;
		}
	}
}

CompoundingScopedID CompoundingScopedID::Push(const char* id, bool counted) {
	if (id == nullptr || eastl::CharStrlen(id) == 0) {
		id = "<unnamed>";
		counted = true;
	}

	eastl::string compoundedID = GetID();
	if (!compoundedID.empty()) {
		compoundedID.append(".");
	}
	compoundedID.append(id);

	return CompoundingScopedID(eastl::move(compoundedID), counted);
}

const eastl::string& CompoundingScopedID::GetID() {
	static const eastl::string NULL_STR = "";
	return sIDs.empty() ? NULL_STR : sIDs.top();
}

//=======================================================

namespace {
void RenderSuperOrSubScript(bool isSuper, const char* string) {
	constexpr float scalar = 0.6f;

	const float fontSize = ImGui::GetFont()->FontSize;
	const float fontScale = ImGui::GetFont()->Scale;
	const float offset = fontSize * (isSuper ? -0.5f : 0.2f);

	static ImFont fnt = *ImGui::GetFont();
	fnt.FontSize = fontSize * scalar;
	fnt.Scale = fontScale * scalar;
	ImGui::PushFont(&fnt);

	const ImVec2 screenPos = ImGui::GetCursorScreenPos();
	ImGui::SetCursorScreenPos(ImVec2(screenPos.x, screenPos.y + offset));
	ImGui::Text(string);

	ImGui::PopFont();
}
}

void Superscript(const char* string) {
	RenderSuperOrSubScript(true, string);
}

void Subscript(const char* string) {
	RenderSuperOrSubScript(false, string);
}

bool SliderPlusMinusFloat(float* v, float v_min, float v_max, float smallChange, float bigChange, float width) {
	constexpr ImVec2 buttonSize = ImVec2(40.0f, 40.0f);
	constexpr float buttonSpacing = 2.0f;
	constexpr float minSliderWidth = 60.0f;
	constexpr float buttonSpacingRequired = (buttonSpacing + buttonSize.x) * 2.0f;

	const bool hasBigChangeButtons = bigChange > 0.0f;
	const float floatSliderWidth = eastl::max(minSliderWidth + (!hasBigChangeButtons ? buttonSpacingRequired : 0.0f), width);

	ImGuiStyle& imStyle = ImGui::GetStyle();
	const ImVec2 defaultFramePadding = imStyle.FramePadding;
	imStyle.FramePadding = ImVec2(0.0f, (buttonSize.y - ImGui::GetTextLineHeight()) * 0.5f);

	const auto scopedID = XR::IMGUI::CompoundingScopedID::Push("SliderPlusMinusFloat", true);

	bool changeWasMade = false;
	const auto adjustValue = [&changeWasMade, v, v_min, v_max](float amount) {
		changeWasMade = true;
		*v = Math::clamp(*v + amount, v_min, v_max);
	};

	ImGui::PushItemWidth(buttonSize.x);
	if (hasBigChangeButtons) {
		if (ImGui::Button("<<", buttonSize)) {
			adjustValue(-bigChange);
		}
		Tooltip("%f", -bigChange);
		ImGui::SameLine(0.0f, buttonSpacing);
	}
	if (ImGui::Button("<", buttonSize)) {
		adjustValue(-smallChange);
	}
	Tooltip("%f", -smallChange);
	ImGui::PopItemWidth();
	ImGui::SameLine(0.0f, buttonSpacing);

	ImGui::PushItemWidth(floatSliderWidth);
	changeWasMade = ImGui::SliderFloat("", v, v_min, v_max) || changeWasMade;
	ImGui::PopItemWidth();

	ImGui::SameLine(0.0f, buttonSpacing);
	ImGui::PushItemWidth(buttonSize.x);
	if (ImGui::Button(">", buttonSize)) {
		adjustValue(smallChange);
	}
	Tooltip("+%f", smallChange);
	if (hasBigChangeButtons) {
		ImGui::SameLine(0.0f, buttonSpacing);
		if (ImGui::Button(">>", buttonSize)) {
			adjustValue(bigChange);
		}
		Tooltip("+%f", bigChange);
	}
	ImGui::PopItemWidth();

	imStyle.FramePadding = defaultFramePadding;

	return changeWasMade;
}

ImU32 ColorLerp(ImU32 color1, ImU32 color2, float delta) {
	const ImColor c1 = color1;
	const ImColor c2 = color2;
	const int newR = static_cast<int>(round(255.f * (c1.Value.x + (c2.Value.x - c1.Value.x) * delta)));
	const int newG = static_cast<int>(round(255.f * (c1.Value.y + (c2.Value.y - c1.Value.y) * delta)));
	const int newB = static_cast<int>(round(255.f * (c1.Value.z + (c2.Value.z - c1.Value.z) * delta)));
	const int newA = static_cast<int>(round(255.f * (c1.Value.w + (c2.Value.w - c1.Value.w) * delta)));
	return IM_COL32(newR, newG, newB, newA);
}


////////////////////////////////////////////////////////////////////
//
//  Shapes
//
////////////////////////////////////////////////////////////////////

void Shapes::ShapeBase::SetBounds(float minX, float minY, float maxX, float maxY) {
	mMin = { minX, minY };
	mMax = { maxX, maxY };
}

void Shapes::ShapeBase::SetPosition(const ImVec2& position, const ImVec2& origin) {
	ImVec2 newMin;
	newMin.x = position.x - GetWidth() * origin.x;
	newMin.y = position.y - GetHeight() * origin.y;
	SetBounds(newMin.x, newMin.y, newMin.x + GetWidth(), newMin.y + GetHeight());
}

ImVec2 Shapes::ShapeBase::GetPosition(const ImVec2& origin) const {
	ImVec2 position;
	position.x = mMin.x + GetWidth() * origin.x;
	position.y = mMin.y + GetHeight() * origin.y;
	return position;
}

void Shapes::ShapeBase::SetSize(const ImVec2& size) {
	ImVec2 newMin;
	newMin.x = (1.f - mOrigin.x) * size.x;
	newMin.y = (1.f - mOrigin.y) * size.y;
	SetBounds(newMin.x, newMin.y, newMin.x + size.x, newMin.y + size.y);
}

void Shapes::ShapeBase::Draw(ImDrawList* pDrawList) {
	Platform_Assert(pDrawList);
	if (pDrawList == nullptr) {
		return;
	}

	const bool drawFilled = ImColor(mVisualData.mFillColour).Value.w != 0;
	const bool drawBorder = ImColor(mVisualData.mBorderColour).Value.w != 0;
	if (!drawFilled && !drawBorder) {
		return;
	}

	// We might only want to -visually- change the size of the shape while keeping the actual
	// shape dimensions the same, so we don't call 'Scale' directly
	ImVec4 scaledSize = GetScaledBounds(mVisualData.mXScale, mVisualData.mYScale, mOrigin);
	if (drawFilled) {
		DrawFilledImpl(pDrawList, { scaledSize.x, scaledSize.y }, { scaledSize.z, scaledSize.w });
	}
	if (drawBorder) {
		DrawBorderImpl(pDrawList, { scaledSize.x, scaledSize.y }, { scaledSize.z, scaledSize.w });
	}
}

ImVec4 Shapes::ShapeBase::GetScaledBounds(float xScale, float yScale, const ImVec2& pivot) const {
	if (xScale == 1.f && yScale == 1.f) {
		return { mMin.x, mMin.y, mMax.x, mMax.y };
	}

	const ImVec2 size = GetSize();
	const ImVec2 newSize = { size.x * xScale, size.y * yScale };
	ImVec4 scaledVals;
	scaledVals.x = mMin.x + (size.x - newSize.x) * pivot.x;
	scaledVals.y = mMin.y + (size.y - newSize.y) * pivot.y;
	scaledVals.z = mMin.x + newSize.x;
	scaledVals.w = mMin.y + newSize.y;
	return scaledVals;
}

void Shapes::ShapeBase::Scale(float xScale, float yScale, const ImVec2& pivot) {
	const ImVec4 scaledVals = GetScaledBounds(xScale, yScale, pivot);
	SetBounds(scaledVals.x, scaledVals.y, scaledVals.z, scaledVals.w);
}

bool Shapes::ShapeBase::Contains(const ImVec2& point, bool inclusiveCheck) const {
	if (inclusiveCheck) {
		return mMin.x <= point.x && mMax.x >= point.x
			&& mMin.y <= point.y && mMax.y >= point.y;
	}
	return mMin.x < point.x && mMax.x > point.x
		&& mMin.y < point.y && mMax.y > point.y;
}


//
//  Interactable shapes
//

bool Shapes::Interactable::ListenToButton(ButtonID buttonIndex) {
	if (!IsValidMouseButtonID(buttonIndex)) {
		Platform_Assert(false);
		return false;
	}

	mActiveButtonIDs[static_cast<int>(buttonIndex)] = true;
	return true;
}

bool Shapes::Interactable::RemoveButton(ButtonID buttonIndex) {
	if (!IsValidMouseButtonID(buttonIndex)) {
		Platform_Assert(false);
		return false;
	}

	mActiveButtonIDs[static_cast<int>(buttonIndex)] = false;
	if (buttonIndex == mButtonIDDown) {
		mButtonIDDown = ButtonID::Unknown;
		SetState(State::Idle);
	}
	return true;
}

void Shapes::Interactable::Update(const ImVec2& newPosition) {
	// Ensure the drawn element is positioned correctly
	UpdatePosition(newPosition);

	if (!mIsInteractable || mState == State::Disabled) {
		return;
	}

	ImGuiIO io = ImGui::GetIO();

	for (int buttonIndex = 0; buttonIndex < static_cast<int>(ButtonID::Count); ++buttonIndex) {
		const ButtonID buttonID = static_cast<ButtonID>(buttonIndex);
		if (!mActiveButtonIDs[buttonIndex]) {
			continue;
		}

		// If we are actively engaged with a specific button, ignore other button
		// inputs and focus solely that
		if (mButtonIDDown != ButtonID::Unknown && buttonID != mButtonIDDown) {
			continue;
		}

		switch (mState) {
		case State::Idle:
		{
			if (!Contains(io.MousePos)) {
				break;
			}

			if (mOnMouseEnterCallback != nullptr) {
				mOnMouseEnterCallback(this);
			}

			// Did we click on this button, hold the mouse button, move out, then move back in?
			if (Contains(io.MouseClickedPos[buttonIndex]) && io.MouseDown[buttonIndex]) {
				SetState(State::Down);
			} else {
				SetState(State::Hover);
			}

			break;
		}
		case State::Hover:
		{
			if (!Contains(io.MousePos)) {
				if (mOnMouseExitCallback != nullptr) {
					mOnMouseExitCallback(this);
				}
				SetState(State::Idle);
			} else if (io.MouseClicked[buttonIndex]) {
				mButtonIDDown = buttonID;
				if (mOnMouseButtonDownCallback != nullptr) {
					mOnMouseButtonDownCallback(this, buttonID);
				}
				SetState(State::Down);
			}
			break;
		}
		case State::Down:
		{
			if (!Contains(io.MousePos)) {
				if (mOnMouseExitCallback != nullptr) {
					mOnMouseExitCallback(this);
				}
				SetState(State::Idle);
			} else if (io.MouseReleased[buttonIndex]) {
				mButtonIDDown = ButtonID::Unknown;
				if (mOnMouseButtonUpCallback != nullptr) {
					mOnMouseButtonUpCallback(this, buttonID);
				}
				SetState(State::Hover);
			}
			break;
		}
		}

		// If we have released the button we were actively engaged with, set
		// the id back to unknown
		if (mButtonIDDown != ButtonID::Unknown && !io.MouseDown[static_cast<int>(mButtonIDDown)]) {
			mButtonIDDown = ButtonID::Unknown;
		}
	}
}

void Shapes::Interactable::SetState(State newState) {
	if (newState == mState) {
		return;
	}
	mState = newState;
	RefreshVisuals();
}


//////////////////////////////////////////////////////////////////////
// Rectangle

Shapes::Rectangle::Rectangle(const ImVec4& points) {
	SetBounds(points.x, points.y, points.z, points.w);
}

Shapes::Rectangle::Rectangle(const ImVec2& topLeft, const ImVec2& size) {
	SetBounds(topLeft.x, topLeft.y, topLeft.x + size.x, topLeft.y + size.y);
}

Shapes::Rectangle::Rectangle(float minX, float minY, float maxX, float maxY) {
	SetBounds(minX, minY, maxX, maxY);
}

void Shapes::Rectangle::DrawFilledImpl(ImDrawList* pDrawList, const ImVec2& visualMin, const ImVec2& visualMax) const {
	pDrawList->AddRectFilled(visualMin, visualMax, mVisualData.mFillColour);
}

void Shapes::Rectangle::DrawBorderImpl(ImDrawList* pDrawList, const ImVec2& visualMin, const ImVec2& visualMax) const {
	pDrawList->AddRect(visualMin, visualMax, mVisualData.mBorderColour, 0.f, 0, mVisualData.mBorderThickness);
}


//////////////////////////////////////////////////////////////////////
// Ellipse

Shapes::Ellipse::Ellipse() {
	SetOrigin({ 0.5f, 0.5f });
}

Shapes::Ellipse::Ellipse(const ImVec2& position, float radiusX, float radiusY) {
	SetBounds(position.x - radiusX, position.y - radiusY, position.x + radiusX, position.y + radiusY);
	SetOrigin({ 0.5f, 0.5f });
}

Shapes::Ellipse::Ellipse(const ImVec2& position, float radius) {
	SetBounds(position.x - radius, position.y - radius, position.x + radius, position.y + radius);
	SetOrigin({ 0.5f, 0.5f });
}

void Shapes::Ellipse::DrawFilledImpl(ImDrawList* pDrawList, const ImVec2& visualMin, const ImVec2& visualMax) const {
	const ImVec2 radii((visualMax.x - visualMin.x) * 0.5f, (visualMax.y - visualMin.y) * 0.5f);
	ImVec2 position(visualMin.x + radii.x, visualMin.y + radii.y);
	pDrawList->AddEllipseFilled(position, radii.x, radii.y, mVisualData.mFillColour);
}

void Shapes::Ellipse::DrawBorderImpl(ImDrawList* pDrawList, const ImVec2& visualMin, const ImVec2& visualMax) const {
	const ImVec2 radii((visualMax.x - visualMin.x) * 0.5f, (visualMax.y - visualMin.y) * 0.5f);
	ImVec2 position(visualMin.x + radii.x, visualMin.y + radii.y);
	pDrawList->AddEllipse(position, radii.x, radii.y, mVisualData.mBorderColour, 0.f, 0, mVisualData.mBorderThickness);
}

bool Shapes::Ellipse::Contains(const ImVec2& point, bool inclusiveCheck) const {
	if (!ShapeBase::Contains(point, inclusiveCheck)) {
		return false;
	}

	const ImVec2 position = GetPosition({ 0.5f, 0.5f });
	const ImVec2 size = GetSize();
	const ImVec2 r = { size.x * 0.5f, size.y * 0.5f };

	const float x2 = (point.x - position.x) * (point.x - position.x);
	const float y2 = (point.y - position.y) * (point.y - position.y);

	if (size.x == size.y) {
		// Just check for circle
		if (inclusiveCheck) {
			return x2 + y2 <= r.x * r.x;
		}
		return x2 + y2 < r.x * r.x;
	}

	if (inclusiveCheck) {
		return (x2 / (r.x * r.x)) + (y2 / (r.y * r.y)) <= 1.f;
	}
	return (x2 / (r.x * r.x)) + (y2 / (r.y * r.y)) < 1.f;
}
}
#endif // defined(FXS_ENABLE_IMGUI)
