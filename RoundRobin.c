#include<stdio.h>
#include <stdlib.h>

int * arrival_time; //사용자에게 입력받는 프로세스별 도착시각
int * burst_time; //사용자에게 입력받는 프로세스별 작업시간
int * temp; //프로세스 별 남은 작업 시간

int get_numOfProcess(); //프로세스 개수 입력받기
void get_processData(int limit); //프로세스 데이터 입력받기
int get_timeQuantum(void); //타임슬라이스 입력받기
void doProcess(int *total, int i, int *counter, int time_quantum); //작업하기
void endCheck(int *total, int i, int *counter, int *x, int *wait_time, int *turnaround_time); //이번 반복문에서 작업이 종료됐는지 확인
void nextTurn(int *total, int *const ptr, int limit, int time_quantum); //다음 순서 산정하기
void calc_average(int limit, int *wait_time, int *turnaround_time); //평균 계산하기


//프로세스 개수 입력받기
int get_numOfProcess() {
	int lim;
	printf("\n총 프로세스 수 입력:\t");
	scanf_s("%d", &lim);
	return lim;
}

//프로세스 데이터 입력받기
void get_processData(int limit) {
	printf("\n******첫번째 프로세스 프로세스[1]은 반드시 0ms에 도착해야 합니다.******\n");
	int k;
	for (k = 0; k < limit; k++) {//프로세스 개수만큼 반복
		printf("\n프로세스[%d] 세부 정보 입력\n", k + 1);
		printf("도착 시간:\t");
		scanf_s("%d", &arrival_time[k]); //process[k]의 도착 시간 받아오기
		printf("실행 시간:\t");
		scanf_s("%d", &burst_time[k]); //process[k]의 실행 시간 받아오기
		temp[k] = burst_time[k];
	}
	if (arrival_time[0] != 0) {
		printf("\n첫번째 프로세스 프로세스[1]은 반드시 0ms에 도착해야 합니다. 다시 입력하세요.\n");
		get_processData(limit); //프로세스 데이터 입력 함수 재귀호출
	}
}

//타임슬라이스 입력받기
int get_timeQuantum(void) {
	int time = 0;
	printf("\n******이상적인 타임 슬라이스의 범위는 1 초과 10 미만입니다. (단위:100ms)******\n");
	printf("****** 1<타임 슬라이스<10 범위 내에서 입력해주세요.******");
	printf("\n타임 슬라이스 입력:\t");
	scanf_s("%d", &time);
	//타임 슬라이스 범위 확인 : 1<타임 슬라이스<10이 이상적이라는 가정하에 (단위:100ms)
	if (time <= 1) { //타임 슬라이스가 1 이하이면
		int tqAnswer = 0;
		printf("\n타임 슬라이스가 1 이하이면 문맥교환이 빈번해 비효율적입니다. 계속 진행하시겠습니까?\n\n(1)계속 진행하기\t(2)타임 슬라이스 다시 입력하기\n\n");
		scanf_s("%d", &tqAnswer);
		switch (tqAnswer) {
		case 1:
			break; //스위치문 탈출하고 이후 순서로 진행
		case 2:
			printf("\n타임 슬라이스 입력:\t"); //재입력
			scanf_s("%d", &time);
			break;
		default:
			printf("지원하지 않는 보기입니다.");
			break;
		}
	}
	else if (time >= 10) { //타임슬라이스이 10 이상이면
		int tqAnswer = 0;
		printf("\n타임 슬라이스가 10 이상이면 프로그램이 끊겨보일 수 있습니다. 계속 진행하시겠습니까?\n\n(1)계속 진행하기\t(2)타임 슬라이스 다시 입력하기\n\n");
		scanf_s("%d", &tqAnswer);
		switch (tqAnswer) {
		case 1:
			break;
		case 2:
			printf("\n타임 슬라이스 입력:\t"); //재입력
			scanf_s("%d", &time);
			break;
		default:
			printf("지원하지 않는 보기입니다.\n");
			break;
		}
	}
	return time;
}

//작업하기
void doProcess(int *total, int i, int *counter, int time_quantum) {
	if (temp[i] <= time_quantum && temp[i] > 0) { //현재 남은 작업시간이 타임슬라이스보다 작다면. 즉, 작업에 필요한 마지막 타임 슬라이스면
		*total += temp[i];
		temp[i] = 0;
		*counter = 1; // 이번 for문에서 완료 됐다는 플래그
	}
	else if (temp[i] > 0) {//이번 타임슬라이스에서 작업이 끝나지 않으면
		temp[i] = temp[i] - time_quantum;
		*total = *total + time_quantum;
	}
}

//이번 반복문에서 작업이 종료됐는지 확인
//종료됐다면 출력 및 wait_time, turn_around_time 계산
void endCheck(int *total, int i, int *counter, int *x, int *wait_time, int *turnaround_time) {
	if (temp[i] == 0 && *counter == 1) { //남은 작업시간이 0이고 counter 값이 1이면. 즉, 이번 for문 내에서 작업이 종료됐다면
		*x -= 1;// 남은 프로세스 개수 - 1 
		printf("\n프로세스[%d]\t\t%d\t\t%d\t\t%d", i + 1, burst_time[i], *total - arrival_time[i], *total - arrival_time[i] - burst_time[i]);
		*wait_time += *total - arrival_time[i] - burst_time[i];
		*turnaround_time += *total - arrival_time[i];
		*counter = 0; //다음프로세스가 진행되도록 플래그 0으로 되돌리기
	}
}

//다음 순서 산정하기
void nextTurn(int *total, int *const ptr, int limit, int time_quantum) {
	if (*ptr == limit - 1) { //만약 현재 프로세스가 전체 n개의 프로세스 중 n번째 프로세스라면
		*ptr = 0;
	}
	else if (arrival_time[*ptr + 1] <= *total) { //다음 프로세스가 도착했으면
		*ptr += 1;
	}
	else { // n개 중 n번째 프로세스도 아니고 다음 프로세스가 도착하지 않았다면 
		for (int j = 0; j < limit; j++) { //프로세스들 탐색
			if (temp[j] != 0) { //앞에 아직 종료되지 않은 프로세스가 있다면
				break; // for문 탈출
			}
			else if (j == *ptr) {//현재 차례가 첫번째 프로세스라면
				*total += 1; // 1ms씩 시간 흘려보내기
				break; //for문 탈출
			}
			else if (j == *ptr - 1) { //앞선 프로세스 모두 탐색했는데 모두 종료된 상태이면
				*total += 1; // 1ms씩 시간 흘려보내기
				break; //for문 탈출
			}
			else { continue; } //계속 탐색
		}
		*ptr = 0; //다시 첫번째 프로세스로 차례를 넘김
	}
}

//평균 계산 및 출력하기
void calc_average(int limit, int *wait_time, int *turnaround_time) {
	float average_wait_time, average_turnaround_time;
	average_wait_time = *wait_time * 1.0 / limit;
	average_turnaround_time = *turnaround_time * 1.0 / limit;
	printf("\n\n평균 대기 시간:\t%f", average_wait_time);
	printf("\n평균 반환 시간(대기 시간 + 실행 시간):\t%f\n", average_turnaround_time);
	return;
}

//라운드로빈 스케줄링
int main() {
	int i; //반복문에서 사용되는 변수, 작업하는 프로세스 넘버
	int total; //전체 작업이 완료될 때 까지의 시간
	int counter = 0; //프로세스가 이번 turn에서 정상 종료됐다는 플래그
	int time_quantum; //사용자에게 입력받는 타임슬라이스
	int wait_time = 0; //전 프로세스의 대기시간의 합
	int turnaround_time = 0; // 전 프로세스의 반환시간의 합
	const int limit = get_numOfProcess();//프로세스 총 개수
	int x = limit;//남은 프로세스 개수

	//입력받을 데이터 메모리 공간 동적할당
	arrival_time = (int *)malloc(sizeof(int)*limit);
	burst_time = (int *)malloc(sizeof(int)*limit);
	temp = (int *)malloc(sizeof(int)*limit);

	time_quantum = get_timeQuantum();
	get_processData(limit);

	printf("\n프로세스 ID\t\t실행 시간\t반환 시간\t대기 시간\n");

	for (total = 0, i = 0; x > 0;) {
		doProcess(&total, i, &counter, time_quantum);
		endCheck(&total, i, &counter, &x, &wait_time, &turnaround_time);
		nextTurn(&total, &i, limit, time_quantum);
	}

	calc_average(limit, &wait_time, &turnaround_time);

	return 0;
}