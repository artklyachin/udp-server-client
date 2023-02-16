#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

const int MAX_BUF_SIZE = 256;

// единственный аргумент - номер порта
int main(int argc, char* argv[])
{
    assert(argc == 2);

    const char* ip_str = "127.0.0.1";
    in_addr_t ip = inet_addr(ip_str);

    const char* port_str = argv[1];
    uint16_t port = htons(strtol(port_str, NULL, 10));

    // Создаём сокет
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    // Инициализируем структуру для подключения к серверу
    struct sockaddr_in ipv4_addr = {
        .sin_family = AF_INET, .sin_port = port, .sin_addr = {ip}};
    socklen_t len = sizeof(ipv4_addr);

    char message[MAX_BUF_SIZE], answer[MAX_BUF_SIZE];
    while (1) {

        printf("сообщение к серверу: ");
        fflush(stdout);

        if (scanf("%s", message) == EOF) {
            perror("scanf");
            break;
        }

        // Отправляем серверу сообщение
        int rv = sendto(
            socket_fd,
            &message,
            sizeof(message),
            0,
            (struct sockaddr*)&ipv4_addr,
            sizeof(ipv4_addr));
        if (rv == -1) {
            perror("write");
            break;
        }

        printf("ответ сервера: ");
        fflush(stdout);

        // Получаем от сервера сообщение
        int read_result = recvfrom(
            socket_fd,
            (char*)answer,
            sizeof(answer),
            0,
            (struct sockaddr*)&ipv4_addr,
            &len);
        if (read_result == -1) {
            perror("read");
            break;
        } else if (read_result == 0) {
            perror("closed connection");
            break;
        } else {
            printf("%s\n", answer);
            fflush(stdout);
        }

    }

    // Закрываем соединение и сокет
    shutdown(socket_fd, SHUT_RDWR);
    close(socket_fd);

    return 0;
}