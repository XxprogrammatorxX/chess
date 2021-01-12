#include <iostream>
#include <random>
#include <ctime>
#include "menu.h"

using namespace std;


int main ()
try
{
    srand(time(NULL));
    Chess_Menu menu(1 + rand() % 9);
    return 0;
}
catch (exception& e)
{
    cerr << e.what() << endl;
}
