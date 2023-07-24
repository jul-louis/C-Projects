#include "server.h"
#include "support.h"

FILE* log_file_pointer;
int job_index = 0, port, on, c, listen_fd, conn_fd;
char* log_host_name;
time_t first_transaction, last_transaction;
struct sockaddr_in server_address, client_address;

struct client_queue clients;

int main(int argc, char* argv[]) {
    pid_t server_pid = getpid();
    server_create_log_file(server_pid);
    server_parse_arguments(argc, argv, 2);
    printf("Using port %d\n", port);
    log_file_pointer = fopen(log_host_name, "w");
    fprintf(log_file_pointer, "Using port %d\n", port);

    server_initialize();
    clients = get_queue();
    server_routine();
    close(listen_fd);
    fclose(log_file_pointer);

    return 0;
}

void server_initialize() {
    // The function initialize the server end:
    // socket() - bind() - listen()
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(-1);
    }

    // Set the socket reusable.
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (char*) &on, sizeof(on)) < 0) {
        perror("setsockopt");
        close(listen_fd);
        exit(-1);
    }

    // Non-blocking:
    int flags = fcntl(listen_fd, F_GETFL, 0);
    if (fcntl(listen_fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        perror("fcntl - set non-blocking socket failed. ");
    }

    // bind()
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);

    if (bind(listen_fd, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        perror("bind");
        exit(-1);
    }

    // listen() -- backlog of up to 20 connections
    if (listen(listen_fd, MAX_CONNECTION)) {
        perror("listen");
        exit(-1);
    }
    // Now accepting transactions:
    puts("Waiting for incoming connections...");
}

void server_routine() {
    // Routine to receive work and send "acknowledgement" - Done #
    char buff[1024]="";
    bool server_routine_timeout = false;
    // Timer:
    struct timeval timeout_value, end_value;
    gettimeofday(&timeout_value, NULL);
    while ((!server_routine_timeout)) {
        // Set a time_out for accept.
        conn_fd = accept(listen_fd, (struct sockaddr *) &client_address, (socklen_t *)&c);
        if (recv(conn_fd, buff, 1024, 0)>0) {

            // Got a message: Reset timer
            gettimeofday(&timeout_value, NULL);

            if (strcmp(buff,"end")==0) {
                close(conn_fd);
            } else {
                char* hostname_from;
                puts(buff);
                // get machine name from buff.
                hostname_from = strtok(buff, "//");
                puts(hostname_from);
                // the sender machine name is obtained
                if (hostname_from != NULL) {
                    char * n_string = strtok(NULL, "//");
                    puts(n_string);
                    if (is_purely_digital(n_string)) {
                        int n = 1;
                        sscanf(n_string, "%d", &n);
			            job_index++;
                        log_server_receive(hostname_from, n_string);
                        printf("%d\n",n);
                        Trans(n);
                        log_server_done(hostname_from);
                    }
                }
                // Send job feedback to the sender client: Done #.
                char buff_job[1024];
                sprintf(buff_job, "%d//", job_index);
                // Send back the job index to the client
                send(conn_fd, buff_job, strlen(buff_job), 0);
                memset(buff_job, 0, 1024);
            }
        }
        // Timeout: 30 s - No incoming message -> exit loop
        gettimeofday(&end_value, NULL);
        double time_diff = end_value.tv_sec - timeout_value.tv_sec;
        if (time_diff >= 30) {
            server_routine_timeout = true;
        }
        // Clear buff content.
        memset(buff, 0, 1024);
    }
}

void server_parse_arguments(int argc, char* argv[], int arg_num) {
    // The function parses the command line arguments
    // port range: 5,000 to 64,000; argc == 2
    if (argc != arg_num) {
        fprintf(stderr,"Argument Error: 1 arg mandatory.\n Please give a port number\nUsage: ./server port\n");
        exit(EXIT_FAILURE);
    } else {
        if (is_purely_digital(argv[1])) {
            char* port_str = argv[1];
            sscanf(port_str, "%d", &port);
            if (port>64000 || port < 5000) {
                fprintf(stderr,"Value Error: Valid Range of Port Numbers: [5000, 64000]!\n");
                exit(EXIT_FAILURE);
            }
        } else {
            fprintf(stderr,"Type Error: The Port Number is an Integer!\n");
            exit(EXIT_FAILURE);
        }
    }
}

void server_create_log_file(pid_t server_pid) {
    // get a name and create the log file
    char host_name[MAX_FILENAME_LEN];
    gethostname(host_name, MAX_FILENAME_LEN);

    char pid_string[MAX_PID_LEN];
    sprintf(pid_string, ".%d", server_pid);
    strcat(host_name, pid_string);

    log_host_name = malloc((sizeof (char) * strlen(host_name)));
    strcpy(log_host_name, host_name);
}

void log_server_summary() {
    // todo: summary - each client specifically
    // todo: all transactions. ok
    // todo: implement the queue and store the info.
    double transaction_time = (double)(last_transaction-first_transaction);
    fprintf(log_file_pointer,"%.1f\ttransactions/sec\t(%d/%.2f)", transaction_time/job_index ,job_index, transaction_time);
}

void log_server_receive(char* host, char* n_string) {
    printf("%.2f: # %d (T %s) from %s\n", (double) time(NULL), job_index, n_string, host);
    fprintf(log_file_pointer, "%.2f: # %d (T %s) from %s\n", (double) time(NULL), job_index, n_string, host);
}

void log_server_done(char* host) {
    if (job_index==1) {
        first_transaction = time(NULL);
        last_transaction = first_transaction;
    } else {
        last_transaction = time(NULL);
    }
    printf("%.2f: # %d (Done) from %s\n", (double) last_transaction, job_index, host);
    fprintf(log_file_pointer, "%.2f: # %d (Done) from %s\n", (double) last_transaction, job_index, host);
}
