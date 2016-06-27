/**
   HDF5 Virtual File Driver for SSH


    libssh2 is a client-side C library implementing the SSH2 protocol.

    (c) 2015
    Celong Liu
    Center for Computation & Technology, Louisiana State University, Baton Rouge, USA

    Based on code by
    Werner Benger
    Airborne HydroMapping Software GmbH, Innsbruck, Austria
    Center for Computation & Technology, Louisiana State University, Baton Rouge, USA
    Max-Planck-Institute for Gravitational Physics (Albert-Einstein-Institute), Potsdam, Germany

    Based on code by
    Thomas Radke, Max-Planck-Institute for Gravitational Physics (Albert-Einstein-Institute), Potsdam, Germany

    This work is supported by NSF Award #1251137
    BIGDATA: Small: DCM: Collaborative Research: An efficient, versatile, scalable, and portable storage system for scientific data containers

 */

#include "hdf5ssh.h"

#include <sys/types.h>
#include <hdf5.h>
//#include "libssh2_config.h"
#include <libssh2.h>
#include <libssh2_sftp.h>


#ifdef _WINDOWS
# include <winsock2.h>

#else

# include <netinet/in.h>
# include <sys/socket.h>
# include <arpa/inet.h>

#endif


# ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif

#include <hdf5.h>

#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
	OP_WRITE,
	OP_READ,
	OP_CLOSE,
	OP_OPEN,
	OP_UNKNOWN
} H5FD_SSH_op;


typedef struct H5FD_ssh2_t {
	H5FD_t      pub;                    /*public stuff, must be first   */
	LIBSSH2_SFTP_HANDLE *handle;
	LIBSSH2_SFTP *sftp;
	haddr_t     eoa;                    /*end of allocated region       */
	haddr_t     eof;                    /*end of file; current file size*/
	haddr_t     pos;                    /*current file I/O position     */
	H5FD_SSH_op op;                     /*last operation                */
} H5FD_ssh2_t;



typedef struct	SSH2_info
{
	LIBSSH2_SESSION *session;
}SSH2_info;


#define MAXADDR 3000000000

//#define H5FD_ssh2_get_handle	0

/**
   http://www.hdfgroup.org/HDF5/doc/TechNotes/VFL.html
 */

H5FD_t *H5FD_ssh2_open(const char* filename, unsigned flags, hid_t fapl, haddr_t maxaddr);
herr_t   H5FD_ssh2_close(H5FD_t* file_);
haddr_t	H5FD_ssh2_get_eoa (const H5FD_t *file_, H5FD_mem_t type);
herr_t H5FD_ssh2_set_eoa(H5FD_t *file_, H5FD_mem_t type, haddr_t a);
haddr_t H5FD_ssh2_get_eof(const H5FD_t *file_);
herr_t H5FD_ssh2_read(H5FD_t *file_, H5FD_mem_t type, hid_t dxpl, haddr_t addr, size_t size, void *buffer);
herr_t H5FD_ssh2_write(H5FD_t *file_, H5FD_mem_t type, hid_t dxpl, haddr_t addr, size_t size, const void *buffer);
herr_t H5FD_ssh2_flush(H5FD_t *file_,  hid_t dxpl_id, unsigned closing);
herr_t  H5FD_ssh2_get_handle(H5FD_t *file, hid_t fapl, void**file_handle);


// An optional function to determine whether two open files have the same key. If this function is not present then the library assumes that two files will never be the same.
//int (*cmp)(const H5FD_t *f1, const H5FD_t *f2)
#define H5FD_ssh2_cmp NULL

// An optional function to determine which library optimizations a driver can support.
//int (*query)(const H5FD_t *f, unsigned long *flags)
#define H5FD_ssh2_query NULL

static const H5FD_class_t DriverInfo =
{
    "ssh2",					/*name                  */
    MAXADDR,                                    /*maxaddr               */
    H5F_CLOSE_WEAK,
    NULL,                                       /*sb_size               */
    NULL,                                       /*sb_encode             */
    NULL,                                       /*sb_decode             */
    sizeof (SSH2_info),                                          /*fapl_size             */
    NULL,                                       /*fapl_get              */
    NULL,                                       /*fapl_copy             */
    NULL,                                       /*fapl_free             */
    0,                                          /*dxpl_size             */
    NULL,                                       /*dxpl_copy             */
    NULL,                                       /*dxpl_free             */
    H5FD_ssh2_open,                             /*open                  */
    H5FD_ssh2_close,                            /*close                 */
    H5FD_ssh2_cmp,                              /*cmp                   */
    H5FD_ssh2_query,                            /*query                 */
	NULL,										/*get type map			*/
    NULL,                                       /*alloc                 */
    NULL,                                       /*free                  */
    H5FD_ssh2_get_eoa,                          /*get_eoa               */
    H5FD_ssh2_set_eoa,                          /*set_eoa               */
    H5FD_ssh2_get_eof,                          /*get_eof               */

    H5FD_ssh2_get_handle,

    H5FD_ssh2_read,                             /*read                  */
    H5FD_ssh2_write,                            /*write                 */
    H5FD_ssh2_flush,                            /*flush                 */

	0, // truncate
    0, // lock
    0, // unlock

    H5FD_FLMAP_SINGLE                          /*fl_map                */
};

static	hid_t H5FD_VFD_id = 0;



//204.90.44.22 wuyiyu 7uji9olp-ABCDEFG /home/wuyiyu/testfile

/*const	char username[] = "cliu32",
	publickey[] = "public key",
	privatekey[] = "pravite key",
	passphrase[] = "physicsK8DVZRMN!";
*/

hid_t	registerHDF5VFDSSH2();

herr_t	H5Fopenssh2(hid_t plist_id)
{
    //printf("entering H5Fopenssh2\n");
    herr_t	retval;
	//hid_t	plist_id;
	hid_t	driver_id = registerHDF5VFDSSH2();
	SSH2_info *what = (SSH2_info*)malloc(sizeof (SSH2_info));
	//by wuyi, init the session after connection established.
	what->session = NULL;// libssh2_session_init();

	H5Eclear2(H5E_DEFAULT);
	//plist_id = H5Pcreate(H5P_FILE_CREATE);

	H5Pset_driver( plist_id, driver_id, what);
    //printf("leaving H5Fopenssh2\n");

	return 0;
}


// The function which opens or creates a new file.
// filepath = username@hostname:passphrase:filename
H5FD_t *H5FD_ssh2_open(const char* filepath, unsigned flags, hid_t fapl, haddr_t maxaddr)
{
    //printf("entering H5FD_ssh2_open\n");
    char* path = (char*)malloc((strlen(filepath) + 1)*sizeof(char));
    memcpy(path, filepath, 1 + strlen(filepath));
    //printf("filepath = %s\n", filepath);
    //printf("path = %s\n", path);
    char *username = strtok(path, "@");
	char *hostname = strtok(NULL, ":");
    char *passphrase = strtok(NULL, ":");
	char *filename = strtok(NULL, "");
    LIBSSH2_SFTP_ATTRIBUTES attrs;

	H5FD_ssh2_t* file = (H5FD_ssh2_t*) malloc (sizeof (H5FD_ssh2_t));
	SSH2_info* sshinfo;
	//unsigned long Rflags = LIBSSH2_FXF_READ;
	long	mode = 0755;

	int rc;
	unsigned long hostaddr;
	int sock, i, auth_pw = 0;
	struct sockaddr_in sin;

	//char	filename[2048] = "/home/wuyiyu/testfile";
	int	filename_len = strlen(filename);

	//file = (H5FD_ssh2_t* )malloc(sizeof(H5FD_ssh2_t) );

//LIBSSH2_FXF_WRITE
//LIBSSH2_FXF_CREAT
//LIBSSH2_FXF_TRUNC
//LIBSSH2_FXF_EXCL

#ifdef WIN32
	WSADATA wsadata;

	WSAStartup(MAKEWORD(2,0), &wsadata);
#endif
	//////////////////////////////////////////////////////////////////////////
	hostaddr = inet_addr(hostname);
	sock = socket(AF_INET, SOCK_STREAM, 0);

	sin.sin_family = AF_INET;
	sin.sin_port = htons(22);
	sin.sin_addr.s_addr = hostaddr;
	if (connect(sock, (struct sockaddr*)(&sin),
		sizeof(struct sockaddr_in)) != 0) {
			perror("error");
			fprintf(stderr, "failed to connect!\n");
			exit(-1);
	}
	//////////////////////////////////////////////////////////////////////////

	sshinfo = (SSH2_info*)H5Pget_driver_info( fapl );

	if (!sshinfo)
	{
		sshinfo = (SSH2_info*) malloc(sizeof(SSH2_info));
		sshinfo->session=NULL;
	}


	if (!sshinfo->session)
	{

	  //fprintf(stderr, "****************81\n");
		sshinfo->session = libssh2_session_init();
		libssh2_session_set_blocking(sshinfo->session, 1);

		rc = libssh2_session_startup(sshinfo->session, sock);
		if(rc) {
			fprintf(stderr, "Failure establishing SSH session: %d\n", rc);
			perror("error");
			return NULL;
		}
//	libssh2_session_startup(LIBSSH2_SESSION *session, int socket);

	// http://www.libssh2.org/wiki/index.php/Libssh2_userauth_publickey_fromfile_ex
		/*if (libssh2_userauth_publickey_fromfile(sshinfo->session, username, publickey, privatekey, passphrase) <0 )
		{
			puts("NO auth!");
			return 0;
		}*/
		if (libssh2_userauth_password(sshinfo->session, username, passphrase)) {
			puts("NO auth!");
			return NULL;
		} else {
			//puts("auth!");
		}
	}

	if (libssh2_userauth_authenticated( sshinfo->session ) )
	{
		//puts("YAY!");
	}
	else
	{
		puts("NOPE!");
		return NULL;
	}

	//fprintf(stderr, "libssh2_sftp_init()!\n");
	file->sftp = libssh2_sftp_init(sshinfo->session);

	// http://www.libssh2.org/wiki/index.php/Libssh2_sftp_open_ex
	file->handle = libssh2_sftp_open_ex(file->sftp, filename, filename_len,
					    flags, mode, LIBSSH2_SFTP_OPENFILE );


	if (!file->handle) {
		fprintf(stderr, "Unable to open file with SFTP\n");
	}
	//fprintf(stderr, "libssh2_sftp_open() is done, now send data!\n");

    ////
    libssh2_sftp_fstat_ex(file->handle, &attrs, 0);
    //printf("size of the file = %d\n", attrs.filesize);
    ////
	file->op = OP_UNKNOWN;
	file->pos=0;
    file->eof = attrs.filesize;

	H5FD_t* tmpFile = (H5FD_t*)file;
	tmpFile->maxaddr = maxaddr;
    free(path);

    //printf("leaving H5FD_ssh2_open\n");
	return tmpFile;
}

// The function which ends access to a file.
herr_t   H5FD_ssh2_close(H5FD_t* file_)
{
    //printf("entering H5FD_ssh2_close\n");
	H5FD_ssh2_t *file = (H5FD_ssh2_t*)file_;
    if(!(file->handle)) printf("empty\n");
	libssh2_sftp_close(file->handle);
	libssh2_sftp_shutdown( file->sftp);

	free(file);
	//fprintf(stderr, "all done\n");
    //printf("leaving H5FD_ssh2_close\n");

	return 0;
}


// A function to query how much of the format address space has been allocated.
haddr_t	H5FD_ssh2_get_eoa (const H5FD_t *file_, H5FD_mem_t type)
{
    //printf("entering H5FD_ssh2_get_eoa\n");
	// http://www.libssh2.org/wiki/index.php/Libssh2_sftp_tell
	//libssh2_sftp_tell - get the current read/write position indicator for a file
	//size_t libssh2_sftp_tell(LIBSSH2_SFTP_HANDLE *handle);
	const H5FD_ssh2_t *file = (const H5FD_ssh2_t *)file_;
    haddr_t ret_value;

    //printf("libssh2_sftp_tell() = %d\n", libssh2_sftp_tell(file->handle));
    ret_value = file->eoa;
    //printf("file->eoa = %d\n", ret_value);
    //printf("leaving H5FD_ssh2_get_eoa\n");
	return ret_value;
}

// A function to set the end of address space.
herr_t H5FD_ssh2_set_eoa(H5FD_t *file_, H5FD_mem_t type, haddr_t a)
{
    //printf("entering H5FD_ssh2_set_eoa\n");
	H5FD_ssh2_t *file = (H5FD_ssh2_t *)file_;
	file->eoa = a;

    //printf("leaving H5FD_ssh2_set_eoa\n");
	return 0;
}

// A function to return the current end-of-file marker value.
haddr_t H5FD_ssh2_get_eof(const H5FD_t *file_)
{
    //printf("entering H5FD_ssh2_get_eof\n");
	// http://www.libssh2.org/wiki/index.php/Libssh2_sftp_tell
	//libssh2_sftp_tell - get the current read/write position indicator for a file
	//size_t libssh2_sftp_tell(LIBSSH2_SFTP_HANDLE *handle);
	    H5FD_ssh2_t *file = (H5FD_ssh2_t *)file_;
	if (file->eoa>file->eof) {
        //if (-1==libssh2_sftp_seek(file->handle, file->eoa-1)) return -1;
		libssh2_sftp_seek(file->handle, file->eoa);
		if (file->handle == NULL)
		{
			return -1;
		}
        if (libssh2_sftp_write(file->handle, "", 1)!=1) return -1;
        (file->eof) = file->eoa;
    }
    //printf("leaving H5FD_ssh2_get_eof\n");
	return file->eof;
}

// A function to read data from a file.

// http://www.hdfgroup.org/HDF5/doc/TechNotes/VFL.html#SEC20

//herr_t H5FD_ssh2_read(H5FD_t *file, H5FD_mem_t type, hid_t dxpl, haddr_t addr, hsize_t size, void *buffer)
herr_t H5FD_ssh2_read(H5FD_t *file_, H5FD_mem_t type, hid_t dxpl, haddr_t addr, size_t size, void *buffer)
{
    //printf("entering H5FD_ssh2_read\n");
	size_t 	got;
    int fsize = (int)size;
	H5FD_ssh2_t *file = (H5FD_ssh2_t *)file_;
    assert(buffer);
	/* Check for overflow conditions */
    //if (REGION_OVERFLOW(addr, size)) return -1;
    //
    if (addr+size>file->eof)
    {
        printf("SSH VFD: file address overflowed\n");
        return -1;
    }

    //libssh2_sftp_seek(file->handle, addr);
	//got = libssh2_sftp_read(File->handle, buffer, size);
    //    buffer = malloc(sizeof(char));
	while (fsize>0){
    	char mem[1024];
        memset(mem, 0, 1024);
		// http://www.libssh2.org/wiki/index.php/Libssh2_sftp_seek
		libssh2_sftp_seek(file->handle, addr);
        got = libssh2_sftp_read(file->handle, mem, 1024);

        if (got < 0)
        {
        	/* error */
        	puts("ERROR!");
        	return -1;
		}
        if (got < sizeof(mem)) {
            /* end of file but not end of format address space */
            fsize = 0;
        }


        if (fsize < got)
        {
            memcpy(buffer, mem, fsize);
            buffer = (char*)buffer + fsize;

            addr += fsize;

        }
        else
        {
            memcpy(buffer, mem, got);
            buffer = (char*)buffer + got;
            addr += got;

        }
        fsize -= got;

    }

    file->pos = addr;
    file->eoa = addr;
    file->op = OP_READ;

    //printf("file->eoa = %d\n", file->eoa);
    //printf("leaving H5FD_ssh2_read\n");
	return 0;
}

// A function to write data to a file.
//herr_t H5FD_ssh2_write(H5FD_t *file, H5FD_mem_t type, hid_t dxpl, haddr_t addr, hsize_t size, const void *buffer)
herr_t H5FD_ssh2_write(H5FD_t *file_, H5FD_mem_t type, hid_t dxpl, haddr_t addr, size_t size, const void *buffer)
{
    //printf("entering H5FD_ssh2_write\n");
	int got;
	char *buf = (char *)buffer;
	H5FD_ssh2_t *file = (H5FD_ssh2_t *)file_;
	do
	{
		libssh2_sftp_seek(file->handle, addr);
		got = libssh2_sftp_write(file->handle, buf, size);
		if (got < 0) return -1;

		buffer = (char*)buffer + got;
		size -= got;
		addr += got;
	} while (size);

	file->pos = addr;
	file->eof = addr;
	if(addr > file->eoa) file->eoa = addr;
	file->op = OP_WRITE;

    //printf("leaving H5FD_ssh2_write\n");
	return 0;
}

//herr_t H5FD_ssh2_flush(H5FD_t *file)
herr_t H5FD_ssh2_flush(H5FD_t *file_,  hid_t dxpl_id, unsigned closing)
{
    //printf("entering H5FD_ssh2_flush\n");
	H5FD_ssh2_t *file = (H5FD_ssh2_t *)file_;
    if (file->eoa>file->eof) {
        //if (-1==libssh2_sftp_seek(File->handle, file->eoa-1)) return -1;
		libssh2_sftp_seek(file->handle, file->eoa-1);
		if(NULL == file->handle) return -1;
        if (libssh2_sftp_write(file->handle, "", 1)!=1) return -1;
        file->eof = file->eoa;
        file->pos = file->eoa;
        file->op = OP_WRITE;
    }

    //printf("leaving H5FD_ssh2_flush\n");
	return 0;
}

herr_t  H5FD_ssh2_get_handle(H5FD_t *file, hid_t fapl, void**file_handle)
{
    //printf("entering H5FD_ssh2_get_handle\n");
    //printf("leaving H5FD_ssh2_get_handle\n");
	return 0;
}


hid_t	registerHDF5VFDSSH2()
{
    //printf("entering registerHDF5VFDSSH2\n");
	if (!H5FD_VFD_id)
	{
		H5FD_VFD_id = H5FDregister(&DriverInfo);
		//printf("register end\n");
	}

    //printf("leaving registerHDF5VFDSSH2\n");
	return H5FD_VFD_id;
}
/*
int main()
{
	H5Fopenssh2();
	return 0;
	//cout<<"read\n";
	H5FD_mem_t type = H5FD_MEM_DEFAULT;
	H5FD_t *file = H5FD_ssh2_open("204.90.44.22:/home/wuyiyu/testfile", LIBSSH2_FXF_READ, H5FD_VFD_id, INT32_MAX);
	void* buf = calloc(1, 1024*sizeof(char));
	H5FD_ssh2_read(file, type, 0, 0, 1024*sizeof(char), buf);
	//cout<<"context of file: "<<(char*)buf<<endl;
	H5FD_ssh2_close(file);

	//cout<<"\n\nwrite\n";
	file = H5FD_ssh2_open("204.90.44.22:/home/wuyiyu/testfile", LIBSSH2_FXF_WRITE, H5FD_VFD_id, INT32_MAX);
	char* buf1 = "hello ssh2";
	if(-1 == H5FD_ssh2_write(file, type, 0, 0, strlen(buf1), buf1)) //cout<<"ERROR WRITING\n";
	H5FD_ssh2_close(file);

	//cout<<"\n\nread\n";
	file = H5FD_ssh2_open("204.90.44.22:/home/wuyiyu/testfile", LIBSSH2_FXF_READ, H5FD_VFD_id, INT32_MAX);
	H5FD_ssh2_read(file, type, 0, 0, 1024*sizeof(char), buf);
	//cout<<"context of file: "<<(char*)buf<<endl;
	H5FD_ssh2_close(file);
	return 0;

}
*/
