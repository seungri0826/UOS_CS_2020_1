# define _CRT_SECURE_NO_WARNINGS
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# define SIZE 10000001

void fix_heap(int* list, int root, int k, int bound);
void heap_sort(int* list, int n);
void print_hash(int n);

int main(void)
{
	FILE* fp;
	clock_t start, end;
	double rt;
	int res;
	int simple_h[101] = { 0 };
	int sum = 0;
	int* dat = (int*)malloc(SIZE * sizeof(int));

	if (dat == NULL)
	{
		printf("메모리가 부족합니다.\n");
		exit(1);
	}

	fp = fopen("unsorted10000000.txt", "r");
	if (fp == NULL)
	{
		printf("파일이 열리지 않았습니다.\n");
		return 1;
	}

	for (int i = 1; i < SIZE + 1; i++)
	{
		res = fscanf(fp, "%d", &dat[i]);
		if (res == EOF)
			break;
	}
	fclose(fp);
	//printf("불러오기 끝\n");

	start = clock();
	heap_sort(dat, SIZE - 1);
	end = clock();
	//printf("힙 정렬 끝\n");

	rt = (double)end - start; // 힙 정렬 수행 시간 계산
	printf("\n < Heap Sort >\n");
	printf(" - CPU Spec:\ti5-8250U\n");
	printf(" - Memory:\t8GB\n");
	printf(" - 수행 시간:\t%lf ms\n", rt);

	// dat[5000000]~dat[5000100]까지의 101개 수 각각을 오른쪽 다섯자리까지만 잘라 simple_h[]에 저장
	for (int i = 5000000; i < 5000101; i++)
	{
		int t = dat[i] / 100000;
		simple_h[i - 5000000] = dat[i] - 100000 * t;
		sum += simple_h[i - 5000000];
	}

	print_hash(sum); // simple_h[]에 저장된 수의 합을 다시 오른쪽 다섯자리까지만 잘라 hash value 출력
	free(dat);
	return 0;
}

void fix_heap(int* list, int root, int k, int bound)
{
	int vacant = root;
	while (2 * vacant <= bound) {
		int largerChild = 2 * vacant;
		if (2 * vacant < bound && list[2 * vacant + 1] > list[2 * vacant])
			largerChild = 2 * vacant + 1;
		if (k < list[largerChild])
		{
			list[vacant] = list[largerChild];
			vacant = largerChild;
		}
		else
			break;
	}
	list[vacant] = k;
}

void heap_sort(int* list, int n)
{
	// int형의 나눗셈이므로 floor(n / 2) == n / 2
	for (int i = n / 2; i >= 1; i--)
		fix_heap(list, i, list[i], n);

	for (int heapSize = n; heapSize >= 2; heapSize--)
	{
		int max = list[1];
		fix_heap(list, 1, list[heapSize], heapSize - 1);
		list[heapSize] = max;
	}
}

// Simple Hash 값을 출력하는 함수
void print_hash(int n)
{
	int t = n / 100000;
	int hash = n - 100000 * t;
	if (hash < 10)
		printf(" - Hash 값:\t0000%d\n", hash);
	else if (hash < 100)
		printf(" - Hash 값:\t000%d\n", hash);
	else if (hash < 1000)
		printf(" - Hash 값:\t00%d\n", hash);
	else if (hash < 10000)
		printf(" - Hash 값:\t0%d\n", hash);
	else
		printf(" - Hash 값:\t%d\n", hash);
}