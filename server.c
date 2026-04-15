/*===============================================
*   文件名称：server.c
*   创 建 者：     
*   创建日期：2026年01月09日
*   描    述：gcc -o server server.c
================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8000 // 定义服务器监听的端口号
#define BUF_SIZE 1024 // 定义缓冲区大小

int main() {
    int server_fd, new_socket; // server_fd: 监听套接字, new_socket: 与客户端通信的连接套接字
    struct sockaddr_in address; // 用于存储服务器地址信息
    int addrlen = sizeof(address);
    char buffer[BUF_SIZE] = {0}; // 用于接收客户端消息的缓冲区

    // 1. 创建套接字
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    printf("服务器套接字创建成功\n");

    // 2. 设置套接字选项，允许地址重用
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // 3. 绑定地址和端口
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("绑定端口成功：%d.\n", PORT);

    // 4. 开始监听
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("正在监听端口：%d...\n", PORT);

    // 5. 循环接受连接
    while(1) {
        printf("\n等待客户端连接...\n");
        
        // accept阻塞等待客户端连接
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                                (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("客户端连接成功. IP: %s, Port: %d\n", 
               inet_ntoa(address.sin_addr), ntohs(address.sin_port));

        // 6. 与客户端通信循环（轮询）
        while(1) {
            // 清空缓冲区
            memset(buffer, 0, BUF_SIZE);
            
            // 接收客户端消息（阻塞）
            ssize_t valread = read(new_socket, buffer, BUF_SIZE);
            if (valread == 0) {
                // 客户端主动关闭连接（read返回0）
                printf("客户端 %s:%d 已断开连接\n", 
                       inet_ntoa(address.sin_addr), ntohs(address.sin_port));
                break;
            } else if (valread < 0) {
                // 读取失败
                perror("read failed");
                break;
            }

            // 打印客户端消息
            printf("收到客户端消息: %s", buffer);

            // 构造回复消息（echo + 确认）
            char reply[BUF_SIZE];
            snprintf(reply, BUF_SIZE, "服务器已收到: %s", buffer);
            
            // 发送回复给客户端
            send(new_socket, reply, strlen(reply), 0);
            printf("已回复客户端: %s\n", reply);

            // 如果客户端发送exit，主动断开当前连接
            if (strncmp(buffer, "exit", 4) == 0) {
                printf("客户端请求断开，关闭连接\n");
                break;
            }
        }

        // 7. 关闭当前客户端的连接套接字
        close(new_socket);
    }

    close(server_fd); // 理论上不会执行到
    return 0;
}