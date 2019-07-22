// SPDX-License-Identifier: GPL-2.0
#include <asm/unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>
#ifndef __NR_bug1
#error bug1 system call not defined
#endif

#ifndef __NR_bug2
#error bug2 system call not defined
#endif

#ifndef __NR_bug3
#error bug3 system call not defined
#endif

#ifndef __NR_bug4
#error bug4 system call not defined
#endif

#ifndef __NR_bug5
#error bug5 system call not defined
#endif

#ifndef __NR_bug6
#error bug6 system call not defined
#endif

#ifndef __NR_bug7
#error bug7 system call not defined
#endif

#ifndef __NR_bug8
#error bug8 system call not defined
#endif

#ifndef __NR_bug9
#error bug9 system call not defined
#endif

#ifndef __NR_bug10
#error bug10 system call not defined
#endif

#ifndef __NR_bug11
#error bug11 system call not defined
#endif

#ifndef __NR_bug12
#error bug12 system call not defined
#endif

int main(int argc, const char *argv[])
{
	int rc, i;
	char *sys_number = (char *) argv[1];
	int sys_call_no = 0;
	char b[] = "abcdefghi";

	rc = sscanf(sys_number, "%d", &sys_call_no);
	switch (sys_call_no) {
	case 1:
		rc = syscall(__NR_bug1, sys_number);
		break;
	case 2:
		rc = syscall(__NR_bug2, sys_number);
		break;
	case 3:
		rc = syscall(__NR_bug3, sys_number);
		break;
	case 4:
		rc = syscall(__NR_bug4, sys_number);
		break;
	case 5:
		rc = syscall(__NR_bug5, sys_number);
		break;
	case 6:
		rc = syscall(__NR_bug6, sys_number);
		break;
	case 7:
		rc = syscall(__NR_bug7, sys_number);
		break;
	case 8:
		rc = syscall(__NR_bug8, sys_number);
		break;
	case 9:
		i = 1;
		rc = syscall(__NR_bug9, &i);
		break;
	case 10:
		rc = syscall(__NR_bug10, sys_number);
		break;
	case 11:
		rc = syscall(__NR_bug11, sys_number);
		break;
	case 12:		
		rc = syscall(__NR_bug12, b);
		break;
	default:
		printf("Please enter the valid argument from 1 to 12\n");
		return -1;
	}

	if (rc == 0)
		printf("syscall returned %d\n", rc);
	else
		printf("syscall returned %d (errno=%d)\n", rc, errno);

	exit(rc);
}
