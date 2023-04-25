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
#include "sec_comp_base_test.h"
#include <string>
#include "sec_comp_log.h"
#include "sec_comp_err.h"

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::Security::SecurityComponent;

namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompBaseTest"};

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
static constexpr uint32_t TEST_DIFF_COLOR = 0;

static void BuildSecCompBaseInfo(nlohmann::json& jsonComponent)
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
}
}

void SecCompBaseTest::SetUpTestCase()
{}

void SecCompBaseTest::TearDownTestCase()
{}

void SecCompBaseTest::SetUp()
{
    SC_LOG_INFO(LABEL, "setup");
}

void SecCompBaseTest::TearDown()
{}

/**
 * @tc.name: FromJson001
 * @tc.desc: Test SecCompBase from json success
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompBaseTest, FromJson001, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildSecCompBaseInfo(jsonComponent);
    SecCompBase comp;
    ASSERT_TRUE(comp.FromJson(jsonComponent));
}

/**
 * @tc.name: FromJson002
 * @tc.desc: Test empty SecCompBase from json
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompBaseTest, FromJson002, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    SecCompBase comp;
    ASSERT_FALSE(comp.FromJson(jsonComponent));
}

/**
 * @tc.name: FromJson003
 * @tc.desc: Test location button from wrong type json
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompBaseTest, FromJson003, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    BuildSecCompBaseInfo(jsonComponent);
    SecCompBase comp;
    ASSERT_TRUE(comp.FromJson(jsonComponent));

    jsonComponent[JSON_SC_TYPE] = WRONG_TYPE;
    ASSERT_FALSE(comp.FromJson(jsonComponent));

    jsonComponent[JSON_SC_TYPE] = 0;
    ASSERT_FALSE(comp.FromJson(jsonComponent));
}

/**
 * @tc.name: FromJson004
 * @tc.desc: Test location button from wrong rect json
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompBaseTest, FromJson004, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    SecCompBase comp;
    jsonComponent[JSON_SC_TYPE] = LOCATION_COMPONENT;
    nlohmann::json wrongJson = nlohmann::json::parse("{", nullptr, false);
    jsonComponent[JSON_RECT] = wrongJson;
    ASSERT_FALSE(comp.FromJson(jsonComponent));

    BuildSecCompBaseInfo(jsonComponent);
    ASSERT_TRUE(comp.FromJson(jsonComponent));

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, WRONG_TYPE},
        {JSON_RECT_Y, TEST_COORDINATE},
        {JSON_RECT_WIDTH, TEST_COORDINATE},
        {JSON_RECT_HEIGHT, TEST_COORDINATE}
    };
    ASSERT_FALSE(comp.FromJson(jsonComponent));

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, TEST_COORDINATE},
        {JSON_RECT_Y, WRONG_TYPE},
        {JSON_RECT_WIDTH, TEST_COORDINATE},
        {JSON_RECT_HEIGHT, TEST_COORDINATE}
    };
    ASSERT_FALSE(comp.FromJson(jsonComponent));

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, TEST_COORDINATE},
        {JSON_RECT_Y, TEST_COORDINATE},
        {JSON_RECT_WIDTH, WRONG_TYPE},
        {JSON_RECT_HEIGHT, TEST_COORDINATE}
    };
    ASSERT_FALSE(comp.FromJson(jsonComponent));

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, TEST_COORDINATE},
        {JSON_RECT_Y, TEST_COORDINATE},
        {JSON_RECT_WIDTH, TEST_COORDINATE},
        {JSON_RECT_HEIGHT, WRONG_TYPE}
    };
    ASSERT_FALSE(comp.FromJson(jsonComponent));
}

/**
 * @tc.name: FromJson005
 * @tc.desc: Test location button from wrong size json
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompBaseTest, FromJson005, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    SecCompBase comp;
    BuildSecCompBaseInfo(jsonComponent);
    ASSERT_TRUE(comp.FromJson(jsonComponent));

    nlohmann::json jsonPadding = nlohmann::json {
        { JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JSON_PADDING_RIGHT_TAG, TEST_DIMENSION },
        { JSON_PADDING_BOTTOM_TAG,TEST_DIMENSION },
        { JSON_PADDING_LEFT_TAG, TEST_DIMENSION },
    };

    jsonComponent[JSON_SIZE_TAG] = nlohmann::json {
        { JSON_FONT_SIZE_TAG, WRONG_TYPE },
        { JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    ASSERT_FALSE(comp.FromJson(jsonComponent));

    jsonComponent[JSON_SIZE_TAG] = nlohmann::json {
        { JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JSON_ICON_SIZE_TAG, WRONG_TYPE },
        { JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    ASSERT_FALSE(comp.FromJson(jsonComponent));

    jsonComponent[JSON_SIZE_TAG] = nlohmann::json {
        { JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JSON_TEXT_ICON_PADDING_TAG, WRONG_TYPE },
        { JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    ASSERT_FALSE(comp.FromJson(jsonComponent));

    jsonPadding = nlohmann::json {
        { JSON_PADDING_TOP_TAG, WRONG_TYPE },
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
    ASSERT_FALSE(comp.FromJson(jsonComponent));

    jsonPadding = nlohmann::json {
        { JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JSON_PADDING_RIGHT_TAG, WRONG_TYPE },
        { JSON_PADDING_BOTTOM_TAG,TEST_DIMENSION },
        { JSON_PADDING_LEFT_TAG, TEST_DIMENSION },
    };
    jsonComponent[JSON_SIZE_TAG] = nlohmann::json {
        { JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    ASSERT_FALSE(comp.FromJson(jsonComponent));

    jsonPadding = nlohmann::json {
        { JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JSON_PADDING_RIGHT_TAG, TEST_DIMENSION },
        { JSON_PADDING_BOTTOM_TAG,WRONG_TYPE },
        { JSON_PADDING_LEFT_TAG, TEST_DIMENSION },
    };
    jsonComponent[JSON_SIZE_TAG] = nlohmann::json {
        { JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    ASSERT_FALSE(comp.FromJson(jsonComponent));

    jsonPadding = nlohmann::json {
        { JSON_PADDING_TOP_TAG, TEST_DIMENSION },
        { JSON_PADDING_RIGHT_TAG, TEST_DIMENSION },
        { JSON_PADDING_BOTTOM_TAG,TEST_DIMENSION },
        { JSON_PADDING_LEFT_TAG, WRONG_TYPE },
    };
    jsonComponent[JSON_SIZE_TAG] = nlohmann::json {
        { JSON_FONT_SIZE_TAG, TEST_SIZE },
        { JSON_ICON_SIZE_TAG, TEST_SIZE },
        { JSON_TEXT_ICON_PADDING_TAG, TEST_SIZE },
        { JSON_PADDING_SIZE_TAG, jsonPadding },
    };
    ASSERT_FALSE(comp.FromJson(jsonComponent));
}

/**
 * @tc.name: FromJson006
 * @tc.desc: Test location button from wrong border and parent json
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompBaseTest, FromJson006, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    SecCompBase comp;
    BuildSecCompBaseInfo(jsonComponent);
    ASSERT_TRUE(comp.FromJson(jsonComponent));

    jsonComponent[JSON_BORDER_TAG] = nlohmann::json {
        { JSON_BORDER_WIDTH_TAG, WRONG_TYPE },
    };
    ASSERT_FALSE(comp.FromJson(jsonComponent));

    BuildSecCompBaseInfo(jsonComponent);
    jsonComponent[JSON_PARENT_TAG] = nlohmann::json {
        { JSON_PARENT_EFFECT_TAG, WRONG_TYPE },
    };
    ASSERT_FALSE(comp.FromJson(jsonComponent));
}

/**
 * @tc.name: Compare001
 * @tc.desc: Test SecCompBase compare
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompBaseTest, Compare001, TestSize.Level1)
{
    nlohmann::json jsonComponent;
    SecCompBase comp1;
    BuildSecCompBaseInfo(jsonComponent);
    ASSERT_TRUE(comp1.FromJson(jsonComponent));
    SecCompBase comp2 = comp1;

    comp1.type_ = SAVE_COMPONENT;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2));
    comp1.type_ = LOCATION_COMPONENT;

    comp1.fontSize_ = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2));
    comp1.fontSize_ = TEST_SIZE;

    comp1.iconSize_ = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2));
    comp1.iconSize_ = TEST_SIZE;

    comp1.padding_.top = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2));
    comp1.padding_.top = TEST_DIMENSION;

    comp1.padding_.right = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2));
    comp1.padding_.right = TEST_DIMENSION;

    comp1.padding_.bottom = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2));
    comp1.padding_.bottom = TEST_DIMENSION;

    comp1.padding_.left = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2));
    comp1.padding_.left = TEST_DIMENSION;

    comp1.textIconPadding_ = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2));
    comp1.textIconPadding_ = TEST_SIZE;

    comp1.borderWidth_ = DEFAULT_DIMENSION;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2));
    comp1.borderWidth_ = TEST_SIZE;

    comp1.fontColor_.value = TEST_DIFF_COLOR;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2));
    comp1.fontColor_.value = TEST_COLOR_2;

    comp1.bgColor_.value = TEST_DIFF_COLOR;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2));
    comp1.bgColor_.value = TEST_COLOR_1;

    comp1.iconColor_.value = TEST_DIFF_COLOR;
    ASSERT_FALSE(comp1.CompareComponentBasicInfo(&comp2));
    comp1.iconColor_.value = TEST_COLOR_3;

    ASSERT_TRUE(comp1.CompareComponentBasicInfo(&comp2));
}
