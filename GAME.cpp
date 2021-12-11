#include <cstdlib>		//library yang berisi fungsi umum seperti matematika,pencarian,pengurutan,konversi dan pembanding
#include <ncurses.h>
#include <iostream>
using namespace std;	

//deklarasi variabel
int width = 116;
int height = 20;
int x, y, makan_x, makan_y, score;
bool gameOver;

//untuk arah
enum arah_ular {STOP=0, LEFT, RIGHT, UP, DOWN};
arah_ular arah;

//deklarasi Variabel untuk ekor
int ekor_x[100], ekor_y[100];
int EKOR;
    
//fungsi pengaturan
void pengaturan() {
    //pengaturan layar
    initscr();
    clear();
    curs_set(0);	 //untuk menyembunyikan cursor
   					 //untuk menetapkan variabel permainan
    gameOver = false;
    arah = STOP;
    x = width/2;
    y = height/2;
    makan_x = (rand() % width) + 1;
    makan_y = (rand() % height) + 1;
    score = 0;
}

// untuk gambar jendelanya
void jendela() {
    clear();	
    //untuk mencetak batas, makanan, kepala, dan ekor
    for(int i = 0; i < height + 2; i++) {
        for(int j = 0; j < width + 2; j++) {
            if (i == 0 || i == 21) {
                mvprintw(i,j,"  ");
            } else if (j == 0 || j == 117) {
                mvprintw(i,j,"  ");
            } else if (i == y && j == x) {
                mvprintw(i,j,"O");
            } else if (i == makan_y && j == makan_x) {
                mvprintw(i,j,"+");
            } else {
                for(int k = 0; k < EKOR; k++) {
                    if (ekor_x[k] == j && ekor_y[k] == i) {
                        mvprintw(i,j,"o");
                    }
                }
            }
        }
    }
    //untuk mencetak score
    attroff(COLOR_PAIR(1));
	init_pair(2,COLOR_WHITE,COLOR_GREEN);
	attron(COLOR_PAIR(2));
    mvprintw(23,50,"Score : %d",score);
    //untuk merefresh jendelannya
    refresh();
    return;
}  // Sinta

void input() {
    //Keypad dan kecepatan permainan
    keypad(stdscr, TRUE);
    //untuk mengubah arah
    switch(getch()) {
        case 97:
            //memeriksa apakah ular ga menabrak dirinya sendiri
            if (arah != RIGHT) {
                arah = LEFT;
            }
            break;
        case 100:
            if (arah != LEFT) {
                arah = RIGHT;
            }
            break;
        case 115:
            if (arah != UP) {
                arah = DOWN;
            }
            break;
        case 119:
            if (arah != DOWN) {
                arah = UP;
            }
            break;
        case KEY_LEFT:
            if (arah != RIGHT) {
                arah = LEFT;
            }
            break;
        case KEY_RIGHT:
            if (arah != LEFT) {
                arah = RIGHT;
            }
            break;
        case KEY_DOWN:
            if (arah != UP) {
                arah = DOWN;
            }
            break;
        case KEY_UP:
            if (arah != DOWN) {
                arah = UP;
            }
            break;
        //untuk keluar dari tombol
        case 113:
            gameOver = true;
            break;
    }
}

void ular() {
	start_color();		//untuk warna
	init_pair(1,COLOR_RED,COLOR_BLUE);
	attron(COLOR_PAIR(1));
    //Variabel untuk mengatur nilai ekor
    int baris = ekor_x[0];
    int kolom = ekor_y[0];
    int baris2, kolom2;
    ekor_x[0] = x;
    ekor_y[0] = y;

    //looping untuk menambah nilai ekor
    for(int i = 1; i < EKOR; i++) {
        baris2 = ekor_x[i];
        kolom2 = ekor_y[i];
        ekor_x[i] = baris;
        ekor_y[i] = kolom;
        baris = baris2;
        kolom = kolom2;
    }
    //untuk mengubah arah ular
    switch(arah) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }		// Adli
	    
    //Game over jika kena pembatas jendelannya
	if (x > width || x < 1 || y < 1 || y > height) {
        gameOver = true;
        attroff(COLOR_PAIR(2));	// attroff = Ketentuan untuk menentukan warna
		init_pair(3,COLOR_WHITE,COLOR_GREEN);
		attron(COLOR_PAIR(3));
		clear();
            mvprintw(10,18,"===================================> GAME OVER <===================================");
    }
    //Game over jika menabrak ekor
    for(int i = 0; i < EKOR; i++) {
        if (ekor_x[i] == x && ekor_y[i] == y) {
            gameOver = true;
            attroff(COLOR_PAIR(2));	
			init_pair(3,COLOR_WHITE,COLOR_GREEN);
			attron(COLOR_PAIR(3));
			clear();
         mvprintw(10,18,"===================================> GAME OVER <===================================");
        }
    }
    //untuk meningkatkan skor, menambah makanan baru, dan menambah ekor
    if (x == makan_x && y == makan_y) {
        score++;
        makan_x = (rand() % width) + 1;
        makan_y = (rand() % height) + 1;
        
        //perulangan untuk memastikan buah ga muncul  ular
        for(int i = 0; i < EKOR; i++) {
            while (ekor_x[i] == makan_x && ekor_y[i] == makan_y) {
                makan_x = (rand() % width) + 1;
                makan_y = (rand() % height) + 1;
            }
        }
        EKOR++;
    }
}

int main() {
	char nama[50];	
	string anggota[3] = {"adinda", "sinta", "adli"};
	
	initscr();
	
	printw("Masukkan Nama anda : ");
	getstr(nama);
	
	if(nama == anggota[0] || nama == anggota[1] || nama == anggota[2]){
    pengaturan();
    //Looping untuk melanjutkan permainan
    while(!gameOver) {
        jendela();
        input();
        ular();
    }
	}else {	
	clear();
	mvprintw (10,35,"~~~~ Anda Tidak terdaftar sebagai anggota !! ~~~~");
}

    getch();
    endwin();
    return 0;
}		// Adinda
