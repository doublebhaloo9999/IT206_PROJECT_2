#include<iostream>
#include<windows.h>
using namespace std;
int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Define and initialize hConsole
    for(int i=0 ; i<15 ; i++ ) {
        SetConsoleTextAttribute( hConsole, i  ); 
        cout<<i<<" : " << "COLOR "<<endl; 
    }

    return 0;
}
