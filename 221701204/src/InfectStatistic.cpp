/*********************************
FileName:InfectStatistic.cpp
Author:����ϣ
Function:����״��ͳ�� 
**********************************/

#pragma warning(disable:4996)
#include <io.h>		
#include <iostream>
#include <fstream>
#include <cstring>
#include <windows.h>
#include<vector>
#include<algorithm>
#include<map>
#include<string>

using namespace std;

class Analysis
{
public:
	char outpath[1005];
	string province[40];
	map<string, int>mi;
	map<string, int>mi1;
	map<string, int>mi2;
	map<string, int>mi3;
	string province_of[40];
	string type[20];
	string Begin(string com);
	string Check(int date_,char *date);
	int province_num, type_num;
	int out[40][6];
	int Num(string tmp);
	void FindAllFile(const char* path, const char* format, const char* date);
	void Change(int province1,int province2,int type1,int type2,int num,int l);
	void Init();
	void Out();
	void CheckType(int num);
	void CheckAgain();
	void LineDetail(string tmp);
    int Detail(string p, string t) 
    {
        return out[mi[p]][mi2[t]];
    }
    
};

int Analysis::Num(string tmp)
{
    int num = 0;
    for (int i = tmp.size() - 1,c = 1;i >= 0;i--)
	{
		if (tmp[i] >= '0' && tmp[i] <= '9')num += (tmp[i] - '0') * c, c *= 10;
	}
	return num;
}

void Analysis::Change(int province1, int province2,int type1,int type2,int num,int l)
{
	if (province2 == 0 && type2 == 0)
	{
		if (l == 100)out[province1][type1] += num;
		else if (l == 101)out[province1][type1] -= num;
		else if (l == 102)
		{
			out[province1][mi2["��Ⱦ����"]] -= num;
			out[province1][mi2["����"]] += num;	
		}
		else 
		{
			out[province1][mi2["����"]] += num;
			out[province1][mi2["��Ⱦ����"]] -= num;
		}
	}
	else if (province2 == 0 && type2 != 0)
	{
		out[province1][mi2["���ƻ���"]] -= num;
		out[province1][mi2["��Ⱦ����"]] += num;
	}
	else 
	{
		out[province1][type1] -= num;
		out[province2][type1] += num;
	}
}

void Analysis::LineDetail(string tmp)
{
	int province1 = 0, province2 = 0;
	string s = "";
	int l = 0;
	for (int j = 0;j < tmp.size();j++) 
	{
		if (tmp[j] == ' ')
		{
			if (!mi.count(s))
			{
				s = "";
				continue;
			}
			mi3[s] = 1;
			if (mi[s] >= 100)l = mi[s];
			else if (province1 == 0)province1 = mi[s];
			else province2 = mi[s];
			s = "";
		}
		else s = s + tmp[j];
	}
	int type1 = 0, type2 = 0;
	string ss = "";
	for (int j = 0;j < tmp.size();j++)
	{
		if (tmp[j] == ' ')
		{
			if (!mi2.count(ss))
			{
				ss = "";
				continue;
			}
			if (type1 == 0)type1 = mi2[ss];
			else type2 = mi2[ss];
			ss = "";
		}
		else 
		{
			ss = ss + tmp[j];
		}
	}
	int num = Num(tmp);
					//	cout<<fl1<<' '<<fl2<<' '<<f1<<' '<<f2<<' '<<num<<endl;
	Change(province1,province2,type1,type2,num,l);
}

void Analysis::FindAllFile(const char* path, const char* format, const char* date)
{
    char newpath[200];
    char newpaths[200];
    strcpy(newpath, path);
    strcpy(newpaths, path);
	strcat(newpath, "\\*.*");    // ��Ŀ¼�������"\\*.*"���е�һ������
	int handle;
	int returnflag = 0;
	_finddata_t findData;
	handle = _findfirst(newpath, &findData);
	if (handle == -1)
	{
		// ����Ƿ�ɹ�
		return;
	}
    while (_findnext(handle, &findData) == 0) 
    {
        if (findData.attrib & _A_SUBDIR) {
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;
			strcpy(newpath, path);
			strcat(newpath, "\\");
			strcat(newpath, findData.name);
			FindAllFile(newpath, format, date);
		}
        else 
        {
            if (strstr(findData.name, format))
			{     //�ж��ǲ���txt�ļ� 
				
				int size = strlen(date);
				int flg = 1;
				if (size != 0)
				{
					for (int i = 0;i < size;i++)
					{
						if (findData.name[i] != date[i])flg = 0;
						if (findData.name[i] > date[i])returnflag = 1;
					}
				}
				if (returnflag == 1)return ;
				if (flg == 1&&size != 0)returnflag = 1;
				freopen(outpath, "w", stdout);
				char nowpath[200];
				strcpy(nowpath, newpaths);
				strcat(nowpath, "\\");
				strcat(nowpath, findData.name);
				ifstream fin(nowpath);
				if (!fin.is_open())cout << "error";
				string tmp;
				while (getline(fin, tmp))
				{
					if (tmp[0] == '/' && tmp[1] == '/')continue;
					LineDetail(tmp);
				}
				if (returnflag == 1)return;
			}
		}
	}
	_findclose(handle);    // �ر��������
}

void Analysis::Init()
{
	memset(out, 0, sizeof(out));
	for (int i = 0;i <= 35;i++)province[i] = "";
	mi.clear();
	mi1.clear();
	mi2.clear();
	mi3.clear();
	province_num = 0, type_num = 0;
	mi["����"] = 100;
	mi["�ų�"] = 101;
	mi["����"] = 102;
	mi["����"] = 103;
	mi["ȫ��"] = 1;
	mi["����"] = 2, mi["����"] = 3, mi["����"] = 4;
	 mi["����"] = 5, mi["����"] = 6, mi["�㶫"] = 7;
	mi["����"] = 8, mi["����"] = 9, mi["����"] = 10;
	mi["�ӱ�"] = 11, mi["����"] = 12, mi["������"] = 13;
	mi["����"] = 14, mi["����"] = 15, mi["����"] = 16;
	mi["����"] = 17, mi["����"] = 18, mi["����"] = 19;
	mi["���ɹ�"] = 20, mi["����"] = 21, mi["�ຣ"] = 22;
	 mi["ɽ��"] = 23, mi["ɽ��"] = 24, mi["����"] = 25;
	mi["�Ϻ�"] = 26, mi[""] = 27, mi["�Ĵ�"] = 28;
	mi["̨��"] = 29, mi["���"] = 30, mi["����"] = 31;
	mi["���"] = 32, mi["�½�"] = 33, mi["����"] = 34,mi["�㽭"];
	province_of[1] = "ȫ��", province_of[2] = "����", province_of[3] = "����";
	province_of[4] = "����", province_of[5] = "����", province_of[6] = "����";
	province_of[7] = "�㶫", province_of[8] = "����", province_of[9] = "����";
	province_of[10] = "����", province_of[11] = "�ӱ�", province_of[12] = "����" ;
	province_of[13] = "������", province_of[14] = "����", province_of[15] = "����";
	province_of[16] = "����", province_of[17] = "����", province_of[18] = "����";
	province_of[19] = "����", province_of[20] = "���ɹ�", province_of[21] = "����";
	province_of[22] = "�ຣ", province_of[23] = "ɽ��", province_of[24] = "ɽ��";
	province_of[25] = "����", province_of[26] = "�Ϻ�", province_of[28] = "�Ĵ�";
	province_of[29] = "̨��", province_of[30] = "���", province_of[31] = "����";
	province_of[32] = "���", province_of[33] = "�½�", province_of[34] = "����";
	province_of[35] = "�㽭"; province_of[27]="-";
}


void Analysis::Out()
{
	for (int i = 2;i < 36;i++)
	{
		for (int j = 1;j < type_num;j++)
		{
			out[1][j] += out[i][j];
		}
	}
	for (int i = 1;i < 36;i++)
	{
		bool flg= province_num == 0 &&(mi3.count(province_of[i]) || i == 1);
		if (mi1.count(province_of[i]) || flg ) 
		{
			cout << province_of[i] << ' ';
			for (int j = 1;j < type_num;j++)
			{
				cout << type[j] << out[i][j] << "�� ";
			}
			if (mi1[province_of[i]] == 1 || province_num == 0)
			{
				cout << "\n";
			}
		}
	}
	cout << "// ���ĵ�������ʵ���ݣ���������ʹ��" << "\n";
}

string Analysis::Check(int date_,char *date)
{
	string data="";
	if (date_ > 0)
	{
		for (int i = 0;i < date_;i++)data = data + date[i];
		data = data + " ";
	}
	if (province_num > 0)
	{
		for (int i = 0;i < province_num;i++)data = data + province[i];
		data = data + " ";
	}
	if (type_num > 1)
	{
		for (int i = 1;i < type_num;i++)data = data + type[i];
		data = data + " ";
	}
	return data;
}

void Analysis::CheckType(int num)
{
	int c = num;
	if (!mi2.count("��Ⱦ����"))mi2["��Ⱦ����"] = c, type[c++] = "��Ⱦ����";
	if (!mi2.count("���ƻ���"))mi2["���ƻ���"] = c, type[c++] = "���ƻ���";
	if (!mi2.count("����"))mi2["����"] = c, type[c++] = "����";
	if (!mi2.count("����"))mi2["����"] = c, type[c++] = "����";
	mi2["ȷ���Ⱦ"] = 11;
}

void Analysis::CheckAgain()
{
	if (type_num == 0)
	{
		int c = 1;
		if (!mi2.count("��Ⱦ����"))mi2["��Ⱦ����"] = c, type[c++] = "��Ⱦ����";
		if (!mi2.count("���ƻ���"))mi2["���ƻ���"] = c, type[c++] = "���ƻ���";
		if (!mi2.count("����"))mi2["����"] = c, type[c++] = "����";
		if (!mi2.count("����"))mi2["����"] = c, type[c++] = "����";
		mi2["ȷ���Ⱦ"] = 11;
		type_num = c;
	}
}

string Analysis::Begin(string com)
{
	Init();
	char inpath[1005], date[105];
	
	int cc = 0;
	int size = com.size();
	for (int i = 0;i < size;i++)
	{
		string s = "";
		while (i < size && com[i] != ' ')
		{
			s = s + com[i];
			i++;
		}
		i++;
		if (s == "-log")
		{
			int c = 0;
			while (i < size && com[i] != ' ')
			{
				inpath[c] = com[i];
				i++, c++;
			}
			inpath[c] = '\0';
		}
		else if (s == "-out")
		{
			int c = 0;
			while (i < size && com[i] != ' ')
			{
				outpath[c] = com[i];
				i++, c++;
			}
			outpath[c] = '\0';
		}
		else if (s == "-date")
		{
			int c = 0;
			while (i < size && com[i] != ' ')
			{
				date[c] = com[i];
				i++, c++;
			}
			cc = c;
			date[c] = '\0';
		}
		else if (s == "-type")
		{
			int c = 1;
			string ss = "";
			while (i < size)
			{
				ss = ss + com[i];
				i++;
				if (com[i] == ' ' || i == size)
				{
					if (ss == "ip")mi2["��Ⱦ����"] = c, type[c++] = "��Ⱦ����";
					else if (ss == "sp")mi2["���ƻ���"] = c, type[c++] = "���ƻ���";
					else if (ss == "cure")mi2["����"] = c, type[c++] = "����";
					else if (ss == "dead")mi2["����"] = c, type[c++] = "����";
					i++;
					ss = "";
				}
			}
			type_num = c;
			CheckType(c);
		}
		else if (s == "-province")
		{
			int c = 0;
			while (i < size && (com[i] >= 'z' || com[i] <= 'a') && com[i] != '-')
			{
				if (com[i] == ' ')mi1[province[c]] = 1, c++;
				else province[c] = province[c] + com[i];
				i++;
			}
			if (i == size)mi1[province[c]] = 1, c++;
			if (com[i] >= 'z' || com[i] <= 'a')i--;
			province_num = c;
		}
		else if(com[i]==' ');
		else i--;
	}
	CheckAgain();
	FindAllFile(inpath, ".log.txt", date);
	return Check(cc,date);
	
}


int main(int argc, char* argv[])
{
	Analysis A;
	string s = "";
	for(int i=0;i<argc;i++)
	{
		int size=strlen(argv[i]);
		for(int j=0;j<size;j++)s=s+argv[i][j];
		s=s+" ";
	}
	//cout<<s;
	A.Begin(s);
	A.Out();
	
	return 0;
}


