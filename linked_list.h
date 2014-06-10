#include<string.h>
#include<iostream>
using namespace std;
class list;

class node
{
	char data[12];
	node*link;

	public:
		node(char a[])
		{
			link=NULL;
			strcpy(data, a);
		}

		friend class list;
		friend int compare(list l1, list l2);
};

class list
{
	node *head;

	public:
		list()
		{
			head=NULL;
		}

		void insert(char a[])
		{
			node *p=head;
			if(p)
			{
				while(p->link)
					p = p->link;

				p->link = new node(a);
			}
			else
			{
				head = new node(a);
			}
		}

		void Delete(int num)
		{
			if(num==0)
			{
				node *p = head;
				head = head -> link;
										//cout<<"Deleted node is "<<p->data<<endl;
				delete p;
				return;
			}
			int cnt=0;
			node *p, *q;
			q=NULL;
			p=head;
			while(cnt<num)
			{
				++cnt;
				q=p;
				p = p->link;
			}
			q->link = p->link;
										//cout<<"Deleted node is "<<p->data<<endl;
			delete p;
		}

		void display()
		{
			node *p=head;
			while(p)
			{
				cout<<p->data<<endl;
				p=p->link;
			}
										//cout<<"Count of nodes : "<<count()<<endl;
		}

		char *lastNode()
		{
			node *p=head;
			while(p->link)
				p=p->link;
			return p->data;
		}

		int count()
		{
			node* p=head;
			int cnt=0;
			while(p)
			{
				++cnt;
				p=p->link;
			}
			return cnt;
		}

		char *getdata(node *p)
		{
			return p->data;
		}

		bool isNULL(node *p)
		{
			if(p)
				return 0;
			return 1;
		}

		friend void update_metadata(char []);
		friend int compare(list l1, list l2);
};


int compare(list l1, list l2)
{
	node *p1, *p2;
	p1 = l1.head;
	p2 = l2.head;
	int cnt=0;
	while(p1 && p2 && !strcmp(p1->data, p2->data) )
	{
		p1 = p1->link;
		p2 = p2->link;
		++cnt;
	}
	return cnt;
}


