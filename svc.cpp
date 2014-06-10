/*
	Problem Definition:- Simple version controlling

	Author:-
		BYTErians Group;
		Group no. 28


	Assumptions :-


		1) There must be only one operarion of either deletion or appending between 2 commit.
			(though, case for no operation between 2 commit, is handled)
		2) Appeding is done at the end only.
			(that constraint was specified in problem statement)
		3) Version asked for displaying must be valid version i.e. that version must be available


	Idea:-


			Here we are not using multiple files, each one for all different versions.
		It would have contained un-necessary multiple copies of same data.

			We take the advantage of the fact that difference between two versions will always be
		the only one operation. So we are keeping track of difference between previous version and
		current version only.

			For that, we have maintained an extra metadata file and in that file we insert the required
		information about difference between two versions.


	Data Structure:-


			As we are not maintaining the separate files for each version, we need to make use of
		our extra metadata file. And using that file we are building the content of corresponding version
		as and when query is asked.

			for that we need insertion and deletion operations most of the times and hence we use
		Linked List data structure.

*/

#include<iostream>
#include "linked_list.h"
using namespace std;
#include<fstream>
#include<stdlib.h>


/*
	This function checks the given argument for deciding type of operation.
	The logic is :--
		if all character in argument are digits then it must be a version no.
		otherwise its a filename
*/
bool check_arg(char arg[])
{
	for(int i=0; arg[i]; i++)
		if(!isdigit(arg[i]))
			return 1;
	return 0;
}

/*
	This function makes linked list of data in previous version.
*/
void build(list &file)
{
	//open file metadata
	fstream meta;
	meta.open("metadata.txt", ios::in);
	if(meta)
	{
		bool operation;
		/*
			operation: 0 if deletion
					    1 if appending
		*/

		char data[12];	// gets data which is appended

		int line_no;		// gets line no. which is deleted

		meta>>operation;
		while(!meta.eof())
		{
			if(operation)
			{
				//appending
				meta>>data;
				file.insert(data);
			}
			else
			{
				//deletion
				meta>>line_no;
				file.Delete(line_no);
			}
			meta>>operation;
		}
		meta.close();
	}
}

/*
	This function makes linked list of data in current version.
*/

void build(list &file, char filename[])
{
	//open file main
	fstream main;
	main.open(filename, ios::in);
    int count=0;
							//cout<<filename<<endl;
	char data[12];
	strcpy(data, "");
	main>>data;
	if(strlen(data)>9)
    {
        cout<<"\nCharacter width exceeded";
        exit(0);
    }
    count++;                        //cout<<data<<endl;
	while(!main.eof())
	{

		file.insert(data);
							//cout<<data<<endl;
		strcpy(data, "");
		main>>data;
        count++;
		if(strlen(data)>9)
        {
        cout<<"Character width exceeded";
        exit(0);
        }
	}
	if(count>20)
    {
        cout<<"\nTotal number of lines should be 20";
        exit(0);
    }
	if(strcmp(data, "")){
		//cout<<data<<endl;
		file.insert(data);
	}
							//cout<<"out"<<endl;
	main.close();
}


/*
	This function makes linked list of data in specified version.
*/

void build(list &file, int n)
{
							//cout<<"Here"<<n<<endl;
	//open file metadata
	fstream meta;
	meta.open("metadata.txt", ios::in);
	if(meta)
	{
		bool operation;
		/*
			operation: 0 if deletion
					    1 if appending
		*/
		char data[12];	// gets data which is appended
		int line_no;		// gets line no. which is deleted

		meta>>operation;
		int cnt = 0;
		while(!meta.eof() && cnt<n)
		{
			if(operation)
			{
				//appending
				meta>>data;
				file.insert(data);
			}
			else
			{
				//deletion
				meta>>line_no;
				file.Delete(line_no);
			}
			meta>>operation;
			++cnt;
		}
		meta.close();
	}
}


/*
	For every commit operation, we need to enter the difference of current version and previous version
	into metadata file.
*/
void update_metadata(char filename[])
{
	ofstream metadata;

	list meta, main;
	build(meta);			//previous version
	build(main, filename);	//current version

										//cout<<"Meta linked list:"<<endl;
										//meta.display();
										//cout<<"Main linked list:"<<endl;
										//main.display();

	metadata.open("metadata.txt", ios::app);

	/*
		if newer version is smaller than previous version; then deletion operation is performed.
		if newer version is larger than previous version; then appending operation is performed.

		and for un-necessary commit operation checking,
		if both version are of same size, then no changes are made to previous version and we
		avoid creating an un-necessary new version.
	*/
	if(main.count() < meta.count())				//newer < previous
	{
		//deletion
		metadata<<"0 "<<compare(meta, main)<<endl;
	}
	else if(main.count() > meta.count())			//newer > previous
	{
		//insertion
									//		cout<<"Here in meta else"<<endl;
		metadata<<"1 "<<main.lastNode()<<endl;
	}
	else			//newer = previous
		cout<<"No Changes made from previous version!!"<<endl;
	metadata.close();
}


/*
	This function prints the contents of specified version of file.
*/
void display_version(char ver_num[])
{
	list version;
	build(version, atoi(ver_num));	//specified version
	version.display();
}


int main(int argc, char *argv[])
{
	//	only 1 argument to 'svc' is allowed; hence other options should be prohibited
	if(argc!=2)
	{
		cout<<"Syntax of command is incorrect!"<<endl;
		exit(0);
	}

	/*
		check_arg returns:--
			0 for displaying nth version of the file
			1 for commit operation
	*/
	if(check_arg(argv[1]))	//0 if number else 1 if filename
	{
		//commit operation
												//cout<<"Commit"<<endl;
		update_metadata(argv[1]);
	}
	else
	{
		//display version
		display_version(argv[1]);
	}
	return 0;
}
