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

#include "sec_comp_info_helper_test.h"
#include "location_button.h"
#include "paste_button.h"
#include "save_button.h"
#include "sec_comp_log.h"
#include "sec_comp_err.h"
#include "display.h"
#include "display_info.h"
#include "display_manager.h"

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::Security::SecurityComponent;

namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompInfoHelperTest"};
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

static const std::string JSON_COLORS_TAG = "colors";
static const std::string JSON_FONT_COLOR_TAG = "fontColor";
static const std::string JSON_ICON_COLOR_TAG = "iconColor";
static const std::string JSON_BG_COLOR_TAG = "bgColor";

static const std::string JSON_BORDER_TAG = "border";
static const std::string JSON_BORDER_WIDTH_TAG = "borderWidth";
static const std::string JSON_PARENT_TAG = "parent";
static const std::string JSON_PARENT_EFFECT_TAG = "parentEffect";

static constexpr float TEST_SIZE = 100.0;
static constexpr double TEST_DIMENSION = 100.0;
static constexpr double TEST_INVALID_DIMENSION = -100.0;
static constexpr uint32_t TEST_COLOR_YELLOW = 0x7fff00;
static constexpr uint32_t TEST_COLOR_RED = 0xff0000;
static constexpr uint32_t TEST_COLOR_BLUE = 0x0000ff;
static constexpr uint32_t TEST_COLOR_INVALID = 0x66000000;
static constexpr uint32_t TEST_COLOR_BLACK = 0x000000;
static constexpr uint32_t TEST_COLOR_WHITE = 0xffffff;
static constexpr uint32_t QUARTER = 4;
static constexpr double ZERO_OFFSET = 0.0F;
static double g_curScreenWidth = 0.0F;
static double g_curScreenHeight = 0.0F;
static double g_testWidth = 0.0F;
static double g_testHeight = 0.0F;

static bool GetScreenSize()
{
    sptr<OHOS::Rosen::Display> display =
        OHOS::Rosen::DisplayManager::GetInstance().GetDefaultDisplaySync();
    if (display == nullptr) {
        return false;
    }

    auto info = display->GetDisplayInfo();
    if (info == nullptr) {
        return false;
    }

    g_curScreenWidth = static_cast<double>(info->GetWidth());
    g_curScreenHeight = static_cast<double>(info->GetHeight());
    return true;
}

static void BuildLocationComponentInfo(nlohmann::json& jsonComponent)
{
    jsonComponent[JSON_SC_TYPE] = LOCATION_COMPONENT;
    jsonComponent[JSON_NODE_ID] = 0;
    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, g_testWidth },
        {JSON_RECT_Y, g_testHeight },
        {JSON_RECT_WIDTH, g_testWidth },
        {JSON_RECT_HEIGHT, g_testHeight }
    };
    jsonComponent[JSON_WINDOW_RECT] = nlohmann::json {
        { JSON_RECT_X, g_testWidth },
        { JSON_RECT_Y, g_testHeight },
        { JSON_RECT_WIDTH, g_testWidth },
        { JSON_RECT_HEIGHT, g_testHeight }
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
}

void SecCompInfoHelperTest::SetUpTestCase()
{
    ASSERT_TRUE(GetScreenSize());
    g_testWidth = (ZERO_OFFSET + g_curScreenWidth) / QUARTER;
    g_testHeight = (ZERO_OFFSET + g_curScreenHeight) / QUARTER;
}

void SecCompInfoHelperTest::TearDownTestCase()
{}

void SecCompInfoHelperTest::SetUp()
{
    SC_LOG_INFO(LABEL, "setup");
}

void SecCompInfoHelperTest::TearDown()
{}

/**
 * @tc.name: ParseComponent001
 * @tc.desc: Test parse component info success
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent001, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);
    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_NE(comp, nullptr);
    ASSERT_TRUE(comp->GetValid());
}

/**
 * @tc.name: ParseComponent002
 * @tc.desc: Test parse component info with empty json
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent002, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    ASSERT_EQ(SecCompInfoHelper::ParseComponent(UNKNOWN_SC_TYPE, jsonComponent), nullptr);

    ASSERT_EQ(SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent), nullptr);
    ASSERT_EQ(SecCompInfoHelper::ParseComponent(PASTE_COMPONENT, jsonComponent), nullptr);
    ASSERT_EQ(SecCompInfoHelper::ParseComponent(SAVE_COMPONENT, jsonComponent), nullptr);
}

/**
 * @tc.name: ParseComponent003
 * @tc.desc: Test parse component info with invalid type
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent003, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);

    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_TRUE(comp->GetValid());

    jsonComponent[JSON_SC_TYPE] = UNKNOWN_SC_TYPE;
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_EQ(comp, nullptr);
}

/**
 * @tc.name: ParseComponent004
 * @tc.desc: Test parse component info with invalid rect
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent004, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);
    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_TRUE(comp->GetValid());

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, TEST_INVALID_DIMENSION },
        {JSON_RECT_Y, g_testHeight },
        {JSON_RECT_WIDTH, g_testWidth },
        {JSON_RECT_HEIGHT, g_testHeight }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, g_testWidth },
        {JSON_RECT_Y, TEST_INVALID_DIMENSION },
        {JSON_RECT_WIDTH, g_testWidth },
        {JSON_RECT_HEIGHT, g_testHeight }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, g_curScreenWidth + 1 },
        {JSON_RECT_Y, g_testHeight },
        {JSON_RECT_WIDTH, g_testWidth },
        {JSON_RECT_HEIGHT, g_testHeight }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, g_testWidth },
        {JSON_RECT_Y, g_curScreenHeight + 1 },
        {JSON_RECT_WIDTH, g_testWidth },
        {JSON_RECT_HEIGHT, g_testHeight }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());
}

/**
 * @tc.name: ParseComponent005
 * @tc.desc: Test parse component info with invalid rect
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent005, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);
    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_TRUE(comp->GetValid());

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, g_testWidth },
        {JSON_RECT_Y, g_testHeight },
        {JSON_RECT_WIDTH, g_curScreenWidth },
        {JSON_RECT_HEIGHT, g_testHeight }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, g_testWidth },
        {JSON_RECT_Y, g_testHeight },
        {JSON_RECT_WIDTH, g_testWidth },
        {JSON_RECT_HEIGHT, g_curScreenHeight }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, g_curScreenWidth - g_testWidth },
        {JSON_RECT_Y, g_testHeight },
        {JSON_RECT_WIDTH, g_testWidth },
        {JSON_RECT_HEIGHT, g_testHeight }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, g_testWidth },
        {JSON_RECT_Y, g_curScreenHeight - g_testHeight },
        {JSON_RECT_WIDTH, g_testWidth },
        {JSON_RECT_HEIGHT, g_testHeight }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, g_testWidth },
        {JSON_RECT_Y, g_testHeight },
        {JSON_RECT_WIDTH, g_curScreenWidth },
        {JSON_RECT_HEIGHT, g_curScreenHeight }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());
}

/**
 * @tc.name: ParseComponent006
 * @tc.desc: Test parse component info with parentEffect active
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent006, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);

    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_TRUE(comp->GetValid());

    jsonComponent[JSON_PARENT_TAG] = nlohmann::json {
        { JSON_PARENT_EFFECT_TAG, true },
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());
}

/**
 * @tc.name: ParseComponent007
 * @tc.desc: Test parse component info with invalid size
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent007, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);

    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_TRUE(comp->GetValid());

    nlohmann::json jsonPadding = nlohmann::json {
        { JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JSON_PADDING_RIGHT_TAG, TEST_DIMENSION },
        { JSON_PADDING_BOTTOM_TAG, TEST_DIMENSION },
        { JSON_PADDING_LEFT_TAG, TEST_DIMENSION },
    };

    jsonComponent[JSON_SIZE_TAG] = nlohmann::json {
        { JSON_FONT_SIZE_TAG, TEST_INVALID_DIMENSION },
        { JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonPadding = nlohmann::json {
        { JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JSON_PADDING_RIGHT_TAG, TEST_DIMENSION },
        { JSON_PADDING_BOTTOM_TAG, TEST_DIMENSION },
        { JSON_PADDING_LEFT_TAG, TEST_DIMENSION },
    };

    jsonComponent[JSON_SIZE_TAG] = nlohmann::json {
        { JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JSON_ICON_SIZE_TAG, TEST_INVALID_DIMENSION },
        { JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonPadding = nlohmann::json {
        { JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JSON_PADDING_RIGHT_TAG, TEST_DIMENSION },
        { JSON_PADDING_BOTTOM_TAG, TEST_DIMENSION },
        { JSON_PADDING_LEFT_TAG, TEST_DIMENSION },
    };

    jsonComponent[JSON_SIZE_TAG] = nlohmann::json {
        { JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JSON_TEXT_ICON_PADDING_TAG, TEST_INVALID_DIMENSION },
        { JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());
}

/**
 * @tc.name: ParseComponent008
 * @tc.desc: Test parse component info with invalid size
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent008, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);
    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_TRUE(comp->GetValid());

    nlohmann::json jsonPadding = nlohmann::json {
        { JSON_PADDING_TOP_TAG, TEST_INVALID_DIMENSION },
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
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonPadding = nlohmann::json {
        { JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JSON_PADDING_RIGHT_TAG, TEST_INVALID_DIMENSION },
        { JSON_PADDING_BOTTOM_TAG, TEST_DIMENSION },
        { JSON_PADDING_LEFT_TAG, TEST_DIMENSION },
    };

    jsonComponent[JSON_SIZE_TAG] = nlohmann::json {
        { JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());
}

/**
 * @tc.name: ParseComponent009
 * @tc.desc: Test parse component info with invalid size
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent009, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);
    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_TRUE(comp->GetValid());

    nlohmann::json jsonPadding = nlohmann::json {
        { JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JSON_PADDING_RIGHT_TAG, TEST_DIMENSION },
        { JSON_PADDING_BOTTOM_TAG, TEST_INVALID_DIMENSION },
        { JSON_PADDING_LEFT_TAG, TEST_DIMENSION },
    };

    jsonComponent[JSON_SIZE_TAG] = nlohmann::json {
        { JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonPadding = nlohmann::json {
        { JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JSON_PADDING_RIGHT_TAG, TEST_DIMENSION },
        { JSON_PADDING_BOTTOM_TAG, TEST_DIMENSION },
        { JSON_PADDING_LEFT_TAG, TEST_INVALID_DIMENSION },
    };

    jsonComponent[JSON_SIZE_TAG] = nlohmann::json {
        { JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());
}

/**
 * @tc.name: ParseComponent010
 * @tc.desc: Test parse component info with invalid color
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent010, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);
    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_TRUE(comp->GetValid());

    jsonComponent[JSON_COLORS_TAG] = nlohmann::json {
        { JSON_FONT_COLOR_TAG, TEST_COLOR_INVALID },
        { JSON_ICON_COLOR_TAG, TEST_COLOR_BLUE },
        { JSON_BG_COLOR_TAG, TEST_COLOR_YELLOW }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_COLORS_TAG] = nlohmann::json {
        { JSON_FONT_COLOR_TAG, TEST_COLOR_RED },
        { JSON_ICON_COLOR_TAG, TEST_COLOR_INVALID },
        { JSON_BG_COLOR_TAG, TEST_COLOR_YELLOW }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_COLORS_TAG] = nlohmann::json {
        { JSON_FONT_COLOR_TAG, TEST_COLOR_RED },
        { JSON_ICON_COLOR_TAG, TEST_COLOR_BLUE },
        { JSON_BG_COLOR_TAG, TEST_COLOR_INVALID }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());
}

/**
 * @tc.name: ParseComponent011
 * @tc.desc: Test parse component info with invalid style
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent011, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);
    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_TRUE(comp->GetValid());

    jsonComponent[JSON_STYLE_TAG] = nlohmann::json {
        { JSON_TEXT_TAG, LocationDesc::NO_TEXT },
        { JSON_ICON_TAG, LocationIcon::NO_ICON },
        { JSON_BG_TAG, SecCompBackground::CIRCLE },
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());
}

/**
 * @tc.name: ParseComponent012
 * @tc.desc: Test parse component info with invalid style
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent012, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);
    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_TRUE(comp->GetValid());

    jsonComponent[JSON_STYLE_TAG] = nlohmann::json {
        { JSON_TEXT_TAG, LocationDesc::SELECT_LOCATION },
        { JSON_ICON_TAG, LocationIcon::LINE_ICON },
        { JSON_BG_TAG, SecCompBackground::CIRCLE },
    };
    jsonComponent[JSON_COLORS_TAG] = nlohmann::json {
        { JSON_FONT_COLOR_TAG, TEST_COLOR_YELLOW },
        { JSON_ICON_COLOR_TAG, TEST_COLOR_BLUE },
        { JSON_BG_COLOR_TAG, TEST_COLOR_YELLOW }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_STYLE_TAG] = nlohmann::json {
        { JSON_TEXT_TAG, LocationDesc::SELECT_LOCATION },
        { JSON_ICON_TAG, LocationIcon::LINE_ICON },
        { JSON_BG_TAG, SecCompBackground::CIRCLE },
    };
    jsonComponent[JSON_COLORS_TAG] = nlohmann::json {
        { JSON_FONT_COLOR_TAG, TEST_COLOR_RED },
        { JSON_ICON_COLOR_TAG, TEST_COLOR_YELLOW },
        { JSON_BG_COLOR_TAG, TEST_COLOR_YELLOW }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_STYLE_TAG] = nlohmann::json {
        { JSON_TEXT_TAG, LocationDesc::SELECT_LOCATION },
        { JSON_ICON_TAG, LocationIcon::LINE_ICON },
        { JSON_BG_TAG, SecCompBackground::NO_BG_TYPE },
    };
    jsonComponent[JSON_COLORS_TAG] = nlohmann::json {
        { JSON_FONT_COLOR_TAG, TEST_COLOR_RED },
        { JSON_ICON_COLOR_TAG, TEST_COLOR_YELLOW },
        { JSON_BG_COLOR_TAG, TEST_COLOR_YELLOW }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());
}

/**
 * @tc.name: ParseComponent013
 * @tc.desc: Test parse component info with invalid style
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent013, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);
    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_TRUE(comp->GetValid());

    jsonComponent[JSON_STYLE_TAG] = nlohmann::json {
        { JSON_TEXT_TAG, LocationDesc::SELECT_LOCATION },
        { JSON_ICON_TAG, LocationIcon::LINE_ICON },
        { JSON_BG_TAG, SecCompBackground::NO_BG_TYPE },
    };
    nlohmann::json jsonPadding = nlohmann::json {
        { JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JSON_PADDING_RIGHT_TAG, MIN_PADDING_WITHOUT_BG },
        { JSON_PADDING_BOTTOM_TAG, MIN_PADDING_WITHOUT_BG },
        { JSON_PADDING_LEFT_TAG, MIN_PADDING_WITHOUT_BG },
    };

    jsonComponent[JSON_SIZE_TAG] = nlohmann::json {
        { JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonPadding = nlohmann::json {
        { JSON_PADDING_TOP_TAG, MIN_PADDING_WITHOUT_BG },
        { JSON_PADDING_RIGHT_TAG, TEST_DIMENSION },
        { JSON_PADDING_BOTTOM_TAG, MIN_PADDING_WITHOUT_BG },
        { JSON_PADDING_LEFT_TAG, MIN_PADDING_WITHOUT_BG },
    };

    jsonComponent[JSON_SIZE_TAG] = nlohmann::json {
        { JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());
}

/**
 * @tc.name: ParseComponent014
 * @tc.desc: Test parse component info with invalid style
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent014, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);
    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_TRUE(comp->GetValid());

    jsonComponent[JSON_STYLE_TAG] = nlohmann::json {
        { JSON_TEXT_TAG, LocationDesc::SELECT_LOCATION },
        { JSON_ICON_TAG, LocationIcon::LINE_ICON },
        { JSON_BG_TAG, SecCompBackground::NO_BG_TYPE },
    };

    nlohmann::json jsonPadding = nlohmann::json {
        { JSON_PADDING_TOP_TAG, MIN_PADDING_WITHOUT_BG },
        { JSON_PADDING_RIGHT_TAG, MIN_PADDING_WITHOUT_BG },
        { JSON_PADDING_BOTTOM_TAG, TEST_DIMENSION },
        { JSON_PADDING_LEFT_TAG, MIN_PADDING_WITHOUT_BG },
    };

    jsonComponent[JSON_SIZE_TAG] = nlohmann::json {
        { JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonPadding = nlohmann::json {
        { JSON_PADDING_TOP_TAG, MIN_PADDING_WITHOUT_BG },
        { JSON_PADDING_RIGHT_TAG, MIN_PADDING_WITHOUT_BG },
        { JSON_PADDING_BOTTOM_TAG, MIN_PADDING_WITHOUT_BG },
        { JSON_PADDING_LEFT_TAG, TEST_DIMENSION },
    };

    jsonComponent[JSON_SIZE_TAG] = nlohmann::json {
        { JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());
}

/**
 * @tc.name: ParseComponent015
 * @tc.desc: Test parse component info with invalid rect
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent015, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);
    jsonComponent[JSON_RECT] = nlohmann::json {
        { JSON_RECT_X, g_testWidth },
        { JSON_RECT_Y, g_testHeight },
        { JSON_RECT_WIDTH, TEST_INVALID_DIMENSION },
        { JSON_RECT_HEIGHT, g_testHeight }
    };
    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_NE(comp, nullptr);
    EXPECT_FALSE(comp->GetValid());
    delete comp;

    jsonComponent[JSON_RECT] = nlohmann::json {
        { JSON_RECT_X, g_testWidth },
        { JSON_RECT_Y, g_testHeight },
        { JSON_RECT_WIDTH, g_testWidth },
        { JSON_RECT_HEIGHT, TEST_INVALID_DIMENSION }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_NE(comp, nullptr);
    EXPECT_FALSE(comp->GetValid());
    delete comp;
}

/**
 * @tc.name: ParseComponent016
 * @tc.desc: Test parse component info with windowRect invalid
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent016, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);
    jsonComponent[JSON_WINDOW_RECT] = nlohmann::json {
        { JSON_RECT_X, g_testWidth },
        { JSON_RECT_Y, g_testHeight + 1 },
        { JSON_RECT_WIDTH, g_testWidth },
        { JSON_RECT_HEIGHT, g_testHeight }
    };
    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_NE(comp, nullptr);
    EXPECT_FALSE(comp->GetValid());
    delete comp;

    jsonComponent[JSON_WINDOW_RECT] = nlohmann::json {
        { JSON_RECT_X, g_testWidth + 1 },
        { JSON_RECT_Y, g_testHeight },
        { JSON_RECT_WIDTH, g_testWidth },
        { JSON_RECT_HEIGHT, g_testHeight }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_NE(comp, nullptr);
    EXPECT_FALSE(comp->GetValid());
    delete comp;

    jsonComponent[JSON_WINDOW_RECT] = nlohmann::json {
        { JSON_RECT_X, g_testWidth },
        { JSON_RECT_Y, g_testHeight },
        { JSON_RECT_WIDTH, g_testWidth - 1 },
        { JSON_RECT_HEIGHT, g_testHeight }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_NE(comp, nullptr);
    EXPECT_FALSE(comp->GetValid());
    delete comp;

    jsonComponent[JSON_WINDOW_RECT] = nlohmann::json {
        { JSON_RECT_X, g_testWidth },
        { JSON_RECT_Y, g_testHeight },
        { JSON_RECT_WIDTH, g_testWidth },
        { JSON_RECT_HEIGHT, g_testHeight - 1 }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_NE(comp, nullptr);
    EXPECT_FALSE(comp->GetValid());
    delete comp;
}

/**
 * @tc.name: CheckComponentValid001
 * @tc.desc: Test CheckComponentValid with invalid color
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompInfoHelperTest, CheckComponentValid001, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);
    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_TRUE(comp->GetValid());
    jsonComponent[JSON_COLORS_TAG] = nlohmann::json {
        { JSON_FONT_COLOR_TAG, TEST_COLOR_RED },
        { JSON_ICON_COLOR_TAG, TEST_COLOR_BLACK },
        { JSON_BG_COLOR_TAG, TEST_COLOR_WHITE }
    };
    ASSERT_TRUE(SecCompInfoHelper::CheckComponentValid(comp));
}

/**
 * @tc.name: CheckComponentValid002
 * @tc.desc: Test CheckComponentValid with invalid text or icon
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompInfoHelperTest, CheckComponentValid002, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);
    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    comp->text_ = static_cast<int32_t>(LocationDesc::UNKNOWN_TEXT);
    ASSERT_TRUE(SecCompInfoHelper::CheckComponentValid(comp));

    comp->text_ = static_cast<int32_t>(LocationDesc::SELECT_LOCATION);
    comp->icon_ = static_cast<int32_t>(LocationIcon::UNKNOWN_ICON);
    ASSERT_TRUE(SecCompInfoHelper::CheckComponentValid(comp));

    comp->text_ = static_cast<int32_t>(LocationDesc::UNKNOWN_TEXT);
    ASSERT_FALSE(SecCompInfoHelper::CheckComponentValid(comp));
}

/**
 * @tc.name: CheckComponentValid003
 * @tc.desc: Test CheckComponentValid with invalid type
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompInfoHelperTest, CheckComponentValid003, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);
    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    comp->type_ = SecCompType::UNKNOWN_SC_TYPE;
    ASSERT_FALSE(SecCompInfoHelper::CheckComponentValid(comp));

    comp->type_ = SecCompType::MAX_SC_TYPE;
    ASSERT_FALSE(SecCompInfoHelper::CheckComponentValid(comp));
}
