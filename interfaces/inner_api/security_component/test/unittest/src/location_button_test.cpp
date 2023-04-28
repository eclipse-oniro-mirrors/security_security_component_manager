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
#include "location_button_test.h"
#include <string>
#include "sec_comp_log.h"
#include "sec_comp_err.h"

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::Security::SecurityComponent;

namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "LocationButtonTest"};

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

static const std::string WRONG_TYPE = "wrongType";
static constexpr float TEST_SIZE = 100.0;
static constexpr double TEST_COORDINATE = 100.0;
static constexpr double TEST_DIMENSION = 100.0;
static constexpr uint32_t TEST_COLOR_1 = 0x7fff00;
static constexpr uint32_t TEST_COLOR_2 = 0xff0000;
static constexpr uint32_t TEST_COLOR_3 = 0x0000ff;

static void BuildLocationComponentInfo(nlohmann::json& jsonComponent)
{
    jsonComponent[JSON_SC_TYPE] = LOCATION_COMPONENT;
    jsonComponent[JSON_RECT] =  nlohmann::json {
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

void LocationButtonTest::SetUpTestCase()
{}

void LocationButtonTest::TearDownTestCase()
{}

void LocationButtonTest::SetUp()
{
    SC_LOG_INFO(LABEL, "setup");
}

void LocationButtonTest::TearDown()
{}

/**
 * @tc.name: FromJson001
 * @tc.desc: Test location button from json success
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(LocationButtonTest, FromJson001, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);
    LocationButton button;
    ASSERT_TRUE(button.FromJson(jsonComponent));
}

/**
 * @tc.name: FromJson002
 * @tc.desc: Test empty location button from json
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(LocationButtonTest, FromJson002, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    LocationButton button;
    ASSERT_FALSE(button.FromJson(jsonComponent));
}

/**
 * @tc.name: FromJson003
 * @tc.desc: Test location button from wrong type params json
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(LocationButtonTest, FromJson003, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);
    LocationButton button;
    ASSERT_TRUE(button.FromJson(jsonComponent));

    jsonComponent[JSON_STYLE_TAG] = nlohmann::json {
        { JSON_TEXT_TAG, WRONG_TYPE },
        { JSON_ICON_TAG, LocationIcon::LINE_ICON },
        { JSON_BG_TAG, LocationBackground::CIRCLE },
    };
    ASSERT_FALSE(button.FromJson(jsonComponent));

    jsonComponent[JSON_STYLE_TAG] = nlohmann::json {
        { JSON_TEXT_TAG, LocationDesc::SELECT_LOCATION },
        { JSON_ICON_TAG, WRONG_TYPE },
        { JSON_BG_TAG, LocationBackground::CIRCLE },
    };
    ASSERT_FALSE(button.FromJson(jsonComponent));

    jsonComponent[JSON_STYLE_TAG] = nlohmann::json {
        { JSON_TEXT_TAG, LocationDesc::SELECT_LOCATION },
        { JSON_ICON_TAG, LocationIcon::LINE_ICON },
        { JSON_BG_TAG, WRONG_TYPE },
    };
    ASSERT_FALSE(button.FromJson(jsonComponent));
}

/**
 * @tc.name: FromJson004
 * @tc.desc: Test location button from wrong value params json
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(LocationButtonTest, FromJson004, TestSize.Level1)
{
nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);
    LocationButton button;

    ASSERT_TRUE(button.FromJson(jsonComponent));

    jsonComponent[JSON_STYLE_TAG] = nlohmann::json {
        { JSON_TEXT_TAG, LocationDesc::UNKNOWN_TEXT },
        { JSON_ICON_TAG, LocationIcon::LINE_ICON },
        { JSON_BG_TAG, LocationBackground::CIRCLE },
    };
    ASSERT_FALSE(button.FromJson(jsonComponent));

    jsonComponent[JSON_STYLE_TAG] = nlohmann::json {
        { JSON_TEXT_TAG, LocationDesc::SELECT_LOCATION },
        { JSON_ICON_TAG, LocationIcon::UNKNOWN_ICON },
        { JSON_BG_TAG, LocationBackground::CIRCLE },
    };
    ASSERT_FALSE(button.FromJson(jsonComponent));

    jsonComponent[JSON_STYLE_TAG] = nlohmann::json {
        { JSON_TEXT_TAG, LocationDesc::SELECT_LOCATION },
        { JSON_ICON_TAG, LocationIcon::LINE_ICON },
        { JSON_BG_TAG, LocationBackground::UNKNOWN_BG },
    };
    ASSERT_FALSE(button.FromJson(jsonComponent));
}

/**
 * @tc.name: CompareLocationButton001
 * @tc.desc: Test compare location button
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(LocationButtonTest, CompareLocationButton001, TestSize.Level1)
{
    LocationButton button1;
    LocationButton button2;

    nlohmann::json jsonComponent;
    BuildLocationComponentInfo(jsonComponent);

    ASSERT_TRUE(button1.FromJson(jsonComponent));
    ASSERT_TRUE(button2.FromJson(jsonComponent));
    ASSERT_TRUE(button1.CompareComponentBasicInfo(&button2));

    button1.text_ = LocationDesc::UNKNOWN_TEXT;
    ASSERT_FALSE(button1.CompareComponentBasicInfo(&button2));
    button1.text_ = LocationDesc::SELECT_LOCATION;

    button1.icon_ = LocationIcon::UNKNOWN_ICON;
    ASSERT_FALSE(button1.CompareComponentBasicInfo(&button2));
    button1.icon_ = LocationIcon::LINE_ICON;

    button1.bg_ = LocationBackground::UNKNOWN_BG;
    ASSERT_FALSE(button1.CompareComponentBasicInfo(&button2));
    button1.bg_ = LocationBackground::CIRCLE;

    ASSERT_TRUE(button1.CompareComponentBasicInfo(&button2));
}
