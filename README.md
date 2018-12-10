# coloreduce

## Description

First year EPFL project.

The goal of Coloreduce is to reduce the number of colors in a picture, depending on the user inputs.

## How to use it

Install imageMagick

Then use :

$ ./projet19_proto<./tests/advanced/tree.txt>output.txt

$ convert -compress none output.txt output.ppm

$ display output.ppm