/*
 *���ܿ������
*/
void init_Main();
void init_Control1();
void init_Control2();
void init_Control3();
void init_Control4();
void init_Control5();
void init_Control6();
void init_Control7();
void init_Control8();

//
//���ܿ��Ʋ���
//
void init_Main() {
	printf("----------------------------------------\n\n");
	printf("\t1��	ƽ���������������ת��\n");
	printf("\t2��	ƽ������������\n");
	printf("\t3��	���������\n");
	printf("\t4��	���������\n");
	printf("\t5��	ͼ�λ���\n");
	printf("\t6��	�������ϵ\n");
	printf("\t7��	��дBMPͼƬ\n");
	printf("\t8��	ʸ������ѹ��\n");
	printf("\t0��	�˳�����\n\n");
	printf("----------------------------------------\n");
	printf("��ѡ���ܱ�ţ�");
	MAIN_STATUS = OPENED;
}

void init_Control1() {
	printf("------------��ѡ��-----------\n\n");
	printf("\t1.���벢�ȷ���ת����\n");
	printf("\t2.���벢����ת�ȷ���\n");
	printf("\t0.������һ��\n\n");
	printf("-----------------------------\n");
	printf("��ѡ�������ţ�");
	CASE1_STATUS = OPENED;
}

void init_Control2() {
	printf("------------��ѡ��-----------\n\n");
	printf("\t1.���귴��\n");
	printf("\t0.������һ��\n\n");
	printf("-----------------------------\n");
	printf("��ѡ�������ţ�");
	CASE2_STATUS = OPENED;
}

void init_Control3() {
	printf("------------��ѡ��-----------\n\n");
	printf("\t1.���������\n");
	printf("\t2.ɾ��������\n");
	printf("\t3.��ѯ������\n");
	printf("\t4.���������\n");
	printf("\t0.������һ��\n\n");
	printf("-----------------------------\n");
	printf("��ѡ�������ţ�");
	CASE3_STATUS = OPENED;
}

void init_Control4() {
	printf("------------��ѡ��-----------\n\n");
	printf("\t1.��������������\n");
	printf("\t2.������ɾ��������\n");
	printf("\t3.�������ѯ������\n");
	printf("\t4.���������������\n");
	printf("\t5.������ɾ��������\n");
	printf("\t6.�����������ļ�\n\n");
	printf("\t0.������һ��\n\n");
	printf("-----------------------------\n");
	printf("��ѡ�������ţ�");
	CASE4_STATUS = OPENED;
}

void init_Control5() {
	printf("------------��ѡ��-----------\n\n");
	printf("\t1.���Ƶ�\n");
	printf("\t2.�����߻���\n");
	printf("\t0.������һ��\n\n");
	printf("-----------------------------\n");
	printf("��ѡ�������ţ�");
	CASE5_STATUS = OPENED;
}

void init_Control6() {
	printf("------------��ѡ��-----------\n\n");
	printf("\t1.����ֱ�ߵĹ�ϵ���������̾���\n");
	printf("\t2.�����߶εĹ�ϵ���������̾���\n");
	printf("\t3.������Ĺ�ϵ���������̺������\n");
	printf("\t0.������һ��\n\n");
	printf("-----------------------------\n");
	printf("��ѡ�������ţ�\n");
	CASE6_STATUS = OPENED;
}

void init_Control7() {
	printf("------------��ѡ��-----------\n\n");
	printf("\t1.��ȡbmp�ļ�\n");
	printf("\t0.������һ��\n\n");
	printf("-----------------------------\n");
	printf("��ѡ�������ţ�");
	CASE7_STATUS = OPENED;
}

void init_Control8() {
	printf("------------��ѡ��-----------\n\n");
	printf("\t1.���෨ѹ��\n");
	printf("\t2.ƫ�Ƿ�ѹ��\n");
	printf("\t3.������˹-�տ˷�ѹ��\n");
	printf("\t0.������һ��\n\n");
	printf("-----------------------------\n");
	printf("��ѡ�������ţ�");
	CASE8_STATUS = OPENED;
}