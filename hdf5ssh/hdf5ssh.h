#ifndef	__HDF5SSH_HDF5SSH_H
#define __HDF5SSH_HDF5SSH_H

#include <hdf5.h>
#include "hdf5sshDllApi.h"

#ifdef __cplusplus
extern "C" {
#endif

extern herr_t	H5Fopenssh2(hid_t plist_id);
	
#ifdef __cplusplus
}
#endif


#endif /* __HDF5SSH_HDF5SSH_H */


