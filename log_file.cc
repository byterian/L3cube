/*
Group Name : Byterians

Group NUmber : 28

Assignment : 1

Title : Study of HTTP logfile.

Problem Statement:
A HTTP log file is given in '.txt' format.
Read the Logfile and present the information to display the following :
1)IP addresses of each client requesting.
2)Number of requests made by each client.
3)Total Number of requests made to a particular webite.
4)Requests made by each client.
5)Requests made by a particular client.
6)Requests made by different clients within given time period.


HTTP Log File Format:

[IP address of client] [RFC 1413 identity of client] [userid of client] [date,time,timezone] [Request line from client] [HTTP Status code returned] [Size of the document] [Refered from] [User-agent].

*/

#include<fstream>
#include<stdlib.h>
#include<iostream>
#include<string.h>
#include<unistd.h>

using namespace std;

char * months[12]={"January","February","March","April","May","June","July","August","September","October","November","December"};

//structure to store information about a particular website.
typedef struct website
{
	char url[200];
	int no_of_hits;
}website;

//structure to store information about a particular client.
typedef struct client
{
  char ip[15];
  int requests_made;
}client;

class analyzer
{
	char line[1000],phrase[1000]; //Used for seperating different components of logfile.
	int no_of_clients;
	client clients[500];          //Number of clients that can connect.
	website websites[500];        //Number of websites that can be referred.
	int no_of_websites;
	public:
	analyzer();
	void init();
	void clients_requesing();
	void request_of_each_client(char *);
	void request_of_all_clients();
	void parse(char ip[],char timestamp[],char method[],char url[],char refer[],char user_agent[],int * status_code,long long int*size_sent);
	int is_seperator(char,bool);
	void request_to_each_website();
	void parse(char url[],char method[]);
	void between_timestamp(char start[],char end[]);
	bool falls_between(int values[][6],char timestamp[]);
	void parse(char [],int*,int*,int*,int*,int*,int*);
	void help();
};

int main(int argc,char *argv[])
{
  	analyzer a;
	argc--;
  	switch(argc)
  	{
        	case 1:
        	if(!strcmp(argv[1],"-c"))
        	a.clients_requesing();
        	else if(!strcmp(argv[1],"-w"))	
        	a.request_to_each_website();
        	else if(!strcmp(argv[1],"-n"))
		a.request_of_all_clients();
		else if(!strcmp(argv[1],"-h"))
		a.help();
		else
		cout<<endl<<"No such option :"<<argv[1]<<" exists. Try '-h' for help."<<endl;		
		break;
		
		case 2:
		if(!strcmp(argv[1],"-pc"))
		a.request_of_each_client(argv[2]);
		else
		cout<<endl<<"No such option :"<<argv[1]<<" exists. Try '-h' for help."<<endl;
		break;
		
		case 3:
		if(!strcmp(argv[1],"-t"))
		a.between_timestamp(argv[2],argv[3]);
		else
		cout<<endl<<"No such option :"<<argv[1]<<" exists. Try '-h' for help."<<endl;
		break;
	}
	return 0;
}

//This Function Reads the entire logfile and prints the IP addresses of all clients requesting and number of requests made by each client.

void analyzer::clients_requesing()
{

int i,k;

cout<<endl<<"--------------------Clients Requesting--------------------"<<endl;

no_of_clients=0;


init();

  while(1)
  {
     strcpy(line,"");
     fflush(stdin);
     cin.getline(line,1000,'\n');
     i=0;
     k=0;
     strcpy(phrase,"");
     if(strlen(line)>1)
     {
        while(line[i]!=' ')
        {
            phrase[i]=line[i];
            i=i+1;
        }
        phrase[i]=0;
        for(k=0;k<no_of_clients;k++)
        {
            if(!strcmp(clients[k].ip,phrase))
            {
                break;
            }
        }
        if(k==no_of_clients)
        {
            strcpy(clients[k].ip,phrase);
            no_of_clients++;
        }
        
        clients[k].requests_made++;
     }
     else
     {
     	break;
     }
  }
  
  
  cout<<endl<<endl;
  
  for(i=0;i<no_of_clients;i++)
  {
  	cout<<endl<<clients[i].ip<<"\t\t"<<clients[i].requests_made<<endl;
  }

  cout<<endl<<endl<<"Total number of clients requesting :: "<<no_of_clients<<endl<<"----------------------------------------------------------"<<endl<<endl;
  
  
  


}

//This Function prints the information regarding each request made by a particular client as requested by the user.

void analyzer::request_of_each_client(char *ip_adress)
{

    char ip[15],timestamp[50],method[5],url[100],refer[100],user_agent[100],line2[1000],ref[100];
    int status_code,dummy;
    long long int size_sent;
    int i,j,flag;

    no_of_clients=0;


    init();

    cout<<endl<<"--------------------------------------------------Client:"<< ip_adress<<"--------------------------------------------------------------"<<endl;
    	 printf("\n\nSrNo  Methd                    Website  Cde  Size Snt                                 Page Requested                        Timestamp");
	 printf("\n\n----  -----  -------------------------  ---  --------  ---------------------------------------------  -------------------------------");
    //main part

    strcpy(clients[no_of_clients].ip,ip_adress);
    dummy=0;
 
  
   while (1) 
    {      
                line[0]=0;
                line[2]=0;
                fflush(stdin);
                cin.getline(line2,1000);   //we read the file line by line.
                if(strlen(line2)>1)
                {
                	strcpy(line,line2);
                	parse(ip,timestamp,method,url,refer,user_agent,&status_code,&size_sent);        
                	
                	
                	/*Each line stores information about one request made by the client to the server.
                	Each line read, is parsed.
                	Parsing decomposes one line into different components.
                	Each component stores some information about the request made.
                	Value of these components is stored into different variables by the parse function.
                	Hence we pass the memory addresses of these variables to the function.
                	*/
                	
                	
                        if(!strcmp(ip_adress,ip))
                        {
                        
                        	//This seperates the page requested and the actual domain name from the request line.
                                i=0;
		    		while(url[i]!='/')
		    		i++;
		    		j=i+1;
		    		while(url[j]!='?' && url[j]!=' ')
		    		{  
		    		  ref[j-i-1]=url[j];
		    		  j++;
		    		}
		    		ref[j-i-1]=0;
		    		url[i]=0;
		    		flag=0;
                                for(i=0;i<4;i++)
		    		{
		    			line2[i]=url[i];
		    		}
		    		line2[i]=0;
		    		if(strcmp(line2,"www."))
		    		{
		    			strcpy(line2,"www.");
		    			strcat(line2,url);
		    			strcpy(url,line2);
		    		}
		    		
		    		
                            printf("\n\n%4d  %5s  %25s  %3d  %8lld  %45s  %31s",clients[no_of_clients].requests_made+1,method,url,status_code,size_sent,ref,timestamp);
                            dummy=clients[no_of_clients].requests_made;
                            clients[no_of_clients].requests_made=dummy+1;
                        }
              	}
              	else
              	{
              		break;
              	}
              	
    }
    cout<<endl<<"---------------------------------------------------------------------------------------------------------------------------"<<endl<<endl;
    

}


//This is the main function which separates different components of single request.
void analyzer::parse(char ip[],char timestamp[],char method[],char url[],char refer[],char user_agent[],int * status_code,long long int * size_sent)
{
    int i,j,k,l;
    int len=strlen(line);
    bool quote_open=false;
    int phrasenumber;


    i=0;
    j=0;
    k=0;
    l=0;
    phrase[k]=0;
    phrasenumber=1;
    
    
    /*
    We know that every component has fixed position in a particular line/statement in th log file.
    Hence we identify each component based on its serial number in the line.
    Value of each component is stored in character array phrase[].
    Phrases maybe multi-word statements.
    We have to carefully identify the seperators, seperating the phrases.  
    */


    while(i<=len)
    {
        if( (line[i]=='"') || (line[i]=='[') || (line[i]==']') )
        {
            quote_open=(!quote_open);
        }
        else
        {
            if(is_seperator(line[i],quote_open))
            {
                phrase[k]=0;
                k=0;
                //categorize phrases----------------------------------------------------------

                switch(phrasenumber)
                {

                    case 1:
                        strcpy(ip,phrase);
                        break;

                    case 2:

                        break;

                    case 3:

                        break;

                    case 4:
                        strcpy(timestamp,phrase);
                        break;

                    case 5:
                        for(j=0;phrase[j]!=' ';j++)
                        {
                            method[j]=phrase[j];
                        }
                        method[j]=0;
                        l=0;
                        j++;
                        while(j<strlen(phrase))
                        {
                            url[l++]=phrase[j++];
                        }
                        url[l]=0;
                        break;

                    case 6:
                        *(status_code)=atoi(phrase);
                        break;

                    case 7:
                        *(size_sent)=atoll(phrase);
                        break;

                    case 8:
                        strcpy(refer,phrase);
                        break;

                    case 9:
                        strcpy(user_agent,phrase);
                        break;

                    default:
                        cout<<endl<<"Error : Unrecognized field "<<phrase<<endl;
                        break;


                }
                // ---------------------------------------------------------------------------
                phrasenumber++;
            }
            else
            {
                phrase[k++]=line[i];
            }
        }

        i++;
    }

}

//Identifies whether (char ch) is a seperator or not.
int analyzer::is_seperator(char ch,bool quote_open)
{
    if((ch==' '||ch==0)&&quote_open==0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//This function scans the entire file. Goes through every request stored in the file. And prints
//all the domain names that have been requested by different clients and the number of hits per website.
void analyzer::request_to_each_website()
{

    char url[200],line2[1000],method[10];
    int dummy,i,flag;
   
    init();
   
    while(1)
    {
    	
    	line2[0]=0;
    	line[0]=0;
    	fflush(stdin);
    	cin.getline(line2,1000);
    	if(strlen(line2)>1)
    	{
    		strcpy(line,line2);
    		parse(url,method); //Returns requestline for each request.
    		i=0;
    		while(url[i]!='/')
    		  i++;	
    		url[i]=0;
    		flag=0;
    		for(i=0;i<4;i++)
    		{
    			line2[i]=url[i];
    		}
    		line2[i]=0;
    		if(strcmp(line2,"www."))
    		{
    			strcpy(line2,"www.");
    			strcat(line2,url);
    			strcpy(url,line2);
    		}
    		
    		/*
    		We store all the domain names those which have been referred until now.
    		We go through this entire list.
    		If the domain name isnt present then we make a new entry.
    		*/
    		
    		for(i=0;i<no_of_websites;i++)         
    		{
    		 	if(!strcmp(websites[i].url,url))
    		 	{
    		 		flag=1;
				break;
    		 	}
    		}
    		websites[i].no_of_hits=websites[i].no_of_hits+1;
    		if(!flag)
    		{	
    		    
    		    strcpy(websites[no_of_websites].url,url);
    		    no_of_websites++;
    		}
    		
    	}
    	else
    	{
    		break;
    	}
    
    }
    
    
    cout<<endl<<endl<<"Total number of websites:: "<<no_of_websites<<endl<<endl;
    
    cout<<endl<<"--------------------------------Number of Hits For Each Website-------------------------------------------"<<endl;
    
    for(i=0;i<no_of_websites;i++)
    {
    	printf("\n%50s\t\t%6d\n",websites[i].url,websites[i].no_of_hits);
    }
    
   cout<<endl<<"-----------------------------------------------------------------------------------------------------------"<<endl<<endl;
}


//This parse function only returns the request line from the HTTP request.
void analyzer::parse(char url[],char method[])
{
    int i,j,k,l;
    int len=strlen(line);
    bool quote_open=false;
    int phrasenumber;


    i=0;
    j=0;
    k=0;
    l=0;
    phrase[k]=0;
    phrasenumber=1;


    while(i<=len)
    {
        if( (line[i]=='"') || (line[i]=='[') || (line[i]==']') )
        {
            quote_open=(!quote_open);
        }
        else
        {
            if(is_seperator(line[i],quote_open))
            {
                phrase[k]=0;
                k=0;
                //categorize phrases----------------------------------------------------------

                if(phrasenumber==5)
                {
                   
                        for(j=0;phrase[j]!=' ';j++)
                        {
                            method[j]=phrase[j];
                        }
                        method[j]=0;
                        l=0;
                        j++;
                        while(j<strlen(phrase))
                        {
                            url[l++]=phrase[j++];
                        }
                        url[l]=0;
                        return;
                }
                phrasenumber++;
            }
            else
            {
                phrase[k++]=line[i];
            }
        }
        i++;
    }
}

//This function displays each and every request made by each client.
void analyzer::request_of_all_clients()
{
	char ip[15],timestamp[50],method[5],url[100],refer[100],user_agent[100],line2[1000];
    	int status_code,dummy;
    	long long int size_sent;
	int pings;
	int i,flag,j;
	
	pings=0;
	
	init();
	
	printf("\n\n----------------------------------------------------------------------------------------------------------------------------------------------\n\n");
	
	printf("\n\nSrNo  Source Ip Address  Methd                    Website  Cde  Size Snt                                 Page Requested                        Timestamp");
	printf("\n\n----  -----------------  -----  -------------------------  ---  --------  ---------------------------------------------  -------------------------------");
	
	while(1)
	{
		line[0]=0;
		line2[0]=0;
		cin.getline(line2,1000);
		if(strlen(line2)>1)
		{
			strcpy(line,line2);
			parse(ip,timestamp,method,url,refer,user_agent,&status_code,&size_sent);
			
			        //Same function defined above is called to obtain different components of the request.
			
				i=0;
		    		while(url[i]!='/')
		    		  i++;
		    		j=i+1;
		    		while(url[j]!='?' && url[j]!=' ')
		    		{  
		    		  refer[j-i-1]=url[j];
		    		  j++;
		    		}
		    		refer[j-i-1]=0;
		    		url[i]=0;
		    		flag=0;
		    		for(i=0;i<4;i++)
		    		{
		    			line2[i]=url[i];
		    		}
		    		line2[i]=0;
		    		if(strcmp(line2,"www."))
		    		{
		    			strcpy(line2,"www.");
		    			strcat(line2,url);
		    			strcpy(url,line2);
		    		}
                            	printf("\n\n%4d  %17s  %5s  %25s  %3d  %8lld  %45s  %31s",pings+1,ip,method,url,status_code,size_sent,refer,timestamp);
                            	pings++;
		
		}
		else
		break;
	}	
	
	printf("\n\n----------------------------------------------------------------------------------------------------------------------------------------------\n\n");
	

}

//This function displays all the requests made between a particular time period.
//The time period may vary from a few seonds to many days.
//Start is the timestamp that indicates the start of the time period.
//End is the timestamp that indicates end of the time period.
void analyzer::between_timestamp(char start[],char end[])
{
    	char ip[15],timestamp[50],method[5],url[100],refer[100],user_agent[100],line2[1000];
    	int status_code,dummy;
    	long long int size_sent;
	int pings;
	int values[3][6];
	int i,flag,j;
	
	pings=0;
	
	/*
	Different components of timestamp include year,month,day,hour,minute,second.
	This parse function stores above mentioned components in the addresses of variables passed to it.
	Integer array values[][] stores each and every component of the Start timestamp and End timestamp.
	values[0][] stores components of Start.
	values[1][] stores components of End.
	*/
	
	parse(start,&values[0][0],&values[0][1],&values[0][2],&values[0][3],&values[0][4],&values[0][5]);
	parse(end,&values[1][0],&values[1][1],&values[1][2],&values[1][3],&values[1][4],&values[1][5]);
	
	printf("\n\n----------------------------------------------------------------------------------------------------------------------------------------------\n\n");
	
	printf("\n\n SrNo  Source Ip Address  Methd                    Website  Cde  Size Snt                                 Page Requested                        Timestamp");
	printf("\n\n-----  -----------------  -----  -------------------------  ---  --------  ---------------------------------------------  -------------------------------");
	
	init();
	
	while(1)
	{
		line[0]=0;
		line2[0]=0;
		cin.getline(line2,1000);
		if(strlen(line2)>1)
		{
			strcpy(line,line2);
			parse(ip,timestamp,method,url,refer,user_agent,&status_code,&size_sent);
			
			/*
			Check whether the timestamp lies within the given boundaries.
			*/
			
			if(falls_between(values,timestamp))
			{
				i=0;
		    		while(url[i]!='/')
		    		  i++;
		    		j=i+1;
		    		while(url[j]!='?' && url[j]!=' ')
		    		{  
		    		  refer[j-i-1]=url[j];
		    		  j++;
		    		}
		    		refer[j-i-1]=0;
		    		url[i]=0;
		    		flag=0;
		    		for(i=0;i<4;i++)
		    		{
		    			line2[i]=url[i];
		    		}
		    		line2[i]=0;
		    		if(strcmp(line2,"www."))
		    		{
		    			strcpy(line2,"www.");
		    			strcat(line2,url);
		    			strcpy(url,line2);
		    		}
                            	printf("\n\n%5d  %17s  %5s  %25s  %3d  %8lld  %45s  %31s",pings+1,ip,method,url,status_code,size_sent,refer,timestamp);
                            	pings++;
			}
		}
		else
		break;
	}	
	
	
	printf("\n\n----------------------------------------------------------------------------------------------------------------------------------------------\n\n");
	
}

//this function returns true if the timestamp[] lies within the boundaries of the time period.

bool analyzer::falls_between(int values[][6], char timestamp[])
{
	int i,j;
	int year,month,day,hour,minute,second;
	int ret;
	
	for(i=0;timestamp[i]!=' ';i++);
	timestamp[i]=0;
	
	//Components of the timestamp are seperated and stored in value[2][].
	parse(timestamp,&values[2][0],&values[2][1],&values[2][2],&values[2][3],&values[2][4],&values[2][5]);
	
	for(i=0;(i<6 && values[0][i]==values[2][i]); i++);
	
	if(i==6)
	{
		return true;
	}
	else
	{
		if(values[0][i]>values[2][i])
		{
			return false;
		}
		else if(values[0][i]<values[2][i])
		{
			for(j=0;(j<6 && values[1][j]==values[2][j]);j++);
			if(j==6)
			return true;
			else
			{
				if(values[1][j]<values[2][j])
				{
					return false;
				}
				else if(values[1][j]>values[2][j])
				{
					return true;
				}
			}
		}
	}

}


//This function seperates and stores different components of the timestamp passed to it.
void analyzer::parse(char timestamp[],int * year,int * month,int * day,int * hour,int * minute,int * second)
{
        int i,j,wordnumber,len=strlen(timestamp);
        char word[10];
        int k;
        
        i=0;
        wordnumber=1;
        j=0;
        word[j]=0;
        
        while(i<=len)
        {
        
           if(timestamp[i]=='/'||timestamp[i]==':'||timestamp[i]==0)
           {
           	word[j]=0;
           	j=0;
           	switch(wordnumber)
           	{
           		case 1:
           		  *day=atoi(word);
           		break;
           		
           		case 2:
           		  for(k=0;k<12;k++)
           		  {
				if(!strcmp(word,months[k]))
				{
					(*month)=k+1;
					break;				
				}           		  
           		  }
           		break;
           		       
           		case 3:
           		  *year=atoi(word); 
           		break;
           		
           		case 4:
           		  *hour=atoi(word);
           		break;
           		
           		case 5:
           		  *minute=atoi(word);
           		break;
           		
           		case 6:
           		  *second=atoi(word);
           		break;
           	}
           	
           	wordnumber++;
           }
           else
           {
           	word[j++]=timestamp[i];
           }
           
           i++;
        }
        
        if(wordnumber==4)
        {
        	(*hour)=(*minute)=(*second)=0;
        }
}

//-----------------------help function----------------------------

void analyzer::help()
{

cout<<endl<<"To display IP addresses of all te clients requesting :"<<endl;
cout<<"Arguments passed : -c"<<endl;
cout<<"General Format :"<<endl<<"./a.out -c < [input file] > [output file]"<<endl;

cout<<endl<<"To display number of requests made to different websites :"<<endl;
cout<<"Arguments passed : -w"<<endl;
cout<<"General Format :"<<endl<<"./a.out -w < [input file] > [output file]"<<endl;

cout<<endl<<"To display each and every requet made by all the clients :"<<endl;
cout<<"Arguments passed : -n"<<endl;
cout<<"General Format :"<<endl<<"./a.out -n < [input file] > [output file]"<<endl;

cout<<endl<<"To display all requests made by paricular client :"<<endl;
cout<<"Arguments passed : -pc,client ip address"<<endl;
cout<<"Argument's format : "<<endl<<"client ip address : aaa.bbb.ccc.ddd"<<endl;
cout<<"General Format :"<<endl<<"./a.out -pc [client ip address] < [input file] > [output file]"<<endl;

cout<<endl<<"To all requests made within a particular time period :"<<endl;
cout<<"Arguments passed : -t, Start timestamp, End timestamp"<<endl;
cout<<"Argument's format : "<<endl<<"Timestamp : date/month/year:hours:minutes:seconds"<<endl;
cout<<"General Format :"<<endl<<"./a.out -t [Start timestamp] [End timestamp] < [input file] > [output file]"<<endl<<endl;

}

//--------------------intialization functions--------------------
void analyzer::init()
{
    int i,k;

    line[0]=0;
    no_of_clients=0;
    no_of_websites=0;
    for(i=0;i<200;i++)
    {
        clients[i].requests_made=0;
        clients[i].ip[0]=0;
        
    }
    
    for(i=0;i<200;i++)
	{
	 	websites[i].no_of_hits=0;
	 	websites[i].url[0]=0;
	}
    
}

analyzer::analyzer()
{
    int i,k;

    line[0]=0;
    no_of_clients=0;
    no_of_websites=0;
    for(i=0;i<200;i++)
    {
        clients[i].requests_made=0;
        clients[i].ip[0]=0;
    }
    
    for(i=0;i<200;i++)
	{
	 	websites[i].no_of_hits=0;
	 	websites[i].url[0]=0;
	}

}
