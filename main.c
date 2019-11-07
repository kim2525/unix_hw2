#include<cstdio>
#include<algorithm>
#include<cstdlib>
#include<unistd.h>
#include<cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

using namespace std;
int main(int argc, char **argv)
{
	int fp = open("/dev/tty",O_WRONLY);
	dup2(fp , STDERR_FILENO);
	int in_code = 0, opt;
	char* args[1000];
	int argno = 0;
	int pp = -1;
	int dp = -1;
	int mm = -1;
	int sandp = -1;
	setenv("sandplace","./sandbox.so",2);
	setenv("dir",".",1);
	char temp[100];
	strcpy(temp,getenv("dir"));
	for(int i = 1; i < argc; i++)
	{
		if(strcmp(argv[i],"-p") == 0 && pp == -1) pp = i;
		if(strcmp(argv[i],"-d") == 0 && dp == -1) dp = i;
		if(strcmp(argv[i],"--") == 0 && mm == -1) mm = i;
	}
	while ((opt = getopt(argc, argv, "d:p:")) != -1) 
	{
               switch (opt) {
               case 'p':
		   if(!(mm != -1 && optind - 1 > mm)) 
		   {
			   if((pp != 1 && dp != 1) || (dp == 1 && pp != 3))
			   {
			fprintf(stderr,"usage: ./sandbox [-p sopath] [-d basedir] [--] cmd [cmd args ...]\n-p: set the path to sandbox.so, default = ./sandbox.so\n-d: restrict directory, default = .\n--: seperate the arguments for sandbox and for the executed command\n");
				exit(1);
			   }
			   char real_name[100];
			   //realpath(argv[optind],real_name);
			   realpath(optarg,real_name);
			   strcat(real_name, "/sandbox.so");
			   setenv("sandplace",real_name,1);
			   //FILE *file = fopen(real_name,"r");
			   int file = open(real_name,O_RDONLY);
			   if (file== -1)
			   {
				   fprintf(stderr, "[sandbox] .so not found\n");
				   exit(1);
			   }
		   }
                   break;
               case 'd':
		   if(!(mm != -1 && optind - 1 > mm)) 
		   {
			   if((pp != 1 && dp != 1) || (pp == 1 && dp != 3))
			   {
			fprintf(stderr,"usage: ./sandbox [-p sopath] [-d basedir] [--] cmd [cmd args ...]\n-p: set the path to sandbox.so, default = ./sandbox.so\n-d: restrict directory, default = .\n--: seperate the arguments for sandbox and for the executed command\n");
			   exit(1);
			   }
			   char real_name[100];
			   setenv("dir",optarg,1);
			   realpath(optarg,real_name);
			   DIR* dir = opendir(real_name);
			   if (!dir)
			   {
				   fprintf(stderr, "[sandbox] dir not found\n");
				   exit(1);
			   }

		   }
		   break;
		default:
			fprintf(stderr,"usage: ./sandbox [-p sopath] [-d basedir] [--] cmd [cmd args ...]\n-p: set the path to sandbox.so, default = ./sandbox.so\n-d: restrict directory, default = .\n--: seperate the arguments for sandbox and for the executed command\n");
		   break;
               }
        }
	if(getenv("sandplace"))
	setenv("LD_PRELOAD",getenv("sandplace"),1);
	//printf("%s\n",getenv("dir"));
	int start = 1;
	if(mm != -1) start = mm + 1;
	else if(pp != -1 || dp != -1) start = max(pp,dp) + 2;
	for(int i = start; i < argc; i++) args[argno++] = argv[i];
	char ls_res[100];
	//char cmd[100] = "which ";
	//FILE *fp;
	//strcat(cmd,args[0]);
       	//fp = popen(cmd, "r");
	//if(fp != NULL)	fgets(ls_res,sizeof(ls_res)-1,fp);
	execvp(args[0],args);
	//int l = strlen(ls_res);
	//ls_res[l - 1] = '\0';
	//execv(ls_res,args);
}
