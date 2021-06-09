/*

��Ŀ���ƣ�ѧ���ɼ�����ϵͳ
���ߣ�valieo

*/

/*
˵����

ÿ�½�һ���ɼ����ͻ�����һ����ɼ���ͬ�����ı��ļ�
ÿɾ��һ���ɼ����ͻ����һ����ɼ���ͬ�����ı��ļ�

�ֳ������ѽ����������ɼ��������ƺͳɼ�����ѧ�������£�
Class01   30��				��Ӧ���ļ���Class01.txt
�����2��  0��				��Ӧ���ļ��������2��.txt

data.txt�ļ����ڴ洢�ɼ������ƺͳɼ�������

�������⽨������ɼ�����Ҳ�����ڳɼ�����������Ӷ��ѧ����Ϣ��ѧ����ϢԽ�࣬�ɼ��ֲ�ͼ��Խ����
*/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//���ֵ���󳤶�
#define MaxNameLen 25

//�洢�ɼ������������Ƶ��ļ���
const char FileName[] = "data.txt";


//���ѧ����Ϣ�ķ�����ͷ����β��������ǰ���򷽷����룩
enum InputMethod { byHead, byEnd, inOrder };
//ö�ٽṹ--���Ƴɼ�(������ơ����������Ӣ����ִ�ʷ)���ܳɼ�
enum Course { programming, math, physical, english, history, total };
//��¼��ǰ����ʽ
enum SortMethod { byProgramming, byMath, byPhysical, byEnglish, byHistory, byTotal, byId, mess };

enum SortMethod sortMethod = byId;	//ȫ�ֱ��� Ĭ��ͨ��id��С��������


//�����Ϣ
typedef struct Identity {
	long id;				//ѧ��
	char name[MaxNameLen];	//����
}Identity;
//ѧ����Ϣ
typedef struct Student {
	Identity identity;	//���
	float score[total + 1];	//�洢5��ѧ�Ƴɼ�+1���ܳɼ�
	struct Student* next;
}Student;


/*---------------------------��������---------------------------*/
void MainMenu();
void Menu();
char** ReadFileNameByFile(int* tableNum);
void WriteFileNameByFile(char** tableName, int tableNum);
Student* ReadStuInfoByFile(char* fileName);
void WriteStuInfoByFile(Student* head);
Student* CreateList(char* name);
Student* CreateNode(Identity identity, float* score);
void InsertByHead(Student* head, Identity identity, float* score);
void InsertByEnd(Student* head, Identity identity, float* score);
void InsertInOrder(Student* head, Identity identity, float* score);
void InputData(Student* head, enum InputMethod method);
Student* AddList(char*** tableName, int* tableNum);
Student* EnterTable(char* name, char** tableName, int tableNum);
void DelById(Student* head, int id);
void DelByName(Student* head, char* name);
void ClearList(Student* head);
void DestoryList(Student** head);
void DelTable(char* name, char** tableName, int* tableNum);
Student* FindById(Student* head, long id);
Student* FindByName(Student* head, char* name);
void ChangeNode(Student* node, Identity identity, float* score);
void SortById(Student* head);
void Sort(Student* head, enum SortMethod method);
void PrintList(Student* head);
void PrintNode(Student* node);
void PrintNoPass(Student* head);
void PrintPass(Student* head);
void PrintTable(char** tableName, int tableNum);
void PrintDistribution(Student* head, enum Course course);


int main()
{
	system("title ѧ���ɼ�����ϵͳ");		//��������
	
	
	int again = 1;	//�Ƿ��˳�����
	int menu1 = 1;	//��ǰ״̬���˵�һ
	int menu2 = 1;  //��ǰ״̬���ǲ˵�һ
	while (again)
	{
		Student* target = NULL; //ָ����ҵ����Ǹ��ڵ�
		Student* head = NULL;	//����ͷ
		char** tableName = NULL; //�ɼ�������
		int tableNum = 0;		//�ɼ�������
		int method;				//������byChinese,byTotal��

		//һ���˵�	���˵�
		while (menu1 && again)
		{

			tableName = ReadFileNameByFile(&tableNum);
			char name[MaxNameLen];
			int key1 = 0;;
			MainMenu();
			scanf_s("%d", &key1);
			system("cls");
			switch (key1)
			{
			case 1:
				head = AddList(&tableName, &tableNum);
				if (head != NULL)
				{
					WriteFileNameByFile(tableName, tableNum);//����ɼ�������
					WriteStuInfoByFile(head);//�����½��ĳɼ���
					menu1 = 0;
					menu2 = 1;	//��������˵�
				}
				break;
			case 2:
				PrintTable(tableName, tableNum);
				printf("������ɼ������ƣ�");
				scanf_s("%s", name, MaxNameLen);
				head = EnterTable(name, tableName, tableNum);
				if (head == NULL)
				{
					printf("δ���ҵ��óɼ���\n");
				}
				else
				{
					menu1 = 0;
					menu2 = 1;
				}
				break;
			case 3:
				PrintTable(tableName, tableNum);
				break;
			case 4:
				PrintTable(tableName, tableNum);
				printf("������Ҫɾ���ĳɼ������ƣ�");
				scanf_s("%s", name, MaxNameLen);
				DelTable(name, tableName, &tableNum);
				WriteFileNameByFile(tableName, tableNum);
				break;
			case 0:
				//�ͷ�tableName		tableName�Ƕ���ָ��
				for (int i = 0; i < tableNum; ++i)
				{
					free(tableName[i]);
				}
				free(tableName);
				again = 0;	//�˳�����
				break;
			default:
				printf("�������\n\n");
				break;
			}
			//system("cls");
		}

		system("cls");

		//�����˵�
		while (menu2 && again)
			{
				Menu();
				int key2;
				scanf_s("%d", &key2);
				system("cls");
				int isDel = 0;//Ĭ�ϲ�ɾ���ɼ���

				long id;
				char name[MaxNameLen];
				switch (key2)
				{
				case 1:
					InputData(head, byHead);
					break;
				case 2:
					InputData(head, byEnd);
					break;
				case 3:
					InputData(head, inOrder);
					break;
				case 4:
					printf("������Ҫɾ����id��");
					scanf_s("%ld", &id);
					DelById(head, id);
					break;
				case 5:
					printf("������Ҫɾ����name��");
					scanf_s("%s", name, MaxNameLen);
					DelByName(head, name);
					break;
				case 6:
					ClearList(head);
					break;
				case 7:
					printf("��ɾ���������ɳ�����ȷ��ɾ���óɼ�������1.Yes  2.No����\n");
					isDel = 0;
					scanf_s("%d", &isDel);
					if (isDel == 1)
					{
						//���´洢�ɼ������ƺ��������ļ�
						DelTable(head->identity.name, tableName, &tableNum);
						WriteFileNameByFile(tableName, tableNum);
						DestoryList(&head);
						menu2 = 0;	//ɾ���󷵻ص����˵�
						menu1 = 1;
					}
					else
						printf("��ȡ��ɾ������\n");
					break;
				case 8:
					printf("������Ҫ���ҵ�id��");
					scanf_s("%ld", &id);
					target = FindById(head, id);
					if (target)
					{
						printf("ID      Name       Programming   Math      Physical  English   History   Average   Total\n");
						PrintNode(target);
					}
					break;
				case 9:
					printf("������Ҫ���ҵ�name��");
					scanf_s("%s", name, MaxNameLen);
					target = FindByName(head, name);
					if (target)
					{
						printf("ID      Name       Programming   Math      Physical  English   History   Average   Total\n");
						PrintNode(target);
					}
					break;
				case 10:
					if (target == NULL)
					{
						printf("���Ȳ��ҵ���Ҫ�޸ĵ��Ǹ�ѧ����Ϣ��Ȼ���ٽ����޸Ĳ�����ʹ�ù���8��9��\n");
					}
					else
					{
						printf("�㽫�޸�id:%ld,name:%s����Ϣ\n�밴�����и�ʽ�����ѧ����������Ϣ\nID Name Programming Math Physical English History\n", target->identity.id, target->identity.name);
						Identity identity;
						float score[total] = { 0 };
						int k = scanf_s("%ld %s ", &identity.id, identity.name, MaxNameLen);
						int i;
						for (i = 0; i < total; ++i)
						{
							if (k == 2 && scanf_s("%f", &score[i]) == 1)
							{
								continue;
							}
							else
							{
								printf("�����ʽ����\n");
								break;
							}
						}
						if (i == total)
						{
							ChangeNode(target, identity, score);
							Sort(head, sortMethod);
						}
					}
					break;
				case 11:
					printf("��ѡ���������ݷ�����\n1.Programming  2.Math  3.Physical  4.English  5.History  6.Total  7.ID\n");
					scanf_s("%d", &method);
					Sort(head, (SortMethod)(method - 1));
					break;
				case 12:
					PrintNoPass(head);
					break;
				case 13:
					PrintPass(head);
					break;
				case 14:
					PrintList(head);
					break;
				case 15:
					printf("��ѡ�����гɼ��е�һ��\n1.Programming  2.Math  3.Physical  4.English  5.History  6.Average\n");
					scanf_s("%d", &method);
					PrintDistribution(head, (Course)(method - 1));
					break;
				case 16:
					WriteStuInfoByFile(head);
					break;
				case 0:
					printf("δ����Ĳ������ᶪʧ(0.ȷ�Ϸ���  1.ȡ������)\n");
					scanf_s("%d", &method);
					if (method == 0)
					{
						DestoryList(&head);	//����ʱ���������ͷ�malloc������ڴ�
						menu2 = 0;
						menu1 = 1;
					}
					break;
				default:
					printf("�������\n\n");
					break;
				}

				printf("���س���ʾ�˵�");
				getchar();		//�Ե�����keyֵʱ�Ļس�
				getchar();
				system("cls");
			}
	}
	return 0;
}



/*---------------------------����---------------------------*/
//���˵�	�Գɼ����Ĳ���
void MainMenu()
{
	printf("\n");
	printf("\t                               ѧ���ɼ�����ϵͳ\n");
	printf("\t*****************************************************************************\n");
	printf("\t*                                  ���˵�                                   *\n");
	printf("\t*                1.  �����µĳɼ���                                         *\n");
	printf("\t*                2.  �������еĳɼ���                                       *\n");
	printf("\t*                3.  ��ʾ���гɼ���                                         *\n");
	printf("\t*                4.  ɾ���ɼ���                                             *\n");
	printf("\t*                0.  �˳�����ϵͳ                                           *\n");
	printf("\t*****************************************************************************\n");
	printf("\n\n");
	printf("���빦�ܣ�");
}
//�����˵�	�Գɼ�������Ϣ�Ĳ���
void Menu()
{
	printf("\n\
        ---------------------------------------------------------------\n\
                                ѧ���ɼ�����ϵͳ\n\
        ===============================================================\n\
        ����>>          1.ͷ������\n\
                        2.β������\n\
                        3.��������\n\
        ɾ��>>          4.ɾ����Ϣ��id��\n\
                        5.ɾ����Ϣ��name��\n\
                        6.�����Ϣ\n\
                        7.ɾ���ɼ���\n\
        ��ѯ>>          8.����id��ѯ\n\
                        9.����name��ѯ\n\
        ����>>          10.������Ϣ\n\
        ����>>          11.����\n\
        ���>>          12.������ѧ��\n\
                        13.����ѧ��\n\
                        14.����ѧ��\n\
                        15.�ɼ��ֲ�����߷�\n\
        ����>>          16.����\n\
        ����>>          0.����\n\
        ===============================================================\n\
");
	printf("���빦�ܣ�");
}


/*-------------------------�ļ�����--------------------------*/
//��ȡ�ɼ������ƺͳɼ�������
char** ReadFileNameByFile(int* tableNum)
{
	FILE* fp;
	fopen_s(&fp, FileName, "r");
	if (fp == NULL)
	{
		*tableNum = 0;
		return NULL;
	}
	*tableNum = 0;	//n���ɼ���
	int k = fscanf_s(fp, "%d\n", tableNum);
	if (*tableNum < 1 || k < 1)
	{
		fclose(fp);
		return NULL;
	}
	char** scoreName = (char**)malloc(sizeof(char*) * (*tableNum));
	for (int i = 0; i < *tableNum; ++i)
	{
		scoreName[i] = (char*)malloc(sizeof(char) * MaxNameLen);
		fscanf_s(fp, "%s\n", scoreName[i], MaxNameLen);
	}
	fclose(fp);
	return scoreName;
}
//д��ɼ������ƺ�����
void WriteFileNameByFile(char** tableName, int tableNum)
{
	FILE* fp;
	fopen_s(&fp, FileName, "w");
	if (fp == NULL)
	{
		printf("���ļ�ʧ�ܣ��޷�д��\n");
		return;
	}
	fprintf(fp, "%d\n", tableNum);		//�ļ���һ�У��ɼ�������
	for (int i = 0; i < tableNum; ++i)
	{
		fprintf(fp, "%s\n", tableName[i]);
	}
	fclose(fp);
}
//��ȡѧ����Ϣ
Student* ReadStuInfoByFile(char* fileName)
{
	FILE* fp;
	char name[MaxNameLen + 4];	//���ļ����ƺ�����ӡ�.txt���ļ���׺
	strcpy_s(name, MaxNameLen, fileName);
	int len = strlen(fileName);
	name[len] = '.';
	name[len + 1] = 't';
	name[len + 2] = 'x';
	name[len + 3] = 't';
	name[len + 4] = '\0';
	fopen_s(&fp, name, "r");
	if (fp == NULL)
	{
		printf("���ļ�ʧ�ܣ��޷���ȡ\n");
		return NULL;
	}
	int method;
	fscanf_s(fp, "%d", &method);
	sortMethod = (enum SortMethod)method;
	Student* head = (Student*)malloc(sizeof(Student));
	fscanf_s(fp, "%ld %s ", &head->identity.id, head->identity.name, MaxNameLen);
	for (int i = 0; i <= total; ++i)
	{
		fscanf_s(fp, "%f", &head->score[i]);
	}
	int n = head->identity.id;	//������
	head->next = NULL;
	Student* pre = head;
	for (int i = 0; i < n; ++i)
	{
		Student* node = (Student*)malloc(sizeof(Student));
		node->next = NULL;
		fscanf_s(fp, "%ld %s ", &node->identity.id, node->identity.name, MaxNameLen);
		for (int i = 0; i <= total; ++i)
		{
			fscanf_s(fp, "%f", &node->score[i]);
		}
		pre->next = node;
		pre = node;
	}
	fclose(fp);
	return head;
}
//д��ѧ����Ϣ
void WriteStuInfoByFile(Student* head)
{
	if (head == NULL)
	{
		return;
	}
	char name[MaxNameLen + 4];	//���ļ����ƺ�����ӡ�.txt���ļ���׺
	strcpy_s(name, MaxNameLen, head->identity.name);
	int len = strlen(head->identity.name);
	name[len] = '.';
	name[len + 1] = 't';
	name[len + 2] = 'x';
	name[len + 3] = 't';
	name[len + 4] = '\0';
	FILE* fp;
	fopen_s(&fp, name, "w");
	if (fp == NULL)
	{
		printf("���ļ�ʧ�ܣ��޷�д��\n");
		return;
	}
	Student* tmp = head;
	fprintf(fp, "%d\n", sortMethod);		//�ļ���һ�У���ǰ�ɼ������򷽷�����
	while (tmp != NULL)
	{
		fprintf(fp, "%ld %s ", tmp->identity.id, tmp->identity.name);
		for (int i = 0; i < total; ++i)
		{
			fprintf(fp, "%.1f ", tmp->score[i]);
		}
		fprintf(fp, "%.1f\n", tmp->score[total]);
		tmp = tmp->next;
	}
	fclose(fp);
	printf("����ɹ�\n\n");
}


/*-------------------------��������---------------------------*/
//��������ͷ-------ok
Student* CreateList(char* name)
{
	Student* head = (Student*)malloc(sizeof(Student));
	if (head == NULL)
	{
		printf("malloc error in CreateList function\n");
		return NULL;
	}
	//���ݳ�ʼ��
	strcpy_s(head->identity.name, MaxNameLen, name);
	for (int i = 0; i <= total; ++i)
	{
		head->score[i] = 0;
	}
	head->identity.id = 0;		//��ͷ�ڵ��У�id�����洢������
	head->next = NULL;
	return head;
}
//�����ڵ�--------ok
Student* CreateNode(Identity identity, float* score)
{
	Student* node = (Student*)malloc(sizeof(Student));
	node->identity = identity;
	int i;
	float sum = 0;		//�ܷ�
	for (i = 0; i < total; ++i)	//total��ö���������ͣ������Ϊ��Ŀ�ɼ�������
	{
		node->score[i] = score[i];
		sum += score[i];
	}
	node->score[i] = sum;		//��¼�ܷ�
	node->next = NULL;
	return node;
}


/*----------------------------��---------------------------*/
//��ͷ������ڵ�------ok
void InsertByHead(Student* head, Identity identity, float* score)
{
	if (head == NULL)return;
	head->identity.id++;	//����ͷ�ڵ��е�������
	Student* node = CreateNode(identity, score);
	node->next = head->next;
	head->next = node;
	//��������
	sortMethod = mess;
}

//��β������ڵ�-----ok
void InsertByEnd(Student* head, Identity identity, float* score)
{
	if (head == NULL) return;
	head->identity.id++;	//����ͷ�ڵ��е�������
	Student* node = CreateNode(identity, score);
	Student* end = head;	//�˴�ÿ�β���β��ʱ��Ҫ���������������������Ż�
	while (end->next != NULL)
	{
		end = end->next;
	}
	end->next = node;
	end = node;
	//��������
	sortMethod = mess;
}

//����С˳�����ڵ�	ǰ�᣺ԭ��������---ok
void InsertInOrder(Student* head, Identity identity, float* score)
{
	if (head == NULL)return;
	if (sortMethod == mess && head->next != NULL)
	{
		printf("��ǰѧ����ϢΪ����(�Ѿ�������ͷ����β����������)�������������������\n");
		return;
	}

	++(head->identity.id);
	Student* node = CreateNode(identity, score);
	//������ֱ�Ӳ���
	if (head->next == NULL)
	{
		head->next = node;
		return;
	}

	//��id˳�����(id������)
	if (sortMethod == byId)
	{
		Student* pre = head;
		Student* tmp = NULL;
		while (pre->next != NULL)
		{
			tmp = pre->next;
			//����ͷ�����м����
			if (node->identity.id <= tmp->identity.id)
			{
				pre->next = node;
				node->next = tmp;
				return;
			}
			//����ĩβ���
			else if (node->identity.id > tmp->identity.id && tmp->next == NULL)
			{
				tmp->next = node;
				return;
			}
			pre = pre->next;
		}
	}
	//���ɼ�˳����루�ɼ�Ϊ����
	else if (sortMethod != mess)
	{
		Student* pre = head;
		Student* tmp = NULL;
		while (pre->next != NULL)
		{
			tmp = pre->next;
			//����ͷ�����м����
			if (node->score[sortMethod] >= tmp->score[sortMethod])
			{
				pre->next = node;
				node->next = tmp;
				return;
			}
			//����ĩβ���
			else if (node->score[sortMethod] < tmp->score[sortMethod] && tmp->next == NULL)
			{
				tmp->next = node;
				return;
			}
			pre = pre->next;
		}
	}
}

//����ѧ����Ϣ
void InputData(Student* head, enum InputMethod method)
{
	if (head == NULL)
	{
		printf("�óɼ��������ڻ��ѱ�ɾ��\n\n");
		return;
	}
	//�����ǰ�����ǿ�����������������ô�Ͳ��ܽ����������
	if (sortMethod == mess && head->next != NULL)
	{
		printf("��ǰѧ����ϢΪ����(�Ѿ�������ͷ����β����������)�������������������\n");
		return;
	}
	int n, k;
	Identity identity;
	float score[total] = { 0 };
	printf("Ҫ��ӵ�ѧ��������");
	scanf_s("%d", &n);
	if (n < 1)
	{
		printf("������������С��1\n\n");
		return;
	}
	printf("�밴���¸�ʽ����ѧ����Ϣ\nID Name Programming Math Physical English History\n");
	for (int i = 0; i < n; ++i)
	{
		k = scanf_s("%ld %s ", &identity.id, &identity.name, MaxNameLen);
		if (k != 2)
		{
			printf("�����ʽ����\n\n");
			return;
		}
		for (int i = 0; i < total; ++i)
		{
			if (scanf_s("%f", &score[i]) != 1)
			{
				printf("�����ʽ����\n\n");
				return;
			}
		}
		//���ID�Ƿ��ظ�
		Student* tmp = head;
		while (tmp->next != NULL)
		{
			tmp = tmp->next;
			if (tmp->identity.id == identity.id)
			{
				printf("�Ѿ�����ID��%d��ѧ����Ϣ\n\n", identity.id);
				return;
			}
		}
		//�����������Ƿ���Ϸ�Χ
		for (int i = 0; i < total; ++i)
		{
			if (score[i] < 0 || score[i]>100)
			{
				printf("����ĳɼ����󣬳ɼ��ķ�ΧΪ0~100\n\n");
				return;
			}
		}

		if (method == byHead)
		{
			InsertByHead(head, identity, score);
		}
		else if (method == byEnd)
		{
			InsertByEnd(head, identity, score);
		}
		else
		{
			InsertInOrder(head, identity, score);
		}
	}
	printf("�����Ϣ�ɹ�\n\n");
}

//����һ���ɼ���---�õ�����ָ��
Student* AddList(char*** tableName, int* tableNum)
{
	printf("�����½��ɼ������ƣ�");
	char name[MaxNameLen];
	scanf_s("%s", name, MaxNameLen);
	for (int i = 0; i < *tableNum; ++i)
	{
		if (strcmp(name, (*tableName)[i]) == 0)
		{
			printf("�������Ѵ���\n\n");
			return NULL;
		}
	}
	++(*tableNum);
	char** newTableName = (char**)malloc(sizeof(char*) * (*tableNum));
	int i = 0;
	for (i = 0; i < (*tableNum) - 1; ++i)
	{
		newTableName[i] = (char*)malloc(sizeof(char) * MaxNameLen);
		strcpy_s(newTableName[i], MaxNameLen, (*tableName)[i]);
	}
	newTableName[i] = (char*)malloc(sizeof(char) * MaxNameLen);
	strcpy_s(newTableName[i], MaxNameLen, name);
	if ((*tableName) != NULL)
		free(*tableName);
	*tableName = newTableName;
	Student* head = CreateList(name);	//����һ����Ϊname������
	printf("�½��ɹ�\n\n");
	return head;
}

//�������еĳɼ���
Student* EnterTable(char* name, char** tableName, int tableNum)
{
	for (int i = 0; i < tableNum; ++i)
	{
		if (strcmp(name, tableName[i]) == 0)
		{
			return ReadStuInfoByFile(name);
		}
	}
	return NULL;
}


/*----------------------------ɾ---------------------------*/
//����IDɾ���ڵ�------ok
void DelById(Student* head, int id)
{
	if (head == NULL)
	{
		printf("�óɼ��������ڻ��ѱ�ɾ��\n\n");
		return;
	}
	else if (head->next == NULL)
	{
		printf("�óɼ�����ϢΪ��\n\n");
		return;
	}
	
	Student* pre = head;	//ǰ��Ľڵ�
	while (pre->next != NULL)
	{
		Student* tmp = pre->next;	//�ֽڵ�

		if (tmp->identity.id == id)
		{
			head->identity.id--;	//����ͷ�ڵ��е�������
			pre->next = tmp->next;
			free(tmp);	//�ͷſռ�
			printf("ɾ���ɹ�\n\n");
			return;
		}
		pre = pre->next;
	}
	printf("δ���ҵ�id��%d��ѧ��\n\n", id);
}

//����nameɾ���ڵ�-----ok
void DelByName(Student* head, char* name)
{
	if (head == NULL)
	{
		printf("�óɼ��������ڻ��ѱ�ɾ��\n\n");
		return;
	}
	else if (head->next == NULL)
	{
		printf("�óɼ�����ϢΪ��\n\n");
		return;
	}
	head->identity.id--;	//����ͷ�ڵ��е�������
	Student* pre = head;	//ǰ��Ľڵ�
	while (pre->next != NULL)
	{
		Student* tmp = pre->next;	//�ֽڵ�

		if (strcmp(tmp->identity.name, name) == 0)
		{
			pre->next = tmp->next;
			free(tmp);	//�ͷſռ�
			printf("ɾ���ɹ�\n\n");
			return;
		}
		pre = pre->next;
	}
	printf("δ���ҵ�name��%s��ѧ��\n\n", name);
}

//�������ɾ�����нڵ㣬��������ͷ�ڵ㣩------ok
void ClearList(Student* head)
{
	if (head == NULL)
	{
		printf("�óɼ��������ڻ��ѱ�ɾ��\n\n");
		return;
	}
	else if (head->next == NULL)
	{
		printf("�óɼ�����Ϣ��Ϊ��\n\n");
		return;
	}
	head->identity.id = 0;	//����ͷ�ڵ��е�������
	Student* tmp = head->next;
	head->next = NULL;	//������
	Student* p = NULL;
	while (tmp)
	{
		p = tmp->next;
		free(tmp);
		tmp = p;
	}
	//������Ĭ�ϵ�����ʽ��byId
	sortMethod = byId;
	printf("��ճɼ����ɹ�\n\n");
}

//��������(ɾ�����нڵ㣬������ͷ�ڵ�)---ok
//ͨ������ָ������޸�ָ���ֵ
void DestoryList(Student** head)
{
	if (*head == NULL)
	{
		printf("��������\n\n");
		return;
	}
	Student* tmp = *head;
	while (*head)
	{
		*head = tmp->next;
		free(tmp);
		tmp = *head;;
	}
	*head = NULL;
}

//ɾ���ɼ���,ͬʱɾ���ļ�
void DelTable(char* name, char** tableName, int* tableNum)
{
	int found = 0;
	for (int i = 0; i < *tableNum; ++i)
	{
		if (strcmp(name, tableName[i]) == 0)
		{
			found = 1;
			while (i < *tableNum - 1)
			{
				strcpy_s(tableName[i], MaxNameLen, tableName[i + 1]);
				++i;
			}
			free(tableName[i]);	//�ͷ��ڴ棨tableNameָ��Ŀռ������malloc����ģ�
			break;
		}
	}
	if (found == 0)
	{
		printf("δ���ҵ��óɼ���\n\n");
		return;
	}
	char fileName[MaxNameLen + 4];	//���ļ����ƺ�����ӡ�.txt���ļ���׺
	strcpy_s(fileName, MaxNameLen, name);
	int len = strlen(name);
	fileName[len] = '.';
	fileName[len + 1] = 't';
	fileName[len + 2] = 'x';
	fileName[len + 3] = 't';
	fileName[len + 4] = '\0';
	--(*tableNum);
	if (remove(fileName) == 0)		//ɾ���ļ�
	{
		printf("ɾ���ɼ����ɹ�\n\n");
	}
	else
	{
		printf("ɾ���ļ�ʧ��\n\n");
	}
}


/*----------------------------��---------------------------*/
//����ID����ĳһ���ڵ�---ok
Student* FindById(Student* head, long id)
{
	if (head == NULL)
	{
		printf("�óɼ��������ڻ��ѱ�ɾ��\n\n");
		return NULL;
	}
	else if (head->next == NULL)
	{
		printf("�óɼ�����ϢΪ��\n\n");
		return NULL;
	}
	if (head == NULL)return NULL;
	Student* pMove = head->next;
	while (pMove != NULL)
	{
		if (pMove->identity.id == id)
		{
			return pMove;
		}
		pMove = pMove->next;
	}
	printf("δ�ҵ���ѧ����Ϣ\n\n\n");
	return NULL;	//�Ҳ������ؿ�ָ��
}

//�������ֲ���ĳһ���ڵ�---ok
Student* FindByName(Student* head, char* name)
{
	if (head == NULL)
	{
		printf("�óɼ��������ڻ��ѱ�ɾ��\n\n");
		return NULL;
	}
	else if (head->next == NULL)
	{
		printf("�óɼ�����ϢΪ��\n\n");
		return NULL;
	}
	Student* pMove = head->next;
	while (pMove != NULL)
	{
		if (strcmp(pMove->identity.name, name) == 0)
		{
			return pMove;
		}
		pMove = pMove->next;
	}
	printf("δ�ҵ���ѧ����Ϣ\n\n\n");
	return NULL;	//�Ҳ������ؿ�ָ��
}


/*----------------------------��---------------------------*/
//���Ľڵ���Ϣ---ok
void ChangeNode(Student* node, Identity identity, float* score)
{
	if (node == NULL)return;
	node->identity = identity;
	int i;
	float sum = 0;		//�ܷ�
	for (i = 0; i < total; ++i)	//total��ö���������ͣ������Ϊ��Ŀ�ɼ�������
	{
		node->score[i] = score[i];
		sum += score[i];
	}
	node->score[total] = sum;		//��¼�ܷ�
	printf("������Ϣ�ɹ�\n\n");
}



/*---------------------------����--------------------------*/
//�ɼ���Ȩ�أ��ɼ�����������ͬ�����Ķ�id��������;
//id����������
//�ɼ������ǽ���

//����id��������	ð������---ok
void SortById(Student* head)
{
	if (head == NULL || head->next == NULL)return;
	int n = head->identity.id;	//������
	Student* pre = NULL;
	Student* left = NULL;
	Student* right = NULL;
	for (int i = 0; i < n - 1; ++i)
	{
		pre = head;
		left = pre->next;
		right = left->next;
		while (right != NULL)
		{
			if (left->identity.id > right->identity.id)
			{
				pre->next = right;
				left->next = right->next;
				right->next = left;
			}
			pre = pre->next;
			left = pre->next;
			right = left->next;
		}
	}
	printf("�������\n\n");
}
//�ܵ����������ڲ�������id���򡢳ɼ�����
void Sort(Student* head, enum SortMethod method)
{
	if (head == NULL)
	{
		printf("�óɼ��������ڻ��ѱ�ɾ��\n\n");
		return;
	}
	else if (head->next == NULL)
	{
		printf("�óɼ�����ϢΪ��\n\n");
		return;
	}
	//����id����
	if (method == byId)
	{
		SortById(head);
		sortMethod = method;
		return;
	}
	//���ճɼ�����
	int n = head->identity.id;	//������
	Student* pre = NULL;
	Student* left = NULL;
	Student* right = NULL;
	for (int i = 0; i < n - 1; ++i)
	{
		pre = head;
		left = pre->next;
		right = left->next;
		while (right != NULL)
		{
			if (left->score[method] < right->score[method])
			{
				pre->next = right;
				left->next = right->next;
				right->next = left;
			}
			//���������ͬ���ǾͰ���id����
			else if (left->score[method] == right->score[method])
			{
				if (left->identity.id > right->identity.id)
				{
					pre->next = right;
					left->next = right->next;
					right->next = left;
				}
			}

			pre = pre->next;
			left = pre->next;
			right = left->next;
		}
	}
	sortMethod = method;
	printf("�������\n\n");
}


/*---------------------------��ӡ--------------------------*/
//��ӡ��������---ok
void PrintList(Student* head)
{
	if (head == NULL)
	{
		printf("�óɼ��������ڻ��ѱ�ɾ��\n\n");
		return;
	}
	else if (head->next == NULL)
	{
		printf("�óɼ�����ϢΪ��\n\n");
		return;
	}
	Student* p = head;
	printf("���  ID      Name       Programming   Math      Physical  English   History   Average   Total\n");
	int n = 0;
	while (p->next != NULL)
	{
		++n;	//ѧ����ţ�������
		p = p->next;
		printf("%-6d%-8ld%-15s", n, p->identity.id, p->identity.name);
		for (int i = 0; i < total; ++i)
		{
			printf("%-10.1f", p->score[i]);
		}
		printf("%-10.1f%-10.1f\n", p->score[total] / total, p->score[total]);
	}
	printf("\n\n");
}
//��ӡָ���ڵ�����
void PrintNode(Student* node)
{
	if (node == NULL)return;
	printf("%-8ld%-15s", node->identity.id, node->identity.name);
	for (int i = 0; i < total; ++i)
	{
		printf("%-10.1f", node->score[i]);
	}
	printf("%-10.1f%-10.1f\n", node->score[total] / total, node->score[total]);
}
//��ӡ������ѧ����Ϣ---ֻҪ��һ�Ʋ�����Ϊ������ѧ��
void PrintNoPass(Student* head)
{
	if (head == NULL)
	{
		printf("�óɼ��������ڻ��ѱ�ɾ��\n\n");
		return;
	}
	else if (head->next == NULL)
	{
		printf("�óɼ�����ϢΪ��\n\n");
		return;
	}
	Student* p = head;
	printf("���  ID      Name       Programming   Math      Physical  English   History   Average   Total\n");
	int n = 0;
	while (p->next != NULL)
	{
		p = p->next;
		for (int i = 0; i < total; ++i)
		{
			if (p->score[i] < 60)
			{
				++n;
				printf("%-6d", n);
				PrintNode(p);
				break;
			}
		}
	}
	if (n == 0)
		printf("�޲������ѧ��\n\n");
}
//��ӡ����ѧ����Ϣ
void PrintPass(Student* head)
{
	if (head == NULL)
	{
		printf("�óɼ��������ڻ��ѱ�ɾ��\n\n");
		return;
	}
	else if (head->next == NULL)
	{
		printf("�óɼ�����ϢΪ��\n\n");
		return;
	}
	Student* p = head;

	int n = 0;
	printf("���  ID      Name       Programming   Math      Physical  English   History   Average   Total\n");
	while (p->next != NULL)
	{
		p = p->next;
		int pass = 1;
		for (int i = 0; i < total; ++i)
		{
			if (p->score[i] < 60)
			{
				pass = 0;
			}
		}
		if (pass)
		{
			++n;
			printf("%-6d", n);
			PrintNode(p);
		}
	}
	if (n == 0)
		printf("�޼����ѧ��\n\n");
}
//��ӡ���гɼ�������
void PrintTable(char** tableName, int tableNum)
{
	if (tableName == NULL || tableNum < 1)
	{
		printf("δ�����ɼ���\n\n");
		return;
	}
	printf("���    ����\n");
	for (int i = 0; i < tableNum; ++i)
	{
		printf("%-8d%s\n", i + 1, tableName[i]);
	}
	printf("\n\n");
}
//�ɼ��ֲ�(���Ƴɼ���ƽ����)		0��100�֣�ÿ���10�ֵ�����
void PrintDistribution(Student* head, enum Course course)
{
	if (head == NULL)
	{
		printf("�óɼ��������ڻ��ѱ�ɾ��\n\n");
		return;
	}
	else if (head->next == NULL)
	{
		printf("�óɼ�����ϢΪ��\n\n");
		return;
	}

	int num[10] = { 0 };
	Student* tmp = head;
	float min = 100.0;			//��ͷ�
	float max = 0.0;			//��߷�
	int n;
	float score = 0;

	while (tmp->next != NULL)
	{
		tmp = tmp->next;

		if (course == total)
			score = (tmp->score[course]) / course;	//����ƽ����
		else
			score = tmp->score[course];
		//��¼��߷�ѧ��
		if (score > max)
			max = score;
		//��¼��ͷ�
		if (score < min)
			min = score;
		//����������ε�����
		n = (int)(score / 10);
		if (n == 10)
			num[9]++;
		else
			num[n]++;
	}
	printf("\n��������%d\n", head->identity.id);
	printf("��߷֣�%.1f\t��ͷ֣�%.1f", max, min);
	float f[10] = { 0.0 };			//�������ε�Ƶ��
	int num2[10] = { 0 };			//Ҫ��ӡ��*���ĸ���
	for (int j = 0; j < 10; ++j)
	{
		//����������ε�Ƶ��
		f[j] = (double)num[j] / (head->identity.id);	
		//��Ƶ��ת��Ϊ0~100������  ����*���ĸ���
		num2[j] = (int)(f[j] * 100 + 0.5);				
	}
	printf("\nscale\tnumber\n");
	for (int j = 0; j < 9; ++j)
	{
		printf("%2d~%2d:  ", j * 10, (j + 1) * 10 - 1);
		printf("%2d\t", num[j]);
		for (int i = 0; i < num2[j]; ++i)
		{
			printf("*");
		}
		printf("\n");
	}
	printf("90~100: ");
	printf("%2d\t", num[9]);
	for (int i = 0; i < num2[9]; ++i)
	{
		printf("*");
	}
	printf("\n\n");
}