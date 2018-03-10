#include <iostream>
#include <limits.h>
#include <bitset>
#include <queue>
#include <stack>

typedef unsigned long long ull;

class Board;

using namespace std;

int ctoi(char x);

//2進数表示
void showBit(ull value) {
    cout << bitset<sizeof(ull) * CHAR_BIT>(value) << endl;
}

void showBoard(ull value) {
    for(ull i=0; i<8; i++){
        cout << bitset<8>( (value >> (7ull-i)*8ull)) << endl;
    }
    cout << endl;
    return;
    /*
    string str = std::bitset<sizeof(ull) * CHAR_BIT>(value).to_string<char, std::char_traits<char>, std::allocator<char> >();
    cout << str << endl;

    //上位ビットから
    cout.clear();
    stringstream ss;
    auto board = bitset<sizeof(ull) * CHAR_BIT>(value);
    for (int i = str.size() - 1; i >= 0;) {
        for (int j = 0; j < 8; j++) {
            ss << str[i - j];
        }
        ss << endl;
        i -= 8;
    }
    cout << ss.str() << endl;*/
}

class Board {

public:
    bool blackTurn = true;  //通常黒から
    int nowTurn = 0;

    ull black = 0;
    ull white = 0;
    ull canBlackBoard = 0, canWhiteBoard = 0;

    //初期化
    void init() {
        white = 68853694464;
        black = 34628173824;

        blackTurn = true;
        nowTurn = 0;
        reloadCanBoard();
        cout << "init" << endl;

    }


    void reloadCanBoard() {
        canBlackBoard = getPutBlack();
        canWhiteBoard = getPutWhite();
    }

    void nextTurn() {
        nowTurn++;
        if ((black | white) == ULLONG_MAX) {
            cout << "fin" << endl;
        }
        reloadCanBoard();
        if (blackTurn) { //白ターンに
            if (canWhiteBoard == 0) {
                cout << "白パス" << endl;
                blackTurn = true;
            }
            blackTurn = false;
            cout << "白ターン" << endl;
        } else {
            if (canBlackBoard == 0) {
                cout << "黒パス" << endl;
                blackTurn = false;
            }
            blackTurn = true;
            cout << "黒ターン" << endl;
        }
        showBoard(black | white);

    }

    void inputAndPut() {
        char x, y;
        if (blackTurn) cout << "黒ターン" << endl;
        else cout << "白ターン" << endl;
        int yi;
        while (true) {
            cout << "座標入力待ち(a～h1～8):";
            cin >> x >> y;
            yi = ctoi(y);
            if ('a' <= x && x <= 'h' && 0 <= yi && yi <= 8) {
                if (!canPut(makeBoard(x, yi))) {
                    cout << "設置不可場所です" << endl;

                    continue;
                }
                break;
            }else{
                cin.clear();
                cin.seekg(0);
                cout << "re input" << endl;
            }
        }
        if (blackTurn) putBlack(makeBoard(x, yi));
        else putWhite(makeBoard(x, yi));
    }

    bool canPut(ull pos) {
        if (blackTurn) {
            return (canBlackBoard & pos) != 0;
        } else {
            return (canWhiteBoard & pos) != 0;
        }
    }

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
        if (x < 0 || x > 7 || y < 0 || y > 7) {
            cout << "err" << endl;
            return 0ull;
        }
        return (ull) 1 << (8 - x + 56 - y * 8 - 1);
    }

    ull makeBoard(char x, int y) {
        if (x < 'a' || x > 'h' || y < 1 || y > 8) {
            cout << "err" << endl;
            return 0ull;
        }
        return makeBoard(x - 'a', y - 1);
    }
/*
    //y列目のおける場所
    ull getPutLine(ull myBoard, ull enemyBoard, int y) {
        /*
        y = 7 - y;    //上位ビットがy==0なので
        myBoard = myBoard >> ((8 * y));
        enemyBoard = enemyBoard >> (8 * y);
        //showBit(myBoard);
        //showBit(enemyBoard);
        uint8_t m = myBoard << 1;
        uint8_t e = enemyBoard;
        return ((ull) (~(m | e) & ~(myBoard | enemyBoard) & (m + e))) << ((8 * y));

        //左側方向の処理
        ull w = white & 0x7e7e7e7e7e7e7e7e; //端の1列が0、それ以外は1な盤面
        ull t = w & (black << 1);           //黒石の左隣にある白石を調べる
        t |= w & (t << 1);                  //その隣の
        t |= w & (t << 1);                  //隣の・・・
        t |= w & (t << 1);
        t |= w & (t << 1);
        t |= w & (t << 1);                    //一度にひっくり返せる石は6つまで
        ull blank = ~(black | white);       // 空白の箇所
        ull mobility = blank & (t << 1);    //着手出来るのは空白のマスだけ
        return mobility;
    }
*/
    //右から左にみて置ける場所を探索
    ull getPutRtoL(ull myBoard, ull enemyBoard) {
        ull sum = 0;
        //左側方向の処理
        ull w = enemyBoard & 0x7e7e7e7e7e7e7e7e; //端の1列が0、それ以外は1な盤面
        ull t = w & (myBoard << 1);           //黒石の左隣にある白石を調べる
        t |= w & (t << 1);                  //その隣の
        t |= w & (t << 1);                  //隣の・・・
        t |= w & (t << 1);
        t |= w & (t << 1);
        t |= w & (t << 1);                    //一度にひっくり返せる石は6つまで
        ull blank = ~(myBoard | enemyBoard);       // 空白の箇所
        ull mobility = blank & (t << 1);    //着手出来るのは空白のマスだけ
        return mobility;
    }

    //斜め方向のおける場所
    ull getPutSlash(ull myBoard, ull enemyBoard) {
        ull sum = 0;
        // 右上方向
        ull masked_enemy = enemyBoard & 0x007e7e7e7e7e7e00;
        ull t = masked_enemy & (myBoard << 7);
        for (int i = 0; i < 5; i++) {
            t |= masked_enemy & (t << 7);
        }
        sum |= (t << 7);

        // 左上方向
        masked_enemy = enemyBoard & 0x007e7e7e7e7e7e00;
        t = masked_enemy & (myBoard << 9);
        for (int i = 0; i < 5; i++) {
            t |= masked_enemy & (t << 9);
        }
        sum |= (t << 9);

        // 右下方向
        masked_enemy = enemyBoard & 0x007e7e7e7e7e7e00;
        t = masked_enemy & (myBoard >> 9);
        for (int i = 0; i < 5; i++) {
            t |= masked_enemy & (t >> 9);
        }
        sum |= (t >> 9);

        // 左下方向
        masked_enemy = enemyBoard & 0x007e7e7e7e7e7e00;
        t = masked_enemy & (myBoard >> 7);
        for (int i = 0; i < 5; i++) {
            t |= masked_enemy & (t >> 7);
        }
        sum |= (t >> 7);

        return sum & getFree();
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

    ull rotation180(ull x) {
        const ull h1 = 0x5555555555555555;
        const ull h2 = 0x3333333333333333;
        const ull h4 = 0x0F0F0F0F0F0F0F0F;
        const ull v1 = 0x00FF00FF00FF00FF;
        const ull v2 = 0x0000FFFF0000FFFF;
        x = ((x >> 1) & h1) | ((x & h1) << 1);
        x = ((x >> 2) & h2) | ((x & h2) << 2);
        x = ((x >> 4) & h4) | ((x & h4) << 4);
        x = ((x >> 8) & v1) | ((x & v1) << 8);
        x = ((x >> 16) & v2) | ((x & v2) << 16);
        x = (x >> 32) | (x << 32);
        return x;
    }


    //自分を黒としたときのおける場所(8方向
    ull getPutBlack() {
        cout << "getput" << endl;
        ull sum = 0ull;
        //4方向
        sum = sum | getPutRtoL(black, white);

        //回転
        ull black2 = black;
        ull white2 = white;
        for (int i = 0; i < 3; i++) {
            black2 = rotationR90(black2);
            white2 = rotationR90(white2);
            if (i == 0) {
                sum = sum | rotationL90(getPutRtoL(black2, white2));
            } else if (i == 1) {
                sum = sum | rotation180(getPutRtoL(black2, white2));
            } else if (i == 2) {
                sum = sum | rotationR90(getPutRtoL(black2, white2));
            }
        }
        return sum | getPutSlash(black, white);//8方向
    }

    //自分を白としたときのおける場所
    ull getPutWhite() {
        ull sum = 0ull;
        //4方向
        sum = sum | getPutRtoL(white, black);
        //回転
        ull black2 = black;
        ull white2 = white;
        for (int i = 0; i < 3; i++) {
            black2 = rotationR90(black2);
            white2 = rotationR90(white2);
            if (i == 0) {
                sum = sum | rotationL90(getPutRtoL(white2, black2));
            } else if (i == 1) {
                sum = sum | rotation180(getPutRtoL(white2, black2));
            } else if (i == 2) {
                sum = sum | rotationR90(getPutRtoL(white2, black2));
            }
        }
        return sum | getPutSlash(white, black);//8方向
    }

    //石を反転させる
    //posで置く場所指定
    ull makeReverse(ull myBoard, ull enemyBoard, ull pos) {
        int i;
        ull mask, rev = 0, rev_cand;

        // 右方向
        rev_cand = 0;
        mask = 0x7e7e7e7e7e7e7e7e;
        for (i = 1; ((pos >> i) & mask & enemyBoard) != 0; i++) {
            rev_cand |= (pos >> i);
        }
        if (((pos >> i) & myBoard) != 0) rev |= rev_cand;

        // 左方向
        rev_cand = 0;
        mask = 0x7e7e7e7e7e7e7e7e;
        for (i = 1; ((pos << i) & mask & enemyBoard) != 0; i++) {
            rev_cand |= (pos << i);
        }
        if (((pos << i) & myBoard) != 0) rev |= rev_cand;

        // 上方向
        rev_cand = 0;
        mask = 0x00ffffffffffff00;
        for (i = 1; ((pos << 8 * i) & mask & enemyBoard) != 0; i++) {
            rev_cand |= (pos << 8 * i);
        }
        if (((pos << 8 * i) & myBoard) != 0) rev |= rev_cand;

        // 下方向
        rev_cand = 0;
        mask = 0x00ffffffffffff00;
        for (i = 1; ((pos >> 8 * i) & mask & enemyBoard) != 0; i++) {
            rev_cand |= (pos >> 8 * i);
        }
        if (((pos >> 8 * i) & myBoard) != 0) rev |= rev_cand;

        // 右上方向
        rev_cand = 0;
        mask = 0x007e7e7e7e7e7e00;
        for (i = 1; ((pos << 7 * i) & mask & enemyBoard) != 0; i++) {
            rev_cand |= (pos << 7 * i);
        }
        if (((pos << 7 * i) & myBoard) != 0) rev |= rev_cand;

        // 左上方向
        rev_cand = 0;
        mask = 0x007e7e7e7e7e7e00;
        for (i = 1; ((pos << 9 * i) & mask & enemyBoard) != 0; i++) {
            rev_cand |= (pos << 9 * i);
        }
        if (((pos << 9 * i) & myBoard) != 0) rev |= rev_cand;

        // 右下方向
        rev_cand = 0;
        mask = 0x007e7e7e7e7e7e00;
        for (i = 1; ((pos >> 9 * i) & mask & enemyBoard) != 0; i++) {
            rev_cand |= (pos >> 9 * i);
        }
        if (((pos >> 9 * i) & myBoard) != 0) rev |= rev_cand;

        // 左下方向
        rev_cand = 0;
        mask = 0x007e7e7e7e7e7e00;
        for (i = 1; ((pos >> 7 * i) & mask & enemyBoard) != 0; i++) {
            rev_cand |= (pos >> 7 * i);
        }
        if (((pos >> 7 * i) & myBoard) != 0) rev |= rev_cand;

        return rev;
    }

    //石の設置（リバースあり,チェック無し
    void putBlack(ull pos) {
        ull putBoard = makeReverse(black, white, pos);
        black |= putBoard | pos;
        white ^= putBoard;
    }

    //石の設置（リバースあり,チェック無し
    void putWhite(ull pos) {
        ull putBoard = makeReverse(white, black, pos);
        white |= putBoard | pos;
        black ^= putBoard;
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

    //横になければtrue
    bool checkSideFree(ull pos) {
        ull mask = 0x8181818181818181;
        return (pos & mask) == 0;
    }

    //上下の辺に位置してたらtrue
    bool checkUpSideNow(ull pos) {
        ull mask = 0xff000000000000ff;
        return (pos & mask) == 0;
    }

    //posの座標から左右にビットを立てる
    ull sideUpBit(ull pos) {
        ull sideLMask = 0x8080808080808080;
        ull sideRMask = 0x0101010101010101;
        ull tmp;
        ull sum = pos;  //結果
        tmp = pos & ~sideLMask;  //左辺にある箇所を排除
        sum |= tmp << 1ull;     //左シフト
        tmp = pos & ~sideRMask;
        sum |= tmp >> 1ull;     //右シフト
        return sum;
    }

    //指定した場所の周りの空きマスの場所取得
    ull getAroundFree(ull pos) {
        //右シフト、左シフト, -8, +8
        ull sideLMask = 0x8080808080808080;
        ull sideRMask = 0x0101010101010101;
        ull sideUMask = 0xff00000000000000;
        ull sideDMask = 0x00000000000000ff;
        ull tmp;
        ull sum = pos;  //結果
        sum |= sideUpBit(pos);  //中心左右

        tmp = pos & ~sideUMask;
        tmp = tmp << 8ull;
        sum |= sideUpBit(tmp);  //上

        tmp = pos & ~sideDMask;
        tmp |= tmp >> 8ull;
        sum |= sideUpBit(tmp);  //下
        sum = sum & getFree();
        return sum;
    }

    int cntAroundFree(ull pos) {
        return cntBits(getAroundFree(pos));
    }

    //開放度探索
    void openMinSearch() {
        stack<int> stc; //下位から開放度を積み上げてく
        ull canPut, tmpPut;
        ull myBoard, enBoard;
        if (blackTurn) {
            canPut = canBlackBoard;
            myBoard = black;
            enBoard = white;
        } else {
            canPut = canWhiteBoard;
            myBoard = white;
            enBoard = black;
        }
        cout << "解法" << endl;
        showBoard(canPut);
        while (true) {
            if (canPut == 0) break; //何処もたってないなら終了
            tmpPut = canPut & -canPut;//右の立ってるビット取り出し
            int cnt = cntAroundFree(makeReverse(myBoard, enBoard, tmpPut));
            stc.push(cnt);

            canPut = canPut & ~tmpPut;
        }
        cout << "開放度" << endl;
        while (!stc.empty()) {
            cout << stc.top() << " ";
            stc.pop();
        }
        cout << endl;
    }
};

int ctoi(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    return -1;
}

int inputNum(int mn, int mx) {
    int tmp;
    while (true) {
        cin >> tmp;
        if (tmp >= mn && tmp <= mx) {
            return tmp;
        } else {
            cout << "入力をやりなおしてください" << endl;
            cin.clear();
            cin.seekg(0);
        }
    }
}

int main() {


    Board *board = new Board;
    board->black = 0x0000181824020000;
    board->white = 0x00000607183c0000;
    //showBoard(board->getPutRtoL(board->black, board->white));
    showBoard(board->getPutBlack());

    cout << "put" << endl;
    board->reloadCanBoard();
    board->openMinSearch();


    cout << board->cnt() << endl;
    showBoard(board->black);
    showBoard(board->white);
    //showBit(board->getPutLine(board->black, board->white, 7));

    board->init();
    showBoard(board->black);
    cout.flush();
    cout.clear();
    cin.clear();/*
    while (true) {
        cout << "開放度探索:1, 設置:2" << endl;
        int tmp=0;
        //tmp = inputNum(1, 2);
        if (tmp == 1) {
            board->openMinSearch();
        }
        board->openMinSearch();
        board->inputAndPut();
        board->nextTurn();

        showBoard(board->getPutBlack());
        cout.flush();
        cout.clear();
        cin.clear();

    }
*/
    return 0;
}
//https://chessprogramming.wikispaces.com/Flipping+Mirroring+and+Rotating