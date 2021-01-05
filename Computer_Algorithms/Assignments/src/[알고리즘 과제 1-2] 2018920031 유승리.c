# define _CRT_SECURE_NO_WARNINGS
# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# define SIZE 10000000

void insertion_sort(int* list, int n);
void print_hash(int n);

int main(void)
{
	FILE* fp;
	clock_t start, end;
	double rt;
	int res;
	int simple_h[101] = { 0 };
	int sum = 0;
	int* dat = (int*) malloc(SIZE * sizeof(int));

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
	insertion_sort(dat, SIZE); // 삽입정렬 실행
	end = clock();
	//printf("삽입정렬 끝\n");

	rt = (double)end - start; // 삽입정렬 동작시간 계산
	printf("\n - Computer CPU: i5-8250U\n");
	printf(" - Memory: 8.00GB\n");
	printf(" - Running Time(ms): %lf\n", rt);
	printf(" - Running Time(min): %lf\n", (rt / 60000));

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

// 삽입정렬 함수
void insertion_sort(int* list, int n)
{
	int i, j, key;
	for (i = 1; i < n; i++)
	{
		key = list[i];
		for (j = i - 1; j >= 0 && list[j] > key; j--)
			list[j + 1] = list[j];
		list[j + 1] = key;
	}
}

// Simple Hash 값을 출력하는 함수
void print_hash(int n)
{
	int t = n / 100000;
	int hash = n - 100000 * t;
	if (hash < 10)
		printf(" - Hash Value: 0000%d\n", hash);
	else if (hash < 100)
		printf(" - Hash Value: 000%d\n", hash);
	else if (hash < 1000)
		printf(" - Hash Value: 00%d\n", hash);
	else if (hash < 10000)
		printf(" - Hash Value: 0%d\n", hash);
	else
		printf(" - Hash Value: %d\n", hash);
}