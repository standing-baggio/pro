
 #include<iostream>
 #include<raylib.h>
 #include "ping.h"
 #include "menu.h"
 using namespace std;
 int player_score=0;  //tracks the prayer_score and cpu_score
 int cpu_score=0;
 class Ball{
  public:
    float x,y;
    int speed_x,speed_y;
    int radius;
 
 void Draw(){
    DrawCircle(x,y,radius,WHITE); //inbuilt raylib function which draws a circle /ball:
 }
 void Update(){ // for the movement of ball, collision detection, point update 
    x+=speed_x;
    y+=speed_y;
    if(y+radius>=GetScreenHeight()||y-radius<=0){ //condition to make the ball bounce in y direction:

      speed_y*=-1;
  }
    if(x+radius>=GetScreenWidth()){
      cpu_score++;
      ResetBall();
  }
    if(x-radius<=0){
      player_score++;
      ResetBall();
  }
 }
 void ResetBall(){
    x=GetScreenWidth()/2;
    y=GetScreenHeight()/2;
    int speed_choices[2]={-1,1};   //array with two values.
    speed_x*=speed_choices[GetRandomValue(0,1)];  //generates -ve or +ve speed depending upon the random index generated i.e 0 or 1.
    speed_y*=speed_choices[GetRandomValue(0,1)];

 }

 };

 class Paddle{
  protected: void limitMovement(){
  if(y<=0){
      y=0;    //won't let padddle go further up
    }
    if(y+height>=GetScreenHeight()){
      y=GetScreenHeight()-height;//won't let paddle go further down
    }
  }

  public:
  float x,y; //position of the paddle.
  float width,height;
  int speed;
  void Draw(){
    DrawRectangle(x,y,width,height,WHITE);

  }
  void Update(){ //keeps track of the paddle movement
    if(IsKeyDown(KEY_UP)){ //IsKeyDown is a inbuilt raylib method.
      y=y-speed;
    }
    if(IsKeyDown(KEY_DOWN)){
      y=y+speed;
    }
    limitMovement();// restricts paddle movement within the window:
  }
 };

 class CpuPaddle:public Paddle{
  public:
  void Update(int ball_y){
    if(y+height/2>ball_y){  //for middle of the paddle lower than the y_ball, paddle up
      y=y-speed;
    }
    if(y+height/2<=ball_y) //for middle of the paddle higher than the y_ball, paddle down.

    {
      y=y+speed;
    }
    limitMovement();
  }
 };
 CpuPaddle cpu;
 Ball ball;
 int i;
 Paddle player;
 void ResetGame() {
    player_score = 0;
    cpu_score = 0;

    ball.ResetBall();

    player.x = 1280 - player.width - 10;
    player.y = 800 / 2 - player.height / 2;

    cpu.x = 10;
    cpu.y = 800 / 2 - cpu.height / 2;
}



 
 int ping(){
    cout<<"Starting the game"<<endl;
    const int screen_width=1280;
    const int screen_height=800;
  //  InitWindow(screen_width,screen_height,"My Pong Game");
   SetTargetFPS(100);
   ball.radius=20;
   ball.x=screen_height/2, //initial ball position
   ball.y=screen_height/2;
   ball.speed_x=7;
   ball.speed_y=7;
   player.width=25;  //player paddle width and height.
   player.height=120;
   player.x=screen_width-player.width-10;
   player.y=screen_height/2-player.height/2;
   player.speed=6;
   cpu.height=120;
   cpu.width=25;
   cpu.x=10;
   cpu.y=screen_height/2-cpu.height/2;
   cpu.speed=6;

   while(WindowShouldClose()==false){
   BeginDrawing();
    if (IsKeyPressed(KEY_Q)) {
      menu(); // Exit the loop when "q" is pressed
    }
     if (player_score >3 || cpu_score > 3) {
            ResetGame();
        }
   //Updating;
   ball.Update();  //ball movement, point update, reset game:
   player.Update(); // paddle movement of user
   cpu.Update(ball.y); //paddle movement of cpu
   //checking for collision of ball with either cpu or player paddle with inbuilt raylib function: 

   // we 've to input center of the circle, its radius and the dimension and position of the rect/paddle:

   if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{player.x,player.y,player.width,player.height})){
    ball.speed_x*=-1; // throws the ball in reverse x direction.
   }
   if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{cpu.x,cpu.y,cpu.width,cpu.height})){
    ball.speed_x*=-1;
   }

   //drawing:
   ClearBackground(BLACK);
  ball.Draw();
  cpu.Draw();
  
  
  DrawLine(screen_width/2,0,screen_width/2,screen_height,WHITE); //starting position(x,y) , endposition(x,y) and color of line:
  player.Draw();
  DrawText(TextFormat("%i",cpu_score),screen_width/4-20,35,35,WHITE); //x position, yposition, font size, color
  DrawText(TextFormat("%i",player_score),3*screen_width/4-20,35,35,WHITE);
  DrawText("CPU", screen_width / 4 - 30, 0, 40, WHITE);  // Position text above CPU score
  DrawText("Player", 3 * screen_width / 4 - 55, 0, 40, WHITE);  // Position text above player score

   EndDrawing();

   }
   CloseWindow();
   return 0;
 }