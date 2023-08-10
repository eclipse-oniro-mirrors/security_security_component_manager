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

static constexpr float TEST_SIZE = 100.0;
static constexpr double TEST_DIMENSION = 100.0;
static constexpr double TEST_INVALID_DIMENSION = -100.0;
static constexpr uint32_t TEST_COLOR_YELLOW = 0xff7fff00;
static constexpr uint32_t TEST_COLOR_RED = 0xffff0000;
static constexpr uint32_t TEST_COLOR_BLUE = 0xff0000ff;
static constexpr uint32_t TEST_COLOR_INVALID = 0x66000000;
static constexpr uint32_t TEST_COLOR_BLACK = 0x00000000;
static constexpr uint32_t TEST_COLOR_WHITE = 0xffffffff;
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
    jsonComponent[JsonTagConstants::JSON_SC_TYPE] = LOCATION_COMPONENT;
    jsonComponent[JsonTagConstants::JSON_NODE_ID] = 0;
    jsonComponent[JsonTagConstants::JSON_RECT] =  nlohmann::json {
        {JsonTagConstants::JSON_RECT_X, g_testWidth },
        {JsonTagConstants::JSON_RECT_Y, g_testHeight },
        {JsonTagConstants::JSON_RECT_WIDTH, g_testWidth },
        {JsonTagConstants::JSON_RECT_HEIGHT, g_testHeight }
    };
    jsonComponent[JsonTagConstants::JSON_WINDOW_RECT] = nlohmann::json {
        { JsonTagConstants::JSON_RECT_X, g_testWidth },
        { JsonTagConstants::JSON_RECT_Y, g_testHeight },
        { JsonTagConstants::JSON_RECT_WIDTH, g_testWidth },
        { JsonTagConstants::JSON_RECT_HEIGHT, g_testHeight }
    };
    nlohmann::json jsonPadding = nlohmann::json {
        { JsonTagConstants::JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_RIGHT_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_BOTTOM_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_LEFT_TAG, TEST_DIMENSION },
    };

    jsonComponent[JsonTagConstants::JSON_SIZE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_PADDING_SIZE_TAG, jsonPadding },
    };

    jsonComponent[JsonTagConstants::JSON_COLORS_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_FONT_COLOR_TAG, TEST_COLOR_RED },
        { JsonTagConstants::JSON_ICON_COLOR_TAG, TEST_COLOR_BLUE },
        { JsonTagConstants::JSON_BG_COLOR_TAG, TEST_COLOR_YELLOW }
    };

    jsonComponent[JsonTagConstants::JSON_BORDER_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_BORDER_WIDTH_TAG, TEST_SIZE },
    };
    jsonComponent[JsonTagConstants::JSON_PARENT_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_PARENT_EFFECT_TAG, false },
    };
    jsonComponent[JsonTagConstants::JSON_STYLE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_TEXT_TAG, LocationDesc::SELECT_LOCATION },
        { JsonTagConstants::JSON_ICON_TAG, LocationIcon::LINE_ICON },
        { JsonTagConstants::JSON_BG_TAG, SecCompBackground::CIRCLE },
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

    jsonComponent[JsonTagConstants::JSON_SC_TYPE] = UNKNOWN_SC_TYPE;
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
    SecCompRect rect = {
        .x_ = g_testWidth,
        .y_ = g_testHeight,
        .width_ = g_testWidth,
        .height_ = g_testHeight
    };
    SecCompRect windowRect = {
        .x_ = g_testWidth,
        .y_ = g_testHeight,
        .width_ = g_testWidth,
        .height_ = g_testHeight
    };
    ASSERT_TRUE(SecCompInfoHelper::CheckRectValid(rect, windowRect));

    rect.x_ = TEST_INVALID_DIMENSION;
    ASSERT_FALSE(SecCompInfoHelper::CheckRectValid(rect, windowRect));
    rect.y_ = TEST_INVALID_DIMENSION;
    ASSERT_FALSE(SecCompInfoHelper::CheckRectValid(rect, windowRect));
    rect.x_ = g_curScreenWidth + 1;
    ASSERT_FALSE(SecCompInfoHelper::CheckRectValid(rect, windowRect));
    rect.y_ = g_curScreenHeight + 1;
    ASSERT_FALSE(SecCompInfoHelper::CheckRectValid(rect, windowRect));
    rect.width_ = g_curScreenWidth;
    ASSERT_FALSE(SecCompInfoHelper::CheckRectValid(rect, windowRect));
    rect.height_ = g_curScreenHeight;
    ASSERT_FALSE(SecCompInfoHelper::CheckRectValid(rect, windowRect));
    rect.x_ = g_curScreenWidth - g_testWidth;
    ASSERT_FALSE(SecCompInfoHelper::CheckRectValid(rect, windowRect));
    rect.y_ = g_curScreenHeight - g_testHeight;
    ASSERT_FALSE(SecCompInfoHelper::CheckRectValid(rect, windowRect));
}

/**
 * @tc.name: ParseComponent005
 * @tc.desc: Test parse component info with windowRect invalid
 * @tc.type: FUNC
 * @tc.require: AR000HO9J7
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent005, TestSize.Level1)
{
    SecCompRect rect = {
        .x_ = g_testWidth,
        .y_ = g_testHeight,
        .width_ = g_testWidth,
        .height_ = g_testHeight
    };
    SecCompRect windowRect = {
        .x_ = g_testWidth,
        .y_ = g_testHeight,
        .width_ = g_testWidth,
        .height_ = g_testHeight
    };
    ASSERT_TRUE(SecCompInfoHelper::CheckRectValid(rect, windowRect));

    windowRect.x_ = g_testWidth + 1;
    ASSERT_FALSE(SecCompInfoHelper::CheckRectValid(rect, windowRect));
    windowRect.y_ = g_testHeight + 1;
    ASSERT_FALSE(SecCompInfoHelper::CheckRectValid(rect, windowRect));
    windowRect.width_ = g_testWidth - 1;
    ASSERT_FALSE(SecCompInfoHelper::CheckRectValid(rect, windowRect));
    windowRect.height_ = g_testHeight - 1;
    ASSERT_FALSE(SecCompInfoHelper::CheckRectValid(rect, windowRect));
    windowRect.width_ = TEST_INVALID_DIMENSION;
    ASSERT_FALSE(SecCompInfoHelper::CheckRectValid(rect, windowRect));
    windowRect.height_ = TEST_INVALID_DIMENSION;
    ASSERT_FALSE(SecCompInfoHelper::CheckRectValid(rect, windowRect));
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

    jsonComponent[JsonTagConstants::JSON_PARENT_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_PARENT_EFFECT_TAG, true },
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
        { JsonTagConstants::JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_RIGHT_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_BOTTOM_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_LEFT_TAG, TEST_DIMENSION },
    };

    jsonComponent[JsonTagConstants::JSON_SIZE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_FONT_SIZE_TAG, TEST_INVALID_DIMENSION },
        { JsonTagConstants::JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonPadding = nlohmann::json {
        { JsonTagConstants::JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_RIGHT_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_BOTTOM_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_LEFT_TAG, TEST_DIMENSION },
    };

    jsonComponent[JsonTagConstants::JSON_SIZE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_ICON_SIZE_TAG, TEST_INVALID_DIMENSION },
        { JsonTagConstants::JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonPadding = nlohmann::json {
        { JsonTagConstants::JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_RIGHT_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_BOTTOM_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_LEFT_TAG, TEST_DIMENSION },
    };

    jsonComponent[JsonTagConstants::JSON_SIZE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_TEXT_ICON_PADDING_TAG, TEST_INVALID_DIMENSION },
        { JsonTagConstants::JSON_PADDING_SIZE_TAG, jsonPadding },
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
        { JsonTagConstants::JSON_PADDING_TOP_TAG, TEST_INVALID_DIMENSION },
        { JsonTagConstants::JSON_PADDING_RIGHT_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_BOTTOM_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_LEFT_TAG, TEST_DIMENSION },
    };

    jsonComponent[JsonTagConstants::JSON_SIZE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonPadding = nlohmann::json {
        { JsonTagConstants::JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_RIGHT_TAG, TEST_INVALID_DIMENSION },
        { JsonTagConstants::JSON_PADDING_BOTTOM_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_LEFT_TAG, TEST_DIMENSION },
    };

    jsonComponent[JsonTagConstants::JSON_SIZE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_PADDING_SIZE_TAG, jsonPadding },
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
        { JsonTagConstants::JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_RIGHT_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_BOTTOM_TAG, TEST_INVALID_DIMENSION },
        { JsonTagConstants::JSON_PADDING_LEFT_TAG, TEST_DIMENSION },
    };

    jsonComponent[JsonTagConstants::JSON_SIZE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonPadding = nlohmann::json {
        { JsonTagConstants::JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_RIGHT_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_BOTTOM_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_LEFT_TAG, TEST_INVALID_DIMENSION },
    };

    jsonComponent[JsonTagConstants::JSON_SIZE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_PADDING_SIZE_TAG, jsonPadding },
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

    jsonComponent[JsonTagConstants::JSON_COLORS_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_FONT_COLOR_TAG, TEST_COLOR_INVALID },
        { JsonTagConstants::JSON_ICON_COLOR_TAG, TEST_COLOR_BLUE },
        { JsonTagConstants::JSON_BG_COLOR_TAG, TEST_COLOR_YELLOW }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JsonTagConstants::JSON_COLORS_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_FONT_COLOR_TAG, TEST_COLOR_RED },
        { JsonTagConstants::JSON_ICON_COLOR_TAG, TEST_COLOR_INVALID },
        { JsonTagConstants::JSON_BG_COLOR_TAG, TEST_COLOR_YELLOW }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JsonTagConstants::JSON_COLORS_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_FONT_COLOR_TAG, TEST_COLOR_RED },
        { JsonTagConstants::JSON_ICON_COLOR_TAG, TEST_COLOR_BLUE },
        { JsonTagConstants::JSON_BG_COLOR_TAG, TEST_COLOR_INVALID }
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

    jsonComponent[JsonTagConstants::JSON_STYLE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_TEXT_TAG, NO_TEXT },
        { JsonTagConstants::JSON_ICON_TAG, NO_ICON },
        { JsonTagConstants::JSON_BG_TAG, SecCompBackground::CIRCLE },
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

    jsonComponent[JsonTagConstants::JSON_STYLE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_TEXT_TAG, LocationDesc::SELECT_LOCATION },
        { JsonTagConstants::JSON_ICON_TAG, LocationIcon::LINE_ICON },
        { JsonTagConstants::JSON_BG_TAG, SecCompBackground::CIRCLE },
    };
    jsonComponent[JsonTagConstants::JSON_COLORS_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_FONT_COLOR_TAG, TEST_COLOR_YELLOW },
        { JsonTagConstants::JSON_ICON_COLOR_TAG, TEST_COLOR_BLUE },
        { JsonTagConstants::JSON_BG_COLOR_TAG, TEST_COLOR_YELLOW }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JsonTagConstants::JSON_STYLE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_TEXT_TAG, LocationDesc::SELECT_LOCATION },
        { JsonTagConstants::JSON_ICON_TAG, LocationIcon::LINE_ICON },
        { JsonTagConstants::JSON_BG_TAG, SecCompBackground::CIRCLE },
    };
    jsonComponent[JsonTagConstants::JSON_COLORS_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_FONT_COLOR_TAG, TEST_COLOR_RED },
        { JsonTagConstants::JSON_ICON_COLOR_TAG, TEST_COLOR_YELLOW },
        { JsonTagConstants::JSON_BG_COLOR_TAG, TEST_COLOR_YELLOW }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JsonTagConstants::JSON_STYLE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_TEXT_TAG, LocationDesc::SELECT_LOCATION },
        { JsonTagConstants::JSON_ICON_TAG, LocationIcon::LINE_ICON },
        { JsonTagConstants::JSON_BG_TAG, SecCompBackground::NO_BG_TYPE },
    };
    jsonComponent[JsonTagConstants::JSON_COLORS_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_FONT_COLOR_TAG, TEST_COLOR_RED },
        { JsonTagConstants::JSON_ICON_COLOR_TAG, TEST_COLOR_YELLOW },
        { JsonTagConstants::JSON_BG_COLOR_TAG, TEST_COLOR_YELLOW }
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

    jsonComponent[JsonTagConstants::JSON_STYLE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_TEXT_TAG, LocationDesc::SELECT_LOCATION },
        { JsonTagConstants::JSON_ICON_TAG, LocationIcon::LINE_ICON },
        { JsonTagConstants::JSON_BG_TAG, SecCompBackground::NO_BG_TYPE },
    };
    nlohmann::json jsonPadding = nlohmann::json {
        { JsonTagConstants::JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_RIGHT_TAG, MIN_PADDING_WITHOUT_BG },
        { JsonTagConstants::JSON_PADDING_BOTTOM_TAG, MIN_PADDING_WITHOUT_BG },
        { JsonTagConstants::JSON_PADDING_LEFT_TAG, MIN_PADDING_WITHOUT_BG },
    };

    jsonComponent[JsonTagConstants::JSON_SIZE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonPadding = nlohmann::json {
        { JsonTagConstants::JSON_PADDING_TOP_TAG, MIN_PADDING_WITHOUT_BG },
        { JsonTagConstants::JSON_PADDING_RIGHT_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_BOTTOM_TAG, MIN_PADDING_WITHOUT_BG },
        { JsonTagConstants::JSON_PADDING_LEFT_TAG, MIN_PADDING_WITHOUT_BG },
    };

    jsonComponent[JsonTagConstants::JSON_SIZE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_PADDING_SIZE_TAG, jsonPadding },
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

    jsonComponent[JsonTagConstants::JSON_STYLE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_TEXT_TAG, LocationDesc::SELECT_LOCATION },
        { JsonTagConstants::JSON_ICON_TAG, LocationIcon::LINE_ICON },
        { JsonTagConstants::JSON_BG_TAG, SecCompBackground::NO_BG_TYPE },
    };

    nlohmann::json jsonPadding = nlohmann::json {
        { JsonTagConstants::JSON_PADDING_TOP_TAG, MIN_PADDING_WITHOUT_BG },
        { JsonTagConstants::JSON_PADDING_RIGHT_TAG, MIN_PADDING_WITHOUT_BG },
        { JsonTagConstants::JSON_PADDING_BOTTOM_TAG, TEST_DIMENSION },
        { JsonTagConstants::JSON_PADDING_LEFT_TAG, MIN_PADDING_WITHOUT_BG },
    };

    jsonComponent[JsonTagConstants::JSON_SIZE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonPadding = nlohmann::json {
        { JsonTagConstants::JSON_PADDING_TOP_TAG, MIN_PADDING_WITHOUT_BG },
        { JsonTagConstants::JSON_PADDING_RIGHT_TAG, MIN_PADDING_WITHOUT_BG },
        { JsonTagConstants::JSON_PADDING_BOTTOM_TAG, MIN_PADDING_WITHOUT_BG },
        { JsonTagConstants::JSON_PADDING_LEFT_TAG, TEST_DIMENSION },
    };

    jsonComponent[JsonTagConstants::JSON_SIZE_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JsonTagConstants::JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());
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
    jsonComponent[JsonTagConstants::JSON_COLORS_TAG] = nlohmann::json {
        { JsonTagConstants::JSON_FONT_COLOR_TAG, TEST_COLOR_RED },
        { JsonTagConstants::JSON_ICON_COLOR_TAG, TEST_COLOR_BLACK },
        { JsonTagConstants::JSON_BG_COLOR_TAG, TEST_COLOR_WHITE }
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
    comp->text_ = UNKNOWN_TEXT;
    ASSERT_TRUE(SecCompInfoHelper::CheckComponentValid(comp));

    comp->text_ = static_cast<int32_t>(LocationDesc::SELECT_LOCATION);
    comp->icon_ = UNKNOWN_ICON;
    ASSERT_TRUE(SecCompInfoHelper::CheckComponentValid(comp));

    comp->text_ = UNKNOWN_TEXT;
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
