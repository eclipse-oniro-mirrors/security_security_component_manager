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
#include "location_button_sample_build.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
static const std::string JSON_STYLE_TAG = "style";
static const std::string JSON_TEXT_TAG = "text";
static const std::string JSON_ICON_TAG = "icon";
static const std::string JSON_BG_TAG = "bg";
static const std::string JSON_NODE_ID = "nodeId";
static const std::string JSON_RECT = "rect";
static const std::string JSON_WINDOW_RECT = "windowRect";
static const std::string JSON_SC_TYPE = "type";
static const std::string JSON_RECT_X = "x";
static const std::string JSON_RECT_Y = "y";
static const std::string JSON_RECT_WIDTH = "width";
static const std::string JSON_RECT_HEIGHT = "height";
static const std::string JSON_SIZE_TAG = "size";
static const std::string JSON_FONT_SIZE_TAG = "fontSize";
static const std::string JSON_ICON_SIZE_TAG = "iconSize";
static const std::string JSON_PADDING_SIZE_TAG = "paddingSize";
static const std::string JSON_PADDING_LEFT_TAG = "left";
static const std::string JSON_PADDING_TOP_TAG = "top";
static const std::string JSON_PADDING_RIGHT_TAG = "right";
static const std::string JSON_PADDING_BOTTOM_TAG = "bottom";
static const std::string JSON_TEXT_ICON_PADDING_TAG = "textIconSpace";
static const std::string JSON_RECT_WIDTH_TAG = "width";
static const std::string JSON_RECT_HEIGHT_TAG = "height";
static const std::string JSON_INVALID_TAG = "test";
static const std::string JSON_COLORS_TAG = "colors";
static const std::string JSON_FONT_COLOR_TAG = "fontColor";
static const std::string JSON_ICON_COLOR_TAG = "iconColor";
static const std::string JSON_BG_COLOR_TAG = "bgColor";
static const std::string JSON_BORDER_TAG = "border";
static const std::string JSON_BORDER_WIDTH_TAG = "borderWidth";
static const std::string JSON_PARENT_TAG = "parent";
static const std::string JSON_PARENT_EFFECT_TAG = "parentEffect";
static const std::string WRONG_TYPE = "wrongType";
static constexpr float TEST_SIZE = 100.0;
static constexpr double TEST_COORDINATE = 100.0;
static constexpr double TEST_DIMENSION = 100.0;
static constexpr uint32_t TEST_COLOR_YELLOW = 0xffffff00;
static constexpr uint32_t TEST_COLOR_RED = 0xffff0000;
static constexpr uint32_t TEST_COLOR_BLUE = 0xff0000ff;

void LocationButtonSampleBuild::BuildLocationComponentInfo(nlohmann::json& jsonComponent)
{
    jsonComponent[JSON_SC_TYPE] = LOCATION_COMPONENT;
    jsonComponent[JSON_RECT] = nlohmann::json {
        {JSON_RECT_X, TEST_COORDINATE },
        {JSON_RECT_Y, TEST_COORDINATE },
        {JSON_RECT_WIDTH, TEST_COORDINATE },
        {JSON_RECT_HEIGHT, TEST_COORDINATE }
    };
    jsonComponent[JSON_NODE_ID] = 0;
    jsonComponent[JSON_WINDOW_RECT] = nlohmann::json {
        {JSON_RECT_X, TEST_COORDINATE },
        {JSON_RECT_Y, TEST_COORDINATE },
        {JSON_RECT_WIDTH, TEST_COORDINATE },
        {JSON_RECT_HEIGHT, TEST_COORDINATE }
    };
    nlohmann::json jsonPadding = nlohmann::json {
        { JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JSON_PADDING_RIGHT_TAG, TEST_DIMENSION },
        { JSON_PADDING_BOTTOM_TAG, TEST_DIMENSION },
        { JSON_PADDING_LEFT_TAG, TEST_DIMENSION },
    };

    jsonComponent[JSON_SIZE_TAG] = nlohmann::json {
        { JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JSON_PADDING_SIZE_TAG, jsonPadding },
    };

    jsonComponent[JSON_COLORS_TAG] = nlohmann::json {
        { JSON_FONT_COLOR_TAG, TEST_COLOR_RED },
        { JSON_ICON_COLOR_TAG, TEST_COLOR_BLUE },
        { JSON_BG_COLOR_TAG, TEST_COLOR_YELLOW }
    };

    jsonComponent[JSON_BORDER_TAG] = nlohmann::json {
        { JSON_BORDER_WIDTH_TAG, TEST_SIZE },
    };
    jsonComponent[JSON_PARENT_TAG] = nlohmann::json {
        { JSON_PARENT_EFFECT_TAG, false },
    };
    jsonComponent[JSON_STYLE_TAG] = nlohmann::json {
        { JSON_TEXT_TAG, LocationDesc::SELECT_LOCATION },
        { JSON_ICON_TAG, LocationIcon::LINE_ICON },
        { JSON_BG_TAG, SecCompBackground::CIRCLE },
    };
}
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
