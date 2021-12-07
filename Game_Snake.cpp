#include <iostream>
#include <cstdlib>
#include <ctime>
#include <list>
#include <string>
#include <ncurses.h>
#define waktu_habis 200

using namespace std;
typedef struct point {
	int y, x;
} Point;

typedef struct makan {
	Point pos;
	bool present = false;
} makan;

typedef struct ular {
	Point kepala;
	list<Point> badan;
} ular;

bool operator==(const Point& lhs, const Point& rhs) {
	return lhs.y == rhs.y && lhs.x == rhs.x;
}

void tempatmakan(makan *makan) {						//fungsi
	makan->pos.y = rand()/((RAND_MAX + 1u)/LINES);
	makan->pos.x = rand()/((RAND_MAX + 1u)/COLS);
	attron(COLOR_PAIR(1));
	mvaddch(makan->pos.y, makan->pos.x, ACS_DIAMOND);
	attroff(COLOR_PAIR(1));
	makan->present = true;
}

void posisiular(ular &ular, makan &makan, int arah) {
	ular.badan.push_back(ular.kepala);					//fungsi push_back() menambahkan elemen baru (ular.kepala)
	switch(arah) {										//percabangan
		case 0:
			if (ular.kepala.y == 0)
				ular.kepala.y = LINES;
			else
				--ular.kepala.y;
			mvaddch(ular.kepala.y, ular.kepala.x, ACS_UARROW);
		break;
		case 1:
			if (ular.kepala.x == 0)
				ular.kepala.x = COLS;
			else
				--ular.kepala.x;
			mvaddch(ular.kepala.y, ular.kepala.x, ACS_LARROW);
		break;
		case 2:
			if (ular.kepala.y == LINES - 1)
				ular.kepala.y = 0;
			else
				++ular.kepala.y;
			mvaddch(ular.kepala.y, ular.kepala.x, 'o');
		break;
		case 3:
			if (ular.kepala.x == COLS - 1)
				ular.kepala.x = 0;
			else
				++ular.kepala.x;
			mvaddch(ular.kepala.y, ular.kepala.x, ACS_RARROW);
		break;
	}
	if (ular.kepala == makan.pos) {
		makan.present = false;
		ular.badan.push_back(ular.badan.back());					
	}

	mvaddch(ular.badan.front().y, ular.badan.front().x, ' ');
	if (ular.badan.size() > 1)
		mvaddch(ular.badan.back().y, ular.badan.back().x, ACS_BULLET);	//	//fungsi back() mengembalikan nilai elemen terakhir dalam daftar
	ular.badan.pop_front(); 											//fungsi pop_front() menghapus elemen pertama dari daftar dan mengurangi ukuran
}
//perulangan
bool tabrakan(ular &ular) {
	for (auto it = ular.badan.begin(); it != ular.badan.end(); ++it)  //fungsi begin() mengembalikan itrator yang menunjuk ke elemen pertama dari daftar
		if (ular.kepala == *it)										//fungsi end() mengembalikan nilai elemen pertama dalam daftar
			return true;
	return false;
}

int kecepatan(int panjang) {
	if (waktu_habis - 10 * panjang > 20)
		return waktu_habis - 10 * panjang;
	return 20;
}

int main() {
	
	//int x = 1, y = 1;
	//initscr();
	char nama[100];
	string anggota[3] = {"adinda", "sinta", "adli"};
	
	
	
	initscr();
	
	printw("Masukkan Nama anda : ");
	getstr(nama);	

	
	if(nama == anggota[0] || nama == anggota[1] || nama == anggota[2]){
	clear();
	cbreak();
	noecho();
	keypad(stdscr, true);
	curs_set(0); // menyembunyikan cursor

	void tempatmakan(makan*);
	void posisiular(ular&, makan&, int);
	bool tabrakan(ular&);
	int kecepatan(int);
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);

	srand(time(nullptr));

	string msg = "Game Over...\n";
	string score = "Score: ";
	int arah, ch;
	arah = -1;

	ular ular;
	ular.kepala = {(LINES - 1) / 2, (COLS - 1) / 2};
	makan makan;

	mvaddch(ular.kepala.y, ular.kepala.x, ACS_BULLET);
	while((ch = getch()) != KEY_F(1)) {
		timeout(kecepatan(ular.badan.size()));							//fungsi size() mengembalikan jumlah elemen dalam daftar
		switch(ch) {
			case 'w':
			case KEY_UP:
				arah = 0;
			break;
			case 'a':
			case KEY_LEFT:
				arah = 1;
			break;
			case 's':
			case KEY_DOWN:
				arah = 2;
			break;
			case 'd':
			case KEY_RIGHT:
				arah = 3;
			break;
		}
		posisiular(ular, makan, arah);
		// untuk memastikan makanan berada di dalam jendela
		if (makan.pos.y >= LINES || makan.pos.x >= COLS)
			makan.present = false;
		if (!makan.present)
			tempatmakan(&makan);
		if (tabrakan(ular))
			break;
		refresh();
	}
	
	clear();
	mvprintw(LINES/2, (COLS - msg.length())/2, "%s", msg.c_str());			
	mvprintw(LINES/2 + 1, (COLS - msg.length())/2, "%s %lu", score.c_str(), ular.badan.size());
	timeout(-1);
	refresh();
	}else{
	printf ("Anda Tidak terdaftar sebagai anggota !!");
}



	

	getch();
	endwin();
	return 0;
}
