#ifndef CONNECTIONH
#define CONNECTION

int setup_connection_client(const char *hostname, int port, int verbose,int N);
int setup_connection_host(int port, int verbose, int a, int N);
void serv_accept(int acceptfd);

#endif
