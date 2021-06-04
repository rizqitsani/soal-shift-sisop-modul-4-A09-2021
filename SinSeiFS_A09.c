#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/wait.h>
#define SEGMENT 1024

static const char *dirPath = "/home/daffainfo/Downloads";

char kode1[10] = "AtoZ_";
char kode2[10] = "RX_";
char kode3[10] = "A_is_a_";

void WriteLog(char *c, int type)
{
	FILE *logFile = fopen("/home/daffainfo/SinSeiFS.log", "a+");
	time_t currTime;
	struct tm *time_info;
	time(&currTime);
	time_info = localtime(&currTime);
	int yr = time_info->tm_year, mon = time_info->tm_mon, day = time_info->tm_mday, hour = time_info->tm_hour, min = time_info->tm_min, sec = time_info->tm_sec;
	if (type == 1)
	{ //info
		fprintf(logFile, "INFO::%d%d%d-%d:%d:%d::%s\n", yr, mon, day, hour, min, sec, c);
	}
	else if (type == 2)
	{ //warning
		fprintf(logFile, "WARNING::%d%d%d-%d:%d:%d::%s\n", yr, mon, day, hour, min, sec, c);
	}
	fclose(logFile);
}

void encrypt1(char *str)
{
	//encrypt AtoZ_
}

void decrypt1(char *str)
{
	//decrypt AtoZ_
}

void encrypt2(char *str)
{
	//encrypt RX_
}

void decrypt2(char *str)
{
	//decrypt RX_
}

void encrypt3(char *str)
{
	//encrypt A_is_a_
}

void decrypt3(char *str)
{
	//decrypt A_is_a_
}

long file_size(char *name)
{
	FILE *fp = fopen(name, "rb"); //must be binary read to get bytes
	long size = -1;
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		size = ftell(fp) + 1;
		fclose(fp);
	}
	return size;
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	char *enc1 = strstr(path, kode1);
	if (enc1 != NULL)
	{
		decrypt1(enc1);
	}
	char *enc2 = strstr(path, kode2);
	if (enc2 != NULL)
	{
		decrypt2(enc2);
	}
	char *enc3 = strstr(path, kode3);
	if (enc3 != NULL)
	{
		decrypt2(enc3);
	}
	char newPath[1000];
	int res;
	sprintf(newPath, "%s%s", dirPath, path);
	res = lstat(newPath, stbuf);
	if (res == -1)
		return -errno;
	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
	char *enc1 = strstr(path, kode1);
	if (enc1 != NULL)
	{
		decrypt1(enc1);
	}
	char *enc2 = strstr(path, kode2);
	if (enc2 != NULL)
	{
		decrypt2(enc2);
	}
	char *enc3 = strstr(path, kode3);
	if (enc3 != NULL)
	{
		decrypt2(enc3);
	}

	char newPath[1000];
	if (strcmp(path, "/") == 0)
	{
		path = dirPath;
		sprintf(newPath, "%s", path);
	}
	else
		sprintf(newPath, "%s%s", dirPath, path);

	int res = 0;
	struct dirent *dir;
	DIR *dp;
	(void)fi;
	(void)offset;
	dp = opendir(newPath);
	if (dp == NULL)
		return -errno;

	while ((dir = readdir(dp)) != NULL)
	{ //rekursif
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = dir->d_ino;
		st.st_mode = dir->d_type << 12;
		if (enc1 != NULL)
		{
			encrypt1(dir->d_name);
		}
		if (enc2 != NULL)
		{
			encrypt2(dir->d_name, enc2);
		}
		if (enc3 != NULL)
		{
			encrypt2(dir->d_name, enc3);
		}
		res = (filler(buf, dir->d_name, &st, 0));
		if (res != 0)
			break;
	}

	closedir(dp);
	return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{

	char newPath[1000];
	if (strcmp(path, "/") == 0)
	{
		path = dirPath;
		sprintf(newPath, "%s", path);
	}
	else
		sprintf(newPath, "%s%s", dirPath, path);

	int res = mkdir(newPath, mode);
	char str[100];
	sprintf(str, "MKDIR::%s", path);
	WriteLog(str, 1);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{

	char newPath[1000];
	if (strcmp(path, "/") == 0)
	{
		path = dirPath;
		sprintf(newPath, "%s", path);
	}
	else
		sprintf(newPath, "%s%s", dirPath, path);
	int res;

	if (S_ISREG(mode))
	{
		res = open(newPath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	}
	else if (S_ISFIFO(mode))
		res = mkfifo(newPath, mode);
	else
		res = mknod(newPath, mode, rdev);
	char str[100];
	sprintf(str, "CREAT::%s", path);
	WriteLog(str, 1);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_unlink(const char *path)
{
	char *enc1 = strstr(path, kode1);
	if (enc1 != NULL)
	{
		decrypt1(enc1); //buat ngebalikin biar bisa dibaca di document
	}
	char *enc2 = strstr(path, kode2);
	if (enc2 != NULL)
	{
		decrypt2(enc2);
	}
	char *enc3 = strstr(path, kode3);
	if (enc3 != NULL)
	{
		decrypt2(enc3);
	}

	char newPath[1000];
	if (strcmp(path, "/") == 0)
	{
		path = dirPath;
		sprintf(newPath, "%s", path);
	}
	else
		sprintf(newPath, "%s%s", dirPath, path);
	char str[100];
	sprintf(str, "REMOVE::%s", path);
	WriteLog(str, 2);
	int res;
	res = unlink(newPath);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rmdir(const char *path)
{
	char *enc1 = strstr(path, kode1);
	if (enc1 != NULL)
	{
		decrypt1(enc1); //buat ngebalikin biar bisa dibaca di document
	}
	char *enc2 = strstr(path, kode2);
	if (enc2 != NULL)
	{
		decrypt2(enc2);
	}
	char *enc3 = strstr(path, kode3);
	if (enc3 != NULL)
	{
		decrypt2(enc3);
	}

	char newPath[1000];
	sprintf(newPath, "%s%s", dirPath, path);
	char str[100];
	sprintf(str, "RMDIR::%s", path);
	WriteLog(str, 2);
	int res;
	res = rmdir(newPath);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rename(const char *from, const char *to)
{
	char fileFrom[1000], fileTo[1000];
	sprintf(fileFrom, "%s%s", dirPath, from);
	sprintf(fileTo, "%s%s", dirPath, to);

	char str[100];
	sprintf(str, "RENAME::%s::%s", from, to);
	WriteLog(str, 1);
	int res;
	res = rename(fileFrom, fileTo);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{ 
	char newPath[1000];
	sprintf(newPath, "%s%s", dirPath, path);
	int res;
	res = open(newPath, fi->flags);
	if (res == -1)
		return -errno;
	close(res);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{

	char newPath[1000];
	sprintf(newPath, "%s%s", dirPath, path);
	int fd;
	int res;

	(void)fi;
	fd = open(newPath, O_RDONLY);
	if (fd == -1)
		return -errno;
	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;
	close(fd);
	return res;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{

	char newPath[1000];
	sprintf(newPath, "%s%s", dirPath, path);

	int fd;
	int res;
	(void)fi;
	fd = open(newPath, O_WRONLY);
	if (fd == -1)
		return -errno;
	char str[100];
	sprintf(str, "WRITE::%s", path);
	WriteLog(str, 1);
	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;
	close(fd);
	return res;
}

static struct fuse_operations xmp_oper = {

	.getattr = xmp_getattr,
	.readdir = xmp_readdir,
	.read = xmp_read,
	.mkdir = xmp_mkdir,
	.mknod = xmp_mknod,
	.unlink = xmp_unlink,
	.rmdir = xmp_rmdir,
	.rename = xmp_rename,
	.open = xmp_open,
	.write = xmp_write,

};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}