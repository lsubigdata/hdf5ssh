#include <hdf5.h>
#include <libssh2.h>
#include <libssh2_sftp.h>
#include "../hdf5ssh.h"

herr_t my_H5L_iterate_t(hid_t g_id, const char *name, const H5L_info_t *info, void *op_data)
{
    printf("    Found Object '%s' in remote HDF5 file.\n", name);
    return 0;
}

//cliu32@204.90.44.22:physicsK8DVZRMN!:/home/cliu32/XXX.h5
int	main(int argc, char**argv)
{
    //if (argc<2)
    //{
    //    puts("Usage: HDF5SS2Client <username@hostname:passphrase:path>");
    //    return 1;
    //}

    hid_t	plist_id = H5Pcreate (H5P_FILE_ACCESS);

    H5Fopenssh2(plist_id);

    //char * path = "cliu32@204.90.44.22:physicsK8DVZRMN!:/home/cliu32/XXX.h5"; // correct
    char * path = "jtao@127.0.0.1:wagtj22:/home/jtao/Documents/presentations/SIAT2009/demo/WaveDemo/phi.h5"; // wrong
    //hid_t file_id = H5Fopen( argv[1], H5F_ACC_RDONLY, plist_id);
    hid_t file_id = H5Fopen( path, H5F_ACC_RDONLY, plist_id);
    H5Literate(file_id, H5_INDEX_NAME, H5_ITER_NATIVE, 0, my_H5L_iterate_t, 0);
    //hid_t dataset_id = H5Dopen2(file_id, "/dset", H5P_DEFAULT);
    //int dset_data[4][6];
    //herr_t status = H5Dread(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, dset_data);
    //status = H5Dclose(dataset_id);
    //status = H5Fclose(file_id);
    return 0;

}
