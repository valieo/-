/*

项目名称：学生成绩管理系统
作者：valieo

*/

/*
说明：

每新建一个成绩单就会新增一个与成绩单同名的文本文件
每删除一个成绩单就会减少一个与成绩单同名的文本文件

现程序中已建立了两个成绩单，名称和成绩单内学生数如下：
Class01   30人				对应的文件：Class01.txt
计算机2班  0人				对应的文件：计算机2班.txt

data.txt文件用于存储成绩单名称和成绩单数量

可以任意建立多个成绩单，也可以在成绩单内任意添加多个学生信息，学生信息越多，成绩分布图会越明显
*/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//名字的最大长度
#define MaxNameLen 25

//存储成绩单数量和名称的文件名
const char FileName[] = "data.txt";


//添加学生信息的方法（头部、尾部、按当前排序方法插入）
enum InputMethod { byHead, byEnd, inOrder };
//枚举结构--各科成绩(程序设计、高数、大物、英语、近现代史)、总成绩
enum Course { programming, math, physical, english, history, total };
//记录当前排序方式
enum SortMethod { byProgramming, byMath, byPhysical, byEnglish, byHistory, byTotal, byId, mess };

enum SortMethod sortMethod = byId;	//全局变量 默认通过id大小进行排序


//身份信息
typedef struct Identity {
	long id;				//学号
	char name[MaxNameLen];	//姓名
}Identity;
//学生信息
typedef struct Student {
	Identity identity;	//身份
	float score[total + 1];	//存储5个学科成绩+1个总成绩
	struct Student* next;
}Student;


/*---------------------------函数声明---------------------------*/
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
	system("title 学生成绩管理系统");		//窗口名称
	
	
	int again = 1;	//是否退出程序
	int menu1 = 1;	//当前状态：菜单一
	int menu2 = 1;  //当前状态：非菜单一
	while (again)
	{
		Student* target = NULL; //指向查找到的那个节点
		Student* head = NULL;	//链表头
		char** tableName = NULL; //成绩单名称
		int tableNum = 0;		//成绩单数量
		int method;				//方法（byChinese,byTotal）

		//一级菜单	主菜单
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
					WriteFileNameByFile(tableName, tableNum);//保存成绩单名称
					WriteStuInfoByFile(head);//保存新建的成绩单
					menu1 = 0;
					menu2 = 1;	//进入二级菜单
				}
				break;
			case 2:
				PrintTable(tableName, tableNum);
				printf("请输入成绩单名称：");
				scanf_s("%s", name, MaxNameLen);
				head = EnterTable(name, tableName, tableNum);
				if (head == NULL)
				{
					printf("未查找到该成绩单\n");
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
				printf("请输入要删除的成绩单名称：");
				scanf_s("%s", name, MaxNameLen);
				DelTable(name, tableName, &tableNum);
				WriteFileNameByFile(tableName, tableNum);
				break;
			case 0:
				//释放tableName		tableName是二级指针
				for (int i = 0; i < tableNum; ++i)
				{
					free(tableName[i]);
				}
				free(tableName);
				again = 0;	//退出程序
				break;
			default:
				printf("输入错误\n\n");
				break;
			}
			//system("cls");
		}

		system("cls");

		//二级菜单
		while (menu2 && again)
			{
				Menu();
				int key2;
				scanf_s("%d", &key2);
				system("cls");
				int isDel = 0;//默认不删除成绩单

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
					printf("请输入要删除的id：");
					scanf_s("%ld", &id);
					DelById(head, id);
					break;
				case 5:
					printf("请输入要删除的name：");
					scanf_s("%s", name, MaxNameLen);
					DelByName(head, name);
					break;
				case 6:
					ClearList(head);
					break;
				case 7:
					printf("该删除操作不可撤销，确认删除该成绩单？（1.Yes  2.No）：\n");
					isDel = 0;
					scanf_s("%d", &isDel);
					if (isDel == 1)
					{
						//更新存储成绩单名称和数量的文件
						DelTable(head->identity.name, tableName, &tableNum);
						WriteFileNameByFile(tableName, tableNum);
						DestoryList(&head);
						menu2 = 0;	//删除后返回到主菜单
						menu1 = 1;
					}
					else
						printf("已取消删除操作\n");
					break;
				case 8:
					printf("请输入要查找的id：");
					scanf_s("%ld", &id);
					target = FindById(head, id);
					if (target)
					{
						printf("ID      Name       Programming   Math      Physical  English   History   Average   Total\n");
						PrintNode(target);
					}
					break;
				case 9:
					printf("请输入要查找的name：");
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
						printf("请先查找到需要修改的那个学生信息，然后再进行修改操作（使用功能8或9）\n");
					}
					else
					{
						printf("你将修改id:%ld,name:%s的信息\n请按照下列格式输入该学生的最新信息\nID Name Programming Math Physical English History\n", target->identity.id, target->identity.name);
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
								printf("输入格式有误\n");
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
					printf("请选择排序依据方法：\n1.Programming  2.Math  3.Physical  4.English  5.History  6.Total  7.ID\n");
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
					printf("请选择下列成绩中的一项\n1.Programming  2.Math  3.Physical  4.English  5.History  6.Average\n");
					scanf_s("%d", &method);
					PrintDistribution(head, (Course)(method - 1));
					break;
				case 16:
					WriteStuInfoByFile(head);
					break;
				case 0:
					printf("未保存的操作将会丢失(0.确认返回  1.取消返回)\n");
					scanf_s("%d", &method);
					if (method == 0)
					{
						DestoryList(&head);	//返回时销毁链表，释放malloc分配的内存
						menu2 = 0;
						menu1 = 1;
					}
					break;
				default:
					printf("输入错误\n\n");
					break;
				}

				printf("按回车显示菜单");
				getchar();		//吃掉输入key值时的回车
				getchar();
				system("cls");
			}
	}
	return 0;
}



/*---------------------------界面---------------------------*/
//主菜单	对成绩单的操作
void MainMenu()
{
	printf("\n");
	printf("\t                               学生成绩管理系统\n");
	printf("\t*****************************************************************************\n");
	printf("\t*                                  主菜单                                   *\n");
	printf("\t*                1.  建立新的成绩单                                         *\n");
	printf("\t*                2.  进入现有的成绩单                                       *\n");
	printf("\t*                3.  显示所有成绩单                                         *\n");
	printf("\t*                4.  删除成绩单                                             *\n");
	printf("\t*                0.  退出管理系统                                           *\n");
	printf("\t*****************************************************************************\n");
	printf("\n\n");
	printf("进入功能：");
}
//二级菜单	对成绩单中信息的操作
void Menu()
{
	printf("\n\
        ---------------------------------------------------------------\n\
                                学生成绩管理系统\n\
        ===============================================================\n\
        增加>>          1.头部增加\n\
                        2.尾部增加\n\
                        3.有序增加\n\
        删除>>          4.删除信息（id）\n\
                        5.删除信息（name）\n\
                        6.清空信息\n\
                        7.删除成绩单\n\
        查询>>          8.根据id查询\n\
                        9.根据name查询\n\
        更改>>          10.更改信息\n\
        排序>>          11.排序\n\
        输出>>          12.不及格学生\n\
                        13.及格学生\n\
                        14.所有学生\n\
                        15.成绩分布及最高分\n\
        保存>>          16.保存\n\
        返回>>          0.返回\n\
        ===============================================================\n\
");
	printf("进入功能：");
}


/*-------------------------文件操作--------------------------*/
//读取成绩单名称和成绩单数量
char** ReadFileNameByFile(int* tableNum)
{
	FILE* fp;
	fopen_s(&fp, FileName, "r");
	if (fp == NULL)
	{
		*tableNum = 0;
		return NULL;
	}
	*tableNum = 0;	//n个成绩单
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
//写入成绩单名称和数量
void WriteFileNameByFile(char** tableName, int tableNum)
{
	FILE* fp;
	fopen_s(&fp, FileName, "w");
	if (fp == NULL)
	{
		printf("打开文件失败，无法写入\n");
		return;
	}
	fprintf(fp, "%d\n", tableNum);		//文件第一行：成绩单数量
	for (int i = 0; i < tableNum; ++i)
	{
		fprintf(fp, "%s\n", tableName[i]);
	}
	fclose(fp);
}
//读取学生信息
Student* ReadStuInfoByFile(char* fileName)
{
	FILE* fp;
	char name[MaxNameLen + 4];	//给文件名称后面添加“.txt”文件后缀
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
		printf("打开文件失败，无法读取\n");
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
	int n = head->identity.id;	//链表长度
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
//写入学生信息
void WriteStuInfoByFile(Student* head)
{
	if (head == NULL)
	{
		return;
	}
	char name[MaxNameLen + 4];	//给文件名称后面添加“.txt”文件后缀
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
		printf("打开文件失败，无法写入\n");
		return;
	}
	Student* tmp = head;
	fprintf(fp, "%d\n", sortMethod);		//文件第一行：当前成绩单排序方法代号
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
	printf("保存成功\n\n");
}


/*-------------------------创建链表---------------------------*/
//创建链表头-------ok
Student* CreateList(char* name)
{
	Student* head = (Student*)malloc(sizeof(Student));
	if (head == NULL)
	{
		printf("malloc error in CreateList function\n");
		return NULL;
	}
	//数据初始化
	strcpy_s(head->identity.name, MaxNameLen, name);
	for (int i = 0; i <= total; ++i)
	{
		head->score[i] = 0;
	}
	head->identity.id = 0;		//在头节点中，id用来存储链表长度
	head->next = NULL;
	return head;
}
//创建节点--------ok
Student* CreateNode(Identity identity, float* score)
{
	Student* node = (Student*)malloc(sizeof(Student));
	node->identity = identity;
	int i;
	float sum = 0;		//总分
	for (i = 0; i < total; ++i)	//total是枚举数据类型，其序号为科目成绩的数量
	{
		node->score[i] = score[i];
		sum += score[i];
	}
	node->score[i] = sum;		//记录总分
	node->next = NULL;
	return node;
}


/*----------------------------增---------------------------*/
//从头部插入节点------ok
void InsertByHead(Student* head, Identity identity, float* score)
{
	if (head == NULL)return;
	head->identity.id++;	//更新头节点中的链表长度
	Student* node = CreateNode(identity, score);
	node->next = head->next;
	head->next = node;
	//链表无序
	sortMethod = mess;
}

//从尾部插入节点-----ok
void InsertByEnd(Student* head, Identity identity, float* score)
{
	if (head == NULL) return;
	head->identity.id++;	//更新头节点中的链表长度
	Student* node = CreateNode(identity, score);
	Student* end = head;	//此处每次插入尾部时都要遍历整个链表，后续进行优化
	while (end->next != NULL)
	{
		end = end->next;
	}
	end->next = node;
	end = node;
	//链表无序
	sortMethod = mess;
}

//按大小顺序插入节点	前提：原链表有序---ok
void InsertInOrder(Student* head, Identity identity, float* score)
{
	if (head == NULL)return;
	if (sortMethod == mess && head->next != NULL)
	{
		printf("当前学生信息为乱序(已经进行了头部或尾部插入数据)，请排序后进行有序插入\n");
		return;
	}

	++(head->identity.id);
	Student* node = CreateNode(identity, score);
	//空链表直接插入
	if (head->next == NULL)
	{
		head->next = node;
		return;
	}

	//按id顺序插入(id是升序)
	if (sortMethod == byId)
	{
		Student* pre = head;
		Student* tmp = NULL;
		while (pre->next != NULL)
		{
			tmp = pre->next;
			//插在头部和中间情况
			if (node->identity.id <= tmp->identity.id)
			{
				pre->next = node;
				node->next = tmp;
				return;
			}
			//插在末尾情况
			else if (node->identity.id > tmp->identity.id && tmp->next == NULL)
			{
				tmp->next = node;
				return;
			}
			pre = pre->next;
		}
	}
	//按成绩顺序插入（成绩为降序）
	else if (sortMethod != mess)
	{
		Student* pre = head;
		Student* tmp = NULL;
		while (pre->next != NULL)
		{
			tmp = pre->next;
			//插在头部和中间情况
			if (node->score[sortMethod] >= tmp->score[sortMethod])
			{
				pre->next = node;
				node->next = tmp;
				return;
			}
			//插在末尾情况
			else if (node->score[sortMethod] < tmp->score[sortMethod] && tmp->next == NULL)
			{
				tmp->next = node;
				return;
			}
			pre = pre->next;
		}
	}
}

//输入学生信息
void InputData(Student* head, enum InputMethod method)
{
	if (head == NULL)
	{
		printf("该成绩单不存在或已被删除\n\n");
		return;
	}
	//如果当前链表不是空链表，而且是乱序，那么就不能进行有序插入
	if (sortMethod == mess && head->next != NULL)
	{
		printf("当前学生信息为乱序(已经进行了头部或尾部插入数据)，请排序后进行有序插入\n");
		return;
	}
	int n, k;
	Identity identity;
	float score[total] = { 0 };
	printf("要添加的学生个数：");
	scanf_s("%d", &n);
	if (n < 1)
	{
		printf("输入人数不能小于1\n\n");
		return;
	}
	printf("请按如下格式输入学生信息\nID Name Programming Math Physical English History\n");
	for (int i = 0; i < n; ++i)
	{
		k = scanf_s("%ld %s ", &identity.id, &identity.name, MaxNameLen);
		if (k != 2)
		{
			printf("输入格式有误\n\n");
			return;
		}
		for (int i = 0; i < total; ++i)
		{
			if (scanf_s("%f", &score[i]) != 1)
			{
				printf("输入格式有误\n\n");
				return;
			}
		}
		//检查ID是否重复
		Student* tmp = head;
		while (tmp->next != NULL)
		{
			tmp = tmp->next;
			if (tmp->identity.id == identity.id)
			{
				printf("已经存在ID：%d的学生信息\n\n", identity.id);
				return;
			}
		}
		//检查分数输入是否符合范围
		for (int i = 0; i < total; ++i)
		{
			if (score[i] < 0 || score[i]>100)
			{
				printf("输入的成绩有误，成绩的范围为0~100\n\n");
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
	printf("添加信息成功\n\n");
}

//增加一个成绩单---用到三重指针
Student* AddList(char*** tableName, int* tableNum)
{
	printf("输入新建成绩单名称：");
	char name[MaxNameLen];
	scanf_s("%s", name, MaxNameLen);
	for (int i = 0; i < *tableNum; ++i)
	{
		if (strcmp(name, (*tableName)[i]) == 0)
		{
			printf("该名称已存在\n\n");
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
	Student* head = CreateList(name);	//创建一个名为name的链表
	printf("新建成功\n\n");
	return head;
}

//进入现有的成绩单
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


/*----------------------------删---------------------------*/
//根据ID删除节点------ok
void DelById(Student* head, int id)
{
	if (head == NULL)
	{
		printf("该成绩单不存在或已被删除\n\n");
		return;
	}
	else if (head->next == NULL)
	{
		printf("该成绩单信息为空\n\n");
		return;
	}
	
	Student* pre = head;	//前面的节点
	while (pre->next != NULL)
	{
		Student* tmp = pre->next;	//现节点

		if (tmp->identity.id == id)
		{
			head->identity.id--;	//更新头节点中的链表长度
			pre->next = tmp->next;
			free(tmp);	//释放空间
			printf("删除成功\n\n");
			return;
		}
		pre = pre->next;
	}
	printf("未查找到id：%d的学生\n\n", id);
}

//根据name删除节点-----ok
void DelByName(Student* head, char* name)
{
	if (head == NULL)
	{
		printf("该成绩单不存在或已被删除\n\n");
		return;
	}
	else if (head->next == NULL)
	{
		printf("该成绩单信息为空\n\n");
		return;
	}
	head->identity.id--;	//更新头节点中的链表长度
	Student* pre = head;	//前面的节点
	while (pre->next != NULL)
	{
		Student* tmp = pre->next;	//现节点

		if (strcmp(tmp->identity.name, name) == 0)
		{
			pre->next = tmp->next;
			free(tmp);	//释放空间
			printf("删除成功\n\n");
			return;
		}
		pre = pre->next;
	}
	printf("未查找到name：%s的学生\n\n", name);
}

//清空链表（删除所有节点，不包括表头节点）------ok
void ClearList(Student* head)
{
	if (head == NULL)
	{
		printf("该成绩单不存在或已被删除\n\n");
		return;
	}
	else if (head->next == NULL)
	{
		printf("该成绩单信息已为空\n\n");
		return;
	}
	head->identity.id = 0;	//更新头节点中的链表长度
	Student* tmp = head->next;
	head->next = NULL;	//空链表
	Student* p = NULL;
	while (tmp)
	{
		p = tmp->next;
		free(tmp);
		tmp = p;
	}
	//空链表默认的排序方式是byId
	sortMethod = byId;
	printf("清空成绩单成功\n\n");
}

//销毁链表(删除所有节点，包括表头节点)---ok
//通过二重指针才能修改指针的值
void DestoryList(Student** head)
{
	if (*head == NULL)
	{
		printf("链表不存在\n\n");
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

//删除成绩单,同时删除文件
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
			free(tableName[i]);	//释放内存（tableName指向的空间均是由malloc分配的）
			break;
		}
	}
	if (found == 0)
	{
		printf("未查找到该成绩单\n\n");
		return;
	}
	char fileName[MaxNameLen + 4];	//给文件名称后面添加“.txt”文件后缀
	strcpy_s(fileName, MaxNameLen, name);
	int len = strlen(name);
	fileName[len] = '.';
	fileName[len + 1] = 't';
	fileName[len + 2] = 'x';
	fileName[len + 3] = 't';
	fileName[len + 4] = '\0';
	--(*tableNum);
	if (remove(fileName) == 0)		//删除文件
	{
		printf("删除成绩单成功\n\n");
	}
	else
	{
		printf("删除文件失败\n\n");
	}
}


/*----------------------------查---------------------------*/
//根据ID查找某一个节点---ok
Student* FindById(Student* head, long id)
{
	if (head == NULL)
	{
		printf("该成绩单不存在或已被删除\n\n");
		return NULL;
	}
	else if (head->next == NULL)
	{
		printf("该成绩单信息为空\n\n");
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
	printf("未找到该学生信息\n\n\n");
	return NULL;	//找不到返回空指针
}

//根据名字查找某一个节点---ok
Student* FindByName(Student* head, char* name)
{
	if (head == NULL)
	{
		printf("该成绩单不存在或已被删除\n\n");
		return NULL;
	}
	else if (head->next == NULL)
	{
		printf("该成绩单信息为空\n\n");
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
	printf("未找到该学生信息\n\n\n");
	return NULL;	//找不到返回空指针
}


/*----------------------------改---------------------------*/
//更改节点信息---ok
void ChangeNode(Student* node, Identity identity, float* score)
{
	if (node == NULL)return;
	node->identity = identity;
	int i;
	float sum = 0;		//总分
	for (i = 0; i < total; ++i)	//total是枚举数据类型，其序号为科目成绩的数量
	{
		node->score[i] = score[i];
		sum += score[i];
	}
	node->score[total] = sum;		//记录总分
	printf("更改信息成功\n\n");
}



/*---------------------------排序--------------------------*/
//成绩的权重：成绩排名具有相同分数的对id进行排名;
//id排名是升序
//成绩排名是降序

//依据id排序（升序）	冒泡排序---ok
void SortById(Student* head)
{
	if (head == NULL || head->next == NULL)return;
	int n = head->identity.id;	//链表长度
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
	printf("排序完成\n\n");
}
//总的排序函数（内部包含了id排序、成绩排序）
void Sort(Student* head, enum SortMethod method)
{
	if (head == NULL)
	{
		printf("该成绩单不存在或已被删除\n\n");
		return;
	}
	else if (head->next == NULL)
	{
		printf("该成绩单信息为空\n\n");
		return;
	}
	//按照id排序
	if (method == byId)
	{
		SortById(head);
		sortMethod = method;
		return;
	}
	//按照成绩排序
	int n = head->identity.id;	//链表长度
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
			//如果分数相同，那就按照id排序
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
	printf("排序完成\n\n");
}


/*---------------------------打印--------------------------*/
//打印所有数据---ok
void PrintList(Student* head)
{
	if (head == NULL)
	{
		printf("该成绩单不存在或已被删除\n\n");
		return;
	}
	else if (head->next == NULL)
	{
		printf("该成绩单信息为空\n\n");
		return;
	}
	Student* p = head;
	printf("序号  ID      Name       Programming   Math      Physical  English   History   Average   Total\n");
	int n = 0;
	while (p->next != NULL)
	{
		++n;	//学生序号（数量）
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
//打印指定节点数据
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
//打印不及格学生信息---只要有一科不及格即为不及格学生
void PrintNoPass(Student* head)
{
	if (head == NULL)
	{
		printf("该成绩单不存在或已被删除\n\n");
		return;
	}
	else if (head->next == NULL)
	{
		printf("该成绩单信息为空\n\n");
		return;
	}
	Student* p = head;
	printf("序号  ID      Name       Programming   Math      Physical  English   History   Average   Total\n");
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
		printf("无不及格的学生\n\n");
}
//打印及格学生信息
void PrintPass(Student* head)
{
	if (head == NULL)
	{
		printf("该成绩单不存在或已被删除\n\n");
		return;
	}
	else if (head->next == NULL)
	{
		printf("该成绩单信息为空\n\n");
		return;
	}
	Student* p = head;

	int n = 0;
	printf("序号  ID      Name       Programming   Math      Physical  English   History   Average   Total\n");
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
		printf("无及格的学生\n\n");
}
//打印所有成绩单名称
void PrintTable(char** tableName, int tableNum)
{
	if (tableName == NULL || tableNum < 1)
	{
		printf("未建立成绩单\n\n");
		return;
	}
	printf("序号    名称\n");
	for (int i = 0; i < tableNum; ++i)
	{
		printf("%-8d%s\n", i + 1, tableName[i]);
	}
	printf("\n\n");
}
//成绩分布(单科成绩和平均分)		0到100分，每间隔10分的人数
void PrintDistribution(Student* head, enum Course course)
{
	if (head == NULL)
	{
		printf("该成绩单不存在或已被删除\n\n");
		return;
	}
	else if (head->next == NULL)
	{
		printf("该成绩单信息为空\n\n");
		return;
	}

	int num[10] = { 0 };
	Student* tmp = head;
	float min = 100.0;			//最低分
	float max = 0.0;			//最高分
	int n;
	float score = 0;

	while (tmp->next != NULL)
	{
		tmp = tmp->next;

		if (course == total)
			score = (tmp->score[course]) / course;	//计算平均分
		else
			score = tmp->score[course];
		//记录最高分学生
		if (score > max)
			max = score;
		//记录最低分
		if (score < min)
			min = score;
		//计算各分数段的人数
		n = (int)(score / 10);
		if (n == 10)
			num[9]++;
		else
			num[n]++;
	}
	printf("\n总人数：%d\n", head->identity.id);
	printf("最高分：%.1f\t最低分：%.1f", max, min);
	float f[10] = { 0.0 };			//个分数段的频率
	int num2[10] = { 0 };			//要打印“*”的个数
	for (int j = 0; j < 10; ++j)
	{
		//计算各分数段的频率
		f[j] = (double)num[j] / (head->identity.id);	
		//将频率转换为0~100的整数  即“*”的个数
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