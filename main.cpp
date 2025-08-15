#include <bits/stdc++.h>
using namespace std;
enum whosturn {
    player1,player2,gameover
};
enum whowon {
    p1,p2,draw
};
uniform_real_distribution<> dis(0.0, 1.0);
mt19937 Rand(time(nullptr));
struct Matrix {
    int numrows;
    int numcolumns;
    vector<double> numbers;
    Matrix(int numrows_, int numcolumns_) :numrows{numrows_}, numcolumns{numcolumns_} {numbers.resize(numcolumns * numrows);}
    double &get(int row, int col) {
        return numbers[col * numrows + row];
    }
    vector<double> multiply(vector<double> &v) {
        vector<double> out;
        out.resize(numrows);
        assert(v.size() == numcolumns);
        for(int i = 0; i < out.size(); i++) {
            for(int j = 0; j < numcolumns; j++) {
                out[i] += v[j] * get(i, j);
            }
        }
        return out;
    }
    void makerandom() {
        for(int i = 0; i < numbers.size(); i++) {
            numbers[i] = 1 - dis(Rand) * 2;
        }
    }
    void print() {
        for(int i = 0; i < numrows; i++) {
            for(int j = 0; j < numcolumns; j++) {
                cout<<get(i, j)<<' ';
            }
            cout<<'\n';
        }
    }
};
struct gamestate {
    int left;
    whosturn turn;
    whowon whodidwin;
    void reset() {
        left= 21;
        turn = player1;
        whodidwin = draw;
    }
    void domove(int move) {
        left -= (move+1);
        if(turn == player1) {
            turn = player2;
            if(left <= 0) {
                whodidwin = p2;
                turn = gameover;
            }
        }else {
            turn = player1;
            if(left <= 0) {
                whodidwin = p1;
                turn = gameover;
            }
        }
    }
    int numberofmoves(){
        return 3;
    }
    vector<bool> checklegal() {
        vector<bool> ans;
        for(int i = 0; i < numberofmoves(); i++) {
            if(left - i > 0) {
                ans.push_back(1);
            }else {
                ans.push_back(0);
            }
        }
        return ans;
    }
};
int randmove(gamestate*game){
    int move = Rand() % game->numberofmoves();
    while(!game->checklegal()[move]) {
        move = Rand() % game->numberofmoves();
    }
    return move;
}
int optimalmove(gamestate*game) {
    int currentposition = game->left % (game->numberofmoves() + 1);
    if(currentposition == 1) {
        return 0;
    }else {
        currentposition += 4;
        currentposition = currentposition - 2;
        currentposition = currentposition % 4;
        if(game->checklegal()[currentposition]) {
            return currentposition;
        }else {
            cout<<"line 42 wrong\n";
            assert(1 == 2);
        }
    }
}
whowon thegame(int ai1(gamestate *),int ai2(gamestate *)){
    gamestate game;
    game.reset();
    while(game.turn != gameover) {
        if(game.turn == player1) {
            game.domove(ai1(&game));
        }else {
            game.domove(ai2(&game));
        }
    }
    return game.whodidwin;
}
double hundredgames(int ai1(gamestate *), int ai2(gamestate * )) {
    double score = 0; // player 1;
    for(int i = 0; i < 100; i ++) {
        switch(thegame(ai1, ai2)) {
            case p1: score++; break;
            case p2: break;
            case draw: score += 0.5; break;
            // default: score += 10000;
        }
    }
    return score;
}
int pestimalmove(gamestate*game) {
    int currentposition = game->left % (game->numberofmoves() + 1);
    if(currentposition == 1) {
        return min(game->numberofmoves()-1, game->left-1);
    }else {
        currentposition += 4;
        currentposition = currentposition - 2;
        currentposition = currentposition % 4;
        if(game->checklegal()[currentposition]) {
            return currentposition;
        }else {
            cout<<"line 58 wrong\n";
            assert(1 == 2);
        }
    }
}
Matrix aim(3, 21);
struct aidecision {
    vector<double> gamestate;
    vector<double> output;
    int choice;
};
aidecision aimove2(gamestate *game) {
    vector<double> state;
    for(int i = 1; i < 22; i++) {
        state.push_back(game->left == i);
    }
    double sum = 0;
    vector<bool> legal = game->checklegal();
    vector<double> ans = aim.multiply(state);
    for(int i = 0; i < ans.size(); i++) {
        ans[i] = exp(min(ans[i], 5.0));
        if(legal[i]) sum += ans[i];
    }
    double a = dis(Rand) * sum;
    for(int i = 0; i < ans.size(); i++){
        if(legal[i]) a -= ans[i];
        if(a <= 0) {
            return {state, ans, i};
        }
    }
    assert(1 == 2);
}
int aimove(gamestate *game) {
    return aimove2(game).choice;
}
double trainspeed = 0.001;
void changeweights(aidecision ai, bool good) {
    double change = good ? 1.0: -1.0;
    for(int row = 0; row < aim.numrows; row++) {
        double rowchange = row == ai.choice ? 1.0 : -0.5;
        for(int col = 0; col < aim.numcolumns; col++) {
            double dpdm = ai.gamestate[col] * ai.output[row];
            aim.get(row, col) += rowchange * change * dpdm * trainspeed;
        }
    }
}
void aitrain() {
    vector<aidecision> ai1;
    vector<aidecision> ai2;
    gamestate game;
    game.reset();
    while(game.turn != gameover) {
        if(game.turn == player1) {
            aidecision d = aimove2(&game);
            game.domove(d.choice);
            ai1.push_back(d);
        }else {
            aidecision d = aimove2(&game);
            game.domove(d.choice);
            ai2.push_back(d);
        }
    }
    for(int i = 0; i < ai1.size(); i++) {
        changeweights(ai1[i], game.whodidwin== p1);
    }
    for(int i = 0; i < ai2.size(); i++) {
        changeweights(ai2[i], game.whodidwin == p2);
    }
}
typedef int (*aifunction)(gamestate *);

void roundrobin(vector<aifunction> players, vector<string> playernames) {
    for(int i = 0; i < players.size(); i++) {
        for(int j = 0; j < players.size(); j++) {
            cout<<"Player " << playernames[i] <<" played against player " << playernames[j] <<" and " <<playernames[i]<<" got "<< hundredgames(players[i], players[j])<<" points.\n";
        }
    }
}

int main(){
    aim.makerandom();
    for(int i  = 0; i < 100000; i++) aitrain();
    cout<<"MATRIX: \n";
    aim.print();
    cout<<'\n';
    vector<aifunction> ais;
    ais.push_back(randmove);
    ais.push_back(optimalmove);
    ais.push_back(pestimalmove);
    ais.push_back(aimove);
    roundrobin(ais, {"Rand", "Optimistic", "Pesimistic", "HAL9000"});
    return 0;
}
