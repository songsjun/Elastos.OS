
#ifndef __ELASTOS_DROID_SERVER_PM_DEX_ZIPARCHIVE_H__
#define __ELASTOS_DROID_SERVER_PM_DEX_ZIPARCHIVE_H__

#include <elastos/core/Object.h>

using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {
namespace Dex {

class ZipEntry : public Object
{
public:
    CARAPI_(uint32_t) GetCrc32();
};

class ZipArchive : public Object
{
public:
    static CARAPI_(AutoPtr<ZipArchive>) OpenFromFd(
        /* [in] */ Int32 fd,
        /* [in] */ const char* filename,
        /* [in] */ String* error_msg);

    CARAPI_(AutoPtr<ZipEntry>) Find(
        /* [in] */ const char* name,
        /* [in] */ String* error_msg);
};

} // namespace Dex
} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_PM_DEX_ZIPARCHIVE_H__
