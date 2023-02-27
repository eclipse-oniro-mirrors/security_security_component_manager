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
static constexpr double TEST_COORDINATE = 100.0;
static constexpr double WRONG_COORDINATE = -100.0;
static constexpr double TEST_DIFF_COORDINATE = 0.0;
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

    jsonComponent[JSON_SC_TYPE] = LOCATION_COMPONENT;
    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, TEST_COORDINATE},
        {JSON_RECT_Y, TEST_COORDINATE},
        {JSON_RECT_WIDTH, TEST_COORDINATE},
        {JSON_RECT_HEIGHT, TEST_COORDINATE}
    };

    SecCompBase comp;
    comp.FromJson(jsonComponent);
    ASSERT_TRUE(comp.IsValid());
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
    comp.FromJson(jsonComponent);
    ASSERT_FALSE(comp.IsValid());
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
    SecCompBase comp;
    jsonComponent[JSON_SC_TYPE] = "wrongType";
    comp.FromJson(jsonComponent);
    ASSERT_FALSE(comp.IsValid());

    jsonComponent[JSON_SC_TYPE] = 0;
    comp.FromJson(jsonComponent);
    ASSERT_FALSE(comp.IsValid());
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
    comp.FromJson(jsonComponent);
    ASSERT_FALSE(comp.IsValid());

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, "wrongType"},
        {JSON_RECT_Y, TEST_COORDINATE},
        {JSON_RECT_WIDTH, TEST_COORDINATE},
        {JSON_RECT_HEIGHT, TEST_COORDINATE}
    };

    comp.FromJson(jsonComponent);
    ASSERT_FALSE(comp.IsValid());

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, TEST_COORDINATE},
        {JSON_RECT_Y, "wrongType"},
        {JSON_RECT_WIDTH, TEST_COORDINATE},
        {JSON_RECT_HEIGHT, TEST_COORDINATE}
    };
    comp.FromJson(jsonComponent);
    ASSERT_FALSE(comp.IsValid());

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, TEST_COORDINATE},
        {JSON_RECT_Y, TEST_COORDINATE},
        {JSON_RECT_WIDTH, "wrongType"},
        {JSON_RECT_HEIGHT, TEST_COORDINATE}
    };
    comp.FromJson(jsonComponent);
    ASSERT_FALSE(comp.IsValid());

    jsonComponent[JSON_RECT] =  nlohmann::json {
        {JSON_RECT_X, TEST_COORDINATE},
        {JSON_RECT_Y, TEST_COORDINATE},
        {JSON_RECT_WIDTH, TEST_COORDINATE},
        {JSON_RECT_HEIGHT, "wrongType"}
    };
    comp.FromJson(jsonComponent);
    ASSERT_FALSE(comp.IsValid());
}

/**
 * @tc.name: IsValid001
 * @tc.desc: Test SecCompBase invalid
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompBaseTest, IsValid001, TestSize.Level1)
{
    SecCompBase comp;
    comp.type_ = UNKNOWN_SC_TYPE;
    ASSERT_FALSE(comp.IsValid());

    comp.type_ = LOCATION_COMPONENT;
    comp.rect_.x_ = WRONG_COORDINATE;
    ASSERT_FALSE(comp.IsValid());
}

/**
 * @tc.name: Compare001
 * @tc.desc: Test SecCompBase compare
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SecCompBaseTest, Compare001, TestSize.Level1)
{
    SecCompBase comp1;
    comp1.type_ = LOCATION_COMPONENT;
    comp1.rect_.x_ = TEST_COORDINATE;
    comp1.rect_.y_ = TEST_COORDINATE;
    comp1.rect_.width_ = TEST_COORDINATE;
    comp1.rect_.height_ = TEST_COORDINATE;
    SecCompBase comp2 = comp1;

    comp1.type_ = SAVE_COMPONENT;
    ASSERT_FALSE(comp1 == comp2);
    comp1.type_ = LOCATION_COMPONENT;

    comp1.rect_.x_ = TEST_DIFF_COORDINATE;
    ASSERT_FALSE(comp1 == comp2);
    comp1.rect_.x_ = TEST_COORDINATE;

    comp1.rect_.y_ = TEST_DIFF_COORDINATE;
    ASSERT_FALSE(comp1 == comp2);
    comp1.rect_.y_ = TEST_COORDINATE;

    comp1.rect_.width_ = TEST_DIFF_COORDINATE;
    ASSERT_FALSE(comp1 == comp2);
    comp1.rect_.width_ = TEST_COORDINATE;

    comp1.rect_.height_ = TEST_DIFF_COORDINATE;
    ASSERT_FALSE(comp1 == comp2);
    comp1.rect_.height_ = TEST_COORDINATE;
}
