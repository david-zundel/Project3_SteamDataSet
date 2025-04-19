# Steam Game Dataset Analyzer
This project is a terminal based application for analyzing a dataset of Steam games. It compares two datasets: hashmaps and red black trees.

## Features
- Search for a game by name or App ID.
- Find the most expensive or cheapest game.
- Filter games by required age or estimated owners.
- Sort games by price or release date(only a selected few). 
- Dispay the dataset's statistics. 

## Dataset
The dataset used in this project is the Steam Game Dataset, which contains information about various games available on the Steam platform. The dataset includes details such as game name, App ID, price, release date, required age, estimated owners, and more.

### How to Obtain the Dataset
1. Download the dataset from [Steam Game Dataset on Kaggle](https://www.kaggle.com/datasets/fronkongames/steam-games-dataset).
2. Create a `dataset` directory in the root of the project: mkdir dataset
2. Extract the dataset and locate the `games.csv` file.
3. Place the `games.csv` file in the `dataset/` directory of the project.

### Directory Structure
After placing the dataset, your project directory should look like this: 
Project3_SteamDataSet/
├── dataset/
│   └── games.csv
├── src/
│   ├── main.cpp
│   ├── Game.cpp
│   └── Game.h
├── README.md


## How to run
1. Clone the repository to your local machine. Link: https://github.com/david-zundel/Project3_SteamDataSet.git
2. Navigate to the project directory: cd Project3_SteamDataSet
3. Compile the program using the command: g++ -std=c++17 -o GameAnalyzer src/main.cpp src/Hash.cpp src/Game.cpp
4. Run the program using the command: ./GameAnalyzer
5. Follow the on-screen instructions to interact with the application.

## Credits
This project was developed by the following team members for the course COP3530 at the University of Florida:
- David Zundel: [Github](https://github.com/david-zundel)
- Zachary Fisher: [Github](https://github.com/ZachFisher8)
- Jeremiah Chitwood: [Github](https://github.com/Silverbullet69)