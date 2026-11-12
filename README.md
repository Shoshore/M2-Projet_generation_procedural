# Author
* Ryan Rodrigues
* Clément Potier

# Aim of the project
Our aim is to re-implement the paper **Orometry-based Terrain Analysis and Synthesis**, published in November 2019 and made by: OSCAR ARGUDO, ERIC GALIN, ADRIEN PEYTAVIE, AXEL PARIS, JAMES GAIN, ERIC GUÉRIN.

We are going to implement it using C++ and OpenGL.

# Data
The data folder and all it's content come from: [orometry-terrains](https://github.com/oargudo/orometry-terrains)

## Running data processing
- Python version used: 3.15.2
- You will have to download the **requirements.txt** packages and be able to run Jupyter Notebooks.
- Then, in the folder **data/raw_data/regionShapes**, download: [Download Andrew Kirmse's results](https://drive.google.com/file/d/0B3icWNhBosDXRm1pak56blp1RGc/view?usp=sharing)
If the filename isn't **alliso-sorted.txt**, rename it.

- Then, in the folder **data/raw_data/regionShapes**, download: [Download Andrew Kirmse's results](https://drive.google.com/file/d/0B3icWNhBosDXZmlEWldSLWVGOE0/view?usp=sharing&resourcekey=0-TZC_OGOqI5TFdfPE77Yl3g)
If the filename isn't **prominence-p100.txt**, rename it.

- Now you can run **Analysis.ipynb**.