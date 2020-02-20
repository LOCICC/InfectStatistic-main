
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

class analysis
{
public:
	char outpath[1005];
	string province[40];
	map<string, int>mi;
	map<string, int>mi1;
	map<string, int>mi2;
	map<string, int>mi3;
	string p[40];
	string type[20];
	int num, num1, opp;
	int out[40][6];
	int Num(string tmp);

	void findAllFile(const char* path, const char* format, const char* date);
	string begin(string com);
	void init();
	void Out();
	int check;
	int Detail(string p, string t) {
		return out[mi[p]][mi2[t]];
	}
};

int analysis::Num(string tmp)
{
	int num = 0, c = 1;
	for (int i = tmp.size() - 1;i >= 0;i--)
	{
		if (tmp[i] >= '0' && tmp[i] <= '9')num += (tmp[i] - '0') * c, c *= 10;
	}
	return num;
}
void analysis::findAllFile(const char* path, const char* format, const char* date)
{
	check = 1;
	char newpath[200];
	char newpaths[200];
	strcpy(newpath, path);
	strcpy(newpaths, path);
	strcat(newpath, "\\*.*");    // ��Ŀ¼�������"\\*.*"���е�һ������
	int handle;
	int b = 0;
	_finddata_t findData;
	check = 2;
	handle = _findfirst(newpath, &findData);
	if (handle == -1)
	{
		check = 3;
		// ����Ƿ�ɹ�
		return;
	}
	while (_findnext(handle, &findData) == 0) {
		if (findData.attrib & _A_SUBDIR) {
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;
			strcpy(newpath, path);
			strcat(newpath, "\\");
			strcat(newpath, findData.name);
			findAllFile(newpath, format, date);
		}
		else {
			if (strstr(findData.name, format))
			{     //�ж��ǲ���txt�ļ� 
				
				int size = strlen(date);
				int fl = 1;
				if (size != 0)
				{
					for (int i = 0;i < size;i++)
					{
						if (findData.name[i] != date[i])fl = 0;
						if (findData.name[i] > date[i])b = 1;
					}
				}
				if (b == 1)return ;
				if (fl == 1&&size != 0)b = 1;
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
					int fl1 = 0, fl2 = 0;
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
							else if (fl1 == 0)fl1 = mi[s];
							else fl2 = mi[s];
							s = "";
						}
						else s = s + tmp[j];
					}
					int f1 = 0, f2 = 0;
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
							if (f1 == 0)f1 = mi2[ss];
							else f2 = mi2[ss];
							ss = "";
						}
						else ss = ss + tmp[j];
					}
					check = 1;
					int num = Num(tmp);
					//	cout<<fl1<<' '<<fl2<<' '<<f1<<' '<<f2<<' '<<num<<endl;
					if (fl2 == 0 && f2 == 0)
					{
						if (l == 100)out[fl1][f1] += num;
						else if (l == 101)out[fl1][f1] -= num;
						else if (l == 102)out[fl1][mi2["��Ⱦ����"]] -= num, out[fl1][mi2["����"]] += num;
						else out[fl1][mi2["����"]] += num, out[fl1][mi2["��Ⱦ����"]] -= num;
					}
					else if (fl2 == 0 && f2 != 0)out[fl1][mi2["���ƻ���"]] -= num, out[fl1][mi2["��Ⱦ����"]] += num;
					else out[fl1][f1] -= num, out[fl2][f1] += num;
					if (out[fl1][f1] != 0)check = out[fl1][f1];
				}
				if (b == 1)return;
			}
		}
	}
	_findclose(handle);    // �ر��������
}
void analysis::init()
{
	memset(out, 0, sizeof(out));
	for (int i = 0;i <= 35;i++)province[i] = "";
	mi.clear();
	mi1.clear();
	mi2.clear();
	mi3.clear();
	num = 0, num1 = 0;
	mi["����"] = 100;
	mi["�ų�"] = 101;
	mi["����"] = 102;
	mi["����"] = 103;
	mi["ȫ��"] = 1;
	mi["����"] = 2, mi["����"] = 3, mi["����"] = 4, mi["����"] = 5, mi["����"] = 6, mi["�㶫"] = 7;
	mi["����"] = 8, mi["����"] = 9, mi["����"] = 10, mi["�ӱ�"] = 11, mi["����"] = 12, mi["������"] = 13;
	mi["����"] = 14, mi["����"] = 15, mi["����"] = 16, mi["����"] = 17, mi["����"] = 18, mi["����"] = 19;
	mi["���ɹ�"] = 20, mi["����"] = 21, mi["�ຣ"] = 22, mi["ɽ��"] = 23, mi["ɽ��"] = 24, mi["����"] = 25;
	mi["�Ϻ�"] = 26, mi[""] = 27, mi["�Ĵ�"] = 28, mi["̨��"] = 29, mi["���"] = 30, mi["����"] = 31;
	mi["���"] = 32, mi["�½�"] = 33, mi["����"] = 34,mi["�㽭"];
	p[1] = "ȫ��", p[2] = "����", p[3] = "����", p[4] = "����", p[5] = "����", p[6] = "����";
	p[7] = "�㶫", p[8] = "����", p[9] = "����", p[10] = "����", p[11] = "�ӱ�", p[12] = "����" ;
	p[13] = "������", p[14] = "����", p[15] = "����", p[16] = "����", p[17] = "����", p[18] = "����";
	p[19] = "����", p[20] = "���ɹ�", p[21] = "����", p[22] = "�ຣ", p[23] = "ɽ��", p[24] = "ɽ��";
	p[25] = "����", p[26] = "�Ϻ�", p[28] = "�Ĵ�", p[29] = "̨��", p[30] = "���", p[31] = "����";
	p[32] = "���", p[33] = "�½�", p[34] = "����",p[35]="�㽭";
	p[27]="-";
}


void  analysis::Out()
{
	int o = 0;
	for (int i = 2;i < 36;i++)
	{
		for (int j = 1;j < num1;j++)
		{
			out[1][j] += out[i][j];
		}
	}
	for (int i = 1;i < 36;i++)
	{
		if (mi1.count(p[i]) || (num == 0 &&(mi3.count(p[i]) || i == 1))) 
		{
			cout << p[i] << ' ';
			for (int j = 1;j < num1;j++)
			{
				cout << type[j] << out[i][j] << "�� ";
				if (out[i][j]!=0)check = out[i][j];
			}
			if (mi1[p[i]] == 1 || num == 0)
			{
				cout << "\n";
			}
		}
	}
	cout << "// ���ĵ�������ʵ���ݣ���������ʹ��" << "\n";
}
string analysis::begin(string com)
{
	init();
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
			num1 = c;
			if (!mi2.count("��Ⱦ����"))mi2["��Ⱦ����"] = c, type[c++] = "��Ⱦ����";
			if (!mi2.count("���ƻ���"))mi2["���ƻ���"] = c, type[c++] = "���ƻ���";
			if (!mi2.count("����"))mi2["����"] = c, type[c++] = "����";
			if (!mi2.count("����"))mi2["����"] = c, type[c++] = "����";
			mi2["ȷ���Ⱦ"] = 11;
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
			num = c;
		}
		else if(com[i]==' ');
		else i--;
	}
	if (num1 == 0)
	{
		int c = 1;
		if (!mi2.count("��Ⱦ����"))mi2["��Ⱦ����"] = c, type[c++] = "��Ⱦ����";
		if (!mi2.count("���ƻ���"))mi2["���ƻ���"] = c, type[c++] = "���ƻ���";
		if (!mi2.count("����"))mi2["����"] = c, type[c++] = "����";
		if (!mi2.count("����"))mi2["����"] = c, type[c++] = "����";
		mi2["ȷ���Ⱦ"] = 11;
		num1 = c;
	}
	
	findAllFile(inpath, ".log.txt", date);
	string o = "";
	if (cc > 0)
	{
		for (int i = 0;i < cc;i++)o = o + date[i];
		o = o + " ";
	}
	if (num > 0)
	{
		for (int i = 0;i < num;i++)o = o + province[i];
		o = o + " ";
	}
	if (num1 > 1)
	{
		for (int i = 1;i < num1;i++)o = o + type[i];
		o = o + " ";
	}
	return o;

}


int main(int argc, char* argv[])
{
	analysis A;
	string s = "";
	for(int i=0;i<argc;i++)
	{
		int size=strlen(argv[i]);
		for(int j=0;j<size;j++)s=s+argv[i][j];
		s=s+" ";
	}
	//cout<<s;
	A.begin(s);
	A.Out();
	
	return 0;
}


