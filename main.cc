#include <cstdint>
#include <iostream>
#include <math.h>

using namespace std;

void error_nbR(int nbR);
void error_color(int id);
void error_threshold(double invalid_val);
void error_nb_filter(int nb_filter);
short parseNbR();
void parseColorsUsed(short** colorsUsed, short nbR);
void parseThreshold(double* Threshold, int nbR);
int parseNbF();
void thresholding(int xSize, int ySize, short nbR, int max, double* Threshold,
                  short** map);
void filtering(int xSize, int ySize, short nbR, short** map);
void edge(int xSize, int ySize, short** map);
void writeData(int xSize, int ySize, short** colorsUsed, short** map);
void deletePointers(short** map, short** colorsUsed, double* Threshold, int size[2],
                    short nbR);

int main() {
    short nbR = parseNbR();
    short** colorsUsed = new short*[nbR + 1];
    for (int i = 0; i < nbR + 1; i++) {
        colorsUsed[i] = new short[3];
    }
    parseColorsUsed(colorsUsed, nbR);
    double* Threshold = new double[nbR + 1];
    parseThreshold(Threshold, nbR);
    int nbF = parseNbF();
    char tmpChar[2];
    cin >> tmpChar;
    cout << tmpChar << endl;
    int size[2] = {0, 0};
    cin >> size[1] >> size[0];
    cout << size[1] << " " << size[0] << endl;
    int max = 0;
    cin >> max;
    cout << max << endl;
    short** map = new short*[size[0]];
    for (int i = 0; i < size[0]; i++) {
        map[i] = new short[size[1]];
    }
    thresholding(size[0], size[1], nbR, max, Threshold, map);
    for (int p = 0; p < nbF; p++) {
        filtering(size[0], size[1], nbR, map);
    }
    if (nbF > 0) {
        edge(size[0], size[1], map);
    }
    writeData(size[0], size[1], colorsUsed, map);
    deletePointers(map, colorsUsed, Threshold, size, nbR);
    return (0);
}

short parseNbR() {
    short nbR = 0;
    cin >> nbR;
    if (nbR < 2 || nbR > 255) {
        error_nbR(nbR);
        exit(0);
    }
    return (nbR);
}

void parseColorsUsed(short** colorsUsed, short nbR) {
    for (int i = 1; i < nbR + 1; i++) {
        for (int j = 0; j < 3; j++) {
            cin >> colorsUsed[i][j];
            if (colorsUsed[i][j] > 255 || colorsUsed[i][j] < 0) {
                error_color(i);
                exit(0);
            }
        }
    }
}

void parseThreshold(double* Threshold, int nbR) {
    Threshold[0] = 0;
    Threshold[nbR] = 1;
    for (int i = 1; i < nbR; i++) {
        cin >> Threshold[i];
        if (Threshold[i - 1] >= Threshold[i]) {
            error_threshold(Threshold[i]);
            exit(0);
        }
    }
}

int parseNbF() {
    int nbF = 0;
    cin >> nbF;
    if (nbF < 0) {
        error_nb_filter(nbF);
        exit(0);
    }
    return (nbF);
}

void thresholding(int xSize, int ySize, short nbR, int max, double* Threshold,
                  short** map) {
    double tmpDouble = 0;
    int R, G, B = 0;
    bool notTheEnd = true;
    int k = 1;

    for (int i = 0; i < xSize; i++) {
        for (int j = 0; j < ySize; j++) {
            cin >> R;
            cin >> G;
            cin >> B;
            tmpDouble = sqrt(R * R + G * G + B * B) / (sqrt(3) * max);
            notTheEnd = true;
            k = 1;
            while (k <= nbR && notTheEnd) {
                if (tmpDouble >= Threshold[k - 1] && tmpDouble < Threshold[k]) {
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

void filtering(int xSize, int ySize, short nbR, short** map) {
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

void writeData(int xSize, int ySize, short** colorsUsed, short** map) {
    for (int i = 0; i < xSize; i++) {
        for (int j = 0; j < ySize; j++) {
            for (int k = 0; k < 3; k++) {
                cout << colorsUsed[map[i][j]][k] << " ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

void deletePointers(short** map, short** colorsUsed, double* Threshold, int size[2],
                    short nbR) {
    for (int i = 0; i < size[0]; i++) {
        delete[] map[i];
    }
    delete[] map;
    map = NULL;

    for (int i = 0; i < nbR + 1; i++) {
        delete[] colorsUsed[i];
    }
    delete[] colorsUsed;
    delete[] Threshold;
    colorsUsed = NULL;
    Threshold = NULL;
}

void error_nbR(int nbR) { 
    cout << "Invalid number of colors: " << nbR << endl; }

void error_color(int id) { 
    cout << "Invalid color value " << id << endl; }

void error_threshold(double invalid_val) {
    cout << "Invalid threshold value: " << invalid_val << endl;
}

void error_nb_filter(int nb_filter) {
    cout << "Invalid number of filter: " << nb_filter << endl;
}
