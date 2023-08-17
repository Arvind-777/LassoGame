#include <simplecpp>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace simplecpp;

//movingObject.h
#ifndef _MOVINGOBJECT_INCLUDED_
#define _MOVINGOBJECT_INCLUDED_

#include <simplecpp>
#include <vector>
#include <composite.h>
#include <sprite.h>

using namespace simplecpp;

class MovingObject : public Sprite {
  vector<Sprite*> parts;
  double vx, vy;
  public:
  double ax, ay;
  bool paused;
  void initMO(double argvx, double argvy, double argax, double argay, bool argpaused=true) {
    vx=argvx; vy=argvy; ax=argax; ay=argay; paused=argpaused;
  }

 MovingObject(double argvx, double argvy, double argax, double argay, bool argpaused=true)
    : Sprite() {
    initMO(argvx, argvy, argax, argay, argpaused);
  }
 MovingObject(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : Sprite() {
   double angle_rad = angle_deg*PI/180.0;
   double argvx = speed*cos(angle_rad);
   double argvy = -speed*sin(angle_rad);
   initMO(argvx, argvy, argax, argay, argpaused);
  }
  void set_vx(double argvx) { vx = argvx; }
  void set_vy(double argvy) { vy = argvy; }
  void set_ax(double argax) { ax = argax; }
  void set_ay(double argay) { ay = argay; }
  double getXPos();
  double getYPos();
  void reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta);

  void pause() { paused = true; }
  void unpause() { paused = false; }
  bool isPaused() { return paused; }

  void addPart(Sprite* p) {
    parts.push_back(p);
  }
  void nextStep(double t);
  void getAttachedTo(MovingObject *m);
};

#endif

//MovingObject.cpp

void MovingObject::nextStep(double t) {
  if(paused) { return; }
  //cerr << "x=" << getXPos() << ",y=" << getYPos() << endl;
  //cerr << "vx=" << vx << ",vy=" << vy << endl;
  //cerr << "ax=" << ax << ",ay=" << ay << endl;

  for(size_t i=0; i<parts.size(); i++){
    parts[i]->move(vx*t, vy*t);
  }
  vx += ax*t;
  vy += ay*t;
} // End MovingObject::nextStep()

double MovingObject::getXPos() {
  return (parts.size() > 0) ? parts[0]->getX() : -1;
}

double MovingObject::getYPos() {
  return (parts.size() > 0) ? parts[0]->getY() : -1;
}

void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) {
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(argx, argy);
  }
  double angle_rad = angle_deg*PI/180.0;
  double argvx = speed*cos(angle_rad);
  double argvy = -speed*sin(angle_rad);
  vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;
} // End MovingObject::reset_all()

void MovingObject::getAttachedTo(MovingObject *m) {
  double xpos = m->getXPos();
  double ypos = m->getYPos();
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(xpos, ypos);
  }
  initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
}

//coin.h
#ifndef __COIN_H__
#define __COIN_H__



class Coin : public MovingObject {
  double coin_start_x;
  double coin_start_y;
  double release_speed;
  double release_angle_deg;
  double coin_ax;
  double coin_ay;

  // Moving parts
  Circle coin_circle;

 public:
    Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    initCoin();
  }

  void initCoin();
  void resetCoin();

}; // End class Coin

#endif

//lasso.h
#ifndef __LASSO_H__
#define __LASSO_H__

//#define WINDOW_X 1200
//#define WINDOW_Y 960
#define WINDOW_X 800
#define WINDOW_Y 600

#define STEP_TIME 0.05

#define PLAY_X_START 100
#define PLAY_Y_START 0
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-100)

#define LASSO_X_OFFSET 100
#define LASSO_Y_HEIGHT 100
#define LASSO_BAND_LENGTH LASSO_X_OFFSET
#define LASSO_THICKNESS 5

#define COIN_GAP 1

#define RELEASE_ANGLE_STEP_DEG 5
#define MIN_RELEASE_ANGLE_DEG 0
#define MAX_RELEASE_ANGLE_DEG (360-RELEASE_ANGLE_STEP_DEG)
#define INIT_RELEASE_ANGLE_DEG 45

#define RELEASE_SPEED_STEP 20
#define MIN_RELEASE_SPEED 0
#define MAX_RELEASE_SPEED 200
#define INIT_RELEASE_SPEED 100

#define COIN_SPEED 120
#define COIN_ANGLE_DEG 90

#define LASSO_G 30
#define COIN_G 30

#define LASSO_SIZE 10
#define LASSO_RADIUS 50
#define COIN_SIZE 5

class Lasso : public MovingObject {
  double lasso_start_x;
  double lasso_start_y;
  double release_speed;
  double release_angle_deg;
  double lasso_ax;
  double lasso_ay;

  // Moving parts
  Circle lasso_circle;
  Circle lasso_loop;

  // Non-moving parts
  Line lasso_line;
  Line lasso_band;

  // State info
  bool lasso_looped;
  Coin *the_coin;
  int num_coins;

  void initLasso();
 public:
 Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    lasso_ax = argax;
    lasso_ay = argay;
    initLasso();
  }

  void draw_lasso_band();
  void yank();
  void loopit();
  void addAngle(double angle_deg);
  void addSpeed(double speed);

  void nextStep(double t);
  void check_for_coin(Coin *coin);
  int getNumCoins() { return num_coins; }

}; // End class Lasso

#endif

//coin.h

void Coin::initCoin() {
  coin_start_x = (PLAY_X_START+WINDOW_X)/2;
  coin_start_y = PLAY_Y_HEIGHT;
  coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
  coin_circle.setColor(COLOR("yellow"));
  coin_circle.setFill(true);
  addPart(&coin_circle);
}

void Coin::resetCoin() {
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}

//lasso.cpp

void Lasso::draw_lasso_band() {
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  double ylen = len*sin(arad);
  lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
  lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()

void Lasso::initLasso() {
  lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);
  lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
  lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
  lasso_circle.setColor(COLOR("red"));
  lasso_circle.setFill(true);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setColor(COLOR("black"));
  lasso_loop.setFill(true);
  addPart(&lasso_circle);
  addPart(&lasso_loop);
  lasso_looped = false;
  the_coin = NULL;
  num_coins = 0;

  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
  lasso_line.setColor(COLOR("black"));

  lasso_band.setColor(COLOR("green"));
  draw_lasso_band();

} // End Lasso::initLasso()

void Lasso::yank() {
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;
  if(the_coin != NULL) {
    num_coins++;
    the_coin->resetCoin();
  }
  //There was a bug in the initial given code which caused illegal increase in the number of coins captured. The line added below corrects it!
  the_coin = NULL; // BUG CORRECTION
} // End Lasso::yank()

void Lasso::loopit() {
  if(lasso_looped) { return; } // Already looped
  lasso_loop.reset(getXPos(), getYPos(), LASSO_RADIUS);
  lasso_loop.setFill(false);
  lasso_looped = true;
} // End Lasso::loopit()

void Lasso::addAngle(double angle_deg) {
  release_angle_deg += angle_deg;
  if(release_angle_deg < MIN_RELEASE_ANGLE_DEG) { release_angle_deg = MIN_RELEASE_ANGLE_DEG; }
  if(release_angle_deg > MAX_RELEASE_ANGLE_DEG) { release_angle_deg = MAX_RELEASE_ANGLE_DEG; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addAngle()

void Lasso::addSpeed(double speed) {
  release_speed += speed;
  if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
  if(release_speed > MAX_RELEASE_SPEED) { release_speed = MAX_RELEASE_SPEED; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addSpeed()

void Lasso::nextStep(double stepTime) {
  draw_lasso_band();
  MovingObject::nextStep(stepTime);
  if(getYPos() > PLAY_Y_HEIGHT) { yank(); }
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()

void Lasso::check_for_coin(Coin *coinPtr) {
  double lasso_x = getXPos();
  double lasso_y = getYPos();
  double coin_x = coinPtr->getXPos();
  double coin_y = coinPtr->getYPos();
  double xdiff = (lasso_x - coin_x);
  double ydiff = (lasso_y - coin_y);
  double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
  if(distance <= LASSO_RADIUS) {
    the_coin = coinPtr;
    the_coin->getAttachedTo(this);
  }
} // End Lasso::check_for_coin()


main_program {

  initCanvas("Lasso", WINDOW_X, WINDOW_Y);

  while(true)
  {//Start of the Main While loop
  int stepCount = 0;
  float stepTime = STEP_TIME;
  float runTime = -1; // sec; -ve means infinite
  float currTime = 0;

  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;

  int diffLevel; //This variable is for storing the difficulty level that will be selected through the start menu below

 //The block starting below contains the code for the START Menu.
    {
      Rectangle Background(WINDOW_X/2,WINDOW_Y/2,WINDOW_X,WINDOW_Y);
      Rectangle StartBox(WINDOW_X/2,WINDOW_Y/2,200,100);
      Rectangle StartBoxOutline(WINDOW_X/2,WINDOW_Y/2,200,100);
      Rectangle ContBox(WINDOW_X/2,3*WINDOW_Y/4,150,70);
      Rectangle ContOutline(WINDOW_X/2,3*WINDOW_Y/4,150,70);
      Rectangle QuitBox(100,50,100,50);
      Rectangle QuitBoxOutline(100,50,100,50);
      Rectangle InstructionBox(WINDOW_X/2,WINDOW_Y/4,150,70);
      Rectangle InstructionBoxOutline(WINDOW_X/2,WINDOW_Y/4,150,70);

      Background.setColor(COLOR("green"));
      Background.setFill(true);
      StartBox.setColor(COLOR("red"));
      StartBox.setFill(true);
      StartBoxOutline.setColor(COLOR("blue"));
      ContBox.setColor(COLOR("yellow"));
      ContBox.setFill(true);
      ContOutline.setColor(COLOR("blue"));
      QuitBox.setColor(COLOR("yellow"));
      QuitBox.setFill(true);
      QuitBoxOutline.setColor(COLOR("blue"));
      InstructionBox.setColor(COLOR("yellow"));
      InstructionBox.setFill(true);
      InstructionBoxOutline.setColor(COLOR("blue"));

      Text StartText(WINDOW_X/2,WINDOW_Y/2,"START");
      Text ContText(WINDOW_X/2,3*WINDOW_Y/4,"CONTROLS");
      Text QuitBoxText(100,50,"QUIT");
      Text InstructionBoxText(WINDOW_X/2,WINDOW_Y/4,"INSTRUCTIONS");

      Rectangle Background2(WINDOW_X/2,WINDOW_Y/2,WINDOW_X,WINDOW_Y);//Background to be shown when control option clicked
      Background2.setColor(COLOR("yellow"));

      while(true)
      {
        int p;
        p= getClick();
        int px = p/65536;
        int py = p%65536;

        //The if block below is for the INSTRUCTIONS page
        if(px>325&&px<475&&py>115&&py<185)
        {
            Background2.setFill(true);
            Rectangle IPage(WINDOW_X/2,WINDOW_Y/2,6*WINDOW_X/7,WINDOW_Y/2);
            Rectangle BackBox(100,50,100,50);
            Rectangle BackBoxOutline(100,50,100,50);

            BackBox.setColor(COLOR("green"));
            BackBoxOutline.setColor(COLOR("red"));
            BackBox.setFill(true);
            Text BackText (100,50,"BACK");

            Text i1(WINDOW_X/2,183,"The main objective of the game is catching as many coins as possible");
            Text i2(WINDOW_X/2,216,"You have 3 lives at the start of the game, which decrease on failure to catch coins within the boundaries");
            Text i3(WINDOW_X/2,249,"When you have 0 lives left, the game ends and the coins you caught comprise your score");
            Text i4(WINDOW_X/2,282,"There are 3 difficulty modes to play in");
            Text i5(WINDOW_X/2,315,"In EASY mode, the coins are simply thrown up");
            Text i6(WINDOW_X/2,348,"In MEDIUM mode, the coins move in a parabolic path");
            Text i7(WINDOW_X/2,381,"In HARD mode, the coins follow a tougher path, and an additional boundary is introduced");
            Text i8(WINDOW_X/2,414,"The blue lines indicate the boundaries mentioned above");

            while(true)
            {
                int q;
                q= getClick();
                int qx = q/65536;
                int qy = q%65536;
                //The if below is for detecting the click on the BACK option
                if(qx>50&&qx<150&&qy>25&&qy<75)
                {
                 break;
                }
            }
            Background2.setFill(false);
        }
        //End of INSTRUCTIONS page code
        //The if block below is for the CONTROLS page
        if(px>325&&px<475&&py>415&&py<485)
        {
            Background2.setFill(true);
            Rectangle CPage(WINDOW_X/2,WINDOW_Y/2,WINDOW_X/2,WINDOW_Y/2);
            Rectangle BackBox(100,50,100,50);
            Rectangle BackBoxOutline(100,50,100,50);

            BackBox.setColor(COLOR("green"));
            BackBoxOutline.setColor(COLOR("red"));
            BackBox.setFill(true);
            Text BackText (100,50,"BACK");

            Text c1(WINDOW_X/2,183,"Press 't' to Throw The Lasso");
            Text c2(WINDOW_X/2,216,"Press 'y' to Yank The Lasso");
            Text c3(WINDOW_X/2,249,"Press 'l' to Loop The Lasso");
            Text c4(WINDOW_X/2,282,"Press '[' to Decrease angle of Release of Lasso");
            Text c5(WINDOW_X/2,315,"Press ']' to Increase angle of Release of Lasso");
            Text c6(WINDOW_X/2,348,"Press '=' to Increase the throwing speed of Lasso");
            Text c7(WINDOW_X/2,381,"Press '-' to Decrease the throwing speed of Lasso");
            Text c8(WINDOW_X/2,414,"Press 'q' to Exit to Main Menu during the game");

            while(true)
            {
                int q;
                q= getClick();
                int qx = q/65536;
                int qy = q%65536;
                //The if below is for detecting the click on the BACK option
                if(qx>50&&qx<150&&qy>25&&qy<75)
                {
                 break;
                }
            }
            Background2.setFill(false);
        }
        //End of CONTROLS page code
        //The if below is for detecting the click on START button
        if(px>300&&px<500&&py>250&&py<350)
        {
         break;
        }
        //The if below is for detecting the click on the QUIT button
        if(px>50&&px<150&&py>25&&py<75)
        {
         exit(0);
        }
      }//End of getClick() while loop for initial Start Page

      //The whole sequence of hides below is to transition from initial start page to difficulty selection page
      StartBox.hide();
      StartBoxOutline.hide();
      ContBox.hide();
      ContOutline.hide();
      QuitBox.hide();
      QuitBoxOutline.hide();
      StartText.hide();
      ContText.hide();
      QuitBoxText.hide();

      Rectangle easy(400,150,200,100);
      Rectangle medium(400,300,200,100);
      Rectangle hard(400,450,200,100);
      Rectangle easyOutline(400,150,200,100);
      Rectangle mediumOutline(400,300,200,100);
      Rectangle hardOutline(400,450,200,100);

      easy.setColor(COLOR("yellow"));
      medium.setColor(COLOR("yellow"));
      hard.setColor(COLOR("yellow"));
      easyOutline.setColor(COLOR("blue"));
      mediumOutline.setColor(COLOR("blue"));
      hardOutline.setColor(COLOR("blue"));

      easy.setFill(true);
      medium.setFill(true);
      hard.setFill(true);

      Text easyText(400,150,"EASY");
      Text mediumText(400,300,"MEDIUM");
      Text hardText(400,450,"HARD");

      while(true)
      {
        int p;
        p=getClick();
        int px = p/65536;
        int py = p%65536;

        //The if below is for EASY
        if(px>300&&px<500&&py>100&&py<200)
        {
         diffLevel=1;
         break;
        }
        //The if below is for MEDIUM
        if(px>300&&px<500&&py>250&&py<350)
        {
         diffLevel=2;
         break;
        }
        //The if below is for HARD
        if(px>300&&px<500&&py>400&&py<500)
        {
         diffLevel=3;
         break;
        }
      }//End of getClick() while loop for selecting difficulty
    }
  //End of code for the START Menu.

  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);//The X-axis
  b1.setColor(COLOR("blue"));
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);//The Y-axis
  b2.setColor(COLOR("blue"));
  Line b3(0,280,800,280);//This additional line is the extra boundary that gets added in HARD mode
  b3.setColor(COLOR("blue"));
  b3.hide();
  if(diffLevel==3)
  {b3.show();}

  string msg("Cmd: _");

  Rectangle cmdBox(PLAY_X_START+50, PLAY_Y_HEIGHT+20,60,30);
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);

  char coinScoreStr[256];

  Rectangle scoreBox(PLAY_X_START+50, PLAY_Y_HEIGHT+50,60,30);
  sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);

  //The small section below is for the Text that would display the lives left during the game
  char Lives[256];
  int lives_left = 3;//Start at 3 lives
  Rectangle LivesBox(PLAY_X_START+150, PLAY_Y_HEIGHT+35,100,30);//Box enclosing the Lives text
  sprintf(Lives,"Lives Left: %d", lives_left);
  Text LivesIndicator(PLAY_X_START+150, PLAY_Y_HEIGHT+35,Lives);

  paused = true; rtheta = true;
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  double coin_ax;

  if(diffLevel==1)
   {coin_ax=0;}
  else if(diffLevel==2)
   {coin_ax= -COIN_G;}
  else
   {coin_ax= 2*COIN_G;}

  double coin_ay = COIN_G;
  Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);

  // After every COIN_GAP sec, make the coin jump
  double last_coin_jump_end = 0;

  // When t is pressed, throw lasso
  // If lasso within range, make coin stick
  // When y is pressed, yank lasso
  // When l is pressed, loop lasso
  // When q is pressed, quit

  bool game_lost = false;//Bool variable used for storing if game stopped by losing
  bool pressed_q = false;//Bool variable used for storing if game stopped by pressing 'q'
  for(;;) {
    if((runTime > 0) && (currTime > runTime)) { break; }

    XEvent e;
    bool pendingEv = checkEvent(e);
    if(pendingEv) {
      char c = charFromEvent(e);
      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);
      switch(c) {
      case 't':
	lasso.unpause();
	break;
      case 'y':
	lasso.yank();
	if(diffLevel==2){coin.ax= -COIN_G;}
	if(diffLevel==3){coin.ax= 2*COIN_G;}
	break;
      case 'l':
	lasso.loopit();
	lasso.check_for_coin(&coin);
	wait(STEP_TIME*5);
	break;
      case '[':
	if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
	break;
      case ']':
	if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
	break;
      case '-':
	if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
	break;
      case '=':
	if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
	break;
      case 'q':
	pressed_q=true;
	break;
      default:
	break;
      }
    }

    if(pressed_q) //This if block is to make return to main menu possible on pressing 'q'
    {
        Rectangle Confirmation(400,300,300,160);
        Confirmation.setColor(COLOR("yellow"));

        Text Confirmation1(400,260,"Do you want to exit to the Main Menu?");
        Text Confirmation2(400,340,"Press y/n for Yes/No respectively");

        bool exitConfirm = false; // Variable to store status of confirmation on pressing 'q'

        while(true)//This while is for taking in the confirmation status for the exit to main menu
        {
            bool pendingEv = checkEvent(e);
            if(pendingEv)
            {
             char c = charFromEvent(e);
             if(c=='y')
             {
              exitConfirm=true;
              break;
             }
             if(c=='n')
             {
              pressed_q = false;
              break;
             }
           }//End of the if block for pending event
        }//End of while loop for the Exit Confirmation input

        if(exitConfirm)
        {
         break;
        }
    }//End of code for the action of pressing 'q'

    lasso.nextStep(stepTime);

    coin.nextStep(stepTime);
    if(coin.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	coin.unpause();
      }
    }

    bool resetStat;//Conditions for resetting the coin
    if(diffLevel==1)
    {
        resetStat = (coin.getYPos() > PLAY_Y_HEIGHT);
    }
    else if(diffLevel==2)
    {
        resetStat = (coin.getXPos() < PLAY_X_START);
    }
    else if(diffLevel==3)
    {
        resetStat = (coin.getYPos() < 280);
    }

    if(resetStat)
    {
      lives_left--;// Mechanism to decrease lives on being unable to catch coin

      Rectangle LossBlink1(PLAY_X_START+150, PLAY_Y_HEIGHT+35,120,50);
      LossBlink1.setColor(COLOR("red"));
      wait(0.5);
      LossBlink1.hide();              //This repeated red Rectangles creation and hiding in this section is for the Blinking box when a life is lost
      wait(0.5);
      Rectangle LossBlink2(PLAY_X_START+150, PLAY_Y_HEIGHT+35,120,50);
      LossBlink2.setColor(COLOR("red"));
      wait(0.5);
      sprintf(Lives, "Lives Left: %d",lives_left); //Resetting the Lives Indicator message during the second blink to show the new lives left
      LivesIndicator.setMessage(Lives);
      LossBlink2.hide();
      wait(0.5);
      Rectangle LossBlink3(PLAY_X_START+150, PLAY_Y_HEIGHT+35,120,50);
      LossBlink3.setColor(COLOR("red"));
      wait(0.5);
      LossBlink3.hide();
      wait(0.5);

      coin.resetCoin();
      if(diffLevel==2){coin.ax = -COIN_G;}
      if(diffLevel==3){coin.ax = 2*COIN_G;}
      last_coin_jump_end = currTime;
    }

    if(lives_left==0)
    {//This is the case when all lives are lost and the GAME OVER screen is to be displayed
     game_lost = true;
     break;
    }

    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);//Updating number of coins caught

    stepCount++;
    currTime += stepTime;
    wait(stepTime);
  } // End for(;;)

  //The block below is for the GameOver screen that appears when you have lost all 3 lives
  if(game_lost)
  {
    Rectangle GameOverBackground (WINDOW_X/2,WINDOW_Y/2,WINDOW_X,WINDOW_Y);
    Rectangle GameOver(WINDOW_X/2,WINDOW_Y/2,200,100);
    Rectangle GameOverOutline(WINDOW_X/2,WINDOW_Y/2,200,100);
    Rectangle MainMenu(WINDOW_X/4,3*WINDOW_Y/4,150,80);
    Rectangle MainMenuOutline(WINDOW_X/4,3*WINDOW_Y/4,150,80);
    Rectangle Quit(3*WINDOW_X/4,3*WINDOW_Y/4,150,80);
    Rectangle QuitOutline(3*WINDOW_X/4,3*WINDOW_Y/4,150,80);
    Rectangle YourScore(WINDOW_X/2,WINDOW_Y/4,150,100);
    Rectangle YourScoreOutline(WINDOW_X/2,WINDOW_Y/4,150,100);

    GameOverBackground.setColor(COLOR("red"));
    GameOverBackground.setFill(true);
    GameOver.setColor(COLOR("green"));
    GameOver.setFill(true);
    GameOverOutline.setColor(COLOR("blue"));
    MainMenu.setColor(COLOR("yellow"));
    MainMenu.setFill(true);
    MainMenuOutline.setColor(COLOR("blue"));
    Quit.setColor(COLOR("yellow"));
    Quit.setFill(true);
    QuitOutline.setColor(COLOR("blue"));
    YourScore.setColor(COLOR("green"));
    YourScore.setFill(true);
    YourScoreOutline.setColor(COLOR("blue"));

    Text GameOverT(WINDOW_X/2,WINDOW_Y/2,"GAME OVER");
    Text MainMenuT(WINDOW_X/4,3*WINDOW_Y/4,"Main Menu");
    Text QuitT(3*WINDOW_X/4,3*WINDOW_Y/4,"Quit");
    Text YourScoreT1(400,125,"Your Score :");

    char Final_Score[256];
    sprintf(Final_Score,"%d",lasso.getNumCoins());
    Text YourScoreT2(400,175, Final_Score);

    while(true)//While loop for detecting and processing the clicks on the game over screen
    {
        int p;
        p = getClick();
        int px = p/65536;
        int py = p%65536;

        if(px>125&&px<275&&py>410&&py<490)//When clicking on MainMenu Button
        {break;}

        if(px>525&&px<675&&py>410&&py<490)//When clicking on Quit Button
        {exit(0);}

    } //End of getClick while loop
  }//End of GameOver screen Code
  }//End of Main While Loop
  wait(3);
} // End main_program
