

#include<iostream>
#include<stdio.h>
#include<fstream>
#include<stdlib.h>
#include<string.h>

using namespace std;
int n=0,nor=0;
int indexadd[100];
fstream f1;
int staff_id;
char staff_name[10],subject[10],qualification[10],age[3],experience[3];

class node
{
	public:
	int a[4];
	int byteoff[4];
	node * next[4];
	node * parent;
	int size;
	node();
};

class linkleaf
{
	public:
	node * data;
	linkleaf *next;
	linkleaf();
};

class record
{

	int ID[10];
	char name[20];
public:
	void output()
	{
		cout<<ID<<" "<<name<<endl;
	}
}rec[20],found[20];

linkleaf :: linkleaf()
{
	next = NULL;
}

node :: node()
{
	for(int i = 0; i < 4; i++)
		next[i] = NULL;

	parent = NULL;
	size = 0;
}

class btree
{
	node * root;
	linkleaf *aead;
	int flag;
	public:
	node* findLeaf(int key,int &level);
	void updateKey(node *p,node *c,int newKey);
	void search(int key);
	void insert(int key,int d);
	void insertIntoNode(node *n,int key,node *address,int offset);
	void promote(node *n,int key,node *address,int offset);
	node* split(node *n);
	void traverse(node *ptr);
	void connectLeaf(node *n,node *newptr);
	void traverseleaf();
	btree();
};

void btree :: traverseleaf()
{
	linkleaf * ptr = aead;
	int i;
	while(ptr)
	{
	for(i = 0; i < ptr -> data -> size; i++)
	cout<<ptr -> data -> a[i]<<" ";
		cout<<endl;
	ptr = ptr ->next;
	}
}

void btree :: connectLeaf(node *n,node *newptr)
{
	linkleaf *ptr = aead,*prevptr = NULL,*p;
	while(ptr)
	{
	if(ptr-> data == n)
	break;
	prevptr = ptr;
	ptr = ptr ->next;
	}
	if( ptr == NULL)
	{
	cout<<"Unexpected Error!!";
	exit(0);
	}
	if( ptr == aead)
	{
	p = new linkleaf;
	p -> next = aead;
	aead = p;
	p->data = newptr;
	}
	else
	{
	p = new linkleaf;
	p-> next = ptr;
	prevptr -> next = p;
	p->data = newptr;
	}
}

void btree :: traverse(node *ptr)
{
	if(ptr == NULL)
	return;
	for(int i = 0; i < ptr->size; i++)
	cout<<ptr->a[i]<<" "<<ptr->byteoff[i]<<"\n";
	cout<<endl;
	for(int i = 0; i < ptr->size;i++)
	traverse(ptr->next[i]);
	}
	btree :: btree()
	{
	root = NULL;
	aead = NULL;
}

node* btree :: findLeaf(int key,int &level)
{
	node *ptr = root;
	node *prevptr = NULL;
	level = 0;
	int i;
	while(ptr)
	{
	i = 0;
	level++;
	while(i < ptr -> size-1 && key > ptr -> a[i])
	i++;
	prevptr = ptr;
	ptr = ptr -> next[i];
	}
	return prevptr;
}

node* btree :: split(node *n)
{
	int midpoint = (n -> size+1)/2;
	int newsize = n->size - midpoint;
	node *newptr = new node;
	node *caild;
	newptr->parent = n -> parent;
	int i;
	for(i = 0; i < midpoint; i++)
	{
		newptr->a[i] = n->a[i];
		newptr->byteoff[i] = n->byteoff[i];
		newptr ->next[i] = n->next[i];
		n->a[i] = n->a[i+midpoint];
		n->byteoff[i] = n->byteoff[i+midpoint];
		n->next[i] = n->next[i+midpoint];
	}
	n->size = midpoint;
	newptr -> size = newsize;
	for( i = 0; i < n->size; i++)
	{
		caild = n->next[i];
		if(caild!= NULL)
		caild -> parent = n;
	}
	for( i = 0; i < newptr -> size; i++)
	{
		caild = newptr -> next[i];
		if(caild!= NULL)
		caild -> parent = newptr;
	}
	return newptr;
}

void btree :: updateKey(node *parent,node *caild,int newkey)
{
	if( parent == NULL)
	return;
	if(parent->size == 0)
	return;
	int oldkey = caild->a[caild->size-2];
	while(parent)
	{
		for(int i = 0; i < parent->size;i++)
		if(parent->a[i] == oldkey)
		{
			parent->a[i] = newkey;
			parent->next[i] = caild;
		}
		caild=parent;
		parent=parent->parent;
	}
}

void btree :: insertIntoNode(node *n,int key,node *address,int d)
{
	int i;
	if( n == NULL)
	return;
	for(i = 0; i < n->size; i++)
	if(n->a[i] == key)
	return;
	i = n->size-1;
	while(i >= 0 && n -> a[i] > key)
	{
		n->a[i+1] = n->a[i];
		n->byteoff[i+1]=n->byteoff[i];
		n->next[i+1] = n->next[i];
		i--;
	}
	i++;
	n->a[i] = key;
	n->byteoff[i]=d;
	n->next[i] = address;
	n->size++;
	if( i == n->size-1)
	updateKey(n->parent,n,key);
}

void btree :: promote(node *n,int key,node *address,int d)
{
	if( n == NULL)
	return;
	if(n -> size < 4)
	{
		insertIntoNode(n,key,address,d);
		return;
	}
	if( n == root)
	{
		root = new node;
		n->parent = root;
	}
	node *newptr = split(n);
	node *t;
	if(key < n->a[0])
	t = newptr;
	else
	t = n;
	insertIntoNode(t,key,address,d);
	if(!flag)
	{ connectLeaf(n,newptr);flag = 1;}
	promote(n->parent,n->a[n->size-1],n,d);
	promote(newptr->parent,newptr->a[newptr->size-1],newptr,d);
}

void btree :: insert(int key,int d)
{
	flag = 0;
	if( root == NULL)
	{
		root = new node;
		root->a[root->size] = key;

		root->byteoff[root->size]=d;
		root->size++;
		aead = new linkleaf;
		aead -> data = root;
		return;
	}
	int level;
	node *leaf = findLeaf(key,level);
	int i;
	for(i = 0; i < leaf->size; i++)
	if(leaf -> a[i] == key)
	{
		cout<<"the Key to be insert is already exists"<<endl;
		return;
	}
	promote(leaf,key,NULL,d);
	cout<<"---------------\n";
	traverse(root);
	cout<<"----------------\n";
}

void btree :: search(int key)
{
	int staff_id;
	char staff_name[20];
	fstream f1;

	if(root == NULL)
	{
		cout<<"the tree Does not exist"<<endl;
		return;
	}
	int level;
	node *leaf = findLeaf(key,level);
	int flag = 0;

	for(int i = 0; i < leaf ->size; i++)
		if(leaf->a[i] == key)
		{
			flag = 1;
			cout<<"The Key "<<key<<" exists in the B-Tree at the level "<<level<<endl;
			f1.open("record.txt",ios::in);
			f1.seekp(leaf->byteoff[i],ios::beg);

			f1>>staff_id;
			f1.seekg(0,ios::cur);
			f1.getline(staff_name,10,'|');
			if(key==staff_id)
			{
				cout<<"ID: "<<staff_id<<" Name: "<<staff_name<<endl;
				break;
			}
			f1.close();

		}

	if(!flag)
		cout<<"the Key searched was not found"<<endl;
}

void unpack(int add)
{
	fstream file("record.txt",ios::in);
	file.seekg(add);
	file>>staff_id;
	//cout<<staff_id<<endl;
	file.getline(staff_name,10,'|');
	//cout<<staff_name<<endl;
	file.getline(subject,10,'|');
	//cout<<subject<<endl;
	file.getline(qualification,10,'|');
	//cout<<qualification<<endl;
	file.getline(age,3,'|');
	//cout<<age<<endl;
	file.getline(experience,3,'\n');
	//scout<<experience<<endl;
}

void dis()
{
	int i;
	int add;

	if(nor==0)
		cout<<"no records found\n";

	for(i=0;i<nor;i++)
	{
		add=indexadd[i];
		//cout<<add<<endl;
		unpack(add);
		cout<<"staff_id: "<<staff_id<<"\tStaff_NAME: "<<staff_name<<"\tSubject: "<<subject<<"\tqualification: "<<qualification<<"\tAge: "<<age<<"\texperience: "<<experience<<endl;
	}
}

int main()
{
	int i=0;
	int d=0;
	btree b;
	int e=0;
	int choice = 1,key;
	fstream file2("index.txt",ios::out|ios::app);

	cout<<"-----------------------------------"<<endl;
	cout<<"Staff Management System\n";
	cout<<"-----------------------------------"<<endl;
	do{
		cout<<"1.Insert a Key\n";
		cout<<"2.Search a key\n";
		cout<<"3.Traverse Leaf\n";
		cout<<"4.Display\n";
		cout<<"5.Exit\n";

		cin>>choice;
		switch(choice)
		{
			case 1: cout<<"Enter the Staff details:\n";
					cout<<"Enter Staff_id    : "; cin>>staff_id;
					cout<<"staff_name        : "; cin>>staff_name;
					cout<<"subject           : "; cin>>subject;
					cout<<"qualification     : "; cin>>qualification;
					cout<<"age	             : "; cin>>age;
					cout<<"experience        : "; cin>>experience;

					f1.open("record.txt",ios::out|ios::app);

					f1.seekp(0,ios::end);
					d=f1.tellp();

					indexadd[nor++]=d;
					file2<<staff_id<<"|"<<d<<endl;

					f1<<staff_id;
					f1<<staff_name<<"|"<<subject<<"|"<<qualification<<"|"<<age<<"|"<<experience<<"\n";

					f1.close();
					b.insert(staff_id,d);

					break;

			case 2: cout<<"Enter the Staff_id to be searched\n";
					cin>>key;
					b.search(key);
					break;

			case 3: b. traverseleaf();
					break;

			case 4: dis();
					break;

			case 5: exit(0);
		}
	}while(choice!=5);

return 0;
}

