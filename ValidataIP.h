#include <iostream>
#include <vector>
#include <string>

using namespace std;

// 문자열이 숫자 문자열인지 확인
bool isNumber(const string& str)
{
    // std::find_first_not_of: 문자열에서 첫 번째 문자를 검색
    // 이곳에 지정된 문자 이외의 값이 존재하지 않으면 숫자만 있는 것
    return !str.empty() && (str.find_first_not_of("0123456789") == std::string::npos);
}

// 주어진 구분 기준을 사용하여 문자열 'str' 을 분할하는 함수
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

// IP 주소를 확인하는 함수
bool validateIP(string ip)
{
    // 문자열을 토큰으로 분할
    vector<string> list = split(ip, '.');

    // 토큰 크기가 4가 아닌 경우
    if (list.size() != 4) {
        return false;
    }

    // 각 토큰의 유효성을 검사
    for (string str : list)
    {
        // 문자열이 숫자인지 아닌지 확인하고 숫자 범위 벗어남 여부
        if (!isNumber(str) || stoi(str) > 255 || stoi(str) < 0)
            return false;
    }

    return true;
}
