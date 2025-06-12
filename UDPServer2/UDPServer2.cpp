#include "../Common.h"

int main(int argc, char *argv[]) {
    WSADATA wsaData;
    struct sockaddr_in server_addr;
    SOCKET sock;

    u64 time_start, time_end;
    int retval;

    if (argc < 3) {
        msg(MSG, "��Ʈ��ȣ�� ���� ���ϸ��� �߸� ������!");
        msg(MSG, "Syntax: UDPServer.exe PortNumber FileName");
        exit(1);
    }

    // ���� �ʱ�ȭ
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return 1;

    // ���� ����
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) err_quit("socket()");

    // bind()
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1])); // SERVERPORT

    retval = bind(sock, (SOCKADDR*)&server_addr, sizeof(server_addr));
    if (retval == SOCKET_ERROR) err_quit("bind()");

    msg(SERVER, "UDP ���� ���� ���۵� !!!\n");

    // ������ ��ſ� ����� ����
    struct sockaddr_in client_addr;
    int len_addr = sizeof(client_addr);
    int BufferNum, totalBufferNum, readBytes;
    long file_size, totalReadBytes;

    char buf[BUF_SIZE];

    FILE* fp;
    fp = fopen(argv[2], "wb");

    // Ŭ���̾�Ʈ�� ������ ���
    time_start = GetMicroCounter();

    readBytes = recvfrom(sock, buf, BUF_SIZE, 0, (SOCKADDR*)&client_addr, &len_addr);
    file_size = atol(buf);
    totalBufferNum = file_size / BUF_SIZE + 1;
    BufferNum = 0;
    totalReadBytes = 0;

    while (BufferNum != totalBufferNum) {
        readBytes = recvfrom(sock, buf, BUF_SIZE, 0, (SOCKADDR*)&client_addr, &len_addr);
        BufferNum++;
        totalReadBytes += readBytes;
        printf("\r[���ŵ� ���� ũ��: %d/%dbytes] %3.2f%%",
               totalReadBytes, file_size,
               (BufferNum * 100.0) / totalBufferNum);
        if (readBytes > 0)
            fwrite(buf, sizeof(char), readBytes, fp);

        readBytes = sendto(sock, buf, 10, 0, (SOCKADDR*)&client_addr, sizeof(client_addr));
        if (readBytes == SOCKET_ERROR) {
            msg(SERVER, "ERROR");
            break;
        }
    }

    time_end = GetMicroCounter();
    printf("\n\n���� �ð�(Micro seconds) : %d", (int)(time_end - time_start));

    // ���� �ݱ�
    closesocket(sock);

    // ���� ����
    WSACleanup();
    return 0;
}
