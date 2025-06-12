#include "../Common.h"

char* SERVERIP = (char*)"127.0.0.1";

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    struct sockaddr_in server_addr;
    SOCKET sock;

    u64 time_start, time_end;

    if (argc != 4) {
        msg(MSG, "��� �Ķ���� �߸� ������!");
        msg(MSG, "Syntax: UDPClient.exe IP_Address PortNumber FileName");
        exit(1);
    }

    // ���� �ʱ�ȭ
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return 1;

    // ���� ����
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) err_quit("socket()");

    // ���� �ּ� ����ü �ʱ�ȭ
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2])); // SERVERPORT

    msg(CLIENT, "UDP ���� �۽� ���۵� !!!\n");

    // ������ ��ſ� ����� ����
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

    // ������ ������ ���
    time_start = GetMicroCounter();

    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    totalBufferNum = file_size / sizeof(buf) + 1;

    fseek(fp, 0, SEEK_SET);
    BufferNum = 0;
    totalSendBytes = 0;

    _snprintf(buf, sizeof(buf), "%d", file_size);
    sendBytes = sendto(sock, buf, sizeof(char) * 1024, 0, (SOCKADDR*)&server_addr, sizeof(server_addr));

    // ������ ������ ���
    while ((sendBytes = (int)fread(buf, sizeof(char), sizeof(buf), fp)) > 0) {
        sendto(sock, buf, sendBytes, 0, (SOCKADDR*)&server_addr, sizeof(server_addr));
        BufferNum++;
        totalSendBytes += sendBytes;

        printf("���� ������ : %d/%dByte(s) %3.2f%%\r",
            totalSendBytes, file_size,
            (BufferNum * 100.0) / totalBufferNum);

        while (recvfrom(sock, buf, BUF_SIZE, 0, NULL, NULL) != 10) {
            sendto(sock, buf, sendBytes, 0, (SOCKADDR*)&server_addr, sizeof(server_addr));
        }
    }

    time_end = GetMicroCounter();
    printf("\n\n���� �ð�(Micro seconds) : %d\n", (int)(time_end - time_start));

    // ���� �ݱ�
    closesocket(sock);

    // ���� ����
    WSACleanup();
    return 0;
}
