#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/tcp.h>
#include<netinet/udp.h>
#include<netinet/ip_icmp.h>
#include<netdb.h>
#include<string.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<net/if.h>
#include<netinet/if_ether.h>

int main(int argc, char** argv){
	int s,contr;
	struct ifreq ifr;
	unsigned char buf[65536];
	unsigned int buflen=65536*sizeof(unsigned char);
	ssize_t rec;
	int i;
	
	if(argc!=2){
		printf("Número incorrecto de argumentos\n");
		exit(1);
	}
	
	s=socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	contr=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(s<0 || contr<0){
		perror("Error al crear sockets");
		exit(1);
	}
	strcpy(ifr.ifr_name,argv[1]);
	if(ioctl(contr,SIOCGIFFLAGS,&ifr)<0){
		perror("Error al conseguir estado\n");
		exit(1);
	}
	ifr.ifr_flags |= IFF_PROMISC;
	if(ioctl(contr,SIOCSIFFLAGS,&ifr)<0){
		perror("Error activando el modo promiscuo\n");
		exit(1);
	}
	while((rec=recv(s,buf,buflen,0))>0){
		for(i=0;i<rec;i++){
			printf("%c",buf[i]);
		}
		printf("\n");
	}
	ifr.ifr_flags &= ~IFF_PROMISC;
	if(ioctl(contr,SIOCSIFFLAGS,&ifr)<0){
		perror("Error desactivando el modo promiscuo\n");
		exit(1);
	}
	close(contr);
	close(s);
	
	return 0;
}
