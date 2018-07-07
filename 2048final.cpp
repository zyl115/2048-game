
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

struct board{
	vector<vector<int> > matrix; //a matrix is defined by forming a vector of vectors.

	string str(){
		stringstream ss;
		for(int i=0;i<matrix.size();i++){
			for(int j=0;j<matrix[i].size();j++){
				ss<<matrix[i][j]<<"\t";
			}
			ss<<endl;
		} 
		return ss.str();
	}

};

struct location{
	int row;
	int column;
};


void down(board & game);
void up (board & game);
void right(board& game);
void left(board& game);
void two_generator(board& game);
bool checkgameover(const board& game);

int main()
{
	
	string instruction;
	ifstream start;
	string filename;
	int temp;
	vector <int> column;
	vector <vector<int> > row;
	board game;
	bool end=false;
	cout<<"Key in the filename of a text file with 16 numbers to start a game of 2048:"<<endl;
	cin>>filename;
	start.open(filename.c_str());
	if(!start.is_open()){//Use default configuration if filename doesn't exist
		cout<<"File with filename "<<filename<<" does not exist. Game will start with default configurations:"<<endl;
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
				column.push_back(0);
			}
			row.push_back(column);
			column.clear();
		}
		row[3][3]=2;
	} else{//start with the matrix in file
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
				start>>temp;
				column.push_back(temp);
			}
			row.push_back(column);
			column.clear();
		}
	}
	game.matrix=row;
	cout<<game.str()<<endl;
	while(checkgameover(game)==false){
		
		board initial=game;
		cin>>instruction;
		if(instruction=="w"){
			up(game);
		} else if(instruction == "s"){
			down(game);
		} else if(instruction == "a"){
			left(game);
		} else if(instruction == "d"){
			right(game);
		} else{
			cout<<"Invalid instruction. Use only WASD!"<<endl;
		}
		if(!(initial.matrix==game.matrix)){	//only generate 2 and print new matrix if the matrix changes after operation	
			two_generator(game);
			cout<<game.str()<<endl;
		}
	}
	cout<<"GAME OVER"<<endl;//print game over if there is no possible move
	return 0;
}

void down(board & game){
	//move numbers downward
	for(int column=0;column<4;column++){
		for(int row=3;row>0;row--){
			if(game.matrix[row][column]==0){
				int zerorow=row;
				for(int aboverow=row;aboverow>=0;aboverow--){
					if(game.matrix[aboverow][column]!=0){
						game.matrix[zerorow][column]=game.matrix[aboverow][column];
						game.matrix[aboverow][column]=0;
						zerorow=zerorow-1;
					}
				}
			}	
		}
		//combining numbers
		for(int row=3;row>0;row--){
			if(game.matrix[row][column]==game.matrix[row-1][column]){
				game.matrix[row][column]=2*game.matrix[row][column];
				for(int samerow=row-1;samerow>0;samerow--){
					game.matrix[samerow][column]=game.matrix[samerow-1][column];
				}
				game.matrix[0][column]=0;
			}
		}
		
	}
}

void up(board & game){
	//move numbers upward
	for(int column=0;column<4;column++){
		for(int row=0;row<3;row++){
			if(game.matrix[row][column]==0){
				int zerorow=row;
				for(int belowrow=row;belowrow<4;belowrow++){
					if(game.matrix[belowrow][column]!=0){
						game.matrix[zerorow][column]=game.matrix[belowrow][column];
						game.matrix[belowrow][column]=0;
						zerorow=zerorow+1;
						
					}
				}
			}	
		}
		//combining numbers
		for(int row=0;row<3;row++){
			if(game.matrix[row][column]==game.matrix[row+1][column]){
				game.matrix[row][column]=2*game.matrix[row][column];
				for(int samerow=row+1;samerow<3;samerow++){
					game.matrix[samerow][column]=game.matrix[samerow+1][column];
				}
				game.matrix[3][column]=0;
				
			}
		}
		
	}
}


void right(board & game){
	//move numbers to the right
	for(int row=0;row<4;row++){
		for(int column=3;column>0;column--){
			if(game.matrix[row][column]==0){
				int zerocolumn=column;
				for(int leftcolumn=column;leftcolumn>=0;leftcolumn--){
					if(game.matrix[row][leftcolumn]!=0){
						game.matrix[row][zerocolumn]=game.matrix[row][leftcolumn];
						game.matrix[row][leftcolumn]=0;
						zerocolumn=zerocolumn-1;
						
					}
				}
			}	
		}
		//combining numbers
		for(int column=3;column>0;column--){
			if(game.matrix[row][column]==game.matrix[row][column-1]){
				game.matrix[row][column]=2*game.matrix[row][column];
				for(int samecolumn=column-1;samecolumn>0;samecolumn--){
					game.matrix[row][samecolumn]=game.matrix[row][samecolumn-1];
				}
				game.matrix[row][0]=0;
				
			}
		}
		
	}
}


void left(board & game){
	//move numbers upward
	for(int row=0;row<4;row++){
		for(int column=0;column<3;column++){
			if(game.matrix[row][column]==0){
				int zerocolumn=column;
				for(int rightcolumn=column;rightcolumn<4;rightcolumn++){
					if(game.matrix[row][rightcolumn]!=0){
						game.matrix[row][zerocolumn]=game.matrix[row][rightcolumn];
						game.matrix[row][rightcolumn]=0;
						zerocolumn=zerocolumn+1;
				
					}
				}
			}	
		}
		//combining numbers
		for(int column=0;column<3;column++){
			if(game.matrix[row][column]==game.matrix[row][column+1]){
				game.matrix[row][column]=2*game.matrix[row][column];
				for(int samecolumn=column+1;samecolumn<3;samecolumn++){
					game.matrix[row][samecolumn]=game.matrix[row][samecolumn+1];
				}
				game.matrix[row][3]=0;
				
			}
		}
		
	}
}


void two_generator(board& game){ //generates a two at a random zero location
	location temp;
	vector <location> emptysquares;
	for(int column=0;column<4;column++){//record the location of zeros and store them in a vector
		for(int row=0;row<4;row++){
			if(game.matrix[row][column]==0){
				temp.row=row;
				temp.column=column;
				emptysquares.push_back(temp);
			}
		}
	}
	
	if(emptysquares.size()!=0){ //generate "2" at a random location in the vector
		srand (time(0));
		int random_index=rand()%emptysquares.size();
		location two_loct=emptysquares[random_index];
		game.matrix[two_loct.row][two_loct.column]=2;
	}
}

bool checkgameover(const board& game){//to check for possible moves. if there is no possible moves, then game over
	for(int column=0;column<4;column++){
		for(int row=0;row<4;row++){
			if(game.matrix[row][column]==0){ //if there is a zero, then game is not over. return false.
				return false;
			}
		}
	}
	for(int column=0;column<4;column++){
		for(int row=0;row<3;row++){
			if(game.matrix[row][column]==game.matrix[row+1][column]){
				return false;//if there are two adjacent numbers (vertically) that are identical, then numbers can be combined. Game is not over.				
			}
		}
	}
	for(int row=0;row<4;row++){
		for(int column=0;column<3;column++){
			if(game.matrix[row][column]==game.matrix[row][column+1]){
				return false;//if there are two adjacent numbers (horizontally) that are identical, then numbers can be combined. Game is not over.
			}
		}
	}

	return true;
}

