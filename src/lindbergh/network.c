#include <stdio.h>
#include <unistd.h>
#include <string.h> /* for strncpy */
#include <stdlib.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

#include "drawtext.h"

typedef struct sockaddr_in Sockaddr;

char* intfname = "wlp2s0";

int ioctl_intf(unsigned int *param_1, long unsigned int command) {
    //printf("Called ioctl_intf.\n");

    int fd;
    int result;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    /* I want to get an IPv4 IP address */
    ifr.ifr_addr.sa_family = AF_INET;

    /* I want IP address attached to "eth0" */
    strncpy(ifr.ifr_name, intfname, IFNAMSIZ-1);
    result = ioctl(fd, command, &ifr);

    if (result == 0){
        *param_1 = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr.s_addr;
    }

    close(fd);

     /* display result */
    //printf("%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));


    return result;
}

int set_gateway(unsigned int *param_1){
    struct sockaddr_in gateway;
    inet_pton(AF_INET, "192.168.0.1", &(gateway.sin_addr));
    //printf("%s\n", inet_ntoa(((struct sockaddr_in *)&gateway)->sin_addr));
    *param_1 = ((struct sockaddr_in *)&gateway)->sin_addr.s_addr;
    return 0;
}

// Replacement function
int amOsinfoGetNetworkProperty(int param_1) {
    printf("Got amOsinfoGetNetworkProperty\n");
    if (param_1 == 0) {
        return -2; // Original returned 0xfffffffe
    }

    // Set DHCP status to always "yes"
    *(uint *)(param_1 + 8) = 0xfffffffe;

    //GetIP
    unsigned int ip;
    ioctl_intf(&ip, SIOCGIFADDR);
    *(unsigned int *)(param_1 + 0xc) = ip;

    //GetSubnet
    unsigned int subnet;
    ioctl_intf(&subnet, SIOCGIFNETMASK);
    *(unsigned int *)(param_1 + 0x10) = subnet;

    //default route
    unsigned int gateway_value;
    set_gateway(&gateway_value);
    *(unsigned int *)(param_1 + 0x14) = gateway_value;

    //dns1
    struct sockaddr_in dns;
    inet_pton(AF_INET, "1.1.1.1", &(dns.sin_addr));
    //printf("%s\n", inet_ntoa(((struct sockaddr_in *)&dns)->sin_addr));
    *(in_addr_t *) (param_1 + 0x18) = dns.sin_addr.s_addr;

    //dns1
    struct sockaddr_in dns2;
    inet_pton(AF_INET, "1.0.0.1", &(dns2.sin_addr));
    //printf("%s\n", inet_ntoa(((struct sockaddr_in *)&dns2)->sin_addr));
    *(in_addr_t *) (param_1 + 0x18) = dns2.sin_addr.s_addr;

    /*// Specify your custom subnet mask (e.g., 255.255.255.0)
    const char *custom_netmask_str = "255.255.255.0";
    if (inet_pton(AF_INET, custom_netmask_str, &props->netmask) != 1) {
        fprintf(stderr, "Error: Invalid subnet mask format.\n");
        props->netmask = 0;
        return -1; // Indicate an error
    }

    // Specify your custom gateway
    const char *custom_gateway_str = "192.168.0.1";
    if (inet_pton(AF_INET, custom_gateway_str, &props->gateway) != 1) {
        fprintf(stderr, "Error: Invalid gateway format.\n");
        props->gateway = 0;
        return -1; // Indicate an error
    }

    // Specify your custom primary DNS server
    const char *custom_dns1_str = "8.8.8.8";
    if (inet_pton(AF_INET, custom_dns1_str, &props->dns_server1) != 1) {
        fprintf(stderr, "Error: Invalid DNS server 1 format.\n");
        props->dns_server1 = 0;
    }

    // Specify your custom secondary DNS server (optional)
    const char *custom_dns2_str = "8.8.4.4";
    if (inet_pton(AF_INET, custom_dns2_str, &props->dns_server2) != 1) {
        fprintf(stderr, "Error: Invalid DNS server 2 format.\n");
        props->dns_server2 = 0;
    }
    printf("Here.\n");*/

    return 0; // Indicate success
}

uint amOsinfoGetDhcpStatusEth0Ex(unsigned int *param_1){
    return 0;
}
bool getIPAddress(char *ifname, Sockaddr *saddr){
    printf("IP Address gotten for %s", ifname);
    return 1;
}