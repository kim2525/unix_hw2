#include<cstdio>
#include<stdarg.h>
#include<fcntl.h>
#include <sys/stat.h>
#include<cstdlib>
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>
#include<cstring>
#include<unistd.h>
using namespace std;
typedef DIR *(*old_open_dir)(const char *name);
typedef int (*old_chdir)(const char *name);
typedef int (*old_chmod)(const char*name, mode_t mode);
typedef int (*old_chown)(const char*name, uid_t owner, gid_t group);
typedef FILE *(*old_fopen)(const char*name, const char*mode);
typedef int (*old_link)(const char *oldpath, const char* newpath);
typedef int (*old_creat)(const char *name,mode_t mode);
typedef int (*old_mkdir)(const char *name, mode_t mode);
typedef int (*old_open)(const char *name, int flags);
typedef int (*old_open0)(const char *name, int flags, mode_t mode);
typedef int (*old_openat)(int dirfd, const char *name, int flags);
typedef int (*old_openat0)(int dirfd, const char *name, int flags, mode_t mode);
typedef ssize_t (*old_readlink)(const char *name, char *buf, size_t bufsiz);
typedef int (*old_remove)(const char *name);
typedef int (*old_rename)(const char *oldpath, const char *newpath);
typedef int (*old_rmdir)(const char *name);
typedef int (*old_symlink)(const char *target, const char *linkpath);
typedef int (*old_unlink)(const char *name);
typedef int (*old_xstat)(int ver, const char *path, struct stat *stat_buf);
char limit[1000];
char *bg;
int see_in_dir(const char *place, const char *name)
{
	char real_name[100];
	bg = getenv("dir");
	realpath(bg,limit);
	realpath(name,real_name);
	if(strncmp(real_name, limit, strlen(limit)) != 0) 
	{
		printf("[sandbox] %s: access to %s is not allowed\n", place,name);
		return 0;
	}
	return 1;
}
DIR *opendir(const char *name)
{
	if(see_in_dir("opendir",name) == 0) return NULL;
	old_open_dir orig_open;
	orig_open = (old_open_dir)dlsym(RTLD_NEXT,"opendir");
    	return orig_open(name);
}	
int chdir(const char* name)
{		
	if(see_in_dir("chdir",name) == 0) return -1;
	old_chdir orig_open;
	orig_open = (old_chdir) dlsym(RTLD_NEXT,"chdir");
	return orig_open(name);
}
int chmod(const char* name, mode_t mode)
{
	if(see_in_dir("chmod",name) == 0) return -1;
	old_chmod orig_open;
	orig_open = (old_chmod) dlsym(RTLD_NEXT,"chmod");
	return orig_open(name, mode);
}
int chown(const char *name, uid_t owner, gid_t group)
{
	if(see_in_dir("chown",name) == 0) return -1;
	old_chown orig_open;
	orig_open = (old_chown) dlsym(RTLD_NEXT,"chown");
	return orig_open(name, owner, group);
}
FILE *fopen(const char*name, const char*mode)
{
	if(see_in_dir("fopen",name) == 0) return NULL;
	old_fopen orig_open;
	orig_open = (old_fopen) dlsym(RTLD_NEXT,"fopen");
	return orig_open(name, mode);
}
int link(const char*oldpath, const char *newpath)
{
	if(see_in_dir("link",oldpath) == 0) return -1;
	else if(see_in_dir("link",newpath) == 0) return -1;
	old_link orig_open;
	orig_open = (old_link) dlsym(RTLD_NEXT,"link");
	return orig_open(oldpath, newpath);
}
int creat(const char* name, mode_t mode)
{
	if(see_in_dir("creat",name) == 0) return -1;
	old_creat orig_open;
	orig_open = (old_creat) dlsym(RTLD_NEXT,"creat");
	return orig_open(name, mode);
}
int mkdir(const char *name, mode_t mode)
{
	if(see_in_dir("mkdir",name) == 0) return -1;
	old_mkdir orig_open;
	orig_open = (old_mkdir) dlsym(RTLD_NEXT,"mkdir");
	return orig_open(name, mode);
}
int open(const char *name, int flags, ...)
{
	va_list ap;
	int args;
	int argn = 0;
	int flag;
	va_start(ap, flags);
	flag = flags;
	mode_t mode;
	mode = va_arg(ap, mode_t);
	va_end(ap);
	if(see_in_dir("open",name) == 0) return -1;
	old_open orig_open;
	old_open0 orig_open0;
	orig_open = (old_open) dlsym(RTLD_NEXT,"open");
	orig_open0 = (old_open0) dlsym(RTLD_NEXT,"open");
	if(mode>=0 && mode <=777)  return orig_open0(name, flag, mode);
	else return orig_open(name,flag);
}
int openat(int dirfd, const char *name, int flags, ...)
{
	va_list ap;
	int args;
	int argn = 0;
	int flag;
	va_start(ap, flags);
	flag = flags;
	mode_t mode;
	mode = va_arg(ap, mode_t);
	va_end(ap);
	if(see_in_dir("openat",name) == 0) return -1;
	old_openat orig_open;
	orig_open = (old_openat) dlsym(RTLD_NEXT,"openat");
	old_openat0 orig_open0;
	orig_open0 = (old_openat0) dlsym(RTLD_NEXT,"openat");
	if(mode>=0 && mode <=777)  return orig_open0(dirfd, name, flag, mode);
	else return orig_open(dirfd,name,flag);
}
ssize_t readlink(const char *name, char *buf, size_t bufsiz)
{
	if(see_in_dir("readlink",name) == 0) return -1;
	old_readlink orig_open;
	orig_open = (old_readlink) dlsym(RTLD_NEXT,"readlink");
	return orig_open(name, buf, bufsiz);
}
int remove(const char *name)
{
	if(see_in_dir("remove",name) == 0) return -1;
	old_remove orig_open;
	orig_open = (old_remove) dlsym(RTLD_NEXT,"remove");
	return orig_open(name);
}
int rename(const char *oldpath, const char *newpath)
{
	if(see_in_dir("rename",oldpath) == 0) return -1;
	else if(see_in_dir("rename",newpath) == 0) return -1;
	old_rename orig_open;
	orig_open = (old_rename) dlsym(RTLD_NEXT,"rename");
	return orig_open(oldpath, newpath);
}
int rmdir(const char *name)
{
	if(see_in_dir("rmdir",name) == 0) return -1;
	old_rmdir orig_open;
	orig_open = (old_rmdir) dlsym(RTLD_NEXT,"rmdir");
	return orig_open(name);
}
int symlink(const char *target, const char *linkpath)
{
	if(see_in_dir("symlink",target) == 0) return -1;
	else if(see_in_dir("symlink",linkpath) == 0) return -1;
	old_symlink orig_open;
	orig_open = (old_symlink) dlsym(RTLD_NEXT,"symlink");
	return orig_open(target, linkpath);
}
int unlink(const char *name)
{
	if(see_in_dir("unlink",name) == 0) return -1;
	old_unlink orig_open;
	orig_open = (old_unlink) dlsym(RTLD_NEXT,"unlink");
	return orig_open(name);
}
int execl(const char *path, const char *arg, ...)
{
	printf("[sandbox] execl(%s): not allowed\n",arg);
}
int execlp(const char *path, const char *arg, ...)
{
	printf("[sandbox] execlp(%s): not allowed\n",arg);
}
int execle(const char *path, const char *arg, ...)
{
	printf("[sandbox] execle(%s): not allowed\n",arg);
}
int execv(const char *path, char *const argv[])
{
	printf("[sandbox] execv(%s): not allowed\n",argv[0]);
}
int execvp(const char *file, char *const argv[])
{
	printf("[sandbox] execvp(%s): not allowed\n",argv[0]);
}
int execve(const char *filename, char *const argv[], char *const envp[])
{
	printf("[sandbox] execve(%s): not allowed\n",argv[0]);
}
int system(const char *command)
{
	printf("[sandbox] system(%s): not allowed\n",command);
	return 0;
}
int __xstat(int ver, const char *name, struct stat *stat_buf)
{
	if(see_in_dir("__xstat",name) == 0) return -1;
	old_xstat orig_open;
	orig_open = (old_xstat) dlsym(RTLD_NEXT,"__xstat");
	return orig_open(ver,name,stat_buf);
}
