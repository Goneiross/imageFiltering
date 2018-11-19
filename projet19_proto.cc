/** \author PELLERIN Guillaume
 *  \date	dec 2018
 */
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>

using namespace std;

void error_nbR(int nbR);
void error_color(int id);
void error_threshold(double invalid_val);
void error_nb_filter(int nb_filter);

string readLine(string file, uint16_t line) {
  ifstream flux(file, ios::in);
  string data;
  for (uint16_t i = 0; i < line; i++) {
    getline(flux, data);
  }
  flux.close();
  return (data);
}

void readDataLineDouble(string file, uint16_t line, uint16_t startPosition,
                        uint16_t column, double data[]) {
  ifstream flux(file, ios::in);
  string tmpData;
  string tmpData2;
  uint16_t dataM = startPosition;
  bool lastWasSpace = false;
  for (uint16_t i = 0; i < line; i++) {
    getline(flux, tmpData);
  }
  for (uint16_t i = 0; i < tmpData.size(); i++) {
    if (tmpData[i] == ' ') {
      if (lastWasSpace == false) {
        data[dataM] = stod(tmpData2);
        tmpData2.clear();
        dataM++;
        lastWasSpace = true;
      }
    } else if (tmpData[i] == '\n') {
      data[dataM] = stod(tmpData2);
      lastWasSpace = true;
      // BREAK
    } else {
      lastWasSpace = false;
      tmpData2 = tmpData2 + tmpData[i];
    }
  }
  flux.close();
}

void readDataLineInt(string file, uint16_t line, uint16_t startPosition,
                     uint16_t column, unsigned int data[][3]) {
  ifstream flux(file, ios::in);
  string tmpData;
  string tmpData2;
  uint16_t lastPosition = 0;
  uint16_t dataM = startPosition;
  uint16_t dataN = 0;
  bool lastWasSpace = false;

  for (uint16_t i = 0; i < line; i++) {
    getline(flux, tmpData);
  }
  for (uint16_t i = 0; i < tmpData.size(); i++) {
    if (tmpData[i] == ' ') {
      if (dataN < column) {
        if (lastWasSpace == false) {
          data[dataM][dataN] = (unsigned int)stoi(tmpData2);
          tmpData2.clear();
          dataN++;
          lastWasSpace = true;
        }
      } else

      {
        if (lastWasSpace == false) {
          data[dataM][dataN] = (unsigned int)stoi(tmpData2);
          tmpData2.clear();
          dataN = 0;
          dataM++;
          lastWasSpace = true;
        }
      }
    } else if (tmpData[i] == '\n') {
      data[dataM][dataN] = (unsigned int)stoi(tmpData2);
      // BREAK
    } else {
      lastWasSpace = false;
      tmpData2 = tmpData2 + tmpData[i];
    }
  }

  flux.close();
}

int **thresholding(string file, double colors_threshold[], uint8_t nbR) {
  cout << "Parsing" << endl;
  ifstream flux(file, ios::in);

  string data = {};
  string dataTmp = {};
  uint16_t size[2];

  do {
    getline(flux, data);
  } while (data != "P3");

  getline(flux, data);
  uint8_t j = 0;
  data += ' ';
  bool lastWasSpace = false;
  for (uint8_t i = 0; i < data.size(); i++) {
    if ((data[i] != ' ') && (data[i] != '\n')) {
      dataTmp += data[i];
      lastWasSpace = false;
    } else if (lastWasSpace) {
    } else {
      size[j] = stoi(dataTmp);
      lastWasSpace = true;
      dataTmp = {};
      j++;
    }
  }

  getline(flux, data);
  int max = stoi(data);
  int R = 0;
  int G = 0;
  int B = 0;

  int arrayTmp[size[0] * 3] = {};

  getline(flux, data);

  double tmpDouble = 0;
  int **thresholded = new int*[size[1]];
  for(int i = 0; i< size[1]; i++){
    thresholded[i]=new int[size[0]];
  }

  for (uint16_t i = 0; i < size[1]; i++) {

    uint8_t x = 0;
    data += ' ';
    bool lastWasSpace = false;
    for (uint8_t k = 0; k < data.size(); k++) {
      if ((data[k] != ' ') && (data[k] != '\n')) {
        dataTmp += data[k];
        lastWasSpace = false;
      } else if (lastWasSpace) {
      } else {
        arrayTmp[x] = stoi(dataTmp);
        lastWasSpace = true;
        dataTmp = {};
        x++;
      }
    }

    for (int j = 0; j < size[1]; j++) {
      R = arrayTmp[3 * j];
      G = arrayTmp[3 * j + 1];
      B = arrayTmp[3 * j + 2];
      tmpDouble = sqrt(R * R + G * G + B * B) / (sqrt(3) * max);
      for (int k = 0; k < nbR; k++) {
        if (tmpDouble >= colors_threshold[k] && tmpDouble <= colors_threshold[k + 1]) {
          thresholded[i][j] = k + 1;
          k = nbR;
        }
      }
    }
    getline(flux, data);
  }
  cout << "Parsing done" << endl;
  return (thresholded);
}

int main() {

  string file = "tests/elementary/test1.txt";

  unsigned int nbR = (unsigned int)stoi(readLine(file, 1));
  if (nbR < 2) {
    error_nbR(nbR);
  }
  cout << "nbR " << +nbR << endl;
  unsigned int colors_used[nbR + 1][(unsigned int)3];
  cout << "colors_used ";
  for (int i = 0; i < 3; i++) {
    colors_used[0][i] = 0;
  }
  readDataLineInt(file, 2, 1, nbR, colors_used);
  for (int i = 0; i < nbR + 1; i++) {
    for (int j = 0; j < 3; j++) {
      cout << colors_used[i][j] << " ";
    }
    cout << "   ";
  }

  double colors_threshold[nbR + 1];
  colors_threshold[0] = 0;
  colors_threshold[nbR] = 1;
  readDataLineDouble(file, 3, 1, nbR, colors_threshold);
  int **thresholded = thresholding(file, colors_threshold, nbR);

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      cout << thresholded[i][j] << " ";
    }
    cout << endl;
  }

  return 0;
}

void error_nbR(int nbR) { cout << "Invalid number of colors: " << nbR << endl; }

void error_color(int id) { cout << "Invalid color value " << id << endl; }

void error_threshold(double invalid_val) {
  cout << "Invalid threshold value: " << invalid_val << endl;
}

void error_nb_filter(int nb_filter) {
  cout << "Invalid number of filter: " << nb_filter << endl;
}
