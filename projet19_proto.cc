/** \author PELLERIN Guillaume
 *  \date	dec 2018
 */
#include <fstream>
#include <iostream>
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

string readData(string file, uint16_t line) {
  ifstream flux(file, ios::in);
  string data;
  uint16_t endOfLine = 0;
  char tmpRead;
  char tmpNumber[3] = {'0', '0', '0'};
  uint8_t tmpNumberPosition = 2;
  uint8_t dataPosition = 0;

  while (endOfLine < line) {
    flux.get(tmpRead);
    if (tmpRead == '\n') {
      endOfLine++;
      dataPosition++;
      data[dataPosition] = tmpNumber[0] + tmpNumber[1] + tmpNumber[2];
      tmpNumberPosition = 2;
      tmpNumber[0] = 0;
      tmpNumber[1] = 0;
      tmpNumber[2] = 0;
    } else if (tmpRead == 'P') {
      flux.get(tmpRead);
    } else {
      tmpNumber[tmpNumberPosition] = tmpRead;
    }
  }
  cout << data[1] << endl;
  flux.close();
  return (data);
}

int main() {

  string file = "tests/elementary/E01.txt";

  unsigned int colors_number = (unsigned int)stoi(readLine(file, 1));
  unsigned int colors_used[colors_number][(unsigned int)2];
  double colors_threshold[colors_number];
  for (int i = 0; i < 3; i++) {
    colors_used[0][i] = 0;
  }
  for (int i = 0; i < colors_number; i++) {
  }
  colors_threshold[0] = 0;
  colors_threshold[colors_number] = 1;

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
