#include<raylib.h>
#include"conway.h"
#include"menu.h"
#include<vector>
#include<iostream>

class Grid{
    private:
    int rows;
    int columns;
    int cellSize;
    std::vector<std::vector<int>>cells; 

public:
 
    Grid(int width,int height,int cellsize):  //initializer list to initialize the member variables
     rows(height/cellsize),columns(width/cellsize),cellSize(cellsize),cells(rows,std::vector<int>(columns,0)){} 
     //all cells are initialized as 0 i.e non living cell.
 

 void ToggleCell(int row,int column){
   if(IsWithinBounds(row,column)){
    cells[row][column]=!cells[row][column];
   }
 }
void Clear(){
    //iterates over the entire grid and initializes all the grid with value 0;
    for(int row=0;row<rows;row++){
        for(int column=0;column<columns;column++){
          cells[row][column]=0 ;
        }
    }
}
 int GetRows(){
    return rows;
 }
 
 int GetColumns(){
    return columns;
 }
 
 void Draw(){  //iterates over entire grid and draws according to the value of the each cell:
 for(int row=0;row<rows;row++){
    for(int column=0;column<columns;column++ ){
        Color color=cells[row][column]? Color{0,255,0,255}:Color{55,55,55,255};
        //for the colour of the grid or cell.
        DrawRectangle(column*cellSize,row*cellSize,cellSize-1,cellSize-1,color);
    }
 }
 }
 

 bool IsWithinBounds(int row,int column){
    if(row>=0 &&row<rows && column>=0 && column<columns){
    return true;
 }
 return false;
}
 
 int getValue(int row, int column){
    if(IsWithinBounds(row,column)){
        return cells[row][column];
    }
    return 0;
 }




 void SetValue(int row,int column,int value){
    if(IsWithinBounds(row,column)){
        cells[row][column]=value;
    }
    
 }
 

     void FillRandom(){
        //iterates over entire grid and puts random values for the below condition:
        for(int row=0;row<rows;row++){
            for(int column=0;column<columns;column++){
               int randomValue=GetRandomValue(0,4);
               cells[row][column]=(randomValue==4)?1:0 ;
            }
        }
     }
};

class Simulation{
    
    private:
    Grid grid;
    Grid tempGrid;
    bool run;
    public:
    
    Simulation(int width,int height,int cellSize):grid(width,height,cellSize),tempGrid(width,height,cellSize),run(false){
        
    }

void ClearGrid(){
    if(!IsRunning()){ //clear grid if the game is stopped
        grid.Clear();
    }
}
void ToggleCell(int row,int column){
    if(!IsRunning()){
        grid.ToggleCell(row,column); //toggle grid if the game is stopped
    }
}
void CreateRandomState(){  //create random state only if game is stopped
    if(!IsRunning()){
        grid.FillRandom();
    }
}
bool IsRunning(){return run;}
void Start(){
    run=true;
}
void Stop(){
    run=false;
}

int CountLiveNeighbours(int row,int column)
 {
    int liveNeighbors=0;
    std::vector<std::pair<int,int>>neighborOffsets={
        {-1,0}, //Directly above
        {1,0},  //Directly below
        {0,-1},  //to the left
        {0,1},    //to the right
        {-1,-1},//digonal upper left
        {-1,1},//diagonal upper right
        
        {1,-1},//diagonal lower left
        {1,1} //diagonal lower right
    };
  
  

   for(const auto& offset: neighborOffsets){ //auto automatically determines the type of element in the vector, and & signifies that the offset is reference to the 
    int neighborRow=(row+offset.first+grid.GetRows())%grid.GetRows();
    int neighborColumn=(column+offset.second+grid.GetColumns())%grid.GetColumns();
    liveNeighbors+=grid.getValue(neighborRow,neighborColumn); //gives the value for particular cell either 0 or 1// counts live 
   }
   return liveNeighbors;
 
}
void Update(){
    if(IsRunning())
    {
    for(int row=0;row<grid.GetRows();row++){
        for(int column=0;column<grid.GetColumns();column++){

            //iterating over entire grid and checking for the cellvalue and its corresponding no of live neighbors:
            int liveNeighbors=CountLiveNeighbours(row,column);
            int cellValue=grid.getValue(row,column);
            // all the modification is done on a tempGrid which is then assigned to the original grid after each iteration::
            //rule 1: if alive cell has more than 3 or less than 2 live neighbours it dies:
            if(cellValue==1){
                if(liveNeighbors>3|| liveNeighbors<2 ){
                    tempGrid.SetValue(row,column,0);
                }
                else{
                    tempGrid.SetValue(row,column,1);
                }
            }
            //rule2: if a dead cell has exactly 3 live neighbours then it becomes live cell:
            else{
                if(liveNeighbors==3){
                    tempGrid.SetValue(row,column,1);
                }
                else{
                    tempGrid.SetValue(row,column,0); //dead cell remaining dead:
                }
            }
        }
    }
    grid=tempGrid;
}
}


void Draw(){
    grid.Draw();
}

void SetCellValue(int row,int column, int value){
    grid.SetValue(row,column,value);
}






};

int conway(){
    Color GREY={29,29,29,255}; //custom color definition:
    const int WINDOW_WIDTH=1280;
    const int WINDOW_HEIGHT=800;
    const int CELL_SIZE=32;
    int FPS=12;
   Simulation simulation(WINDOW_WIDTH,WINDOW_HEIGHT,CELL_SIZE);
    
     
     



   // InitWindow(WINDOW_WIDTH,WINDOW_HEIGHT,"GAME OF LIFE");
    SetTargetFPS(FPS);
     //std::cout<<simulation.CountLiveNeighbours(5,31)<<std::endl;
    while(WindowShouldClose()==false){
        if (IsKeyPressed(KEY_Q)) {
            menu(); // Exit the loop when "q" is pressed
        }
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){  //for left click:
            Vector2 mousePosition=GetMousePosition(); //gets the mouse position in pixels:
            int row=mousePosition.y/CELL_SIZE;
            int column=mousePosition.x/CELL_SIZE;   //takes rows and columns with mouse clicK:
            simulation.ToggleCell(row,column); // toggles the mouse clicked cell.
        }
         if(IsKeyPressed(KEY_ENTER)){
            simulation.Start();  //starts for enter
            SetWindowTitle("game of life is running...");
         }
         else if(IsKeyPressed(KEY_SPACE)){
            simulation.Stop(); //stops for spacebar:
            SetWindowTitle("game of life has stopped..");
         }
         else if(IsKeyPressed(KEY_F)){
            FPS+=3;
            SetTargetFPS(FPS);
         }
         else if(IsKeyPressed(KEY_S)){
            if(FPS>5){
                FPS-=2;
                SetTargetFPS(FPS);
            }
         }
         else if(IsKeyPressed(KEY_C)){   
            simulation.ClearGrid();
         }
         else if(IsKeyPressed(KEY_R)){ // R to randomstate creation and mouse click to custom grid:
            simulation.CreateRandomState();
         }
    
        BeginDrawing();
         simulation.Update();
        ClearBackground(GREY);
      simulation.Draw();
        EndDrawing();

    }
    return 0;
}