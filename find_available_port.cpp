#include <iostream> // 為了std::cerr等
#include <sys/socket.h> // 提供socket相關函數
#include <netinet/in.h> // 提供sockaddr_in結構
#include <arpa/inet.h> // 提供inet_addr函數
#include <unistd.h> // 提供close函數

int find_available_port(int start_port, int socket_type, const char *ip_address = "0.0.0.0")
{
    int sock = socket(AF_INET, socket_type, 0);
    if (sock == -1)
    {
        std::cerr << "無法創建 socket\n";
        return -1;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip_address);

    for (int port = start_port; port < 65535; ++port)
    {
        addr.sin_port = htons(port);
        if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0)
        {
            close(sock);
            return port;
        }
    }

    close(sock);
    return -1;
}

int main()
{
    int start_port = 8000; // 起始端口
    int port = find_available_port(start_port, SOCK_STREAM); // 尋找可用的TCP端口
    if (port != -1)
    {
        std::cout << "找到可用的端口: " << port << std::endl;
    }
    else
    {
        std::cerr << "未找到可用的端口" << std::endl;
    }
    return 0;
}
