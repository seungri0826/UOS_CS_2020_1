# define _CRT_SECURE_NO_WARNINGS
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# define SIZE 10000000

void merge(int* list, int left, int mid, int right);
void merge_sort(int* list, int left, int right);
void print_hash(int n);

int* sorted; // 추가 공간

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

	sorted = (int*)malloc(SIZE * sizeof(int)); // 추가 공간 동적 할당
	if (sorted == NULL)
	{
		printf("메모리가 부족합니다.\n");
		exit(1);
	}

	start = clock();
	merge_sort(dat, 0, SIZE - 1);
	end = clock();
	//printf("합병 정렬 끝\n");

	free(sorted);

	rt = (double)end - start; // 합병 정렬 수행 시간 계산
	printf("\n < Merge Sort >\n");
	printf(" - CPU Spec:\ti5-8250U\n");
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

void merge(int* list, int left, int mid, int right)
{
	int i, j, k, l;
	i = left; j = mid + 1; k = left;

	// 분할 정렬된 list의 합병
	while (i <= mid && j <= right) {
		if (list[i] <= list[j])
			sorted[k++] = list[i++];
		else
			sorted[k++] = list[j++];
	}
	// 남아 있는 레코드의 일괄 복사
	if (i > mid)
		for (l = j; l <= right; l++)
			sorted[k++] = list[l];
	else
		for (l = i; l <= mid; l++)
			sorted[k++] = list[l];
	// 배열 sorted[]의 리스트를 배열 list[]로 재복사
	for (l = left; l <= right; l++)
		list[l] = sorted[l];
}

void merge_sort(int* list, int left, int right)
{
	int mid;
	if (left < right) {
		mid = (left + right) / 2;
		merge_sort(list, left, mid);
		merge_sort(list, mid + 1, right);
		merge(list, left, mid, right);
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