# include <stdio.h>
# include <math.h>
# include <time.h>

unsigned long long my_pow(unsigned long long base, int exp);
int isPrime(unsigned long long num);

int main(void)
{
	clock_t start, end;
	double rt;
	int is_prime;
	unsigned long long num = my_pow(2, 61) - 1;
	//printf("%llu\n", num);
	
	printf("\n ==============================\n");
	printf("      2^61 - 1은 소수인가?\n");
	printf(" ==============================\n");

	start = clock();
	is_prime = isPrime(num);
	end = clock();

	rt = (double)end - start;
	printf("  - CPU Spec:\ti5-8250U\n");
	printf("  - Memory:\t8GB\n");
	printf("  - 수행 시간:\t%lf ms\n", rt);

	if (is_prime)
		printf("  - 소수 여부:\tYes\n");
	else
		printf("  - 소수 여부:\tNo\n");

	return 0;
}

unsigned long long my_pow(unsigned long long base, int exp)
{
	long long res = 1;
	while (exp)
	{
		if (exp & 1)
			res *= base;
		exp >>= 1;
		base *= base;
	}
	return res;
}

int isPrime(unsigned long long num)
{
	if (num == 1)	return 0;
	else if (num % 2 == 0)	return (num == 2) ? 1 : 0;

	double root = sqrt(num);
	//printf("%lf\n", root);
	for (int i = 3; i <= root; i += 2)
		if (num % i == 0)	return 0;
	return 1;
}