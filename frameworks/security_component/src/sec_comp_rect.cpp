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

#include "sec_comp_rect.h"

namespace OHOS {
namespace Security {
namespace SecurityComponent {
bool SecCompRect::IsValid() const
{
    return GreatNotEqual(x_, 0.0) && GreatNotEqual(y_, 0.0) &&
        GreatNotEqual(width_, 0.0) && GreatNotEqual(height_, 0.0);
}

bool SecCompRect::IsRectOverlaped(const SecCompRect& other) const
{
    double xLow = other.x_;
    double xHigh = other.x_ + other.width_ ;
    double yLow = other.y_ ;
    double yHigh = other.y_ + other.height_;

    return (!(GreatOrEqual(xLow, (x_ + width_)) || GreatOrEqual(x_, xHigh))) &&
        (!(GreatOrEqual(y_, yHigh) || GreatOrEqual(yLow, (y_ + height_))));
}

bool SecCompRect::IsInRect(double x, double y) const
{
    return (GreatOrEqual(x, x_) && GreatOrEqual((x_ + width_), x) &&
        GreatOrEqual(y, y_) && GreatOrEqual((y_ + height_), y));
}
}  // namespace SecurityComponent
}  // namespace Security
}  // namespace OHOS
