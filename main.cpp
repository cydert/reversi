#include <iostream>
#include <limits.h>
#include <bitset>
#include <queue>
#include <stack>

typedef unsigned long long ull;

class Board;

using namespace std;

int ctoi(char x);

//2�i���\��
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

    //��ʃr�b�g����
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
    bool blackTurn = true;  //�ʏ퍕����
    int nowTurn = 0;

    ull black = 0;
    ull white = 0;
    ull canBlackBoard = 0, canWhiteBoard = 0;

    //������
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
        if (blackTurn) { //���^�[����
            if (canWhiteBoard == 0) {
                cout << "���p�X" << endl;
                blackTurn = true;
            }
            blackTurn = false;
            cout << "���^�[��" << endl;
        } else {
            if (canBlackBoard == 0) {
                cout << "���p�X" << endl;
                blackTurn = false;
            }
            blackTurn = true;
            cout << "���^�[��" << endl;
        }
        showBoard(black | white);

    }

    void inputAndPut() {
        char x, y;
        if (blackTurn) cout << "���^�[��" << endl;
        else cout << "���^�[��" << endl;
        int yi;
        while (true) {
            cout << "���W���͑҂�(a�`h1�`8):";
            cin >> x >> y;
            yi = ctoi(y);
            if ('a' <= x && x <= 'h' && 0 <= yi && yi <= 8) {
                if (!canPut(makeBoard(x, yi))) {
                    cout << "�ݒu�s�ꏊ�ł�" << endl;

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

    //�΂��u����ĂȂ��ꏊ��Ԃ�
    ull getFree() {
        return ~(black | white);
    }

    //�󔒂����邩
    bool isFull() {
        return !getFree();
    }

    //�w�肵�����W��1�ɂ��ĕԂ�
    //����(0,0)���E��(0,7)
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
    //y��ڂ̂�����ꏊ
    ull getPutLine(ull myBoard, ull enemyBoard, int y) {
        /*
        y = 7 - y;    //��ʃr�b�g��y==0�Ȃ̂�
        myBoard = myBoard >> ((8 * y));
        enemyBoard = enemyBoard >> (8 * y);
        //showBit(myBoard);
        //showBit(enemyBoard);
        uint8_t m = myBoard << 1;
        uint8_t e = enemyBoard;
        return ((ull) (~(m | e) & ~(myBoard | enemyBoard) & (m + e))) << ((8 * y));

        //���������̏���
        ull w = white & 0x7e7e7e7e7e7e7e7e; //�[��1��0�A����ȊO��1�ȔՖ�
        ull t = w & (black << 1);           //���΂̍��ׂɂ��锒�΂𒲂ׂ�
        t |= w & (t << 1);                  //���ׂ̗�
        t |= w & (t << 1);                  //�ׂ́E�E�E
        t |= w & (t << 1);
        t |= w & (t << 1);
        t |= w & (t << 1);                    //��x�ɂЂ�����Ԃ���΂�6�܂�
        ull blank = ~(black | white);       // �󔒂̉ӏ�
        ull mobility = blank & (t << 1);    //����o����̂͋󔒂̃}�X����
        return mobility;
    }
*/
    //�E���獶�ɂ݂Ēu����ꏊ��T��
    ull getPutRtoL(ull myBoard, ull enemyBoard) {
        ull sum = 0;
        //���������̏���
        ull w = enemyBoard & 0x7e7e7e7e7e7e7e7e; //�[��1��0�A����ȊO��1�ȔՖ�
        ull t = w & (myBoard << 1);           //���΂̍��ׂɂ��锒�΂𒲂ׂ�
        t |= w & (t << 1);                  //���ׂ̗�
        t |= w & (t << 1);                  //�ׂ́E�E�E
        t |= w & (t << 1);
        t |= w & (t << 1);
        t |= w & (t << 1);                    //��x�ɂЂ�����Ԃ���΂�6�܂�
        ull blank = ~(myBoard | enemyBoard);       // �󔒂̉ӏ�
        ull mobility = blank & (t << 1);    //����o����̂͋󔒂̃}�X����
        return mobility;
    }

    //�΂ߕ����̂�����ꏊ
    ull getPutSlash(ull myBoard, ull enemyBoard) {
        ull sum = 0;
        // �E�����
        ull masked_enemy = enemyBoard & 0x007e7e7e7e7e7e00;
        ull t = masked_enemy & (myBoard << 7);
        for (int i = 0; i < 5; i++) {
            t |= masked_enemy & (t << 7);
        }
        sum |= (t << 7);

        // �������
        masked_enemy = enemyBoard & 0x007e7e7e7e7e7e00;
        t = masked_enemy & (myBoard << 9);
        for (int i = 0; i < 5; i++) {
            t |= masked_enemy & (t << 9);
        }
        sum |= (t << 9);

        // �E������
        masked_enemy = enemyBoard & 0x007e7e7e7e7e7e00;
        t = masked_enemy & (myBoard >> 9);
        for (int i = 0; i < 5; i++) {
            t |= masked_enemy & (t >> 9);
        }
        sum |= (t >> 9);

        // ��������
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

    //�Ίp����ɉ�](�㉺���]����̍�90)
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

    //�㉺���]
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


    //���������Ƃ����Ƃ��̂�����ꏊ(8����
    ull getPutBlack() {
        cout << "getput" << endl;
        ull sum = 0ull;
        //4����
        sum = sum | getPutRtoL(black, white);

        //��]
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
        return sum | getPutSlash(black, white);//8����
    }

    //�����𔒂Ƃ����Ƃ��̂�����ꏊ
    ull getPutWhite() {
        ull sum = 0ull;
        //4����
        sum = sum | getPutRtoL(white, black);
        //��]
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
        return sum | getPutSlash(white, black);//8����
    }

    //�΂𔽓]������
    //pos�Œu���ꏊ�w��
    ull makeReverse(ull myBoard, ull enemyBoard, ull pos) {
        int i;
        ull mask, rev = 0, rev_cand;

        // �E����
        rev_cand = 0;
        mask = 0x7e7e7e7e7e7e7e7e;
        for (i = 1; ((pos >> i) & mask & enemyBoard) != 0; i++) {
            rev_cand |= (pos >> i);
        }
        if (((pos >> i) & myBoard) != 0) rev |= rev_cand;

        // ������
        rev_cand = 0;
        mask = 0x7e7e7e7e7e7e7e7e;
        for (i = 1; ((pos << i) & mask & enemyBoard) != 0; i++) {
            rev_cand |= (pos << i);
        }
        if (((pos << i) & myBoard) != 0) rev |= rev_cand;

        // �����
        rev_cand = 0;
        mask = 0x00ffffffffffff00;
        for (i = 1; ((pos << 8 * i) & mask & enemyBoard) != 0; i++) {
            rev_cand |= (pos << 8 * i);
        }
        if (((pos << 8 * i) & myBoard) != 0) rev |= rev_cand;

        // ������
        rev_cand = 0;
        mask = 0x00ffffffffffff00;
        for (i = 1; ((pos >> 8 * i) & mask & enemyBoard) != 0; i++) {
            rev_cand |= (pos >> 8 * i);
        }
        if (((pos >> 8 * i) & myBoard) != 0) rev |= rev_cand;

        // �E�����
        rev_cand = 0;
        mask = 0x007e7e7e7e7e7e00;
        for (i = 1; ((pos << 7 * i) & mask & enemyBoard) != 0; i++) {
            rev_cand |= (pos << 7 * i);
        }
        if (((pos << 7 * i) & myBoard) != 0) rev |= rev_cand;

        // �������
        rev_cand = 0;
        mask = 0x007e7e7e7e7e7e00;
        for (i = 1; ((pos << 9 * i) & mask & enemyBoard) != 0; i++) {
            rev_cand |= (pos << 9 * i);
        }
        if (((pos << 9 * i) & myBoard) != 0) rev |= rev_cand;

        // �E������
        rev_cand = 0;
        mask = 0x007e7e7e7e7e7e00;
        for (i = 1; ((pos >> 9 * i) & mask & enemyBoard) != 0; i++) {
            rev_cand |= (pos >> 9 * i);
        }
        if (((pos >> 9 * i) & myBoard) != 0) rev |= rev_cand;

        // ��������
        rev_cand = 0;
        mask = 0x007e7e7e7e7e7e00;
        for (i = 1; ((pos >> 7 * i) & mask & enemyBoard) != 0; i++) {
            rev_cand |= (pos >> 7 * i);
        }
        if (((pos >> 7 * i) & myBoard) != 0) rev |= rev_cand;

        return rev;
    }

    //�΂̐ݒu�i���o�[�X����,�`�F�b�N����
    void putBlack(ull pos) {
        ull putBoard = makeReverse(black, white, pos);
        black |= putBoard | pos;
        white ^= putBoard;
    }

    //�΂̐ݒu�i���o�[�X����,�`�F�b�N����
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

    //���ɂȂ����true
    bool checkSideFree(ull pos) {
        ull mask = 0x8181818181818181;
        return (pos & mask) == 0;
    }

    //�㉺�̕ӂɈʒu���Ă���true
    bool checkUpSideNow(ull pos) {
        ull mask = 0xff000000000000ff;
        return (pos & mask) == 0;
    }

    //pos�̍��W���獶�E�Ƀr�b�g�𗧂Ă�
    ull sideUpBit(ull pos) {
        ull sideLMask = 0x8080808080808080;
        ull sideRMask = 0x0101010101010101;
        ull tmp;
        ull sum = pos;  //����
        tmp = pos & ~sideLMask;  //���ӂɂ���ӏ���r��
        sum |= tmp << 1ull;     //���V�t�g
        tmp = pos & ~sideRMask;
        sum |= tmp >> 1ull;     //�E�V�t�g
        return sum;
    }

    //�w�肵���ꏊ�̎���̋󂫃}�X�̏ꏊ�擾
    ull getAroundFree(ull pos) {
        //�E�V�t�g�A���V�t�g, -8, +8
        ull sideLMask = 0x8080808080808080;
        ull sideRMask = 0x0101010101010101;
        ull sideUMask = 0xff00000000000000;
        ull sideDMask = 0x00000000000000ff;
        ull tmp;
        ull sum = pos;  //����
        sum |= sideUpBit(pos);  //���S���E

        tmp = pos & ~sideUMask;
        tmp = tmp << 8ull;
        sum |= sideUpBit(tmp);  //��

        tmp = pos & ~sideDMask;
        tmp |= tmp >> 8ull;
        sum |= sideUpBit(tmp);  //��
        sum = sum & getFree();
        return sum;
    }

    int cntAroundFree(ull pos) {
        return cntBits(getAroundFree(pos));
    }

    //�J���x�T��
    void openMinSearch() {
        stack<int> stc; //���ʂ���J���x��ςݏグ�Ă�
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
        cout << "��@" << endl;
        showBoard(canPut);
        while (true) {
            if (canPut == 0) break; //�����������ĂȂ��Ȃ�I��
            tmpPut = canPut & -canPut;//�E�̗����Ă�r�b�g���o��
            int cnt = cntAroundFree(makeReverse(myBoard, enBoard, tmpPut));
            stc.push(cnt);

            canPut = canPut & ~tmpPut;
        }
        cout << "�J���x" << endl;
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
            cout << "���͂����Ȃ����Ă�������" << endl;
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
        cout << "�J���x�T��:1, �ݒu:2" << endl;
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