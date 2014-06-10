/*
Title:-  TCP/IP data  

Author:-	
		BYTErians Group;
		Group no. 28

Problem Definition :- 
		To display information of the packets stored in the pcap files.

Output :- 
		Displaying all the information in all the packets.
		Information like Source Ip Address,Destination IP Address,Protocol,TLength,SPort,DPort,Flags,Seq,Ack.


Structures used :-
	
IP HEADER:-

struct ip {
#if BYTE_ORDER == LITTLE_ENDIAN 
	u_char	ip_hl:4,		/* header length */
		ip_v:4;			/* version */
#endif
#if BYTE_ORDER == BIG_ENDIAN 
	u_char	ip_v:4,			/* version */
		ip_hl:4;		/* header length */
#endif
	u_char	ip_tos;			/* type of service */
	short	ip_len;			/* total length */
	u_short	ip_id;			/* identification */
	short	ip_off;			/* fragment offset field */
#define	IP_DF 0x4000			/* dont fragment flag */
#define	IP_MF 0x2000			/* more fragments flag */
	u_char	ip_ttl;			/* time to live */
	u_char	ip_p;			/* protocol */
	u_short	ip_sum;			/* checksum */
	struct	in_addr ip_src,ip_dst;	/* source and dest address */
};
	

TCP HEADER:-

struct tcphdr {
	u_short	th_sport;		/* source port */
	u_short	th_dport;		/* destination port */
	tcp_seq	th_seq;			/* sequence number */
	tcp_seq	th_ack;			/* acknowledgement number */
#if BYTE_ORDER == LITTLE_ENDIAN 
	u_char	th_x2:4,		/* (unused) */
		th_off:4;		/* data offset */
#endif
#if BYTE_ORDER == BIG_ENDIAN 
	u_char	th_off:4,		/* data offset */
		th_x2:4;		/* (unused) */
#endif
	u_char	th_flags;
#define	TH_FIN	0x01
#define	TH_SYN	0x02
#define	TH_RST	0x04
#define	TH_PUSH	0x08
#define	TH_ACK	0x10
#define	TH_URG	0x20
	u_short	th_win;			/* window */
	u_short	th_sum;			/* checksum */
	u_short	th_urp;			/* urgent pointer */
};

*/

#include <stdio.h>
#include <pcap.h>                  //Header file which contains all the structures related to packet header,packet data etc.
#include <stdlib.h>
#include<string.h>
#include <netinet/ip.h>		     //Header file which contains structure related to ip header						
#include <arpa/inet.h>  
#include<netinet/tcp.h>            //Header file which contains structure related to tcp header


int main(int argc,char *argv[])
{

	struct pcap_pkthdr header;			//Structure describing packet header
	const u_char * packet; 				//unsigned character array for storing all the data
	u_char * pkt_ptr;					//Pinter to the pcap file
	pcap_t * handle;					//PCAP handler					
	struct ip * ip_hdr;				//IP header
	struct sockaddr_in sa;				//Structure for Storing socket address
	
	char errbuf[PCAP_ERRBUF_SIZE];		//errbuf in pcap_open functions is assumed to be able to hold at least PCAP_ERRBUF_SIZE chars
    								//PCAP_ERRBUF_SIZE is defined as 256.
	
	char str[INET_ADDRSTRLEN];
	int counter=0;					//Counter for total number of packets.
	int ip_hlen;
	struct tcphdr * tcp_hdr;			//TCP header
	
      handle = pcap_open_offline(argv[1],errbuf);	 //calling pcap library function 
 
        
        if (handle == NULL) 					 //Error in opening the pcap file 	
        { 
        	fprintf(stderr,"Couldn't open pcap file %s: %s\n", argv[1], errbuf); 
        	return 2; 
	  }
	
	
	//Actual work begins from here.
	
	
	printf("\n\nSeq No  Source IP Address  Dstntn IP Address  Protocol  TLength     SPort    DPort   Flags           Seq           Ack");
	
	while (packet = pcap_next(handle,&header))        // header contains information about the packet
	{
		pkt_ptr=(u_char *)packet;			  // Casting a pointer to the packet data	
		
		pkt_ptr=pkt_ptr+14;				  //now pkt_ptr points to the ip header
		
		ip_hdr = (struct ip *)pkt_ptr;              // Casting to store the ip header
		
		if((unsigned int)(ip_hdr->ip_p)==6)         // it is a tcp  packet
		{
			sa.sin_addr=ip_hdr->ip_src;           		//Getting the source ip from the ip header and assigning it to the socket address structure
			inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);
			printf("\n\n%6d  %17s",counter+1,str); 		//Printing source ip address
			sa.sin_addr=ip_hdr->ip_dst;				//Getting the dstn ip from the ip header and assigning it to the socket address structure

			inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);
			printf("  %17s",str);
			strcpy(str,"TCP");
			printf("  %8s  %7d",str,ip_hdr->ip_len);        //Printing total length of the packet
			pkt_ptr=pkt_ptr+4*ip_hdr->ip_hl;			// Pointing the pointer to the start of the tcp header
			tcp_hdr = (struct tcphdr*)pkt_ptr;			//Casting to store the tcp header.
        		printf("     %5u    %5u",ntohs(tcp_hdr->th_sport),ntohs(tcp_hdr->th_dport));    //Getting the source and destination port adresses
			//Printing the status of the flags in the tcp header.
        		printf("  %c%c%c%c%c%c",(tcp_hdr->urg ? 'U' : '*'),(tcp_hdr->ack ? 'A' : '*'),(tcp_hdr->psh ? 'P' : '*'),(tcp_hdr->rst ? 'R' : '*'),(tcp_hdr->syn ? 'S' : '*'),(tcp_hdr->fin ? 'F' : '*')); 

        		printf("    %10u    %10u",ntohl(tcp_hdr->th_seq),ntohl(tcp_hdr->th_ack));  //Printing the sequence number and the aknowledgement number.
        		 
			
		}
		counter++;
	}
	printf("\n\n\nTotal number of packets captured :: %d\n\n\n",counter);
	return 0;
}
