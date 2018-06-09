// -*- C++ -*-
//
//============================================================================//
// Multi-Digits Calculation Library                                           //
// Version : 1.0.0                                                            //
// Developer : Shdowlink                                                      //
// Date : 2018/1/11                                                           //
// Subscribe:                                                                 //
//  This Library can calculate for 800,000 digits Addition and Substraction.  //
//============================================================================//
//
// If i complete this library, I must Rewrite with class.
// About the beginning of the function "this" makes it namespace.
// Use Vector to allocate an array.
//

#ifndef MULTI_DIGIT_CALCULATION
#define MULTI_DIGIT_CALCULATION

// This library can multi-gidits addition and mubtraction.
// Implementation of multiplication and division is not completed yet


#include <iostream>
#include <cmath>
using namespace std;
#define MAX_DIGITS 100000 //1e+5 -> 100,000
#define DIGITS 8
#define MOD 100000000 //1e+8
#define ERROR_OVER_FLOW 1e+3

typedef struct{
  bool sign;
  int digit;
  // Next, use Pointer( Vector )
  int number[MAX_DIGITS];
}multinum;

void multiprint(multinum *num);
void multiswap(multinum *num1,multinum *num2);
bool multiequal(multinum *num1,multinum *num2);
bool multibig(multinum *num1,multinum *num2);
void change_sign(multinum *num,bool change);
void restore_sign(multinum *num,bool change);
void multicheck(multinum *num);

//Multidigits Number's Pointer type
multinum* multiclear(multinum *num);
multinum* multiinput(multinum *num);      // Don't yet
multinum* multiaddition(multinum *num1,multinum *num2,bool pass);
multinum* multisubtraction(multinum *num1,multinum *num2,bool pass);
multinum* multiplication(multinum *num1,multinum *num2);    // Don't yet
multinum* multidivision(multinum *num1,multinum *num2);     // Don't yet

void multiswap(multinum *num1,multinum *num2){
  multinum *index = (multinum*)malloc(sizeof(multinum));
  *index = *num1;
  *num1 = *num2;
  *num2 = *index;
  free(index);
}

void change_sign(multinum *num,bool change = false){
  if(num->sign){
    if(num->number[MAX_DIGITS - num->digit] < 0){
      num->number[MAX_DIGITS - num->digit] *= -1;
    }
  }else{
    if(num->number[MAX_DIGITS - num->digit] > 0){
      num->number[MAX_DIGITS - num->digit] *= -1;
    }
  }
  if(change){
    num->sign = (num->sign) ? false : true;
  }
}

void restore_sign(multinum *num,bool change = false){
  if(num->sign){
    if(num->number[MAX_DIGITS - num->digit] > 0){
      num->number[MAX_DIGITS - num->digit] *= -1;
    }
  }else{
    if(num->number[MAX_DIGITS - num->digit] < 0){
      num->number[MAX_DIGITS - num->digit] *= -1;
    }
  }
  if(change){
    num->sign = (num->sign) ? false : true;
  }
}

bool multiequal(multinum *num1,multinum *num2){
  for(int i = MAX_DIGITS;i > 0;i--){
    if(num1->number[i] != num2->number[i])return false;
  }
  return true;
}

bool multibig(multinum *num1,multinum *num2){
  if(!num1->sign && num2->sign)return false;
  if(num1->digit < num2->digit)return false;
  else if(num1->digit == num2->digit){
    if(num1->number[MAX_DIGITS - num1->digit] < num2->number[MAX_DIGITS - num2->digit]){
      return false;
    }
  }
  return true;
}

multinum* multiclear(multinum *num = NULL){
  multinum *multi;
  multi = (multinum*)malloc(sizeof(multinum));
  if(multi == NULL){
    printf("Couldn't secure.");
    return NULL;
  }
  multi->sign = false;
  multi->digit = 1;
  for(int i = 0;i < MAX_DIGITS;i++){
    multi->number[i] = 0;
  }
  if(num != NULL)num = multi;
  return multi;
}

// Convert String to Integer
multinum* multiconvert(string strnum){
  multinum *num = multiclear();
  if(num == NULL)return NULL;
  if(strnum.size() > DIGITS * MAX_DIGITS){
    printf("< TOO LARGE >\n");
    return NULL;
  }
  //桁数計算
  if(strnum[0] == '-'){
    if((1 < strnum.size() % DIGITS && strnum.size() % DIGITS <= 8))num->digit = strnum.size() / 8 + 1;
    else num->digit = strnum.size() / 8;
  }else{
    if(1 <= strnum.size() && (0 < strnum.size() % DIGITS && strnum.size() % DIGITS <= 7))num->digit = strnum.size() / 8 + 1;
    else num->digit = strnum.size() / 8;
  }
  //値の確定
  for(int i = 0;i < strnum.size();i++){
    if(strnum[strnum.size() - i -1] == '-'){
      //マイナスの場合は sign = true
      num->sign = true;
      if(num->number[MAX_DIGITS - num->digit] != 0)num->number[MAX_DIGITS - num->digit] *= -1;
      else num->number[MAX_DIGITS - num->digit + 1] *= -1;
      break;
    }
    num->number[MAX_DIGITS - (i / DIGITS) - 1] += pow(10,(i % DIGITS)) * (strnum[strnum.size() - i - 1] - '0');
  }
  // cout << "Digits of Number : " << DIGITS * num->digit << endl;
  return num;
}

void check_state(multinum *num){
  printf("sign:%s\n",num->sign ? "true" : "false");
  printf("first:%d\n",num->number[MAX_DIGITS - num->digit]);
}

void multiprint(multinum *num){
  if(num == NULL){
    puts("NULL Pointer");
    return;
  }
  // check_state(num);
  for(int i = MAX_DIGITS - num->digit;i < MAX_DIGITS;i++){
    printf("%08d ",num->number[i]);
  }
  printf("\n");
}

//２数の符号が同じ場合だけ
//それ以外は、（正の数）＋（負の数）-> （正の数）ー（正の数）
multinum* multiaddition(multinum *num1,multinum *num2,bool pass = false){
  multinum *ans = multiclear();
  short cut = 0;
  bool sign = false;
  bool swaped = false;
  //subtractionからの呼び出しは直前の処理まで飛ばす
  if(pass)goto processing;
  //（負の数）＋（正の数）＝（正の数）ー（正の数）
  //（正の数）＋（負の数）＝（正の数）ー（正の数）
  if(num1->sign != num2->sign){
    if(num1->sign){
      change_sign(num1,true);
      if(!multibig(num1,num2)){
        swaped = true;
        multiswap(num1,num2);
      }
      ans = multisubtraction(num1,num2,true);
      if(swaped){
        multiswap(num1,num2);
        change_sign(ans,true);
      }
      restore_sign(num1,true);
      change_sign(ans,true);
    }else{
      change_sign(num2,true);
      if(!multibig(num1,num2)){
        swaped = true;
        multiswap(num1,num2);
      }
      ans = multisubtraction(num1,num2,true);
      if(swaped){
        multiswap(num1,num2);
        change_sign(ans,true);
      }
      restore_sign(num2,true);
    }
    return ans;
  }
  //２数の符号が一緒の時
  processing:;
  if(num1->sign && num2->sign){
    sign = true;
    num1->number[MAX_DIGITS - num1->digit] *= -1;
    num2->number[MAX_DIGITS - num2->digit] *= -1;
  }
  for(int i = MAX_DIGITS - 1 ;i >= 0;i--){
    //これ以上計算できない
    if(MAX_DIGITS - i > num1->digit && MAX_DIGITS - i > num2->digit)break;
    ans->number[i] += (num1->number[i] + num2->number[i] + cut);
    if(i == 0)break;
    if(ans->number[i] < MOD){
      cut = 0;
    }else{
      cut = 1;
      ans->number[i] -= MOD;
    }
  }
  if(ans->number[0] > MOD){
    printf("< OVER FLOW >");
    return NULL;
  }
  for(int i = 0;i < MAX_DIGITS;i++){
    if(ans->number[i] != 0){
      ans->digit = MAX_DIGITS - i;
      if(sign){
        ans->sign = true;
        ans->number[i] *= -1;
        num1->number[MAX_DIGITS - num1->digit] *= -1;
        num2->number[MAX_DIGITS - num2->digit] *= -1;
      }
      break;
    }
  }
  return ans;
}

multinum* multisubtraction(multinum *num1,multinum *num2,bool pass = false){
  multinum *ans = multiclear();
  short cut = 0;
  bool swaped = false;
  //additionからの呼び出しは直前の処理まで飛ばす
  if(pass)goto processing;
  //符号が違う場合
  if(num1->sign != num2->sign){
    if(num1->sign){
      change_sign(num1);
      ans = multiaddition(num1,num2,true);
      restore_sign(num1);
      change_sign(ans);
      return ans;
    }else{
      change_sign(num2);
      ans = multiaddition(num1,num2,true);
      restore_sign(num2);
      return ans;
    }
  }
  //大きい数 - 小さい数
  if(num1->sign && num2->sign){
    change_sign(num1);
    change_sign(num2);
    if(!multibig(num1,num2)){
      swaped = true;
      multiswap(num1,num2);
    }
    ans = multisubtraction(num1,num2,true);
    if(swaped){
      multiswap(num1,num2);
      change_sign(ans,true);
    }
    restore_sign(num1);
    restore_sign(num2);
    change_sign(ans,true);
    return ans;
  }
  //符号が同じ場合
  processing:;
  if(!multibig(num1,num2)){
    swaped = true;
    multiswap(num1,num2);
  }
  for(int i = MAX_DIGITS - 1 ;i >= 0;i--){
    //これ以上計算できない
    if(MAX_DIGITS - i > num1->digit && MAX_DIGITS - i > num2->digit)break;
    ans->number[i] = (num1->number[i] - num2->number[i] - cut);
    if(MAX_DIGITS - i > num1->digit - 1 && MAX_DIGITS - i > num2->digit - 1)break;
    if(ans->number[i] >= 0){
      cut = 0;
    }else{
      cut = 1;
      ans->number[i] += MOD;
    }
  }
  if(ans->number[0] < -MOD){
    printf("< UNDER FLOW >");
    return NULL;
  }
  for(int i = 0;i < MAX_DIGITS;i++){
    if(ans->number[i] != 0){
      ans->digit = MAX_DIGITS - i;
      break;
    }
  }
  //符号確定、桁数
  if(swaped){
    multiswap(num1,num2);
    if(!multibig(num1,num2))change_sign(ans);
  }
  return ans;
}

/*
掛け算の説明:
  大きい数 x 小さい数 にする。
  8桁を区切る -> 00 00 00 00
  桁が小さい順にかけていく。
  桁の位置によって割る数を変える。
  後から数をかけて本来の位置に戻す -> 1000000,10000,100,1
*/

multinum* multiplication(multinum *num1,multinum *num2){
  // Create new Object
  multinum *ans = multiclear();
  multinum *relay = multiclear();
  bool swaped = false;
  int temp = 0;
  int two_dig = 0;
  long long temp_ans = 0;
  if(!multibig(num1,num2)){
    swaped = true;
    multiswap(num1,num2);
  }
  if(num1->digit + num2->digit -1 > MAX_DIGITS){
    printf("< TOO LARGE ANSEWR >");
    return ans;
  }
  for(int i = 0;i < MAX_DIGITS;i++){
    temp = num2->number[i];
    for(int i = 1;i < 5;i++){
      //桁の分割
      int m = pow(10,2 * i);
      int l = pow(10,2 * (i - 1));
      two_dig = ((temp % m) - (temp % l)) / l;
      // printf("%d",((temp % m) - (temp % l)) / l);
    }
  }
  return 0;
}

#endif /* MULTI_DIGIT_CALCULATION */

