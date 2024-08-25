#include <raylib.h>
#include <iostream>
#include "snake.h"
#include<deque>
#include<raymath.h>
#include "menu.h"
using namespace std;

int cellSize = 32;
int cellCountX=1280/cellSize;
int cellCountY=800/cellSize;
double lastUpdateTime=0;
bool ElementInDeque(Vector2 element,deque<Vector2>deque){  //checks if there is a searched vector in the deque.
    for(unsigned int i=0;i<deque.size();i++)                //global function.
    {
        if(Vector2Equals(deque[i],element)){
            return true;
        }

    }
    return false;
}
bool eventTriggered(double interval){
    double currentTime=GetTime();
    if(currentTime-lastUpdateTime>=interval){
        lastUpdateTime=currentTime;
        return true;   // returns true after certain interval has passed.
    }
    return false;
}
class Snake{
    public:
    deque<Vector2> body={Vector2{6,9},Vector2{5,9},Vector2{4,9}};
    Vector2 direction={1,0};
    bool addSegment=false;   //initially false
    void Draw(){
// Corrected loop
for (std::deque<Vector2>::size_type i = 0; i < body.size(); ++i) {
    // Your code here


          float x= body[i].x;
          float y= body[i].y;
          Rectangle segment= Rectangle{x*cellSize,y*cellSize,(float)cellSize,(float)cellSize};
          DrawRectangleRounded(segment,0.5,6,DARKGREEN);
        }
    }
    void Update(){
        
        body.push_front(Vector2Add(body[0],direction)); //moves the snake by continuosly pushing new vector and popping last vector till it eats the food.
        if(addSegment==true){
            addSegment=false;
        }
        else{
            body.pop_back();
        }
    }
    void Reset(){
        body={Vector2{6,9},Vector2{5,9},Vector2{4,9}};
        direction={1,0};
    }

};


class Food {

public:
    Texture2D texture; //raylib data type that stores image data for drawing
    Vector2 position; //stores position of the food.
    
    

    Food(deque<Vector2>snakeBody) {
        cout << "Loading image..." << endl;
        Image image = LoadImage("/home/sumin/Downloads/food.png"); //leads the image from the file to the structure Image.
        if (image.data == nullptr) {
            cerr << "Failed to load image!" << endl;
            return;
        }
        cout << "Image loaded successfully" << endl;
        texture = LoadTextureFromImage(image);  //converts the image into texture2d data type which can be displayed on screen.
        UnloadImage(image);
        position = GenerateRandomPos(snakeBody);
    }

    Vector2 GenerateRandomCell(){
        float x = GetRandomValue(0, cellCountX - 1);
        float y = GetRandomValue(0, cellCountY - 1);
        return Vector2{x,y};
    }
    Vector2 GenerateRandomPos(deque<Vector2>snakeBody) {
        Vector2 position=GenerateRandomCell();
        
        while(ElementInDeque(position,snakeBody)){  
        position=GenerateRandomCell();     //for every match in food and snake body position a new random cell position is generated.
        }
        return position; // Corrected initialization of Vector2
    }

    ~Food() {
        UnloadTexture(texture);
    }

    void Draw() {
        DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
    }
};


class Game{
    public:
    Snake snake=Snake();
    Food food=Food(snake.body); //passed body member variable of the object snake to the food object.
    bool running=true;
    int score=0;
    void GameOver(){
        snake.Reset();
        food.position=food.GenerateRandomPos(snake.body);
        running =false;
        score=0;
    }


    void CheckCollisionWithFood(){
        if(Vector2Equals(snake.body[0],food.position)){
            food.position=food.GenerateRandomPos(snake.body);
            snake.addSegment=true;
            score+=5;
        }
    }
    void CheckCollisionWithEdges(){
        if(snake.body[0].x==cellCountX||snake.body[0].x==-1)
        {
            GameOver();
        }
        if(snake.body[0].y==cellCountY||snake.body[0].y==-1)
        {
            GameOver();
        }
    }
    
  void CheckCollisionwithTail(){
    deque<Vector2>headlessBody=snake.body; //copies the snakebody to other deque
    headlessBody.pop_front(); //removes head
    if(ElementInDeque(snake.body[0],headlessBody)){
        GameOver();
    }


  }
  void DrawScore(){
    DrawText(TextFormat("Score:%04i",score),10,10,20,BLACK);
  }

    void Draw(){
        food.Draw();
        snake.Draw();
        DrawScore();
    }
    void Update(){
        if(running){
        snake.Update();
        CheckCollisionWithFood();
        CheckCollisionWithEdges();
        CheckCollisionwithTail();

    }
    }
    

    
    
  
};
int snake() {
    cout << "Starting the game.." << endl;//InitWindow(cellSize * cellCount, cellSize * cellCount, "Metro Snake");
    // InitWindow(cellSize * cellCount, cellSize * cellCount, "Metro Snake");
    SetTargetFPS(60);

    Game game=Game(); // only game initialization is enough because food and snake are initialized in the game class.
    if (game.food.texture.id == 0) {
        cerr << "Failed to load texture!" << endl;
        CloseWindow();
        return -1;
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        game.Draw();
        if (IsKeyPressed(KEY_Q)) {
            menu(); // Exit the loop when "q" is pressed
        }
        if(eventTriggered(0.2)){
            game.Update();
        }   

        //snake's direction of movement controlled by  the keys
        if(IsKeyPressed(KEY_UP) &&game.snake.direction.y!=1){
            game.snake.direction={0,-1};    // if snake is going down we can't make it go up with a flip:
            game.running=true;

        }
        if(IsKeyPressed(KEY_DOWN) &&game.snake.direction.y!=-1){
            game.snake.direction={0,1};
            game.running=true;

        }
        if(IsKeyPressed(KEY_LEFT) &&game.snake.direction.x!=1){
          game.snake.direction={-1,0};
          game.running=true;
        }
        if(IsKeyPressed(KEY_RIGHT) &&game.snake.direction.x!=-1){
            game.snake.direction={1,0};
            game.running=true;
        }

        ClearBackground(PURPLE);
      
        
        EndDrawing();
    
    }
    CloseWindow();
    return 0; 
}
