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

static const std::string JSON_RECT = "rect";
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
static const std::string JSON_TEXT_ICON_PADDING_TAG = "textIconPadding";
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
static constexpr uint32_t TEST_COLOR_1 = 0x7fff00;
static constexpr uint32_t TEST_COLOR_2 = 0xff0000;
static constexpr uint32_t TEST_COLOR_3 = 0x0000ff;
static constexpr uint32_t TEST_COLOR_INVALID = 0x66000000;
static constexpr double ZERO_OFFSET = 0.0F;
static double CUR_SCREEN_WIDTH = 0.0F;
static double CUR_SCREEN_HEIGHT = 0.0F;
static double TEST_WIDTH = 0.0F;
static double TEST_HEIGHT = 0.0F;
static uint32_t QUARTER = 4;
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

    CUR_SCREEN_WIDTH = static_cast<double>(info->GetWidth());
    CUR_SCREEN_HEIGHT = static_cast<double>(info->GetHeight());
    return true;
}

static void BuildLocationComponentInfo(nlohmann::json& jsonComponent)
{
    jsonComponent[JSON_SC_TYPE] = LOCATION_COMPONENT;
    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, TEST_WIDTH },
        {JSON_RECT_Y, TEST_HEIGHT },
        {JSON_RECT_WIDTH, TEST_WIDTH },
        {JSON_RECT_HEIGHT, TEST_HEIGHT }
    };
    nlohmann::json jsonPadding = nlohmann::json {
        { JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JSON_PADDING_RIGHT_TAG, TEST_DIMENSION },
        { JSON_PADDING_BOTTOM_TAG,TEST_DIMENSION },
        { JSON_PADDING_LEFT_TAG, TEST_DIMENSION },
    };

    jsonComponent[JSON_SIZE_TAG] = nlohmann::json {
        { JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JSON_PADDING_SIZE_TAG, jsonPadding },
    };

    jsonComponent[JSON_COLORS_TAG] = nlohmann::json {
        { JSON_FONT_COLOR_TAG, TEST_COLOR_2 },
        { JSON_ICON_COLOR_TAG, TEST_COLOR_3 },
        { JSON_BG_COLOR_TAG, TEST_COLOR_1 }
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
        { JSON_BG_TAG, LocationBackground::CIRCLE },
    };
}
}

void SecCompInfoHelperTest::SetUpTestCase()
{
    ASSERT_TRUE(GetScreenSize());
    TEST_WIDTH = (ZERO_OFFSET + CUR_SCREEN_WIDTH) / QUARTER;
    TEST_HEIGHT = (ZERO_OFFSET + CUR_SCREEN_HEIGHT) / QUARTER;
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
 * @tc.require:
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
 * @tc.require:
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
 * @tc.require:
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent003, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);

    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_TRUE(comp->GetValid());

    jsonComponent[JSON_SC_TYPE] = UNKNOWN_SC_TYPE;
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());
}

/**
 * @tc.name: ParseComponent004
 * @tc.desc: Test parse component info with invalid rect
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent004, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);

    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_TRUE(comp->GetValid());

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, TEST_INVALID_DIMENSION },
        {JSON_RECT_Y, TEST_HEIGHT },
        {JSON_RECT_WIDTH, TEST_WIDTH },
        {JSON_RECT_HEIGHT, TEST_HEIGHT }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, TEST_WIDTH },
        {JSON_RECT_Y, TEST_INVALID_DIMENSION },
        {JSON_RECT_WIDTH, TEST_WIDTH },
        {JSON_RECT_HEIGHT, TEST_HEIGHT }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, CUR_SCREEN_WIDTH },
        {JSON_RECT_Y, TEST_HEIGHT },
        {JSON_RECT_WIDTH, TEST_WIDTH },
        {JSON_RECT_HEIGHT, TEST_HEIGHT }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, TEST_WIDTH },
        {JSON_RECT_Y, CUR_SCREEN_HEIGHT },
        {JSON_RECT_WIDTH, TEST_WIDTH },
        {JSON_RECT_HEIGHT, TEST_HEIGHT }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, TEST_WIDTH },
        {JSON_RECT_Y, TEST_HEIGHT },
        {JSON_RECT_WIDTH, CUR_SCREEN_WIDTH },
        {JSON_RECT_HEIGHT, TEST_HEIGHT }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, TEST_WIDTH },
        {JSON_RECT_Y, TEST_HEIGHT },
        {JSON_RECT_WIDTH, TEST_WIDTH },
        {JSON_RECT_HEIGHT, CUR_SCREEN_HEIGHT }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, CUR_SCREEN_WIDTH - TEST_WIDTH },
        {JSON_RECT_Y, TEST_HEIGHT },
        {JSON_RECT_WIDTH, TEST_WIDTH },
        {JSON_RECT_HEIGHT, TEST_HEIGHT }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, TEST_WIDTH },
        {JSON_RECT_Y, CUR_SCREEN_HEIGHT - TEST_HEIGHT },
        {JSON_RECT_WIDTH, TEST_WIDTH },
        {JSON_RECT_HEIGHT, TEST_HEIGHT }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, TEST_WIDTH },
        {JSON_RECT_Y, TEST_HEIGHT },
        {JSON_RECT_WIDTH, CUR_SCREEN_WIDTH * CUR_SCREEN_HEIGHT },
        {JSON_RECT_HEIGHT, CUR_SCREEN_WIDTH * CUR_SCREEN_HEIGHT }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());
}

/**
 * @tc.name: ParseComponent005
 * @tc.desc: Test parse component info with parentEffect active
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent005, TestSize.Level1)
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
 * @tc.name: ParseComponent006
 * @tc.desc: Test parse component info with invalid size
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent006, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);

    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_TRUE(comp->GetValid());

    nlohmann::json jsonPadding = nlohmann::json {
        { JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JSON_PADDING_RIGHT_TAG, TEST_DIMENSION },
        { JSON_PADDING_BOTTOM_TAG,TEST_DIMENSION },
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
        { JSON_PADDING_BOTTOM_TAG,TEST_DIMENSION },
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
        { JSON_PADDING_BOTTOM_TAG,TEST_DIMENSION },
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

    jsonPadding = nlohmann::json {
        { JSON_PADDING_TOP_TAG, TEST_INVALID_DIMENSION },
        { JSON_PADDING_RIGHT_TAG, TEST_DIMENSION },
        { JSON_PADDING_BOTTOM_TAG,TEST_DIMENSION },
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
        { JSON_PADDING_BOTTOM_TAG,TEST_DIMENSION },
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
        { JSON_PADDING_BOTTOM_TAG,TEST_INVALID_DIMENSION },
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
        { JSON_PADDING_BOTTOM_TAG,TEST_DIMENSION },
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
 * @tc.name: ParseComponent007
 * @tc.desc: Test parse component info with invalid color
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent007, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);

    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_TRUE(comp->GetValid());

    jsonComponent[JSON_COLORS_TAG] = nlohmann::json {
        { JSON_FONT_COLOR_TAG, TEST_COLOR_INVALID },
        { JSON_ICON_COLOR_TAG, TEST_COLOR_3 },
        { JSON_BG_COLOR_TAG, TEST_COLOR_1 }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_COLORS_TAG] = nlohmann::json {
        { JSON_FONT_COLOR_TAG, TEST_COLOR_2 },
        { JSON_ICON_COLOR_TAG, TEST_COLOR_INVALID },
        { JSON_BG_COLOR_TAG, TEST_COLOR_1 }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_COLORS_TAG] = nlohmann::json {
        { JSON_FONT_COLOR_TAG, TEST_COLOR_2 },
        { JSON_ICON_COLOR_TAG, TEST_COLOR_3 },
        { JSON_BG_COLOR_TAG, TEST_COLOR_INVALID }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());
}

/**
 * @tc.name: ParseComponent008
 * @tc.desc: Test parse component info with invalid style
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompInfoHelperTest, ParseComponent008, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);

    SecCompBase* comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_TRUE(comp->GetValid());

    jsonComponent[JSON_STYLE_TAG] = nlohmann::json {
        { JSON_TEXT_TAG, LocationDesc::NO_TEXT },
        { JSON_ICON_TAG, LocationIcon::NO_ICON },
        { JSON_BG_TAG, LocationBackground::CIRCLE },
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_STYLE_TAG] = nlohmann::json {
        { JSON_TEXT_TAG, LocationDesc::SELECT_LOCATION },
        { JSON_ICON_TAG, LocationIcon::LINE_ICON },
        { JSON_BG_TAG, LocationBackground::CIRCLE },
    };
    jsonComponent[JSON_COLORS_TAG] = nlohmann::json {
        { JSON_FONT_COLOR_TAG, TEST_COLOR_1 },
        { JSON_ICON_COLOR_TAG, TEST_COLOR_3 },
        { JSON_BG_COLOR_TAG, TEST_COLOR_1 }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_STYLE_TAG] = nlohmann::json {
        { JSON_TEXT_TAG, LocationDesc::SELECT_LOCATION },
        { JSON_ICON_TAG, LocationIcon::LINE_ICON },
        { JSON_BG_TAG, LocationBackground::CIRCLE },
    };
    jsonComponent[JSON_COLORS_TAG] = nlohmann::json {
        { JSON_FONT_COLOR_TAG, TEST_COLOR_2 },
        { JSON_ICON_COLOR_TAG, TEST_COLOR_1 },
        { JSON_BG_COLOR_TAG, TEST_COLOR_1 }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_STYLE_TAG] = nlohmann::json {
        { JSON_TEXT_TAG, LocationDesc::SELECT_LOCATION },
        { JSON_ICON_TAG, LocationIcon::LINE_ICON },
        { JSON_BG_TAG, LocationBackground::NO_BG_TYPE },
    };
    jsonComponent[JSON_COLORS_TAG] = nlohmann::json {
        { JSON_FONT_COLOR_TAG, TEST_COLOR_2 },
        { JSON_ICON_COLOR_TAG, TEST_COLOR_1 },
        { JSON_BG_COLOR_TAG, TEST_COLOR_1 }
    };
    comp = SecCompInfoHelper::ParseComponent(LOCATION_COMPONENT, jsonComponent);
    ASSERT_FALSE(comp->GetValid());

    jsonComponent[JSON_STYLE_TAG] = nlohmann::json {
        { JSON_TEXT_TAG, LocationDesc::SELECT_LOCATION },
        { JSON_ICON_TAG, LocationIcon::LINE_ICON },
        { JSON_BG_TAG, LocationBackground::NO_BG_TYPE },
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

    jsonPadding = nlohmann::json {
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
