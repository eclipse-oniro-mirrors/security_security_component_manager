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
#ifndef SECURITY_COMPONET_MOCK_APP_MGR_PROXY_H
#define SECURITY_COMPONET_MOCK_APP_MGR_PROXY_H
#include <string>
#include <gmock/gmock.h>
#include "app_mgr_interface.h"
#include "iremote_proxy.h"

namespace OHOS {
class MockAppMgrProxy : public IRemoteProxy<AppExecFwk::IAppMgr> {
public:
    explicit MockAppMgrProxy(const sptr<IRemoteObject>& impl)
        : IRemoteProxy<AppExecFwk::IAppMgr>(impl) {}
    ~MockAppMgrProxy() = default;

    static MockAppMgrProxy* g_MockAppMgrProxy;

    MOCK_METHOD2(RegisterApplicationStateObserver,
            int32_t(const sptr<AppExecFwk::IApplicationStateObserver>&, const std::vector<std::string>&));
    MOCK_METHOD1(UnregisterApplicationStateObserver, int32_t(const sptr<AppExecFwk::IApplicationStateObserver> &));
    MOCK_METHOD1(GetForegroundApplications, int32_t(std::vector<AppExecFwk::AppStateData>&));
};
} // namespace OHOS
#endif
