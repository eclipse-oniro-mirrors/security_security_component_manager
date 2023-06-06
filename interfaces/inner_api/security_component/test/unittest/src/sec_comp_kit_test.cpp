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
#include "sec_comp_kit_test.h"
#include "location_button.h"
#define private public
#include "sec_comp_caller_authorization.h"
#undef private
#include "sec_comp_err.h"
#include "sec_comp_info.h"
#include "sec_comp_log.h"
#include "sec_comp_tool.h"

using namespace testing::ext;
using namespace OHOS::Security::SecurityComponent;

namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "SecCompKitTest"};
static constexpr float TEST_SIZE = 100.0;
static constexpr double TEST_COORDINATE = 100.0;
static constexpr double TEST_DIMENSION = 100.0;
static constexpr uint32_t TEST_COLOR = 0xffffffff;

static bool TestInCallerCheckList()
{
    int32_t scId = -1;
    struct SecCompClickEvent touch;
    if ((SecCompKit::RegisterSecurityComponent(LOCATION_COMPONENT, "", scId) == SC_SERVICE_ERROR_CALLER_INVALID) ||
        (SecCompKit::UpdateSecurityComponent(scId, "") == SC_SERVICE_ERROR_CALLER_INVALID) ||
        (SecCompKit::ReportSecurityComponentClickEvent(scId, "", touch) == SC_SERVICE_ERROR_CALLER_INVALID) ||
        (SecCompKit::UnregisterSecurityComponent(scId) == SC_SERVICE_ERROR_CALLER_INVALID)) {
        return false;
    }
    return true;
}
}  // namespace

void SecCompKitTest::SetUpTestCase()
{
    SC_LOG_INFO(LABEL, "SetUpTestCase.");
}

void SecCompKitTest::TearDownTestCase()
{
    SC_LOG_INFO(LABEL, "TearDownTestCase.");
}

void SecCompKitTest::SetUp()
{
    SC_LOG_INFO(LABEL, "SetUp ok.");
}

void SecCompKitTest::TearDown()
{
    SC_LOG_INFO(LABEL, "TearDown.");
}

namespace {
static bool TestInCallerNotCheckList()
{
    int32_t scId = -1;
    struct SecCompClickEvent touch;
    if ((SecCompKit::RegisterSecurityComponent(LOCATION_COMPONENT, "", scId) != SC_SERVICE_ERROR_CALLER_INVALID) ||
        (SecCompKit::UpdateSecurityComponent(scId, "") != SC_SERVICE_ERROR_CALLER_INVALID) ||
        (SecCompKit::ReportSecurityComponentClickEvent(scId, "", touch) != SC_SERVICE_ERROR_CALLER_INVALID) ||
        (SecCompKit::UnregisterSecurityComponent(scId) != SC_SERVICE_ERROR_CALLER_INVALID)) {
        return false;
    }
    return true;
}
}

/**
 * @tc.name: ExceptCall001
 * @tc.desc: do kit except call.
 * @tc.type: FUNC
 * @tc.require: AR000HO9IN
 */
HWTEST_F(SecCompKitTest, ExceptCall001, TestSize.Level1)
{
    LocationButton comp;
    comp.fontSize_ = TEST_SIZE;
    comp.iconSize_ = TEST_SIZE;
    comp.padding_.top = TEST_DIMENSION;
    comp.padding_.right = TEST_DIMENSION;
    comp.padding_.bottom = TEST_DIMENSION;
    comp.padding_.left = TEST_DIMENSION;
    comp.textIconSpace_ = TEST_SIZE;
    comp.bgColor_.value = TEST_COLOR;
    comp.fontColor_.value = TEST_COLOR;
    comp.iconColor_.value = TEST_COLOR;
    comp.borderWidth_ = TEST_SIZE;
    comp.parentEffect_ = true;
    comp.type_ = LOCATION_COMPONENT;
    comp.rect_.x_ = TEST_COORDINATE;
    comp.rect_.y_ = TEST_COORDINATE;
    comp.rect_.width_ = TEST_COORDINATE;
    comp.rect_.height_ = TEST_COORDINATE;

    nlohmann::json jsonRes;
    comp.ToJson(jsonRes);
    int32_t scId = -1;
    ASSERT_NE(SC_OK, SecCompKit::RegisterSecurityComponent(LOCATION_COMPONENT, jsonRes.dump(), scId));
    ASSERT_EQ(-1, scId);
    ASSERT_NE(SC_OK, SecCompKit::UpdateSecurityComponent(scId, jsonRes.dump()));

    struct SecCompClickEvent touch = {
        .touchX = TEST_COORDINATE,
        .touchY = TEST_COORDINATE,
        .timestamp = static_cast<uint64_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count())
    };
    EXPECT_NE(SC_OK, SecCompKit::ReportSecurityComponentClickEvent(scId, jsonRes.dump(), touch));
    EXPECT_NE(SC_OK, SecCompKit::UnregisterSecurityComponent(scId));
}

/**
 * @tc.name: ExceptCall001
 * @tc.desc: do kit except call.
 * @tc.type: FUNC
 * @tc.require: AR000HO9IN
 */
HWTEST_F(SecCompKitTest, TestCallerCheck001, TestSize.Level1)
{
    std::vector<uintptr_t> callerList = {
        reinterpret_cast<uintptr_t>(TestInCallerCheckList),
    };
    SecCompUiRegister registerCallback(callerList);
    TestInCallerCheckList();
    TestInCallerNotCheckList();

    SecCompCallerAuthorization::GetInstance().kitCallerList_.clear();
}
