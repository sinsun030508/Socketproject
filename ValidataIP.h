#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ���ڿ��� ���� ���ڿ����� Ȯ��
bool isNumber(const string& str)
{
    // std::find_first_not_of: ���ڿ����� ù ��° ���ڸ� �˻�
    // �̰��� ������ ���� �̿��� ���� �������� ������ ���ڸ� �ִ� ��
    return !str.empty() && (str.find_first_not_of("0123456789") == std::string::npos);
}

// �־��� ���� ������ ����Ͽ� ���ڿ� 'str' �� �����ϴ� �Լ�
vector<string> split(const string& str, char delim)
{
    auto i = 0;
    vector<string> list;

    auto pos = str.find(delim);

    while (pos != string::npos)
    {
        list.push_back(str.substr(i, pos - i));
        i = (int)(pos + 1);
        pos = str.find(delim, pos + 1);
    }

    list.push_back(str.substr(i, str.length()));
    return list;
}

// IP �ּҸ� Ȯ���ϴ� �Լ�
bool validateIP(string ip)
{
    // ���ڿ��� ��ū���� ����
    vector<string> list = split(ip, '.');

    // ��ū ũ�Ⱑ 4�� �ƴ� ���
    if (list.size() != 4) {
        return false;
    }

    // �� ��ū�� ��ȿ���� �˻�
    for (string str : list)
    {
        // ���ڿ��� �������� �ƴ��� Ȯ���ϰ� ���� ���� ��� ����
        if (!isNumber(str) || stoi(str) > 255 || stoi(str) < 0)
            return false;
    }

    return true;
}
