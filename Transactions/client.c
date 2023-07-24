#include "client.h"
#include "support.h"

int port, conn_fd;
pid_t pid;
int n_transaction = 0;
char* log_host_name;
char ip[MAX_IP_LENGTH];
FILE* log_file_pointer;
struct sockaddr_in server_address;

int main(int argc, char* argv[]) {
    client_parse_arguments(argc, argv, 3);
    pid = getpid();
    // Create a log file for the client: machine_name.pid
    client_create_log_file();
    printf("Using port %d\nUsing server address %s\nHost %s\n", port, ip, log_host_name);
    log_file_pointer = fopen(log_host_name, "w");
    fprintf(log_file_pointer, "Using port %d\nUsing server address %s\nHost %s\n", port, ip, log_host_name);

    // Read input from keyboard or file and Send the Job to the server:
    client_parse_work();

    // Todo: Maintain the Log file for the client.
    fprintf(log_file_pointer, "Sent %d transactions\n", n_transaction);
    fclose(log_file_pointer);
    shutdown(conn_fd, SHUT_RDWR);
    close(conn_fd);

    return 0;
}

void client_create_log_file() {
    //TODO: create log file
    char host_name[MAX_FILENAME_LEN];
    gethostname(host_name, MAX_FILENAME_LEN);

    char pid_string[MAX_PID_LEN];
    sprintf(pid_string, ".%d", pid);
    strcat(host_name, pid_string);

    log_host_name = malloc((sizeof (char) * strlen(host_name)));
    strcpy(log_host_name, host_name);
}

void client_parse_arguments(int argc, char* argv[], int arg_num) {
    // The Function parses the command line
    // port: Integer[5,000, 64,000] (to connect, use the one the server uses), ip-address. argc == 3
    if (argc != arg_num) {
        fprintf(stderr,"Argument Error: 2 arg mandatory.\n Please give a port number and IP address\n"
                       "Usage: ./client port ip\n");
        exit(EXIT_FAILURE);
    } else {
        if (is_purely_digital(argv[1])) {
            char* port_str = argv[1];
            strcpy(ip, argv[2]);
            sscanf(port_str, "%d", &port);
            if (port>64000 || port < 5000) {
                fprintf(stderr,"Value Error: Valid Range of Port Numbers: [5000, 64000]!\n");
                exit(EXIT_FAILURE);
            }
        } else {
            fprintf(stderr,"Argument Error: Invalid Port Number or IP Address!\n");
            exit(EXIT_FAILURE);
        }
    }
}

void client_initialize() {
    conn_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip);
    server_address.sin_port = htons(port);

    if (connect(conn_fd, (struct sockaddr*) &server_address, sizeof (server_address)) < 0) {
        perror("client connect");
        exit(-1);
    }

}

void log_client_send(char* value_n) {
    // Write log to client log file: Send job to server
    char* message = "";
    message = malloc((sizeof (char) * ((strlen(value_n)) + strlen(log_host_name) + 8)));
    strcpy(message, log_host_name); strcat(message, "//"); strcat(message, value_n); strcat(message, "//\0");
    puts(message);
    // Todo: send the message to server
    if (send(conn_fd, message, strlen(message), 0) < 0) {
        perror("client send");
    }
    fprintf(log_file_pointer, "%.2f: Send (T %s)\n", (double) time(NULL), value_n);
}

void log_client_receive() {
    // Client sleeps n units
    char job_num[1024];
    if (recv(conn_fd, job_num, 1024, 0)<0) {
        perror("client recv");
    }
    char* job_index = strtok(job_num, "//" );
    fprintf(log_file_pointer, "%.2f: Recv (D %s)\n", (double) time(NULL), job_index);
    memset(job_num, 0, 1024);
}

void client_parse_work() {
    // Read in Input from keyboard or file until EOF.
    char input[MAXIMAL_LENGTH_PER_LINE], instruction[3], value_n[MAXIMAL_LENGTH_PER_LINE-1];

    while (fgets(input, MAXIMAL_LENGTH_PER_LINE, stdin) != NULL) {
        int n = 1;
        strncpy(instruction, input, 1);
        puts(instruction); // See the instruction
        // Transactions:
        if (strcmp(instruction,"T")==0) {
            client_initialize();
            // Get the numerical value of n.
            sscanf(input, "T%s", value_n);
            // Test format: n should be an Integer!!
            if (is_purely_digital(value_n)) {
                log_client_send(value_n);
                log_client_receive();
                n_transaction++;
            }
        } else if (strcmp(instruction,"S")==0) {
            // Route to Sleep with arg n
            sscanf(input, "S%s", value_n);
            if (is_purely_digital(value_n)) {
                sscanf(value_n, "%d", &n);
                // Client sleeps n units
                fprintf(log_file_pointer, "Sleep %s units\n", value_n);
                Sleep(n);
            }

        }
    }
    if (send(conn_fd, "end", strlen("end"), 0) < 0) {
        perror("client send");
    }
}
