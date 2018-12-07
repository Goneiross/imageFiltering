/** \author PELLERIN Guillaume
 *  \date	dec 2018
 */
#include <cstdint>
#include <iostream>
#include <math.h>

using namespace std;

void error_nbR(int nbR);
void error_color(int id);
void error_threshold(double invalid_val);
void error_nb_filter(int nb_filter);

void thresholding(int xSize, int ySize, int nbR, int max, double* colors_threshold,
                  short*** picture, short** map);
void filtering(int xSize, int ySize, int nbR, short** map);
void edge(int xSize, int ySize, short** map);
void writeData(int xSize, int ySize, short** colors_used, short** map);
void deletePointers(short*** picture,short** map, short** colors_used,double* colors_threshold, int size[2], int nbR);

int main() {
    int nbR = 0;
    cin >> nbR;
    if (nbR < 2) {
        error_nbR(nbR);
        return (1);
    }

    short** colors_used = new short*[nbR + 1];
    for (int i = 0; i < nbR + 1; i++) {
        colors_used[i] = new short[3];
    }
    for (int i = 1; i < nbR + 1; i++) {
        for (int j = 0; j < 3; j++) {
            cin >> colors_used[i][j];
            if (colors_used[i][j] > 255 || colors_used[i][j] < 0) {
                error_color(i);
                return (1);
            }
        }
    }

    double* colors_threshold = new double[nbR + 1];
    colors_threshold[0] = 0;
    colors_threshold[nbR] = 1;
    for (int i = 1; i < nbR; i++) {
        cin >> colors_threshold[i];
        if (colors_threshold[i - 1] >= colors_threshold[i]) {
            error_threshold(colors_threshold[i]);
            return (1);
        }
    }

    int nbF = 0;
    cin >> nbF;
    if (nbF < 0) {
        error_nb_filter(nbF);
        return (1);
    }

    char tmpChar[2];
    cin >> tmpChar;
    cout << tmpChar << endl;

    int size[2] = {0, 0};
    cin >> size[1] >> size[0];
    cout << size[1] << " " << size[0] << endl;

    int max = 0;
    cin >> max;
    cout << max << endl;

    short*** picture = new short**[size[0]];
    for (int i = 0; i < size[0]; i++) {
        picture[i] = new short*[size[1]];
        for (int j = 0; j < size[1]; j++) {
            picture[i][j] = new short[3];
        }
    }
    
    short** map = new short*[size[0]];
    for (int i = 0; i < size[0]; i++) {
        map[i] = new short[size[1]];
    }
    thresholding(size[0], size[1], nbR, max, colors_threshold, picture, map); //ATTENTION SEUIL MIN 

    for (int p = 0; p < nbF; p++) {
        filtering(size[0], size[1], nbR, map);
    }
    
    if (nbF > 0) {
        edge(size[0], size[1], map);
    } 
    writeData(size[0], size[1], colors_used, map);
    
    // deletePointers(picture,map,colors_used,colors_threshold, size, nbR);

    return (0);
}

void thresholding(int xSize, int ySize, int nbR, int max, double* colors_threshold,
                  short*** picture, short** map) {
    double tmpDouble = 0;
    int R, G, B = 0;
    bool notTheEnd = true;
    int k = 1;

    for (int i = 0; i < xSize; i++) {
        for (int j = 0; j < ySize; j++) {
            for (int k = 0; k < 3; k++) {
                cin >> picture[i][j][k];
            }
            R = picture[i][j][0];
            G = picture[i][j][1];
            B = picture[i][j][2];
            tmpDouble = sqrt(R * R + G * G + B * B) / (sqrt(3) * max);
            notTheEnd = true;
            k = 1;
            while (k <= nbR && notTheEnd) {
                if (tmpDouble >= colors_threshold[k - 1] &&
                    tmpDouble < colors_threshold[k]) {
                    map[i][j] = k;
                    notTheEnd = false;
                } else if (tmpDouble >= 1) {
                    map[i][j] = nbR;
                    notTheEnd = false;
                }
                k++;
            }
        }
    }
}

void filtering(int xSize, int ySize, int nbR, short** map) {
    int maxColorNb, maxColorValue = 0;
    int testColor[nbR + 1] = {0};
    int tmpMap[xSize][ySize] = {0};
    for (int xPos = 1; xPos < xSize - 1; xPos++) {
        for (int yPos = 1; yPos < ySize - 1; yPos++) {
            for (int i = 0; i < nbR + 1; i++) {
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
            for (int i = 0; i <= nbR; i++) {
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
    for (int i = 1; i < xSize - 1; i++) {
        for (int j = 1; j < ySize - 1; j++) {
            map[i][j] = tmpMap[i][j];
        }
    }
}

void edge(int xSize, int ySize, short** map) {
    for (int i = 0; i < xSize; i++) {
        map[i][0] = 0;
        map[i][ySize - 1] = 0;
    }
    for (int j = 1; j < ySize - 1; j++) {
        map[0][j] = 0;
        map[xSize - 1][j] = 0;
    }
}

void writeData(int xSize, int ySize, short** colors_used, short** map) {
    for (int i = 0; i < xSize; i++) {
        for (int j = 0; j < ySize; j++) {
            for (int k = 0; k < 3; k++) {
                cout << colors_used[map[i][j]][k] << " ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

void deletePointers(short*** picture,short** map, short** colors_used,double* colors_threshold, int size[2], int nbR){
    for (int i = 0; i < size[0]; i++) {
        for (int j = 0; j < size[1]; j++){
            delete [] picture[i][j];
        }
        delete [] picture [i];
        delete [] map[i];
    }
    delete [] picture;
    delete [] map;
    picture = NULL;
    map = NULL;

    for (int i = 0; i < nbR + 1; i++) {
        delete [] colors_used [i];
    }
    delete [] colors_used;
    delete [] colors_threshold;
    colors_used = NULL;
    colors_threshold = NULL;
}

void error_nbR(int nbR) { cout << "Invalid number of colors: " << nbR << endl; }

void error_color(int id) { cout << "Invalid color value " << id << endl; }

void error_threshold(double invalid_val) {
    cout << "Invalid threshold value: " << invalid_val << endl;
}

void error_nb_filter(int nb_filter) {
    cout << "Invalid number of filter: " << nb_filter << endl;
}
