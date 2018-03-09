#include <iostream>
#include <limits.h>
#include <bitset>

typedef unsigned long long ull;

class Board;

using namespace std;

//2進数表示
void showBit(ull value) {
    cout << bitset<sizeof(ull) * CHAR_BIT>(value) << endl;
}

void showBoard(ull value) {
    //上位ビットから
    auto board = bitset<sizeof(ull) * CHAR_BIT>(value);
    for (int i = board.size() - 1; i >= 0;) {
        for (int j = 0; j < 8; j++) {
            cout << board[i - j];
        }
        cout << "\n";
        i -= 8;
    }
}

class Board {
public:
    bool blackTurn = true;  //通常黒から
    int nowTurn;
    int nowIdx;

    ull black;
    ull white;

    //石が置かれてない場所を返す
    ull getFree() {
        return ~(black | white);
    }

    //空白があるか
    bool isFull() {
        return !getFree();
    }

    //指定した座標を1にして返す
    //左上(0,0)→右上(0,7)
    ull makeBoard(int x, int y) {
        return (ull) 1 << (x + y * 8);
    }

    bool canPut(ull piece) {

    }

    //y列目のおける場所
    ull getPutLine(ull myBoard, ull enemyBoard, int y) {
        y = 7 - y;    //上位ビットがy==0なので
        myBoard = myBoard >> ((8 * y));
        enemyBoard = enemyBoard >> (8 * y);
        //showBit(myBoard);
        //showBit(enemyBoard);
        uint8_t m = myBoard << 1;
        uint8_t e = enemyBoard;
        return ((ull) (~(m | e) & ~(myBoard | enemyBoard) & (m + e))) << ((8 * y));
    }

    ull rotation(ull x) {
        uint64_t t = (x ^ (x >> 7)) & 0x00aa00aa00aa00aa;
        x = x ^ t ^ (t << 7);
        t = (x ^ (x >> 14)) & 0x0000cccc0000cccc;
        x = x ^ t ^ (t << 14);
        t = (x ^ (x >> 28)) & 0x00000000f0f0f0f0;
        x = x ^ t ^ (t << 28);
        return x;
    }

    ull rotationL(ull x) {
        return (x << 1) & 0xfefefefefefefefeULL;
    }

    //対角線上に回転(上下反転からの左90)
    ull flipDiagA1H8(ull x) {
        ull t;
        const ull k1 = 0x5500550055005500;
        const ull k2 = 0x3333000033330000;
        const ull k4 = 0x0f0f0f0f00000000;
        t = k4 & (x ^ (x << 28));
        x ^= t ^ (t >> 28);
        t = k2 & (x ^ (x << 14));
        x ^= t ^ (t >> 14);
        t = k1 & (x ^ (x << 7));
        x ^= t ^ (t >> 7);
        return x;
    }

    //上下反転
    ull flipVertical(ull x) {
        return ((x << 56)) |
               ((x << 40) & 0x00ff000000000000) |
               ((x << 24) & 0x0000ff0000000000) |
               ((x << 8) & 0x000000ff00000000) |
               ((x >> 8) & 0x00000000ff000000) |
               ((x >> 24) & 0x0000000000ff0000) |
               ((x >> 40) & 0x000000000000ff00) |
               ((x >> 56));
    }

    ull rotationL90(ull x) {
        return flipVertical(flipDiagA1H8(x));
    }

    ull rotationR90(ull x) {
        return flipDiagA1H8(flipVertical(x));
    }


    //自分を黒としたときのおける場所
    ull getPutBlack() {
        ull sum = 0ull;
        for (int i = 0; i < 8; i++) {
            sum = sum | getPutLine(black, white, i);
        }
/*
        //回転
        ull black2 = rotation(black);
        ull white2 = rotation(white);
        for (int i = 0; i < 8; i++) {
            sum = sum | getPutLine(black2, white2, i);
        }*/
        return sum;// & (getFree());
    }

    //自分を白としたときのおける場所
    ull getPutWhite() {
        ull sum = 0;
        for (int i = 0; i < 8; i++) {
            sum = sum | getPutLine(white, black, i);
        }
        /*
        //回転
        ull black2 = (((black >> 3) | (black << 3)) & 63) ^56;
        ull white2 = (((black >> 3) | (black << 3)) & 63) ^56;
        for (int i = 0; i < 8; i++) {
            sum = sum | getPutLine(white2, black2, i);
        }*/
        return sum & (getFree());
    }

    ull cntBits(ull bits) {
        unsigned long long m[] = {
                0x5555555555555555ULL,
                0x3333333333333333ULL,
                0x0f0f0f0f0f0f0f0fULL,
                0x00ff00ff00ff00ffULL,
                0x0000ffff0000ffffULL,
                0x00000000ffffffffULL,
        };
        bits = (bits & m[0]) + (bits >> 1 & m[0]);
        bits = (bits & m[1]) + (bits >> 2 & m[1]);
        bits = (bits & m[2]) + (bits >> 4 & m[2]);
        bits = (bits & m[3]) + (bits >> 8 & m[3]);
        bits = (bits & m[4]) + (bits >> 16 & m[4]);
        return (bits & m[5]) + (bits >> 32 & m[5]);
    }

    ull cntBlack() {
        return cntBits(black);
    }

    ull cntWhite() {
        return cntBits(white);
    }

    ull cnt() {
        return cntBits((black | white));
    }
};

int main() {
    Board *board = new Board;
    board->black = 0x002002045C0C0000;
    board->white = 0x0010787820301000;

    cout << board->cnt() << endl;
    showBoard(board->black);
    //showBit(board->getPutLine(board->black, board->white, 7));
    cout << endl;
    showBoard(board->rotationR90(board->black));

    //showBit(board->getPutBlack());

    return 0;
}
//https://chessprogramming.wikispaces.com/Flipping+Mirroring+and+Rotating