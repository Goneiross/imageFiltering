/** \author PELLERIN Guillaume
 *  \date	dec 2018
 */
#include <iostream>
#include <math.h>
using namespace std;

void error_nbR(int nbR);
void error_color(int id);
void error_threshold(double invalid_val);
void error_nb_filter(int nb_filter);

void filtering(int xSize, int ySize, int nbR, int** map);

int main()
{
	int nbR = 0;
	cin >> nbR;
	if (nbR < 2) {
        error_nbR(nbR);
		return(1);
    }
 	// ---------------------------------------------- //
	int colors_used[nbR + 1][3] = {0};
	for (int i = 1; i < nbR + 1; i++){
		for (int j = 0; j < 3; j++){
			cin >> colors_used[i][j];
		}
	}

	// ----------------------------------------------- //
	double colors_threshold[nbR + 1];
	colors_threshold[0] = 0;
    colors_threshold[nbR] = 1;
	for (int i = 1; i < nbR; i++){
		cin >> colors_threshold[i];
	}

	// ----------------------------------------------- //
	int nbF = 0;
	cin >> nbF;

	// ----------------------------------------------- //
	char tmpChar[2];
	cin >> tmpChar;
	cout << tmpChar << endl;

	// ----------------------------------------------- //

	int size[2] = {0,0};
	cin >> size[1] >> size[0];
	cout << size[1] << " " << size[0] << endl;

	// ----------------------------------------------- //

	int max = 0;
	cin >> max;
	cout << max << endl;

	// ----------------------------------------------- // Seuillage

	int picture[size[0]][size[1]][3];

	// int map[size[0]][size[1]];
	int** map = new int*[size[0]];
   	for (int i = 0; i < size[0]; i++) {
    	map[i] = new int[size[1]];
    }

	double tmpDouble;
	int R = 0;
	int G = 0;
	int B = 0;
	bool notTheEnd = true;
	int k = 1;

	for (int i = 0; i < size[0]; i++){
		for (int j = 0; j < size[1]; j++){
			for (int k = 0; k < 3; k++){
				cin >> picture[i][j][k];
			}
			R = picture[i][j][0];
			G = picture[i][j][1];
    		B = picture[i][j][2];
			tmpDouble = sqrt(R * R + G * G + B * B) / (sqrt(3) * max);
			notTheEnd = true;
			k = 1;
			while (k <= nbR && notTheEnd){
				if(tmpDouble < colors_threshold[k]){
					map[i][j]=k;
					notTheEnd = false;
				}
				k++;
			}

		}
	}

	// ----------------------------------------------- // Filtrage
	
	for (int p = 0; p < nbF; p++){
		filtering(size[0], size[1], nbR, map);
	}

	if (nbF > 0){
		for (int i = 0; i < size[0]; i++){
			map[i][0] = 0;
			map[i][size[1]-1] = 0; 
		}
		for (int j = 1; j < size[1] - 1; j++){
			map[0][j] = 0;
			map[size[0] - 1][j] = 0; 
		}
	}

	// ----------------------------------------------- // Renvoie

	for (int i = 0; i < size[0]; i++){
		for (int j = 0; j < size[1]; j++){
			for (int k = 0; k < 3; k++){
				cout << colors_used[map[i][j]][k] << " ";
			}
			cout << "    ";
		}
		cout << endl;
	}
	
	

	return 0;
}

void filtering(int xSize, int ySize, int nbR, int** map){
	int maxColorNb = 0;
	int maxColorValue = 0;
	int testColor[nbR + 1] = {0};

	int tmpMap[xSize][ySize] = {0};

	for (int xPos = 1; xPos < xSize - 1; xPos++){
		for (int yPos = 1; yPos < ySize - 1; yPos++){
			
			for (int i = 0; i < nbR + 1; i++){
				testColor[i] = 0;
			}

			for (int i = -1; i <= 1; i++) {
            	for (int j = -1; j <= 1; j++) {
            	    if (i == 0 && j == 0) {
                	} else {
						testColor[map[i + xPos][j + yPos]] += 1;
					}
            	}
        	}
			maxColorNb = 0;
			maxColorValue = 0;
			for (int i = 0; i <= nbR; i++){
				if (testColor[i] > maxColorNb) {
					maxColorNb = testColor[i];
					maxColorValue = i;
				}
			}
        	if (maxColorNb >= 6) {
            	tmpMap[xPos][yPos] = maxColorValue;
        	} else {
            	tmpMap[xPos][yPos] = 0;
        	}
		}
	}
	for (int i = 0; i < xSize; i++){
		for (int j = 0; j < ySize; j++){
			map[i][j]= tmpMap[i][j];
		}
	}
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
