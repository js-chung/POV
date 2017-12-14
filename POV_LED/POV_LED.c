/*
 * POV_LED.c
 *
 *  Created on: 2017. 12. 5.
 *      Author: JS
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "uart.h"

// 7 segments
#define cSegmentPortDir		DDRD // ��ũ�� ��� ����
#define cSegmentPortData	PORTD

char str[3]; //  �������� ����
char time[7];

char key_input;

void GetString(void) {
	unsigned char i = 0;
	unsigned char temp = 0;

	do {
		key_input = getchar();
		printf("%c", key_input);
		str[i] = key_input;
		i++; // -> Skip ?
		if (i >= 2)
			break;
	} while (key_input != 0x0D);  // Enter  getch�� ��� 0x0D(���۸� ������� ����) 0x0A??
	printf("\n");

	if (str[1] == 0x0D) {
		temp = str[0];
		str[1] = temp;
		str[0] = 0x30;
	}
	return;
}

int Clock_Setting(void) {

	do {
		printf("Enter Hour : ");
		GetString();
		time[0] = str[0];
		time[1] = str[1];
	} while (time[0] < 0x30 || time[0] > 0x32 || time[1] < 0x30
			|| time[1] > 0x39 || (time[0]==0x32 && time[1]>0x33) );

	do {
		printf("Enter Minute : ");
		GetString();
		time[2] = str[0];
		time[3] = str[1];
	} while (time[2] < 0x30 || time[2] > 0x35 || time[3] < 0x30
			|| time[3] > 0x39);

	do {
		printf("Enter Second : ");
		GetString();
		time[4] = str[0];
		time[5] = str[1];
	} while (time[4] < 0x30 || time[4] > 0x35 || time[5] < 0x30
			|| time[5] > 0x39);

	printf("if you want to reset press 'r' : ");
	key_input = getchar();
	printf("%c\n", key_input);
	if (key_input == 'r')  // �Է¹��� ���� q�� ��� ����
		return 0; // 1 ��ȯ
	else
		return 1;
	return 0; // 0 ��ȯ
}

int POV_Globe_LED(void) {

	printf("Enter your number [press 'q' to upper menu]: ");
	key_input = getchar(); // �Է� ���� ���� key_input ������ ����
	printf("%c\n", key_input); // �Է¹��� �� ���
	_delay_ms(200);
	if (key_input == 'q') { // �Է¹��� ���� q�� ��� ����
		puts("Goodbye ADC\n");
		return 1; // 1 ��ȯ
	}
	return 0; // 0 ��ȯ
}

void menu(void) {
	puts("\n==================="); // ���ڿ� ��� (�ڵ����� �ٹٲ�)
	puts("     Main Menu     ");
	puts("-------------------");
	puts("1. Clock Setting");
	puts("2. POV Globe LED");
	puts("\nQ. Exit");
}

int main(void) {
	uart_init(9600UL); // UART ��� �ʱ�ȭ (9600bps)

	cSegmentPortDir &= 0xF0; // ��ƮD ���� 4��Ʈ �Է����� ���� (����ũ ����)
	cSegmentPortData = 0x4 << 4;
	// ��ƮD ���� 4��Ʈ 1111 (7-segment�� LED Off)

	stdout = &uart_output; // ǥ�� ��� ���� (printf �Լ��� �ø��� ��Ʈ �ֿܼ� ���)
	stdin = &uart_input; // ǥ�� �Է� ���� (scanf �Լ��� �ø��� ��Ʈ �ֿܼ��� �Է�)

	printf("Hello POV LED!");
	while (1) { // ���� �ݺ��� ����
		menu(); // menu �Լ� ȣ��
		printf("Choose menu : ");
		key_input = uart_getch(); // �Է� ���� ���� key_input ������ ����
		printf("%c\n", key_input); // �Է� ���� �� ���
		switch (key_input) {
		case '1': // key_input ���� 1�� ��� ����
			while (!Clock_Setting()){}
			printf("Time : ");
			puts(time);
				;
			break;
		case '2': // key_input ���� 2�� ��� ����
			while (!POV_Globe_LED())
				;
			break;
		case 'q':
			// key_input ���� q�� ��� break�� ���� ������ ��� ������� ����
		case 'Q': // key_input ���� Q�� ��� ����
			puts("\nGood Main\n");
			return 0; //  main �Լ� ����
		default: // key_input ���� �����ϴ� �������� ���� ��� ����
			printf("\n"); // �ٹٲ�
		}
	}
	return 0;
}
