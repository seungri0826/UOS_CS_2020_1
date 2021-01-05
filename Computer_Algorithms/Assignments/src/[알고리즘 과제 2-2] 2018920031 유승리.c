# define _CRT_SECURE_NO_WARNINGS
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# define SIZE 10000000
# define SWAP(x, y, t) ( (t)=(x), (x)=(y), (y)=(t) )

int partition(int* list, int left, int right);
void quick_sort(int* list, int left, int right);
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

	for (int i = 0; i < SIZE; i++)
	{
		res = fscanf(fp, "%d", &dat[i]);
		if (res == EOF)
			break;
	}
	fclose(fp);
	//printf("불러오기 끝\n");

	start = clock();
	quick_sort(dat, 0, SIZE-1); // 퀵 정렬 실행
	end = clock();
	//printf("퀵 정렬 끝\n");

	rt = (double)end - start; // 퀵 정렬 수행 시간 계산
	printf("\n - CPU Spec:\ti5-8250U\n");
	printf(" - Memory:\t8GB\n");
	printf(" - 수행 시간:\t%lf ms\n", rt);

	// dat[4999999]~dat[5000099]까지의 101개 수 각각을 오른쪽 다섯자리까지만 잘라 simple_h[]에 저장
	for (int i = 4999999; i < 5000100; i++)
	{
		int t = dat[i] / 100000;
		simple_h[i - 4999999] = dat[i] - 100000 * t;
		sum += simple_h[i - 4999999];
	}

	print_hash(sum); // simple_h[]에 저장된 수의 합을 다시 오른쪽 다섯자리까지만 잘라 hash value 출력
	free(dat);
	return 0;
}

int partition(int* list, int left, int right)
{
	int pivot, temp;
	int low, high, mid;

	low = left;
	high = right + 1;
	mid = (left + right) / 2;
	pivot = list[mid]; // 중간 index의 값을 pivot으로 설정
	SWAP(list[left], list[mid], temp); // 배열의 가장 왼쪽 데이터와 pivot 자리 교환

	do {
		do
			low++;
		while (low <= right && list[low] < pivot);
		do
			high--;
		while (high >= left && list[high] > pivot);
		if (low < high)	SWAP(list[low], list[high], temp);
	} while (low < high);
	
	SWAP(list[left], list[high], temp);
	return high;
}

// 퀵 정렬 함수
void quick_sort(int* list, int left, int right)
{
	if (left < right) {
		int q = partition(list, left, right);
		quick_sort(list, left, q - 1);
		quick_sort(list, q + 1, right);
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