#include <string>
#include <iostream>
using namespace std;

struct  CACANA
{
	int a;
	int b;
};

void fillOutCaca(CACANA* c)
{
	c->a = 99;
	c->b = 100;
}

int main()
{
	/*
	CACANA* miMierda;
	miMierda = new CACANA[10];
	for (int i=0; i< 10; i++)
	{
		miMierda[i].a = 128;
		miMierda[i].b = 129;
	}
	cout<<sizeof(miMierda)<<endl;
	for (int i=0; i< 10; i++)
	{
		cout<<miMierda[i].a<<endl;
	}
	delete[] miMierda;
	miMierda = NULL;
	for (int i=0; i< 10; i++)
	{
		cout<<miMierda[i].b<<endl;
	}
	*/
	
	/*
	int a =99;
	int* b;
	b = new int(66);
	cout<<a<<endl;
	cout<<&a<<endl;
	cout<<b<<endl;
	cout<<*b<<endl;
	*/
	
	/*
	float** a;
	
	a = new float *[5];
	a[1] = new float[10];
	a[1][10] = 99;
	cout<<a[1][10]<<endl;
	*/
	
	/*
	CACANA* arrayCaca;
	arrayCaca = new CACANA[5];
	
	for(int i=0; i<5; i++)
	{
		fillOutCaca(&arrayCaca[i]);
		cout<<arrayCaca[i].a<<endl;
	}
	*/
	/*
	string s;
	s = "\0";
	cout<<s<<"*"<<s<<"*"<<endl;
	
	 s = "";
	cout<<s<<"*"<<s<<"*"<<endl;

	cout<<(s=="")<<" "<<(s=="\0")<<" "<<endl;
	*/
	if (-1)
	{
		cout<<"-1 == true"<<endl;
	}
	if (1)
	{
		cout<<"1 == true"<<endl;
	}
	if (0)
	{
		cout<<"0 == true"<<endl;
	}
}


