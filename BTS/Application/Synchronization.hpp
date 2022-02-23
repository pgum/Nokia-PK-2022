#pragma once

#include <mutex>
#include <memory>

namespace bts
{

using SyncGuard = std::recursive_mutex;
using SyncGuardPtr = std::shared_ptr<SyncGuard>;
using SyncLock = std::lock_guard<SyncGuard>;

}
