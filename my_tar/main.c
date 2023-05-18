#include "tar_c_.h"

int main(int acdv, char** argv)
{
    if (acdv > 2)
    {
        if(equals(argv[1], CF))
        {
            goto go_cf;
        }
        if(equals(argv[1], RF))
        {
            goto go_rf;
        }
        if(equals(argv[1], TF))
        {
            goto go_tf;
        }
        if(equals(argv[1], UF))
        {
            goto go_uf;
        }
        if(equals(argv[1], XF))
        {
            goto go_xf;
        }
    }

    if(acdv > 3) {
       if(equals(argv[1], "-c"))
       {
           if(equals(argv[2], "-f"))
           {
                return __cf(acdv, argv, 3);
           }
            return error_of_cf;
       } else if(equals(argv[1], "-r"))
       {
           if(equals(argv[2], "-f"))
           {
                return __rf(acdv, argv, 3);
           }
            return error_of_rf;
       } else if(equals(argv[1], "-t"))
       {
           if(equals(argv[2], "-f"))
           {
                return __tf(argv, 3);
           }
            return error_of_tf;
       } else if(equals(argv[1], "-u"))
       {
           if(equals(argv[2], "-f"))
           {
                return __uf(argv, 3);
           }
            return error_of_uf;
       } else if(equals(argv[1], "-x"))
       {
           if(equals(argv[2], "-f"))
           {
                return __xf(argv, 3);
           }
            return error_of_xf;
       }
    }

    go_cf:
        if(acdv > 3)
        {
            __cf(acdv, argv, 2);
        }
        else 
            printf("My_tar: Failed to create empty archive\n");
        return 0;
    go_rf:
        if(acdv > 3)
        {
            __rf(acdv, argv, 2);
        }
        else
        {
            printf("My_tar: try less\n");
        }
        return 0;
    go_tf:
        return __tf(argv, 2);
    go_uf:
        if(acdv > 3)
        {
            __uf(argv, 2);
        }
        else
        {
            printf("My_tar: try less\n");
        }
        return 0;
    go_xf:
        __xf(argv, 2);
    
    return 0;
}