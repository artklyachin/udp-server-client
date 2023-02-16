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

    const char* port_str = argv[1];
    uint16_t port = htons(strtol(port_str, NULL, 10));

    // Создаём сокет
    int socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_fd == -1) {
        perror("socket_fd");
        exit(1);
    }

    // Инициализируем структуру для подключения к серверу
    struct sockaddr_in ipv4_addr = {
        .sin_family = AF_INET, .sin_port = port, .sin_addr.s_addr = htonl(INADDR_ANY)};
    struct sockaddr_in clnt_addr;

    // назначаем адрес сокету socket_fd
    if( bind(socket_fd , (const struct sockaddr*)&ipv4_addr, sizeof(ipv4_addr) ) == -1)
    {
        perror("bind");
        exit(1);
    }

    printf("ожидание сообщения от клиента.\n");
    fflush(stdout);

    char answer[MAX_BUF_SIZE], message[MAX_BUF_SIZE];
    while (1) {

        // Получаем от клиента сообщение
        socklen_t clnt_addr_len = sizeof(clnt_addr);
        int read_result = recvfrom(
            socket_fd,
            &answer,
            sizeof(answer),
            0,
            (struct sockaddr *) &clnt_addr,
            &clnt_addr_len);
        if (read_result == -1) {
            perror("read");
            break;
        } else if (read_result == 0) {
            perror("closed connection\n");
            break;
        } else {
            printf("сообщение от клиента: ");
            printf("%s\n", answer);
            fflush(stdout);
        };

        printf("ответ клиенту: ");
        fflush(stdout);

        if (scanf("%s", message) == EOF) {
            perror("scanf");
            break;
        }

        // Отправляем клиенту сообщение
        int rv = sendto(
            socket_fd,
            &message,
            sizeof(message),
            0,
            (struct sockaddr*)&clnt_addr,
            clnt_addr_len);
        if (rv == -1) {
            perror("write");
            break;
        }
    }

    // Закрываем соединение и сокет
    shutdown(socket_fd, SHUT_RDWR);
    close(socket_fd);

    return 0;
}