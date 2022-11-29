#include <stdio.h>
#include <sys/syscall.h>
#include <linux/kernel.h>
#include <unistd.h>
#include <errno.h>

#define twodcopy 451


int main()
{
    int src_arr[2][2] = {(1,1),( 2,2)};
    int dest_arr[2][2]; 
	int status=0;


    long sys_call_status;

    sys_call_status = syscall(twodcopy, src_arr, dest_arr, 2);

    if (sys_call_status != EFAULT)
    {
        printf("twodcopy invoked\n");
        int i,j;
		printf("src_arr");
		for(i=0;i<2;i++){
			for(j=0;j<2;j++){
				printf("%d , ",src_arr[i][j]);
			}
			printf("\n");
		}
		printf("dest_arr");
		for(i=0;i<2;i++){
			for(j=0;j<2;j++){
				printf("%d , ",dest_arr[i][j]);
			}
			printf("\n");
		}
    }

    return 0;
}
