// #include <iostream>//needed for cout<<
// #include <windows.h>//needed for setting text cursor position and Sleep()
// int pl[150];//playfield is 15x10
// int fig[16]={56,146,56,146,58,178,184,154,312,210,57,150,27,27,27,27}; //4 blocks in 4 orientations, encoded as binary 3x3 maps (9 bits)
// //So for example L-block is 010 010 110 = 150
// int off=1;//block position (1 row=10)
// int r=1;//flag: game is running?
// int i,j,j2,t,k,dd,pt,spd,dir,nfig; //various helper variables:
// //i,j,j2=loop pointers, t=time, k=collision flag, dd=key delay, pt=points, spd=speed flag, dir=block orientation, nfig=block type
// int np[2];//new position after applying controls, to test if input will cause a collision
// COORD c = {0,0};//for setting console pos. to beginning

// int main(int argc, char** argv) {
// for(i=0;i<150;i++)pl[i]=(i%10==0||i%10==8||i>140);//draw the edge around playfield

// 	while(r){//main loop
// 	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), c);std::cout<<"points:"<<pt<<"     "<<'\n';//set to 0,0 , output points
// 	t++;off+=10*(t%(10-9*spd)==0);if(dd>0)dd--; //advance time, move block down, reduce key delay (dd)

// 	k=0; //no collision?
// 	for(i=0;i<9;i++){k+=pl[off+i%3+i/3*10+10]*((fig[nfig*4+dir]&1<<i)>0);//k>0 when there is something below active block
// 	pl[off+i%3+i/3*10]+=((fig[nfig*4+dir]&1<<i)>0);}//draw the block
// 	for(i=0;i<150;i++)std::cout<<(char)(32+3*pl[i]-22*(i%10==9)); //draw the play field: 32(space) or 35(#) or 10(newline)
// 	if(k==0)for(i=0;i<9;i++)pl[off+i%3+i/3*10]-=((fig[nfig*4+dir]&1<<i)>0); //there was no collision?
// 	//If so, erase the block, it will be drawn lower in next loop
// 	else{if(off<10)r=0;off=1;nfig=rand()%4;dir=0;spd=0;}//collision happened? put the block and create a new one. We are at the top? Game over.

// 	Sleep(50);//bad way of regulating speed
// 	np[1]=off;np[2]=dir;//save the position and angle of the block
// 	if((GetKeyState(VK_ESCAPE) & 0x8000))r=0;
// 	if((GetKeyState(VK_RIGHT) & 0x8000)&&(off%10<6))off++; //right
// 	if((GetKeyState(VK_LEFT) & 0x8000)&&(off%10>0))off--; //left
// 	if((GetKeyState(VK_DOWN) & 0x8000))spd=1; //down - speedup
// 	if((GetKeyState(VK_UP) & 0x8000)&&dd==0){dir=(dir+1)%4;dd=4;}; //up - rotation
// 	k=0;for(i=0;i<9;i++)if(pl[off+i%3+i/3*10]+((fig[nfig*4+dir]&1<<i)>0)>1)k=1;//controls cause the block to collide with something?
// 	if(k==1){off=np[1];dir=np[2];}//collision happened - reset position and angle (e.g. move was impossible)

// 	for(j=0;j<14;j++)//go through field rows
//     {k=1;for(i=1;i<8;i++)if(pl[i+j*10]==0)k=0; //test if we have a whole line
// 	if(k==1){for(i=1;i<8;i++)pl[i+j*10]=0;pt++;//whole line? reove blocks, add points...
// 	for(j2=j;j2>0;j2--)for(i=1;i<8;i++)pl[i+j2*10]=pl[i+j2*10-10];}};//...and move everything above line one block down
// 	}//end of main loop
// }

// #include <iostream>//needed for cout<<
// #include <windows.h>//needed for setting text cursor position and Sleep()
// #include <cstdlib>//needed for rand()

// const int WIDTH = 10;
// const int HEIGHT = 20;
// int pl[WIDTH * HEIGHT];//playfield is 20x10
// int fig[16] = {56, 146, 56, 146, 58, 178, 184, 154, 312, 210, 57, 150, 27, 27, 27, 27}; //4 blocks in 4 orientations, encoded as binary 3x3 maps (9 bits)
// //So for example L-block is 010 010 110 = 150
// int off = 1;//block position (1 row=10)
// int r = 1;//flag: game is running?
// int i, j, j2, t, k, dd, pt, spd, dir, nfig; //various helper variables:
// //i,j,j2=loop pointers, t=time, k=collision flag, dd=key delay, pt=points, spd=speed flag, dir=block orientation, nfig=block type
// int np[2];//new position after applying controls, to test if input will cause a collision
// COORD c = {0, 0};//for setting console pos. to beginning

// int main(int argc, char** argv) {
//     for (i = 0; i < WIDTH * HEIGHT; i++) pl[i] = (i % WIDTH == 0 || i % WIDTH == WIDTH - 1 || i >= WIDTH * (HEIGHT - 1));//draw the edge around playfield

//     while (r) {//main loop
//         SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c); std::cout << "points:" << pt << "     " << '\n';//set to 0,0 , output points
//         t++; off += WIDTH * (t % (10 - 9 * spd) == 0); if (dd > 0) dd--; //advance time, move block down, reduce key delay (dd)

//         k = 0; //no collision?
//         for (i = 0; i < 9; i++) {
//             k += pl[off + i % 3 + i / 3 * WIDTH + WIDTH] * ((fig[nfig * 4 + dir] & 1 << i) > 0);//k>0 when there is something below active block
//             pl[off + i % 3 + i / 3 * WIDTH] += ((fig[nfig * 4 + dir] & 1 << i) > 0);
//         }//draw the block
//         for (i = 0; i < WIDTH * HEIGHT; i++) std::cout << (char)(32 + 3 * pl[i] - 22 * (i % WIDTH == WIDTH - 1)); //draw the play field: 32(space) or 35(#) or 10(newline)
//         if (k == 0) for (i = 0; i < 9; i++) pl[off + i % 3 + i / 3 * WIDTH] -= ((fig[nfig * 4 + dir] & 1 << i) > 0); //there was no collision?
//         //If so, erase the block, it will be drawn lower in next loop
//         else { if (off < WIDTH) r = 0; off = 1; nfig = rand() % 4; dir = 0; spd = 0; }//collision happened? put the block and create a new one. We are at the top? Game over.

//         Sleep(50);//bad way of regulating speed
//         np[0] = off; np[1] = dir;//save the position and angle of the block
//         if ((GetKeyState(VK_ESCAPE) & 0x8000)) r = 0;
//         if ((GetKeyState(VK_RIGHT) & 0x8000) && (off % WIDTH < WIDTH - 4)) off++; //right
//         if ((GetKeyState(VK_LEFT) & 0x8000) && (off % WIDTH > 1)) off--; //left
//         if ((GetKeyState(VK_DOWN) & 0x8000)) spd = 1; //down - speedup
//         if ((GetKeyState(VK_UP) & 0x8000) && dd == 0) { dir = (dir + 1) % 4; dd = 4; }; //up - rotation
//         k = 0; for (i = 0; i < 9; i++) if (pl[off + i % 3 + i / 3 * WIDTH] + ((fig[nfig * 4 + dir] & 1 << i) > 0) > 1) k = 1;//controls cause the block to collide with something?
//         if (k == 1) { off = np[0]; dir = np[1]; }//collision happened - reset position and angle (e.g. move was impossible)

//         for (j = 0; j < HEIGHT - 1; j++)//go through field rows
//         {
//             k = 1; for (i = 1; i < WIDTH - 1; i++) if (pl[i + j * WIDTH] == 0) k = 0; //test if we have a whole line
//             if (k == 1) {
//                 for (i = 1; i < WIDTH - 1; i++) pl[i + j * WIDTH] = 0; pt++;//whole line? remove blocks, add points...
//                 for (j2 = j; j2 > 0; j2--) for (i = 1; i < WIDTH - 1; i++) pl[i + j2 * WIDTH] = pl[i + j2 * WIDTH - WIDTH];
//             };//...and move everything above line one block down
//         }//end of main loop
//     }
// }

// #include <iostream>  //needed for cout<<
// #include <windows.h> //needed for setting text cursor position and Sleep()
// #include <cstdlib>   //needed for rand()

// const int WIDTH = 10;
// const int HEIGHT = 20;

// int pl[WIDTH * HEIGHT];                                                                 // playfield is 20x10
// int fig[16] = {56, 146, 56, 146, 58, 178, 184, 154, 312, 210, 57, 150, 27, 27, 27, 27}; // 4 blocks in 4 orientations, encoded as binary 3x3 maps (9 bits)
// // So for example L-block is 010 010 110 = 150
// int off = 1;                                // block position (1 row=10)
// int r = 1;                                  // flag: game is running?
// int i, j, j2, t, k, dd, pt, spd, dir, nfig; // various helper variables:
// // i,j,j2=loop pointers, t=time, k=collision flag, dd=key delay, pt=points, spd=speed flag, dir=block orientation, nfig=block type
// int np[2];        // new position after applying controls, to test if input will cause a collision
// COORD c = {0, 0}; // for setting console pos. to beginning

// int main(int argc, char **argv)
// {
//     for (i = 0; i < WIDTH * HEIGHT; i++)
//         pl[i] = (i % WIDTH == 0 || i % WIDTH == WIDTH - 1 || i >= WIDTH * (HEIGHT - 1)); // draw the edge around playfield

//     while (r)
//     { // main loop
//         SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
//         std::cout << "points:" << pt << "     " << '\n'; // set to 0,0 , output points
//         t++;
//         off += WIDTH * (t % (10 - 9 * spd) == 0);
//         if (dd > 0)
//             dd--; // advance time, move block down, reduce key delay (dd)

//         k = 0; // no collision?
//         for (i = 0; i < 9; i++)
//         {
//             k += pl[off + i % 3 + i / 3 * WIDTH + WIDTH] * ((fig[nfig * 4 + dir] & 1 << i) > 0); // k>0 when there is something below active block
//             pl[off + i % 3 + i / 3 * WIDTH] += ((fig[nfig * 4 + dir] & 1 << i) > 0);
//         } // draw the block
//         for (i = 0; i < WIDTH * HEIGHT; i++)
//             std::cout << (char)(32 + 3 * pl[i] - 22 * (i % WIDTH == WIDTH - 1)); // draw the play field: 32(space) or 35(#) or 10(newline)
//         if (k == 0)
//             for (i = 0; i < 9; i++)
//                 pl[off + i % 3 + i / 3 * WIDTH] -= ((fig[nfig * 4 + dir] & 1 << i) > 0); // there was no collision?
//         // If so, erase the block, it will be drawn lower in next loop
//         else
//         {
//             if (off < WIDTH)
//                 r = 0;
//             off = 1;
//             nfig = rand() % 4;
//             dir = 0;
//             spd = 0;
//         } // collision happened? put the block and create a new one. We are at the top? Game over.

//         Sleep(50); // bad way of regulating speed
//         np[0] = off;
//         np[1] = dir; // save the position and angle of the block
//         if ((GetKeyState(VK_ESCAPE) & 0x8000))
//             r = 0;
//         if ((GetKeyState(VK_RIGHT) & 0x8000) && (off % WIDTH < WIDTH - 4))
//             off++; // right
//         if ((GetKeyState(VK_LEFT) & 0x8000) && (off % WIDTH > 1))
//             off--; // left
//         if ((GetKeyState(VK_DOWN) & 0x8000))
//             spd = 1; // down - speedup
//         if ((GetKeyState(VK_UP) & 0x8000) && dd == 0)
//         {
//             dir = (dir + 1) % 4;
//             dd = 4;
//             // Check for collision after rotation
//             k = 0;
//             for (i = 0; i < 9; i++)
//                 if (pl[off + i % 3 + i / 3 * WIDTH] + ((fig[nfig * 4 + dir] & 1 << i) > 0) > 1)
//                     k = 1;
//             if (k == 1)
//             {
//                 dir = (dir + 3) % 4; // revert rotation if collision
//             }
//         }; // up - rotation
//         k = 0;
//         for (i = 0; i < 9; i++)
//             if (pl[off + i % 3 + i / 3 * WIDTH] + ((fig[nfig * 4 + dir] & 1 << i) > 0) > 1)
//                 k = 1; // controls cause the block to collide with something?
//         if (k == 1)
//         {
//             off = np[0];
//             dir = np[1];
//         } // collision happened - reset position and angle (e.g. move was impossible)

//         for (j = 0; j < HEIGHT - 1; j++) // go through field rows
//         {
//             k = 1;
//             for (i = 1; i < WIDTH - 1; i++)
//                 if (pl[i + j * WIDTH] == 0)
//                     k = 0; // test if we have a whole line
//             if (k == 1)
//             {
//                 for (i = 1; i < WIDTH - 1; i++)
//                     pl[i + j * WIDTH] = 0;
//                 pt++; // whole line? remove blocks, add points...
//                 for (j2 = j; j2 > 0; j2--)
//                     for (i = 1; i < WIDTH - 1; i++)
//                         pl[i + j2 * WIDTH] = pl[i + j2 * WIDTH - WIDTH];
//             }; //...and move everything above line one block down
//         } // end of main loop
//     }
// }





#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>

const int WIDTH = 10;
const int HEIGHT = 20;
int pl[WIDTH * HEIGHT]; // playfield is 20x10
int fig[16] = {56, 146, 56, 146, 58, 178, 184, 154, 312, 210, 57, 150, 27, 27, 27, 27}; // 4 blocks in 4 orientations, encoded as binary 3x3 maps (9 bits)
// So for example L-block is 010 010 110 = 150
int off = 1; // block position (1 row=10)
int r = 1; // flag: game is running?
int i, j, j2, t, k, dd, pt, spd, dir, nfig; // various helper variables:
// i,j,j2=loop pointers, t=time, k=collision flag, dd=key delay, pt=points, spd=speed flag, dir=block orientation, nfig=block type
int np[2]; // new position after applying controls, to test if input will cause a collision
COORD c = {0, 0}; // for setting console pos. to beginning

void drawPlayfield() {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    std::cout << "points:" << pt << "     " << '\n'; // set to 0,0 , output points
    for (i = 0; i < WIDTH * HEIGHT; i++) {
        std::cout << (char)(32 + 3 * pl[i] - 22 * (i % WIDTH == WIDTH - 1)); // draw the play field: 32(space) or 35(#) or 10(newline)
    }
}

void clearLine(int line) {
    for (i = 1; i < WIDTH - 1; i++) pl[i + line * WIDTH] = 0; // clear the line
    for (j2 = line; j2 > 0; j2--) {
        for (i = 1; i < WIDTH - 1; i++) {
            pl[i + j2 * WIDTH] = pl[i + (j2 - 1) * WIDTH]; // move everything above down
        }
    }
}

bool checkCollision(int newOff, int newDir) {
    for (i = 0; i < 9; i++) {
        if (pl[newOff + i % 3 + i / 3 * WIDTH] + ((fig[nfig * 4 + newDir] & 1 << i) > 0) > 1) {
            return true;
        }
    }
    return false;
}

int main() {
    srand(time(0));
    for (i = 0; i < WIDTH * HEIGHT; i++) {
        pl[i] = (i % WIDTH == 0 || i % WIDTH == WIDTH - 1 || i >= WIDTH * (HEIGHT - 1)); // draw the edge around playfield
    }

    while (r) { // main loop
        drawPlayfield();
        t++;
        off += WIDTH * (t % (10 - 9 * spd) == 0);
        if (dd > 0) dd--; // advance time, move block down, reduce key delay (dd)

        k = 0; // no collision?
        for (i = 0; i < 9; i++) {
            k += pl[off + i % 3 + i / 3 * WIDTH + WIDTH] * ((fig[nfig * 4 + dir] & 1 << i) > 0); // k>0 when there is something below active block
            pl[off + i % 3 + i / 3 * WIDTH] += ((fig[nfig * 4 + dir] & 1 << i) > 0);
        } // draw the block

        if (k == 0) {
            for (i = 0; i < 9; i++) {
                pl[off + i % 3 + i / 3 * WIDTH] -= ((fig[nfig * 4 + dir] & 1 << i) > 0); // there was no collision?
            }
        } else {
            if (off < WIDTH) r = 0; // game over
            off = 1;
            nfig = rand() % 4;
            dir = 0;
            spd = 0;
        } // collision happened? put the block and create a new one. We are at the top? Game over.

        Sleep(50); // bad way of regulating speed
        np[0] = off;
        np[1] = dir; // save the position and angle of the block
        if ((GetKeyState(VK_ESCAPE) & 0x8000)) r = 0;
        if ((GetKeyState(VK_RIGHT) & 0x8000) && (off % WIDTH < WIDTH - 4)) off++; // right
        if ((GetKeyState(VK_LEFT) & 0x8000) && (off % WIDTH > 1)) off--; // left
        if ((GetKeyState(VK_DOWN) & 0x8000)) spd = 1; // down - speedup
        if ((GetKeyState(VK_UP) & 0x8000) && dd == 0) {
            dir = (dir + 1) % 4;
            dd = 4;
            if (checkCollision(off, dir)) {
                dir = (dir + 3) % 4; // revert rotation if collision
            }
        } // up - rotation

        if (checkCollision(off, dir)) {
            off = np[0];
            dir = np[1]; // collision happened - reset position and angle (e.g. move was impossible)
        }

        for (j = 0; j < HEIGHT - 1; j++) { // go through field rows
            k = 1;
            for (i = 1; i < WIDTH - 1; i++) {
                if (pl[i + j * WIDTH] == 0) k = 0; // test if we have a whole line
            }
            if (k == 1) {
                clearLine(j);
                pt++; // whole line? remove blocks, add points...
            }
        } // end of main loop
    }
    return 0;
}