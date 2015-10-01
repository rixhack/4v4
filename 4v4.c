#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "4v4.h"

int wins;
int loses;
int cards_left;
int r=0;
solution best;
int cards=4;
solution resp;
long long int nit=0;
long long int twins=0;
long long int tloses=0;


int main(){
  best.av=0;
  wins=loses=0;
  cards_left=4;
  card c1 = newCard(5,5);
  card c2 = newCard(6,3);
  card c3 = newCard(7,2);
  card c4 = newCard(4,6);
  card c5 = newCard(6,4);
  card c6 = newCard(8,2);
  card c7 = newCard(5,6);
  card c8 = newCard(4,4);
  card cardsp1[] = {c1,c2,c3,c4};
  card cardsp2[] = {c5,c6,c7,c8};
  player player1 = newPlayer(cardsp1);
  player player2 = newPlayer(cardsp2);
  
  state matchstate = newState(player1, player2, 1, 1, 2, 2, 8, 8);
  state* pState = &matchstate;
  //pState->player1.cards[0].played=1;
  //pState->player2.cards[0].played=1;
  //intPair bm = calculateBestMove(pState);
  printf("New match:\n Player 1 cards: \n");
  int i;
  for(i=0;i<4;i++){
    printf("%i: (%i,%i)\n",i,pState->player1.cards[i].pw,pState->player1.cards[i].dm);
  }
  //printf("Play card %i with %i pillz.\n",bm.i1,bm.i2);
  //computeBestValue(pState);
  //printf("Wins: %i; Loses: %i\n",wins,loses);
  //printf("Best move: (%i,%i) with %i pillz. Rate of victory: %f.\n",pState->player1.cards[best.c].pw, pState->player1.cards[best.c].dm, best.p, best.av);
  solution sol = firstRound(pState);
  printf("Best move: (%i,%i) with %i pillz. Rate of victory: %f (%lli wins and %lli loses).\n",pState->player1.cards[sol.c].pw, pState->player1.cards[sol.c].dm, sol.p, sol.av, sol.wins, sol.loses);
  printf("Number of iterations: %lli.\n",nit);
  double tprob = ((double) twins)/(twins+tloses)*100;
  printf("Total wins: %lli; Total loses: %lli; Probability of winning: %lf\n",twins,tloses,tprob);
  
  return 0;
}

card newCard(int pw, int dm){
  card c;
  c.pw = pw;
  c.dm = dm;
  c.played = 0;
  return c;
}

player newPlayer(card cards[]){
  player p;
  int i;
  for(i=0; i<4; i++){
    p.cards[i]=cards[i];
  } 
  return p;
}

state newState(player player1, player player2, int r, int t, int p1, int p2, int l1, int l2){
  state s;
  s.r = r;
  s.t = t;
  s.player1 = player1;
  s.player2 = player2;
  s.p1=p1;
  s.p2=p2;
  s.l1 = l1;
  s.l2 = l2;
  return s;
}

intPair calculateBestMove(state* s){
  float maxav = 0;
  intPair maxpair;
  int round,card1,p1,card2,p2;
  //for(round=s->r;round<5;round++){
    for(card1=0;card1<4;card1++){
      for(p1=0;p1<13;p1++){
        float accvalue = 0;
        int n =0;
        for(card2=0;card2<4;card2++){
          for(p2=0;p2<13;p2++){
            float val =0;
            s->player1.cards[card1].played = 1;
            s->player2.cards[card2].played = 1;
            if(s->player1.cards[card1].pw*(p1+1)>=s->player2.cards[card2].pw*(p2+1)){
              state stateaft = newState(s->player1, s->player2, s->r+1, s->t, s->p1-p1, s->p2-p2, s->l1, s->l2-s->player1.cards[card1].dm);
              val = stateval(&stateaft);
            } else{
                state stateaft = newState(s->player1, s->player2, s->r+1, s->t, s->p1-p1, s->p2-p2, s->l1, s->l2-s->player1.cards[card1].dm);
                val = stateval(&stateaft);
              }
              accvalue += val;
              n++;             
            }
          }
          float average = accvalue/n;
          printf("Card %i, pillz %i, average %f\n", card1, p1, average);
          if(average>maxav){
            maxav = average;
            maxpair.i1 = card1;
            maxpair.i2 = p1;
          }
        }
      }
    return maxpair;
   }

void computeBestValue(state* s){
  int c1,p1,c2,p2,res;
  for(c1=0;c1<cards_left;c1++){
    state s1 = *s;
    if(s->player1.cards[c1].played)
      continue;
    s->player1.cards[c1].played=1;
    for(p1=0;p1<=s->p1;p1++){
      state s2 = *s;
      //printf("%i",s->r);
      if(s->r==1){
        printf("Move: (%i,%i) with %i pillz. Rate of victory: %f.\n",s->player1.cards[c1].pw, s->player1.cards[c1].dm, p1, (wins+0.1)/(loses+0.1));
        if((wins+0.1)/(loses+0.1)>best.av){
          best.c=c1;
          best.p=p1;
          best.av=((double) wins+0.1)/(loses+0.1);
        }
        wins=loses=0;
      }
      for(c2=0;c2<cards_left;c2++){
        state s3 = *s;
        if(s->player2.cards[c2].played)
          continue;
        s->player2.cards[c2].played=1;
        for(p2=0;p2<=s->p2;p2++){
          state s4 = *s;
          //printf("(%i,%i),%i vs (%i,%i),%i\n",s->player1.cards[c1].pw,s->player1.cards[c1].dm,p1,s->player2.cards[c2].pw, s->player2.cards[c2].dm,p2);
          //if(s->r!=r)
            //printf("%i\n",s->r);
          //printf("%i",s->r);
          //r=s->r;
          res = fight(s,c1,p1,c2,p2);
          if(res==1){
            s->l2=s->l2-s->player1.cards[c1].dm;
            s->p1=s->p1-p1;
            s->p2=s->p2-p2;
            if(s->r<4)
              s->r++;
          } else{
            s->l1=s->l1-s->player2.cards[c2].dm;
            s->p1=s->p1-p1;
            s->p2=s->p2-p2;
            if(s->r<4)
              s->r++;
          }
          if(s->l2<=0){
            //printf("P2KO");
            wins++;
            //break;
          } else if(s->l1<=0){
            //printf("P1KO");
            loses++;
            //break;
          } else if(s->r==4){
              if(s->l1>=s->l2)
                wins++;
              else
                loses++;
          } else{
            //updateCards(s,c1,c2);
            computeBestValue(s);
          }
        s = &s4;
        }
      s = &s3;
      }
    s = &s2;
    }
  s = &s1;
  }
}

void updateCards(state* s, int c1, int c2){
  s->player1.cards[c1].played=1;
  s->player2.cards[c2].played=1;
  int i;
  int j=0;
  int k=0;
  for(i=0;i<cards_left;i++){
    if(s->player1.cards[i].played=0){
      s->player1.avcards[j]=s->player1.cards[i];
      j++;
    }
    if(s->player2.cards[i].played=0){
      s->player2.avcards[k]=s->player2.cards[i];
      k++;
    }
  }
}

int fight(state* s, int c1, int p1, int c2, int p2){
  int res;
  if(s->player1.cards[c1].pw*(p1+1)>=s->player2.cards[c2].pw*(p2+1))
    res=1;        
  else
    res=0;
  return res;
}



float stateval(state* s){
  return ((s->p1*s->l1)+0.1)/((s->p2*s->l2)+0.1);
}

solution firstRound(pstate s){
  longintPair wl;
  long long int wlw,wll,wlwmax;
  int c1,p1,c2,p2,l1,l2,res;

  for(c1=0;c1<cards;c1++){
    s->player1.cards[c1].played=1;
    printf("Play card %i: (%i,%i)",c1, s->player1.cards[c1].pw, s->player1.cards[c1].dm);
    for(p1=0;p1<=s->p1;p1++){
      printf(" with %i pillz",p1);
      for(c2=0;c2<cards;c2++){
        s->player2.cards[c2].played=1;
        printf(" vs card %i: (%i,%i)",c2,s->player2.cards[c2].pw,s->player2.cards[c2].dm);
        for(p2=0;p2<=s->p2;p2++){
          printf(" with %i pillz.\n",p2);
          wlw=wll=0;
          wlwmax=0;
          res = fight(s,c1,p1,c2,p2);
          s->p1-=p1;
          s->p2-=p2;
          if(res){
           l2=s->l2;
           s->l2-=s->player1.cards[c1].dm;
          }else{
            l1=s->l1;
            s->l1-=s->player2.cards[c2].dm;
          }
          wl = secondRound(s);
          wlw = wl.i1;
          wll = wl.i2;
          s->p1+=p1;
          s->p2+=p2;
          s->l1=l1;
          s->l2=l2;
        }
      }
      if(wlw>wlwmax){
        wlwmax=wlw;
        resp.wins=wlw;
        resp.loses=wll;
        resp.av=((double) wlw)/(wll+wlw)*100;
        resp.c=c1;
        resp.p=p1;
        printf("Best move: (%i,%i) with %i pillz. Rate of victory: %f.\n",s->player1.cards[resp.c].pw, s->player1.cards[resp.c].dm, resp.p, resp.av);
      }
      s->player2.cards[c2].played=0;
    }
    s->player1.cards[c1].played=0;
    //printf("Wins until now: %lli",wl.i1);
  }
  return resp;
}

longintPair secondRound(pstate s){
  long long int twins2,tloses2;
  twins2=tloses2=0;
  longintPair t2;
  longintPair wl;
  long long int wlw,wll;
  int c1,p1,c2,p2,l1,l2,res;
  wlw=wll=0;
  for(c1=0;c1<cards;c1++){
    if(s->player1.cards[c1].played)
      continue;
    s->player1.cards[c1].played=1;
    for(p1=0;p1<s->p1;p1++){
      for(c2=0;c2<cards;c2++){
        if(s->player2.cards[c2].played)
          continue;
        s->player2.cards[c2].played=1;
        for(p2=0;p2<s->p2;p2++){
          res = fight(s,c1,p1,c2,p2);
          s->p1-=p1;
          s->p2-=p2;
          if(res){
           l2=s->l2;
           s->l2-=s->player1.cards[c1].dm;
           if(s->l2<=0)
            wlw++;
            twins++;
            twins2++;
          } else{
            l1=s->l1;
            s->l1-=s->player2.cards[c2].dm;
            if(s->l1<=0)
              wll++;
              tloses++;
              tloses2++;
            }
          wl = thirdRound(s);
          wl.i1+=wlw;
          wlw=0;
          wl.i2+=wll;
          wll=0;
          s->p1+=p1;
          s->p2+=p2;
          s->l1=l1;
          s->l2=l2;
        }
        s->player2.cards[c2].played=0;
      }
    }
    s->player1.cards[c1].played=0;
  }
  t2.i1=twins2;
  t2.i2=tloses2;
  //return wl;
  return t2;
}

longintPair thirdRound(pstate s){
  long long int twins3,tloses3;
  twins3=tloses3=0;
  longintPair t3;
  longintPair wl;
  long long int wlw,wll;
  int c1,p1,c2,p2,l1,l2,res;
  wlw=wll=0;
  for(c1=0;c1<cards;c1++){
    if(s->player1.cards[c1].played)
      continue;
    s->player1.cards[c1].played=1;
    for(p1=0;p1<s->p1;p1++){
      for(c2=0;c2<cards;c2++){
        if(s->player2.cards[c2].played)
          continue;
        s->player2.cards[c2].played=1;
        for(p2=0;p2<s->p2;p2++){
          res = fight(s,c1,p1,c2,p2);
          s->p1-=p1;
          s->p2-=p2;
          if(res){
           l2=s->l2;
           s->l2-=s->player1.cards[c1].dm;
           if(s->l2<=0)
            wlw++;
            twins++;
            twins3++;
          } else{
            l1=s->l1;
            s->l1-=s->player2.cards[c2].dm;
            if(s->l1<=0)
              wll++;
              tloses++;
              tloses3++;
            }
          wl = fourthRound(s);
          wl.i1+=wlw;
          wlw=0;
          wl.i2+=wll;
          wll=0;
          s->p1+=p1;
          s->p2+=p2;
          s->l1=l1;
          s->l2=l2;
        }
        s->player2.cards[c2].played=0;
      }
    }
    s->player1.cards[c1].played=0;
  }
  t3.i1=twins3;
  t3.i2=tloses3;
  //return wl;
  return t3;
}

longintPair fourthRound(pstate s){
  long long int twins4,tloses4;
  longintPair t4;
  twins4=tloses4=0;
  longintPair wl;
  long long int wlw,wll;
  int c1,p1,c2,p2,l1,l2,res;
  wlw=wll=0;
  wl.i1=0;
  wl.i2=0;
  for(c1=0;c1<cards;c1++){
    if(s->player1.cards[c1].played)
      continue;
    s->player1.cards[c1].played=1;
    for(p1=0;p1<s->p1;p1++){
      for(c2=0;c2<cards;c2++){
        if(s->player2.cards[c2].played)
          continue;
        s->player2.cards[c2].played=1;
        for(p2=0;p2<s->p2;p2++){
          res = fight(s,c1,p1,c2,p2);
          s->p1-=p1;
          s->p2-=p2;
          if(res){
           l2=s->l2;
           s->l2-=s->player1.cards[c1].dm;
           if(s->l2<=0)
            wlw++;
            twins++;
            twins4++;
          } else{
            l1=s->l1;
            s->l1-=s->player2.cards[c2].dm;
            if(s->l1<=0)
              wll++;
              tloses++;
              tloses4++;
            }
          //wl = fourthRound(s);
          wl.i1+=wlw;
          wlw=0;
          wl.i2+=wll;
          wll=0;
          s->p1+=p1;
          s->p2+=p2;
          s->l1=l1;
          s->l2=l2;
        }
        s->player2.cards[c2].played=0;
      }
    }
    s->player1.cards[c1].played=0;
  }
  nit++;
  t4.i1=twins4;
  t4.i2=tloses4;
  //return wl;
  return t4;
}
