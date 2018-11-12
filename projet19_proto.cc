/** \author PELLERIN Guillaume
 *  \date	dec 2018
 */
#include <iostream>
using namespace std;

void error_nbR(int nbR);
void error_color(int id);
void error_threshold(double invalid_val);
void error_nb_filter(int nb_filter);

int main()
{

	return 0;
}

void error_nbR(int nbR)
{
	cout << "Invalid number of colors: " << nbR << endl;
}

void error_color(int id)
{
	cout << "Invalid color value " << id << endl;
}

void error_threshold(double invalid_val)
{
	cout << "Invalid threshold value: " << invalid_val << endl;
}

void error_nb_filter(int nb_filter)
{
	cout << "Invalid number of filter: " << nb_filter << endl;
}
