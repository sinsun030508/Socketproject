#include "../Common.h"

#define SERVERPORT 9000
#define BUFSIZE 512

int main(int argc, char* arvgv[]) {
	int retval;
    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

    // 소켓 생성
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) err_quit("socket()");

    // bind()
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("bind()");

    // 데이터 통신에 사용할 변수
    SOCKET client_sock;
    struct sockaddr_in clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];

    while (1) { // accept()
        addrlen = sizeof(clientaddr);
        retval = recvfrom(sock, buf,BUFSIZE
        , 0 ,(struct sockaddr*)&clientaddr, &addrlen);
        if (retval == SOCKET_ERROR) { err_display("recvfrom()"); break; }
        
        // 받은 데이터 출력
        buf[retval] = '\0';
        char addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        printf("[UCP/%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buf);

        // 데이터 보내기
        retval = sendto(sock, buf, retval, 0,
            (struct sockaddr *)&clientaddr, sizeof(clientaddr));
        if (retval == SOCKET_ERROR) { err_display("sendto()"); break; }
    }

    // 소켓 닫기
    closesocket(sock);
  
WSACleanup();
return 0;

}