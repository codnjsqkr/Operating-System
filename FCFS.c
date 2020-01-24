#include<stdio.h>
#include<stdlib.h>

int get_numOfProcess(); //프로세스 개수 입력받기
void get_Process(int n); //프로세스 별 도착시간, 실행시간 입력받기
void sort(int n, int pos, int i); //구조체 정렬
void swap(int i, int pos); //정렬할때 구조체 내 변수 swap
void doProcess(int *sum, int i, int *avwt, int *avtat); //전체 시간, 프로세스 별 대기시간, 반환시간 계산
void calc_avg(int avwt, int avtat, int i); //평균대기시간, 평균 반환시간 계산 및 출력


//프로세스 구조체 선언
struct process {
	int process; //프로세스 번호
	int arrival; //도착 시간
	int burst; //작업 시간
	int wait; //대기 시간
	int tat; //반환 시간
};

//구조체 생성
struct process *p;

//프로세스 개수 입력받기
int get_numOfProcess() {
	int n;
	printf("프로세스 개수를 입력하세요:\t");
	scanf_s("%d", &n);
	return n;
}

//프로세스 별 도착시간, 실행시간 입력받기
void get_Process(int n) {
	for (int i = 0; i < n; i++){
		p[i].process = i + 1; //결과창에서 보이는 프로세스 번호
		printf("\n프로세스[%d] 정보를 입력하세요.\n", i + 1);
		printf("\n도착 시간:\t");
		scanf_s("%d", &p[i].arrival);
		printf("\n실행 시간:\t");
		scanf_s("%d", &p[i].burst);
	}
}

//정렬할때 구조체 내 변수 swap
void swap(int i, int pos) {
	int temp;
	//도착 시간 swap
	temp = p[i].arrival;
	p[i].arrival = p[pos].arrival;
	p[pos].arrival = temp;
	//작업 시간 swap
	temp = p[i].burst;
	p[i].burst = p[pos].burst;
	p[pos].burst = temp;
	//프로세스 번호 swap
	temp = p[i].process;
	p[i].process = p[pos].process;
	p[pos].process = temp;
}

//구조체 정렬
void sort(int n, int pos, int i) {
	pos = i; //대상 인덱스
	for (int j = i + 1; j < n; j++) { //비교 인덱스
		if (p[j].arrival < p[pos].arrival) { // p[a+1]이 p[a]보다 먼저 도착하면
			pos = j; // swap함수 작동을 위한 변수 설정
		}
	}
	swap(i, pos);
}

//전체 시간, 프로세스 별 대기시간, 반환시간 계산
void doProcess(int *sum, int i, int *avwt, int *avtat) {
	if (i == 0) { //첫번째로 도착한 프로세스면
		p[0].wait = 0; //대기시간은 0
	}
	else {
		*sum = *sum + p[i - 1].burst; //전체 시간에 완료된 앞 프로세스 실행 시간 더해서 업데이트
		p[i].wait = *sum - p[i].arrival; // 대기시간 = 작업 시작 시간 - 도착시간
		if (p[i].wait < 0) { //앞 프로세스가 완료된 이후 도착한 경우
			p[i].wait = 0; //대기시간 = 0;
			*sum = p[i].arrival; //전체 시간을 프로세스 도착시간으로 업데이트
		}
	}
	p[i].tat = p[i].burst + p[i].wait; //반환시간=작업시간+대기시간
	*avwt += p[i].wait; //대기시간의 합 업데이트
	*avtat += p[i].tat; //반환시간의 합 업데이트
}

//평균대기시간, 평균 반환시간 계산 및 출력
void calc_avg(int avwt, int avtat, int i) {
	float average_wait, average_tat;
	average_wait = (float)avwt / i;
	average_tat = (float)avtat / i;
	printf("\n\n평균 대기 시간:\t%f", average_wait);
	printf("\n평균 반환 시간:\t%f\n\n", average_tat);
}

//FCFS 스케줄링
int main() {
	int limit; //프로세스 개수
	int avwt = 0, avtat = 0; //총 대기시간, 총 반환시간
	int pos=0; //정렬함수에서 쓰이는 대상 인덱스
	int sum = 0; //전체 시간
	int i = 0; //for문에서 쓰이는 구조체 번호

	limit = get_numOfProcess(); //프로세스 개수 입력받아 limit에 전달

	p = (struct process*)malloc(sizeof(struct process)*limit); //프로세스 수에 알맞게 구조체 동적할당

	get_Process(limit); //프로세스 데이터 입력받기

	for (i = 0; i < limit; i++) { 
		sort(limit, pos, i); //도착시간 순 정렬
	}

	printf("\n프로세스 번호\t\t반환 시간\t대기 시간");

	for (i = 0; i < limit; i++) {
		doProcess(&sum, i, &avwt, &avtat);
		printf("\n프로세스[%d]\t\t%d\t\t%d", p[i].process, p[i].tat, p[i].wait);
	}

	calc_avg(avwt, avtat, i); //평균대기시간, 평균반환시간 출력

	return 0;
}