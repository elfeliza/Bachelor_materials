#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main()
{
	string a;
	string b;
	string c;
	cin >> a >> b >> c;
	
	if (a <= b && a <= c) 
	{ 
		cout << a << endl;
	}
	
	 else if (b <= a && b <= c) 
	{ 
		cout << b << endl;
	}
	else cout << c;

	return 0;
}



