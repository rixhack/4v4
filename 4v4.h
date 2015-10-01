typedef char* sstring;

typedef struct{
  int i1;
  int i2;
} intPair;

typedef struct{
  long long int i1;
  long long int i2;
} longintPair;

typedef struct{
  int pw;
  int dm;
  int played;
} card;

typedef card* pcard;

typedef struct{
  card cards[4];
  card avcards[4];
} player;

typedef struct{
  int r;
  int t;
  int p1;
  int l1;
  int p2;
  int l2;
  player player1;
  player player2;
} state;

typedef state* pstate;

typedef struct{
  long long int wins;
  long long int loses;
  double av;
  int c;
  int p;
} solution;
  

//extern int wins,loses,cards_left;

void newMatch(state*);
card newCard(int, int);
player newPlayer(card[]);
state newState(player, player, int, int, int, int, int, int);
int fight(state*, int, int, int, int);
float stateval(state*);
intPair calculateBestMove(state*);
void computeBestValue(state*);
void updateCards(state*,int,int);
solution firstRound(pstate);
longintPair secondRound(pstate);
longintPair thirdRound(pstate);
longintPair fourthRound(pstate);
