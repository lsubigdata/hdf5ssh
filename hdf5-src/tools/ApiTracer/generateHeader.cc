#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <algorithm>
#include <functional>


using namespace std;

list<string> functionlist;
list<string>::iterator i; 

int readFunctions( const char * file)
{
ifstream infile; 
string s;

	infile.open(file); 
	if(infile.fail())
	{
		cout << "Cannot open functionlist" << file << "\n"; 
		return 1;
	} 

	cout << "reading functions\n"; 
	
	while(!infile.eof())
        {
		getline(infile, s); 

		if( s.size()>0 )
		{
			cout  << s + "<<\n"; 
			functionlist.push_back(s);
		}
	}
	
	cout << "done reading functions\n"; 

	infile.close();
	return 0;
}


int main(int argc, char* argv[])
{
string s;    
ifstream infile; 

	if (argc != 2)  
	{
		cout << "Usage: bin filename\n"; 
		return 1;
	} 

	if(infile.fail())
	{
		cout << "Cannot open file" <<argv[1]<< "\n"; 
		return 1;
	} 

	readFunctions("functionlist.dat");

	infile.open(argv[1]); 

bool insidefn = false; 
int curlbracketcount = 0;

	while(!infile.eof())
        {
		bool found=false; 
	       

		getline(infile, s); 

		// check line against list
		for( i=functionlist.begin(); i!=functionlist.end(); i++)
		{
		const string&fn = *i; 
		size_t pos = s.find(fn); 

			if( pos == 0 ) // we find it at the first column
			{
				// cout << int((s.find(fn))) << " " << s + "\n";
				found = true; 
			} 
		} 

		if( found != true )
		{
			continue; 
		} 

		// skip forward to first "{"
		while(!infile.eof())
		{
			getline(infile, s); 

			if( s.find("{") > 0)
			{
				insidefn = true; 
				curlbracketcount++;
				break; 
			}
		} 

		


	       

        } 

        infile.close(); 

        return 0; 
}

// g++ hdf5instrumenter.cc -o  hdf5instrumenter -O2
