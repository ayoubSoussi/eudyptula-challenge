#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>

int main()
{

	unsigned int low = 0x90123456, high = 0x12345678;

	printf("low is : %x, and high is : %x\n", low, high);

	unsigned long conc = (((unsigned long) high) << 32) | (unsigned long) low;

	printf("concat is : %lx \n", conc);

	long a = syscall(548, high, low);

	printf("output is : %lu\n", a);
	

	return 0;

}
