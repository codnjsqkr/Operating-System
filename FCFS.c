#include<stdio.h>
#include<stdlib.h>

int get_numOfProcess(); //���μ��� ���� �Է¹ޱ�
void get_Process(int n); //���μ��� �� �����ð�, ����ð� �Է¹ޱ�
void sort(int n, int pos, int i); //����ü ����
void swap(int i, int pos); //�����Ҷ� ����ü �� ���� swap
void doProcess(int *sum, int i, int *avwt, int *avtat); //��ü �ð�, ���μ��� �� ���ð�, ��ȯ�ð� ���
void calc_avg(int avwt, int avtat, int i); //��մ��ð�, ��� ��ȯ�ð� ��� �� ���


//���μ��� ����ü ����
struct process {
	int process; //���μ��� ��ȣ
	int arrival; //���� �ð�
	int burst; //�۾� �ð�
	int wait; //��� �ð�
	int tat; //��ȯ �ð�
};

//����ü ����
struct process *p;

//���μ��� ���� �Է¹ޱ�
int get_numOfProcess() {
	int n;
	printf("���μ��� ������ �Է��ϼ���:\t");
	scanf_s("%d", &n);
	return n;
}

//���μ��� �� �����ð�, ����ð� �Է¹ޱ�
void get_Process(int n) {
	for (int i = 0; i < n; i++){
		p[i].process = i + 1; //���â���� ���̴� ���μ��� ��ȣ
		printf("\n���μ���[%d] ������ �Է��ϼ���.\n", i + 1);
		printf("\n���� �ð�:\t");
		scanf_s("%d", &p[i].arrival);
		printf("\n���� �ð�:\t");
		scanf_s("%d", &p[i].burst);
	}
}

//�����Ҷ� ����ü �� ���� swap
void swap(int i, int pos) {
	int temp;
	//���� �ð� swap
	temp = p[i].arrival;
	p[i].arrival = p[pos].arrival;
	p[pos].arrival = temp;
	//�۾� �ð� swap
	temp = p[i].burst;
	p[i].burst = p[pos].burst;
	p[pos].burst = temp;
	//���μ��� ��ȣ swap
	temp = p[i].process;
	p[i].process = p[pos].process;
	p[pos].process = temp;
}

//����ü ����
void sort(int n, int pos, int i) {
	pos = i; //��� �ε���
	for (int j = i + 1; j < n; j++) { //�� �ε���
		if (p[j].arrival < p[pos].arrival) { // p[a+1]�� p[a]���� ���� �����ϸ�
			pos = j; // swap�Լ� �۵��� ���� ���� ����
		}
	}
	swap(i, pos);
}

//��ü �ð�, ���μ��� �� ���ð�, ��ȯ�ð� ���
void doProcess(int *sum, int i, int *avwt, int *avtat) {
	if (i == 0) { //ù��°�� ������ ���μ�����
		p[0].wait = 0; //���ð��� 0
	}
	else {
		*sum = *sum + p[i - 1].burst; //��ü �ð��� �Ϸ�� �� ���μ��� ���� �ð� ���ؼ� ������Ʈ
		p[i].wait = *sum - p[i].arrival; // ���ð� = �۾� ���� �ð� - �����ð�
		if (p[i].wait < 0) { //�� ���μ����� �Ϸ�� ���� ������ ���
			p[i].wait = 0; //���ð� = 0;
			*sum = p[i].arrival; //��ü �ð��� ���μ��� �����ð����� ������Ʈ
		}
	}
	p[i].tat = p[i].burst + p[i].wait; //��ȯ�ð�=�۾��ð�+���ð�
	*avwt += p[i].wait; //���ð��� �� ������Ʈ
	*avtat += p[i].tat; //��ȯ�ð��� �� ������Ʈ
}

//��մ��ð�, ��� ��ȯ�ð� ��� �� ���
void calc_avg(int avwt, int avtat, int i) {
	float average_wait, average_tat;
	average_wait = (float)avwt / i;
	average_tat = (float)avtat / i;
	printf("\n\n��� ��� �ð�:\t%f", average_wait);
	printf("\n��� ��ȯ �ð�:\t%f\n\n", average_tat);
}

//FCFS �����ٸ�
int main() {
	int limit; //���μ��� ����
	int avwt = 0, avtat = 0; //�� ���ð�, �� ��ȯ�ð�
	int pos=0; //�����Լ����� ���̴� ��� �ε���
	int sum = 0; //��ü �ð�
	int i = 0; //for������ ���̴� ����ü ��ȣ

	limit = get_numOfProcess(); //���μ��� ���� �Է¹޾� limit�� ����

	p = (struct process*)malloc(sizeof(struct process)*limit); //���μ��� ���� �˸°� ����ü �����Ҵ�

	get_Process(limit); //���μ��� ������ �Է¹ޱ�

	for (i = 0; i < limit; i++) { 
		sort(limit, pos, i); //�����ð� �� ����
	}

	printf("\n���μ��� ��ȣ\t\t��ȯ �ð�\t��� �ð�");

	for (i = 0; i < limit; i++) {
		doProcess(&sum, i, &avwt, &avtat);
		printf("\n���μ���[%d]\t\t%d\t\t%d", p[i].process, p[i].tat, p[i].wait);
	}

	calc_avg(avwt, avtat, i); //��մ��ð�, ��չ�ȯ�ð� ���

	return 0;
}