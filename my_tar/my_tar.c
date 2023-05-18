
#include "tar_c_.h"


void null_fill_(char *plan, int s_size)
{
    int d = 0;
    while(d < s_size)
    {
        plan[d++] = 0;
    }
}

void reverstring(char *s)  
{  
    int n, size, mode;  
    size = strlen(s);  
    n = 0;  
    while (n < mode/2)
    {  
        mode = s[n];  
        s[n] = s[size - n - 1];  
        s[size - n - 1] = mode;
        n++;  
    }  
}

void int_tar(tar_struct* tar)
{
    null_fill_(tar->mtime, sizeof(tar->mtime));
    null_fill_(tar->name, sizeof(tar->name));
    null_fill_(tar->mode, sizeof(tar->mode));
    null_fill_(tar->uid, sizeof(tar->uid));
    null_fill_(tar->gid, sizeof(tar->gid));
    null_fill_(tar->tes, sizeof(tar->tes));
    null_fill_(tar->size, sizeof(tar->size));
    null_fill_(tar->chksum, sizeof(tar->chksum));
    null_fill_(tar->linkname, sizeof(tar->linkname));
    null_fill_(tar->magic, sizeof(tar->magic));
    null_fill_(tar->version, sizeof(tar->version));
    null_fill_(tar->uname, sizeof(tar->uname));
    null_fill_(tar->gname, sizeof(tar->gname));
    null_fill_(tar->devmajor, sizeof(tar->devmajor));
    null_fill_(tar->devminor, sizeof(tar->devminor));
    null_fill_(tar->prefix, sizeof(tar->prefix));
    tar->typeflag = 0;
}

void itoa(int num, char c[])
{
    int i, sign;
    if ((sign = num) < 0)  
        num = -num;          
    i = 0;
    do
    {       
        c[i++] = num % 10 + '0';   
    } 
    while ((num /= 10) > 0);     
    if (sign < 0)
        c[i++] = '-';
    c[i] = '\0';
    reverstring(c);
}
bool equals(char* txt, char* txt2)
{
    int i = 0;
    while( txt[i]!=0)
    {
        if(txt[i] != txt2[i])
        {
            return false;
        }
        if(txt2[i] == '\0')
        {
           return false;
        }
        i++;
    }
    if(txt2[i] != '\0')
        return false;
    return true;
}

int check_sum(char *arg)
{
    int n = 0;
    for(int i=0; arg[i]!='\0'; i++)
        n += arg[i];
    // printf(" ----- %d -----\n", n);
    return n;
}

void my_putstring(char* str)
{
    for(int i = 0; str[i] != 0; i++)
    {
        write(1, &str[i], 1);
    }
    write(1, "\n", 1);
}
long my_power(int bn, int power)
{
  int	p;
  long	result;
  p = 1;
  result = 1;
  if (power < 0)
  {
    return (0);
  }
  if (power == 0)
  {
    return (1);
  }
  while (p <= power)
  {
      result = result * bn;
      p = p + 1;
    }
  return (result);
}
long octal_to_dec(int octalnum)
{
    long decimalnum = 0;
    int i = 0;
    // printf("octal : %d\n", octalNumber);
    while(octalnum != 0) {
        decimalnum += (octalnum % 10) * my_power(8,i);
        i++;
        octalnum /= 10;
    }
    i = 1;
    // printf("Size %ld\n", decimalNumber);
    return decimalnum;
} 

int decimal_to_octal(long int n, char* string)
{
    long int i = 0, m = n;
    int check = 0;
    while(m > 0)
    {
        string[i] = m % 8 + '0';
        check += string[i];
        i++;
        m = m / 8;
    }
    reverstring(string);
    return check;
}
int username(char* des, struct stat* fs)
{
    struct passwd *pd = getpwuid(fs->st_uid);
    strcpy(des, pd->pw_name);
    return check_sum(des);
}
int group__name(char* fet, struct stat* fps)
{
    struct group *grp = getgrgid(fps->st_gid);
    strcpy(fet, grp->gr_name);
    return check_sum(fet);
}
long _tar_fill(char* fname, tar_struct* tar)
{
    int_tar(tar);
    struct stat st;
    stat(fname, &st);
    static int chksum = 0;
    chksum += check_sum(fname);
    strcpy(tar->name, fname);
    chksum += decimal_to_octal(st.st_mode, tar->mode);
    chksum += decimal_to_octal(st.st_uid, tar->uid);
    chksum += decimal_to_octal(st.st_gid, tar->gid);
    chksum += decimal_to_octal(st.st_size, tar->size);
    chksum += decimal_to_octal(st.st_mtim.tv_sec, tar->mtime);
    tar->typeflag = '0';
    chksum += '0';
    chksum += decimal_to_octal(st.st_nlink, tar->linkname);
    strcpy(tar->magic, "ustar");
    chksum += check_sum("ustar");
    strcpy(tar->version, "  ");
    chksum += check_sum("  ");
    chksum += username(tar->uname, &st);
    chksum += group__name(tar->gname, &st);
    decimal_to_octal(chksum, tar->chksum);
    return st.st_size;
} 

void full_place(char* fname, char* ret)
{
    int td = open(fname, O_RDONLY);
    int i = 0;
    char b;
    while(read(td, &b, 1) > 0)
    {
        ret[i++] = b;
    }
    ret[i] = '\0';
    // printf("%s\n", des);
}

int __cf(int ac, char** av, int index)
{
    if(access( av[index], F_OK ) != -1)
    {
        if(remove(av[index])!=0)
        {
            printf("my_tar: cannot creat, %s already exist\n", av[index]);
        }
    }
    tar_struct tar;

    creat(av[index], 0000644);
    
    int fd = open(av[index], O_WRONLY | O_APPEND);
    if(fd == -1)
    {
        error_open(av[index]);
        exit(-1);
    }
    for(int i=index+1; i < ac; i++)
    {
        long size, content_size;
        content_size = _tar_fill(av[i], &tar);
        // printf("< - - %s\n", av[i]);
        size = (content_size / 513 + 1) * 512;
        char content[size];
        null_fill_(content, size);
        full_place(av[i], content);
        if(write(fd, &tar, sizeof(tar))  < 0)
        {
            return 1;
        } 
        if(write(fd, content, size) < 0)
        {
            return 1;
        };
        // return 1;
    };
    return 0;
}

int __rf(int ac, char**av, int index)
{

    tar_struct tar;
    // init_tar(&tar);
    int fd = open(av[index],O_WRONLY | O_APPEND);
    if(fd == -1)
    {
        error_open(av[2]);
        exit(-1);
    }
    int i=index+1;
    while( i < ac)
    {
        long size, content_size;
        content_size = _tar_fill(av[i], &tar);
        // printf("< - - %s\n", av[i]);
        size = (content_size / 513 + 1) * 512;
        char content[size];
        null_fill_(content, size);
        full_place(av[i], content);
        if(write(fd, &tar, sizeof(tar)) < 0 )
        {
            return 1;
        } 
        if(write(fd, content, size) < 0)
        {
            return 1;
        }
        i++;

    }
    return 0;
}

void read_tar(char* enter)
{
   int fd = open(enter, O_RDONLY);
   if(fd == -1) 
   {
       error_open(enter);
       exit(-1);
   }
   tar_struct tar;
   while(read(fd, &tar, sizeof(tar_struct)) > 0)
    {
        my_putstring(tar.name);
        long c_size = octal_to_dec(atoi(tar.size));
        long size = (c_size / 513 + 1) * 512;
        lseek(fd, size, 1);
    } 
}
int __tf(char**av, int ind)
{
    read_tar(av[ind]);
    return 0;
}

int __uf(char** av, int ind)
{
    
    int fd = open(av[ind], O_RDWR | O_APPEND);
    if(fd == -1)
    {
        printf("my_tar: %s: Cannot open: No such file or directory\n", av[2]);
        exit(1);
    }
    struct stat fs;
    if(stat(av[ind+1], &fs) == -1)
    {
        return printf("my_tar: %s: Cannot stat: No such file or directory\n", av[ind + 1]);
    };
    tar_struct tar;
    char inside_mtim[12];
    while(read(fd, &tar, sizeof(tar_struct)) > 0) 
    {
        if(equals(tar.name, av[ind+1]))
        {
        //     printf("file  %s - m.time %s\n", tar.name, tar.mtime);
            strcpy(inside_mtim, tar.mtime);
        }
        struct stat cs;
        stat(tar.name, &cs);
        long c_size = (cs.st_size / 513 + 1) * 512;
        lseek(fd, c_size, SEEK_CUR);
    }
    if(fs.st_mtim.tv_sec != octal_to_dec(atoi(inside_mtim)))
    {
                tar_struct new;
                _tar_fill(av[ind+1], &new);
                // printf("write file %s\n", new.name);
                long size = (fs.st_size / 513 + 1) * 512;
                char content[size];
                null_fill_(content, size);
                full_place(av[ind+1], content);
                if(write(fd, &new, sizeof(tar_struct)) < 0)
                {
                    return 1;
                } 
                if(write(fd, content, size) < 0)
                {
                    return 1;
                }
                return 0;
            }
    printf("my_tar: file not found");
    return 1;
}

int found_wr(char* nn, char* made, char* place, long length, long time)
{
    // printf("content size: %ld\n", c_size);
    creat(nn, octal_to_dec(atoi(made)));

    struct stat foot;
    // time_t mtime;
    struct utimbuf new_times;
    stat(nn, &foot);
    // mtime = foo.st_mtime; /* seconds since the epoch */
    new_times.actime = foot.st_atime; /* keep atime unchanged */
    new_times.modtime = time;    /* set mtime to current time */
    utime(nn, &new_times);
    /* set mod time to file */
    foot.st_mtim.tv_sec = time;

    int fd = open(nn, O_WRONLY);
    if(fd < 0)
    {
        error_open(nn);
        exit(-1);
    }
    if(write(fd, place, length) >= 0)
        return 0;
    return -1;

}

int __xf(char** av, int index)
{
    
    int fd = open(av[index], O_RDONLY);
    if(fd == -1)
    {
        error_open(av[index]);
        exit(-1);
    }
    tar_struct tar;
    while(read(fd, &tar, sizeof(tar_struct)) > 0)
    {
            long c_size = octal_to_dec(atoi(tar.size));
            char content[c_size];
            read(fd, content, c_size);
            found_wr(tar.name, tar.mode, content, c_size,
                octal_to_dec(atoi(tar.mtime)));
            long size = (c_size / 513 + 1) * 512 - c_size;

            lseek(fd, size, SEEK_CUR);
    } 
    return 0;
}