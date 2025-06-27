int amOsinfoGetNetworkProperty(unsigned int *param_1);
uint amOsinfoGetDhcpStatusEth0Ex(unsigned int *param_1);
typedef struct sockaddr_in Sockaddr;
bool getIPAddress(char *ifname, Sockaddr *saddr);
int getHostByName(unsigned int *param_1, char* hostname);