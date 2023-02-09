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
#include "app_state_observer_test.h"
#include "sec_comp_log.h"
#include "sec_comp_err.h"

using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::Security::SecurityComponent;

namespace {
static constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {
    LOG_CORE, SECURITY_DOMAIN_SECURITY_COMPONENT, "AppStateObserverTest"};

static constexpr int32_t TEST_UID = 1;
static constexpr int32_t TEST_UID_2 = 2;
}

void AppStateObserverTest::SetUpTestCase()
{}

void AppStateObserverTest::TearDownTestCase()
{}

void AppStateObserverTest::SetUp()
{
    SC_LOG_INFO(LABEL, "setup");
    if (observer_ != nullptr) {
        return;
    }

    observer_ = std::make_shared<AppStateObserver>();
    ASSERT_NE(nullptr, observer_);
}

void AppStateObserverTest::TearDown()
{
    observer_ = nullptr;
}

/**
 * @tc.name: IsProcessForeground001
 * @tc.desc: Test is process foreground
 * @tc.type: FUNC
 * @tc.require:AR000HO9IN
 */
HWTEST_F(AppStateObserverTest, IsProcessForeground001, TestSize.Level1)
{
    ASSERT_FALSE(observer_->IsProcessForeground(TEST_UID));
    AppExecFwk::AppStateData stateData = {
        .uid = TEST_UID
    };
    observer_->AddProcessToForegroundSet(stateData);
    ASSERT_TRUE(observer_->IsProcessForeground(TEST_UID));
}

/**
 * @tc.name: AddProcessToForegroundSet001
 * @tc.desc: Test add process to foreground
 * @tc.type: FUNC
 * @tc.require:AR000HO9IN
 */
HWTEST_F(AppStateObserverTest, AddProcessToForegroundSet001, TestSize.Level1)
{
    AppExecFwk::AppStateData stateData = {
        .uid = TEST_UID
    };
    observer_->AddProcessToForegroundSet(stateData);
    observer_->AddProcessToForegroundSet(stateData);
    ASSERT_TRUE(observer_->IsProcessForeground(TEST_UID));

    AppExecFwk::ProcessData procData = {
        .uid = TEST_UID_2
    };
    observer_->AddProcessToForegroundSet(procData);
    observer_->AddProcessToForegroundSet(procData);
    ASSERT_TRUE(observer_->IsProcessForeground(TEST_UID_2));
}

/**
 * @tc.name: RemoveProcessFromForegroundSet001
 * @tc.desc: Test remove process from foreground
 * @tc.type: FUNC
 * @tc.require:AR000HO9IN
 */
HWTEST_F(AppStateObserverTest, RemoveProcessFromForegroundSet001, TestSize.Level1)
{
    AppExecFwk::ProcessData procData = {
        .uid = TEST_UID
    };
    observer_->AddProcessToForegroundSet(procData);
    ASSERT_TRUE(observer_->IsProcessForeground(TEST_UID));
    observer_->RemoveProcessFromForegroundSet(procData);
    observer_->RemoveProcessFromForegroundSet(procData);
    ASSERT_FALSE(observer_->IsProcessForeground(TEST_UID));
}

/**
 * @tc.name: OnProcessStateChanged001
 * @tc.desc: Test recieve process state changed
 * @tc.type: FUNC
 * @tc.require:AR000HO9IN
 */
HWTEST_F(AppStateObserverTest, OnProcessStateChanged001, TestSize.Level1)
{
    AppExecFwk::ProcessData processData;
    processData.state = AppExecFwk::AppProcessState::APP_STATE_CREATE;
    observer_->OnProcessStateChanged(processData);
    ASSERT_EQ(observer_->foregrandProcList_.size(), static_cast<size_t>(0));

    processData.state = AppExecFwk::AppProcessState::APP_STATE_FOREGROUND;
    processData.uid = TEST_UID;
    observer_->OnProcessStateChanged(processData);
    ASSERT_TRUE(observer_->IsProcessForeground(TEST_UID));

    processData.state = AppExecFwk::AppProcessState::APP_STATE_BACKGROUND;
    processData.uid = TEST_UID;
    observer_->OnProcessStateChanged(processData);
    ASSERT_FALSE(observer_->IsProcessForeground(TEST_UID));
}

/**
 * @tc.name: OnProcessDied001
 * @tc.desc: Test recieve process state died
 * @tc.type: FUNC
 * @tc.require:AR000HO9IN
 */
HWTEST_F(AppStateObserverTest, OnProcessDied001, TestSize.Level1)
{
    AppExecFwk::ProcessData processData;
    processData.state = AppExecFwk::AppProcessState::APP_STATE_FOREGROUND;
    processData.uid = TEST_UID;
    observer_->OnProcessStateChanged(processData);
    ASSERT_TRUE(observer_->IsProcessForeground(TEST_UID));

    // if last process died, the sec_comp service will exit.
    processData.uid = TEST_UID_2;
    observer_->OnProcessStateChanged(processData);
    ASSERT_TRUE(observer_->IsProcessForeground(TEST_UID_2));

    // notify process 1 died
    processData.uid = TEST_UID;
    observer_->OnProcessDied(processData);
    ASSERT_FALSE(observer_->IsProcessForeground(TEST_UID));
}
