#include<stdio.h>
#include <stdlib.h>

int * arrival_time; //����ڿ��� �Է¹޴� ���μ����� �����ð�
int * burst_time; //����ڿ��� �Է¹޴� ���μ����� �۾��ð�
int * temp; //���μ��� �� ���� �۾� �ð�

int get_numOfProcess(); //���μ��� ���� �Է¹ޱ�
void get_processData(int limit); //���μ��� ������ �Է¹ޱ�
int get_timeQuantum(void); //Ÿ�ӽ����̽� �Է¹ޱ�
void doProcess(int *total, int i, int *counter, int time_quantum); //�۾��ϱ�
void endCheck(int *total, int i, int *counter, int *x, int *wait_time, int *turnaround_time); //�̹� �ݺ������� �۾��� ����ƴ��� Ȯ��
void nextTurn(int *total, int *const ptr, int limit, int time_quantum); //���� ���� �����ϱ�
void calc_average(int limit, int *wait_time, int *turnaround_time); //��� ����ϱ�


//���μ��� ���� �Է¹ޱ�
int get_numOfProcess() {
	int lim;
	printf("\n�� ���μ��� �� �Է�:\t");
	scanf_s("%d", &lim);
	return lim;
}

//���μ��� ������ �Է¹ޱ�
void get_processData(int limit) {
	printf("\n******ù��° ���μ��� ���μ���[1]�� �ݵ�� 0ms�� �����ؾ� �մϴ�.******\n");
	int k;
	for (k = 0; k < limit; k++) {//���μ��� ������ŭ �ݺ�
		printf("\n���μ���[%d] ���� ���� �Է�\n", k + 1);
		printf("���� �ð�:\t");
		scanf_s("%d", &arrival_time[k]); //process[k]�� ���� �ð� �޾ƿ���
		printf("���� �ð�:\t");
		scanf_s("%d", &burst_time[k]); //process[k]�� ���� �ð� �޾ƿ���
		temp[k] = burst_time[k];
	}
	if (arrival_time[0] != 0) {
		printf("\nù��° ���μ��� ���μ���[1]�� �ݵ�� 0ms�� �����ؾ� �մϴ�. �ٽ� �Է��ϼ���.\n");
		get_processData(limit); //���μ��� ������ �Է� �Լ� ���ȣ��
	}
}

//Ÿ�ӽ����̽� �Է¹ޱ�
int get_timeQuantum(void) {
	int time = 0;
	printf("\n******�̻����� Ÿ�� �����̽��� ������ 1 �ʰ� 10 �̸��Դϴ�. (����:100ms)******\n");
	printf("****** 1<Ÿ�� �����̽�<10 ���� ������ �Է����ּ���.******");
	printf("\nŸ�� �����̽� �Է�:\t");
	scanf_s("%d", &time);
	//Ÿ�� �����̽� ���� Ȯ�� : 1<Ÿ�� �����̽�<10�� �̻����̶�� �����Ͽ� (����:100ms)
	if (time <= 1) { //Ÿ�� �����̽��� 1 �����̸�
		int tqAnswer = 0;
		printf("\nŸ�� �����̽��� 1 �����̸� ���Ʊ�ȯ�� ����� ��ȿ�����Դϴ�. ��� �����Ͻðڽ��ϱ�?\n\n(1)��� �����ϱ�\t(2)Ÿ�� �����̽� �ٽ� �Է��ϱ�\n\n");
		scanf_s("%d", &tqAnswer);
		switch (tqAnswer) {
		case 1:
			break; //����ġ�� Ż���ϰ� ���� ������ ����
		case 2:
			printf("\nŸ�� �����̽� �Է�:\t"); //���Է�
			scanf_s("%d", &time);
			break;
		default:
			printf("�������� �ʴ� �����Դϴ�.");
			break;
		}
	}
	else if (time >= 10) { //Ÿ�ӽ����̽��� 10 �̻��̸�
		int tqAnswer = 0;
		printf("\nŸ�� �����̽��� 10 �̻��̸� ���α׷��� ���ܺ��� �� �ֽ��ϴ�. ��� �����Ͻðڽ��ϱ�?\n\n(1)��� �����ϱ�\t(2)Ÿ�� �����̽� �ٽ� �Է��ϱ�\n\n");
		scanf_s("%d", &tqAnswer);
		switch (tqAnswer) {
		case 1:
			break;
		case 2:
			printf("\nŸ�� �����̽� �Է�:\t"); //���Է�
			scanf_s("%d", &time);
			break;
		default:
			printf("�������� �ʴ� �����Դϴ�.\n");
			break;
		}
	}
	return time;
}

//�۾��ϱ�
void doProcess(int *total, int i, int *counter, int time_quantum) {
	if (temp[i] <= time_quantum && temp[i] > 0) { //���� ���� �۾��ð��� Ÿ�ӽ����̽����� �۴ٸ�. ��, �۾��� �ʿ��� ������ Ÿ�� �����̽���
		*total += temp[i];
		temp[i] = 0;
		*counter = 1; // �̹� for������ �Ϸ� �ƴٴ� �÷���
	}
	else if (temp[i] > 0) {//�̹� Ÿ�ӽ����̽����� �۾��� ������ ������
		temp[i] = temp[i] - time_quantum;
		*total = *total + time_quantum;
	}
}

//�̹� �ݺ������� �۾��� ����ƴ��� Ȯ��
//����ƴٸ� ��� �� wait_time, turn_around_time ���
void endCheck(int *total, int i, int *counter, int *x, int *wait_time, int *turnaround_time) {
	if (temp[i] == 0 && *counter == 1) { //���� �۾��ð��� 0�̰� counter ���� 1�̸�. ��, �̹� for�� ������ �۾��� ����ƴٸ�
		*x -= 1;// ���� ���μ��� ���� - 1 
		printf("\n���μ���[%d]\t\t%d\t\t%d\t\t%d", i + 1, burst_time[i], *total - arrival_time[i], *total - arrival_time[i] - burst_time[i]);
		*wait_time += *total - arrival_time[i] - burst_time[i];
		*turnaround_time += *total - arrival_time[i];
		*counter = 0; //�������μ����� ����ǵ��� �÷��� 0���� �ǵ�����
	}
}

//���� ���� �����ϱ�
void nextTurn(int *total, int *const ptr, int limit, int time_quantum) {
	if (*ptr == limit - 1) { //���� ���� ���μ����� ��ü n���� ���μ��� �� n��° ���μ������
		*ptr = 0;
	}
	else if (arrival_time[*ptr + 1] <= *total) { //���� ���μ����� ����������
		*ptr += 1;
	}
	else { // n�� �� n��° ���μ����� �ƴϰ� ���� ���μ����� �������� �ʾҴٸ� 
		for (int j = 0; j < limit; j++) { //���μ����� Ž��
			if (temp[j] != 0) { //�տ� ���� ������� ���� ���μ����� �ִٸ�
				break; // for�� Ż��
			}
			else if (j == *ptr) {//���� ���ʰ� ù��° ���μ������
				*total += 1; // 1ms�� �ð� ���������
				break; //for�� Ż��
			}
			else if (j == *ptr - 1) { //�ռ� ���μ��� ��� Ž���ߴµ� ��� ����� �����̸�
				*total += 1; // 1ms�� �ð� ���������
				break; //for�� Ż��
			}
			else { continue; } //��� Ž��
		}
		*ptr = 0; //�ٽ� ù��° ���μ����� ���ʸ� �ѱ�
	}
}

//��� ��� �� ����ϱ�
void calc_average(int limit, int *wait_time, int *turnaround_time) {
	float average_wait_time, average_turnaround_time;
	average_wait_time = *wait_time * 1.0 / limit;
	average_turnaround_time = *turnaround_time * 1.0 / limit;
	printf("\n\n��� ��� �ð�:\t%f", average_wait_time);
	printf("\n��� ��ȯ �ð�(��� �ð� + ���� �ð�):\t%f\n", average_turnaround_time);
	return;
}

//����κ� �����ٸ�
int main() {
	int i; //�ݺ������� ���Ǵ� ����, �۾��ϴ� ���μ��� �ѹ�
	int total; //��ü �۾��� �Ϸ�� �� ������ �ð�
	int counter = 0; //���μ����� �̹� turn���� ���� ����ƴٴ� �÷���
	int time_quantum; //����ڿ��� �Է¹޴� Ÿ�ӽ����̽�
	int wait_time = 0; //�� ���μ����� ���ð��� ��
	int turnaround_time = 0; // �� ���μ����� ��ȯ�ð��� ��
	const int limit = get_numOfProcess();//���μ��� �� ����
	int x = limit;//���� ���μ��� ����

	//�Է¹��� ������ �޸� ���� �����Ҵ�
	arrival_time = (int *)malloc(sizeof(int)*limit);
	burst_time = (int *)malloc(sizeof(int)*limit);
	temp = (int *)malloc(sizeof(int)*limit);

	time_quantum = get_timeQuantum();
	get_processData(limit);

	printf("\n���μ��� ID\t\t���� �ð�\t��ȯ �ð�\t��� �ð�\n");

	for (total = 0, i = 0; x > 0;) {
		doProcess(&total, i, &counter, time_quantum);
		endCheck(&total, i, &counter, &x, &wait_time, &turnaround_time);
		nextTurn(&total, &i, limit, time_quantum);
	}

	calc_average(limit, &wait_time, &turnaround_time);

	return 0;
}