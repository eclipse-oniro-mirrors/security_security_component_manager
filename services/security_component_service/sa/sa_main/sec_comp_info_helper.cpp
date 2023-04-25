/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "sec_comp_info_helper.h"

#include "display.h"
#include "display_info.h"
#include "display_manager.h"
#include "location_button.h"
#include "paste_button.h"
#include "save_button.h"
#include "sec_comp_err.h"
#include "sec_comp_log.h"
#include "sec_comp_tool.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
namespace {
constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompInfoHelper"};
static constexpr double MAX_RECT_PERCENT = 0.1F; // 10%
static constexpr double ZERO_OFFSET = 0.0F;
static std::mutex g_renderLock;
}

SecCompBase* SecCompInfoHelper::ParseComponent(SecCompType type, const nlohmann::json& jsonComponent)
{
    SecCompBase* comp = nullptr;
    switch (type) {
        case LOCATION_COMPONENT:
            comp = ConstructComponent<LocationButton>(jsonComponent);
            break;
        case PASTE_COMPONENT:
            comp = ConstructComponent<PasteButton>(jsonComponent);
            break;
        case SAVE_COMPONENT:
            comp = ConstructComponent<SaveButton>(jsonComponent);
            break;
        default:
            SC_LOG_ERROR(LABEL, "Parse component type unknown");
            break;
    }
    if (comp == nullptr) {
        SC_LOG_ERROR(LABEL, "Parse component failed");
        return comp;
    }

    comp->SetValid(CheckComponentValid(comp));
    return comp;
}

static bool GetScreenSize(double& width, double& height)
{
    sptr<OHOS::Rosen::Display> display =
        OHOS::Rosen::DisplayManager::GetInstance().GetDefaultDisplaySync();
    if (display == nullptr) {
        SC_LOG_ERROR(LABEL, "Get display manager failed");
        return false;
    }

    auto info = display->GetDisplayInfo();
    if (info == nullptr) {
        SC_LOG_ERROR(LABEL, "Get display info failed");
        return false;
    }

    width = static_cast<double>(info->GetWidth());
    height = static_cast<double>(info->GetHeight());
    SC_LOG_DEBUG(LABEL, "display manager Screen width %{public}f height %{public}f",
        width, height);
    return true;
}

static bool CheckRectValid(const SecCompRect& rect)
{
    double curScreenWidth = 0.0F;
    double curScreenHeight = 0.0F;
    if (!GetScreenSize(curScreenWidth, curScreenHeight)) {
        SC_LOG_ERROR(LABEL, "Get screen size is invalid");
        return false;
    }

    if (GreatNotEqual(ZERO_OFFSET, rect.x_) || GreatNotEqual(ZERO_OFFSET, rect.y_) ||
        GreatNotEqual(rect.x_, curScreenWidth) || GreatNotEqual(rect.y_, curScreenHeight)) {
        SC_LOG_ERROR(LABEL, "start point is out of screen");
        return false;
    }

    if (GreatOrEqual((rect.x_ + rect.width_), curScreenWidth) ||
        GreatOrEqual((rect.y_ + rect.height_), curScreenHeight)) {
        SC_LOG_ERROR(LABEL, "rect is out of screen");
        return false;
    }

    // check rect > 10%
    if (GreatOrEqual((rect.width_ * rect.height_), (curScreenWidth * curScreenHeight * MAX_RECT_PERCENT))) {
        SC_LOG_ERROR(LABEL, "rect area is too large");
        return false;
    }

    return true;
}

static bool CheckSecCompBase(const SecCompBase* comp)
{
    if (!CheckRectValid(comp->rect_)) {
        SC_LOG_INFO(LABEL, "check component rect failed.");
        return false;
    }

    if (comp->parentEffect_) {
        SC_LOG_ERROR(LABEL, "parentEffect is active, security component invalid.");
        return false;
    }

    if ((comp->fontSize_ < MIN_FONT_SIZE) || (comp->iconSize_ < MIN_ICON_SIZE) ||
        (comp->padding_.top < MIN_PADDING_SIZE) || (comp->padding_.right < MIN_PADDING_SIZE) ||
        (comp->padding_.bottom < MIN_PADDING_SIZE) || (comp->padding_.left < MIN_PADDING_SIZE) ||
        (comp->textIconPadding_ < MIN_PADDING_SIZE)) {
        SC_LOG_ERROR(LABEL, "size is invalid.");
        return false;
    }

    if (IsColorTransparent(comp->bgColor_) || IsColorTransparent(comp->fontColor_) ||
        IsColorTransparent(comp->iconColor_)) {
        SC_LOG_ERROR(LABEL, "bgColor or fontColor or iconColor is too transparent.");
        return false;
    }

    return true;
}

static bool CheckLocationButton(const LocationButton* comp)
{
    if ((comp->text_ == LocationDesc::NO_TEXT) && (comp->icon_ == LocationIcon::NO_ICON)) {
        SC_LOG_INFO(LABEL, "both text and icon do not exist.");
        return false;
    }

    if (comp->bg_ != LocationBackground::NO_BG_TYPE &&
        (IsColorSimilar(comp->fontColor_, comp->bgColor_) || IsColorSimilar(comp->iconColor_, comp->bgColor_))) {
        SC_LOG_INFO(LABEL, "fontColor or iconColor is similar whith backgroundColor.");
        return false;
    }

    if (comp->bg_ == LocationBackground::NO_BG_TYPE &&
        ((comp->padding_.top != MIN_PADDING_WITHOUT_BG) || (comp->padding_.right != MIN_PADDING_WITHOUT_BG) ||
        (comp->padding_.bottom != MIN_PADDING_WITHOUT_BG) || (comp->padding_.left != MIN_PADDING_WITHOUT_BG))) {
        SC_LOG_INFO(LABEL, "padding can not change without background.");
        return false;
    }

    return true;
}

static bool CheckPasteButton(const PasteButton* comp)
{
    return false;
}

static bool CheckSaveButton(const SaveButton* comp)
{
    return false;
}

bool SecCompInfoHelper::CheckComponentValid(const SecCompBase* comp)
{
    if ((comp == nullptr) || !IsComponentTypeValid(comp->type_)) {
        SC_LOG_INFO(LABEL, "comp is null or type is invalid.");
        return false;
    }

    if (!CheckSecCompBase(comp)) {
        SC_LOG_INFO(LABEL, "SecComp base is invalid.");
        return false;
    }

    bool valid = false;
    switch (comp->type_) {
        case LOCATION_COMPONENT:
            valid = CheckLocationButton(reinterpret_cast<const LocationButton *>(comp));
            break;
        case PASTE_COMPONENT:
            valid = CheckPasteButton(reinterpret_cast<const PasteButton *>(comp));
            break;
        case SAVE_COMPONENT:
            valid = CheckSaveButton(reinterpret_cast<const SaveButton *>(comp));
            break;
        default:
            SC_LOG_ERROR(LABEL, "Parse component type unknown");
            break;
    }

    return valid;
}

int32_t SecCompInfoHelper::RevokeTempPermission(AccessToken::AccessTokenID tokenId,
    const std::shared_ptr<SecCompBase>& componentInfo)
{
    if (componentInfo == nullptr) {
        SC_LOG_ERROR(LABEL, "revoke component is null");
        return SC_SERVICE_ERROR_PERMISSION_OPER_FAIL;
    }
    SecCompType type = componentInfo->type_;
    switch (type) {
        case LOCATION_COMPONENT:
            return AccessToken::AccessTokenKit::RevokePermission(tokenId, "ohos.permission.LOCATION",
                AccessToken::PermissionFlag::PERMISSION_COMPONENT_SET);
        default:
            SC_LOG_ERROR(LABEL, "revoke component type unknown");
            break;
    }
    return SC_SERVICE_ERROR_PERMISSION_OPER_FAIL;
}

int32_t SecCompInfoHelper::GrantTempPermission(AccessToken::AccessTokenID tokenId,
    const std::shared_ptr<SecCompBase>& componentInfo)
{
    if (componentInfo == nullptr) {
        SC_LOG_ERROR(LABEL, "revoke component is null");
        return SC_SERVICE_ERROR_PERMISSION_OPER_FAIL;
    }
    SecCompType type = componentInfo->type_;
    switch (type) {
        case LOCATION_COMPONENT:
            return AccessToken::AccessTokenKit::GrantPermission(tokenId, "ohos.permission.LOCATION",
                AccessToken::PermissionFlag::PERMISSION_COMPONENT_SET);
        default:
            SC_LOG_ERROR(LABEL, "Parse component type unknown");
            break;
    }
    return SC_SERVICE_ERROR_PERMISSION_OPER_FAIL;
}
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
