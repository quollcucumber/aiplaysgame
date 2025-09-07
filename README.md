# AI plays game
This is my program, which is an ai that plays a simple counting game, where you start at 21, 
and count down by either one, two or three. You take turns, and the loser is the person (or AI) to reach 0.

I made three example bots. 
* One that playes randomly
* One that pays optimally
* Another that plays optimally, but if it thinks it will lose, takes a 3, to get the game over with.

My ai is a Matrix, which slowly gets better as it gets trained for longer. The final program prints out the 
Matrix, followed by the results of a round robin 100 games.

## To compile

```bash
g++ main.cpp
./a.out
```

## To run

### NIM: 

To train nim, run `./a.out nim train`
If you want to train some certain number, eg 50000, of iterations, run `./a.out nim train 50000`

If you want to play against the AI, run `./a.out nim play` - note, train before you play

### TIC TAC TOE:

To train tic tac toe, run `./a.out tictactoe train`
If you want to train some certain number, eg 50000, of iterations, run `./a.out tictactoe train 50000`

If you want to play against the AI, run `./a.out tictactoe play` - note, train before you play