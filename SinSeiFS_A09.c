#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static const char *dirpath = "/home/daffainfo/Downloads";
static const char *logpath = "/home/daffainfo/SinSeiFS.log";

void encrypt1(char *path)
{
    //Encryptor nomer 1
}

void decrypt1(char *path)
{
    //Decryptor nomer 1
}

void encrypt2(char *path)
{
    //Encryptor nomer 2
}

void decrypt2(char *path)
{
    //Decryptor nomer 2
}

void encrypt3(char *path)
{
    //Encryptor nomer 3
}

void decrypt3(char *path)
{
    //Decryptor nomer 3
}

void writeLog(char *level, char *cmd_desc)
{
    FILE *fp;
    fp = fopen(logpath, "a+");

    time_t rawtime = time(NULL);

    struct tm *info = localtime(&rawtime);

    char time[100];
    strftime(time, 100, "%d%m%y-%H:%M:%S", info);

    char log[100];
    sprintf(log, "%s::%s::%s\n", level, time, cmd_desc);
    fputs(log, fp);

    fclose(fp);
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char temp[1000];

    strcpy(temp, path);

    if (strcmp(path, "/AtoZ_") == 0)
    {
        decrypt1(temp);
    }
    else if (strcmp(path, "/RX_") == 0)
    {
        decrypt2(temp);
    }
    else if (strcmp(path, "/A_is_a_") == 0)
    {
        decrypt3(temp);
    }

    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, temp);

    res = lstat(fpath, stbuf);

    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
    {
        char temp[1000];
        strcpy(temp, path);
        if (strcmp(path, "/AtoZ_") == 0)
        {
            decrypt1(temp);
        }
        else if (strcmp(path, "/RX_") == 0)
        {
            decrypt2(temp);
        }
        else if (strcmp(path, "/A_is_a_") == 0)
        {
            decrypt3(temp);
        }
        sprintf(fpath, "%s%s", dirpath, temp);
    }

    int res = 0;
    DIR *dp;
    struct dirent *de;

    (void)offset;
    (void)fi;

    dp = opendir(fpath);
    if (dp == NULL)
        return -errno;

    while ((de = readdir(dp)) != NULL)
    {
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
            continue;

        struct stat st;

        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        char tmp[1000];
        strcpy(tmp, de->d_name);
        if (strcmp(path, "/AtoZ_") == 0)
        {
            encrypt1(tmp);
        }
        else if (strcmp(path, "/RX_") == 0)
        {
            encrypt2(tmp);
        }
        else if (strcmp(path, "/A_is_a_") == 0)
        {
            encrypt3(tmp);
        }
        res = (filler(buf, tmp, &st, 0));

        if (res != 0)
            break;
    }

    closedir(dp);
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
    {
        char temp[1000];
        strcpy(temp, path);
        if (strcmp(path, "/AtoZ_") == 0)
        {
            decrypt1(temp);
        }
        else if (strcmp(path, "/RX_") == 0)
        {
            decrypt2(temp);
        }
        else if (strcmp(path, "/A_is_a_") == 0)
        {
            decrypt3(temp);
        }
        sprintf(fpath, "%s%s", dirpath, temp);
    }

    int res = 0;
    int fd = 0;

    (void)fi;

    fd = open(fpath, O_RDONLY);

    if (fd == -1)
        return -errno;

    res = pread(fd, buf, size, offset);
    if (res == -1)
        res = -errno;

    close(fd);

    char desc[128];
    sprintf(desc, "READ::%s", fpath);
    writeLog("INFO", desc);

    return res;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
    {
        char temp[1000];
        strcpy(temp, path);
        if (strcmp(path, "/AtoZ_") == 0)
        {
            decrypt1(temp);
        }
        else if (strcmp(path, "/RX_") == 0)
        {
            decrypt2(temp);
        }
        else if (strcmp(path, "/A_is_a_") == 0)
        {
            decrypt3(temp);
        }
        sprintf(fpath, "%s%s", dirpath, temp);
    }

    int fd;
    int res;

    (void)fi;
    fd = open(fpath, O_WRONLY);
    if (fd == -1)
        return -errno;

    res = pwrite(fd, buf, size, offset);
    if (res == -1)
        res = -errno;

    close(fd);

    char desc[128];
    sprintf(desc, "WRITE::%s", fpath);
    writeLog("INFO", desc);

    return res;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
    char fpath[1000];
    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
    {
        char temp[1000];
        strcpy(temp, path);
        if (strcmp(path, "/AtoZ_") == 0)
        {
            decrypt1(temp);
        }
        else if (strcmp(path, "/RX_") == 0)
        {
            decrypt2(temp);
        }
        else if (strcmp(path, "/A_is_a_") == 0)
        {
            decrypt3(temp);
        }
        sprintf(fpath, "%s%s", dirpath, temp);
    }
    int res;

    res = open(fpath, fi->flags);
    if (res == -1)
        return -errno;

    close(res);

    char desc[128];
    sprintf(desc, "OPEN::%s", fpath);
    writeLog("INFO", desc);

    return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
    char fpath[1000];
    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
    {
        char temp[1000];
        strcpy(temp, path);
        if (strcmp(path, "/AtoZ_") == 0)
        {
            decrypt1(temp);
        }
        else if (strcmp(path, "/RX_") == 0)
        {
            decrypt2(temp);
        }
        else if (strcmp(path, "/A_is_a_") == 0)
        {
            decrypt3(temp);
        }
        sprintf(fpath, "%s%s", dirpath, temp);
    }
    int res;

    res = truncate(fpath, size);
    if (res == -1)
        return -errno;

    char desc[128];
    sprintf(desc, "TRUNC::%s", fpath);
    writeLog("INFO", desc);

    return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
    char fpath[1000];
    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
    {
        char temp[1000];
        strcpy(temp, path);
        if (strcmp(path, "/AtoZ_") == 0)
        {
            decrypt1(temp);
        }
        else if (strcmp(path, "/RX_") == 0)
        {
            decrypt2(temp);
        }
        else if (strcmp(path, "/A_is_a_") == 0)
        {
            decrypt3(temp);
        }
        sprintf(fpath, "%s%s", dirpath, temp);
    }

    int res;
    /* On Linux this could just be 'mknod(path, mode, rdev)' but this
	   is more portable */
    if (S_ISREG(mode))
    {
        res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
        if (res >= 0)
            res = close(res);
    }
    else if (S_ISFIFO(mode))
        res = mkfifo(fpath, mode);
    else
        res = mknod(fpath, mode, rdev);
    if (res == -1)
        return -errno;

    char desc[128];
    sprintf(desc, "CREATE::%s", fpath);
    writeLog("INFO", desc);

    return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
    char fpath[1000];
    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
    {
        char temp[1000];
        strcpy(temp, path);
        if (strcmp(path, "/AtoZ_") == 0)
        {
            decrypt1(temp);
        }
        else if (strcmp(path, "/RX_") == 0)
        {
            decrypt2(temp);
        }
        else if (strcmp(path, "/A_is_a_") == 0)
        {
            decrypt3(temp);
        }
        sprintf(fpath, "%s%s", dirpath, temp);
    }
    int res;

    res = mkdir(fpath, mode);
    if (res == -1)
        return -errno;

    char desc[128];
    sprintf(desc, "MKDIR::%s", fpath);
    writeLog("INFO", desc);

    return 0;
}

static int xmp_rmdir(const char *path)
{
    char fpath[1000];
    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
    {
        char temp[1000];
        strcpy(temp, path);
        if (strcmp(path, "/AtoZ_") == 0)
        {
            decrypt1(temp);
        }
        else if (strcmp(path, "/RX_") == 0)
        {
            decrypt2(temp);
        }
        else if (strcmp(path, "/A_is_a_") == 0)
        {
            decrypt3(temp);
        }
        sprintf(fpath, "%s%s", dirpath, temp);
    }
    int res;

    res = rmdir(fpath);
    if (res == -1)
        return -errno;

    char desc[128];
    sprintf(desc, "RMDIR::%s", fpath);
    writeLog("WARNING", desc);

    return 0;
}

static int xmp_rename(const char *from, const char *to)
{
    char c_from[1000];
    char c_to[1000];
    if (strcmp(from, "/") == 0)
    {
        from = dirpath;
        sprintf(c_from, "%s", from);
    }
    else
    {
        char temp[1000];
        strcpy(temp, from);
        if (strcmp(from, "/AtoZ_") == 0)
        {
            decrypt1(temp);
        }
        else if (strcmp(from, "/RX_") == 0)
        {
            decrypt2(temp);
        }
        else if (strcmp(from, "/A_is_a_") == 0)
        {
            decrypt3(temp);
        }
        sprintf(c_from, "%s%s", dirpath, temp);
    }

    if (strcmp(to, "/") == 0)
    {
        to = dirpath;
        sprintf(c_to, "%s", to);
    }
    else
    {
        char temp[1000];
        strcpy(temp, to);
        if (strcmp(to, "/AtoZ_") == 0)
        {
            decrypt1(temp);
        }
        else if (strcmp(to, "/RX_") == 0)
        {
            decrypt2(temp);
        }
        else if (strcmp(to, "/A_is_a_") == 0)
        {
            decrypt3(temp);
        }
        sprintf(c_to, "%s%s", dirpath, temp);
    }

    int res;

    res = rename(c_from, c_to);
    if (res == -1)
        return -errno;

    char desc[128];
    sprintf(desc, "RENAME::%s::%s", c_from, c_to);
    writeLog("INFO", desc);

    return 0;
}

static int xmp_unlink(const char *path)
{
    char fpath[1000];
    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
    {
        char temp[1000];
        strcpy(temp, path);
        if (strcmp(path, "/AtoZ_") == 0)
        {
            decrypt1(temp);
        }
        else if (strcmp(path, "/RX_") == 0)
        {
            decrypt2(temp);
        }
        else if (strcmp(path, "/A_is_a_") == 0)
        {
            decrypt3(temp);
        }
        sprintf(fpath, "%s%s", dirpath, temp);
    }
    int res;

    res = unlink(fpath);
    if (res == -1)
        return -errno;

    char desc[128];
    sprintf(desc, "UNLINK::%s", fpath);
    writeLog("WARNING", desc);

    return 0;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .mknod = xmp_mknod,
    .mkdir = xmp_mkdir,
    .unlink = xmp_unlink,
    .rmdir = xmp_rmdir,
    .rename = xmp_rename,
    .truncate = xmp_truncate,
    .open = xmp_open,
    .read = xmp_read,
    .write = xmp_write,
};

int main(int argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
