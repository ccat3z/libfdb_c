#include "fdb_c_types.h"

#ifdef __cplusplus
extern "C" {
#endif

using FDBRWLockHodler = struct fdb_rwlocks_holder;

FDBFuture *fdb_delay(double seconds);
FDBRWLockHodler *fdb_rwlocks_create();
void fdb_rwlocks_shared(FDBRWLockHodler *holder, const char *key);
void fdb_rwlocks_exclusive(FDBRWLockHodler *holder, const char *key);
FDBFuture *fdb_rwlocks_lock(FDBRWLockHodler *holder);
void fdb_rwlocks_free(FDBRWLockHodler *holder);

#ifdef __cplusplus
}
#endif
