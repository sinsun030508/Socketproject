#include "../Common.h"

char* SERVERIP = (char*)"127.0.0.1";

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    struct sockaddr_in server_addr;
    SOCKET sock;

    u64 time_start, time_end;

    if (argc != 4) {
        msg(MSG, "명령 파라미터 잘못 지정됨!");
        msg(MSG, "Syntax: UDPClient.exe IP_Address PortNumber FileName");
        exit(1);
    }

    // 윈속 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return 1;

    // 소켓 생성
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) err_quit("socket()");

    // 소켓 주소 구조체 초기화
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2])); // SERVERPORT

    msg(CLIENT, "UDP 파일 송신 시작됨 !!!\n");

    // 데이터 통신에 사용할 변수
    struct sockaddr_in client_addr;
    int len_addr = sizeof(client_addr);
    int BufferNum, totalBufferNum, sendBytes;
    long file_size, totalSendBytes;

    char buf[BUF_SIZE];
    FILE* fp;
    fp = fopen(argv[3], "rb");
    if (fp == NULL) {
        msg(MSG, notFound);
        exit(1);
    }

    // 서버와 데이터 통신
    time_start = GetMicroCounter();

    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    totalBufferNum = file_size / sizeof(buf) + 1;

    fseek(fp, 0, SEEK_SET);
    BufferNum = 0;
    totalSendBytes = 0;

    _snprintf(buf, sizeof(buf), "%d", file_size);
    sendBytes = sendto(sock, buf, sizeof(char) * 1024, 0, (SOCKADDR*)&server_addr, sizeof(server_addr));

    // 서버와 데이터 통신
    while ((sendBytes = (int)fread(buf, sizeof(char), sizeof(buf), fp)) > 0) {
        sendto(sock, buf, sendBytes, 0, (SOCKADDR*)&server_addr, sizeof(server_addr));
        BufferNum++;
        totalSendBytes += sendBytes;

        printf("파일 전송중 : %d/%dByte(s) %3.2f%%\r",
            totalSendBytes, file_size,
            (BufferNum * 100.0) / totalBufferNum);

        while (recvfrom(sock, buf, BUF_SIZE, 0, NULL, NULL) != 10) {
            sendto(sock, buf, sendBytes, 0, (SOCKADDR*)&server_addr, sizeof(server_addr));
        }
    }

    time_end = GetMicroCounter();
    printf("\n\n전송 시간(Micro seconds) : %d\n", (int)(time_end - time_start));

    // 소켓 닫기
    closesocket(sock);

    // 윈속 종료
    WSACleanup();
    return 0;
}
