#include "stdio.h"
#include <math.h>
#include <string.h>

int __attribute__((optimize("O0"))) mod(int a, int b) {
    int result = a % b;
    if (result < 0)
        result += b;
    return result;
}

int __attribute__((optimize("O0"))) msgToInt(char s[]){
    int val = 0;
    for(size_t i = 0;i < strlen(s);i++){
        val += (int) pow(2,(int) s[i] - 'a');
    }
    return val;
}

int __attribute__((optimize("O0"))) strToMsg(char input[], int msgs[2][2], int index) {
    char beforePipe[10000] = "";
    char afterPipe[10000] = "";
    int pipe = 0;
    int pipePos;
    for(int i = 0; i<(int)strlen(input);i++){
        if(input[i] == '|' && !pipe){
            pipe = 1;
            pipePos = i;
        } else if(input[i] >= 'a' && input[i] <= 'z'){
            if(!pipe){
                beforePipe[i] = input[i];
            }else{
                afterPipe[i-pipePos-1] = input[i];
            }
        } else if(input[i] == '\n'){
            if(i == 1) return 1;
            break;
        } else{
            return 1;
        }
    }
    if(pipe && strcmp(input,"|") != 1){
        msgs[index][0] = strcmp(beforePipe,"") != 0 ? msgToInt(beforePipe) : 0;
        msgs[index][1] = strcmp(afterPipe,"") != 0 ? msgToInt(afterPipe) + msgs[index][0] : msgs[index][0];
        return 0;
    }
    return 1;
}

void __attribute__((optimize("O0"))) extendedGcd(int a, int b, int * gcd, int * alpha, int * beta){
    if(a==0){
        *gcd = b;
        *alpha = 0;
        *beta = 1;
        return;
    }
    if(b==0){
        *gcd = a;
        *alpha = 1;
        *beta = 0;
        return;
    }
    int lastB;
    int q;

    int alphaPrev = 1;
    int alphaCur = 0;
    int alphaTmp;

    int betaPrev = 0;
    int betaCur = 1;
    int betaTmp;
    while(1){
        lastB = b;
        q = (int) a/b;
        b = a % b;
        a = lastB;
        if(b == 0) {
            *gcd = a;
            *alpha = alphaCur;
            *beta = betaCur;
            return;
        }
        alphaTmp = alphaCur;
        alphaCur = alphaPrev - q * alphaCur;
        alphaPrev = alphaTmp;

        betaTmp = betaCur;
        betaCur = betaPrev - q * betaCur;
        betaPrev = betaTmp;
    }
}

int __attribute__((optimize("O0"))) solve(int congruence1[2],int congruence2[2], int solution[2]){
    int a = congruence1[0];
    int m = congruence1[1];

    int b = congruence2[0];
    int n = congruence2[1];

    int moduloGcd, alpha, beta;
    extendedGcd(m, n, &moduloGcd, &alpha, &beta);

    if((a-b) % moduloGcd != 0) return 1;
    int moduloLcm = ((int) (m / moduloGcd)) * n;
    int k = (int)((a - b) / moduloGcd);
    int res = a - m * alpha * k;

    solution[0] = mod(res,moduloLcm);
    solution[1] = moduloLcm;
    return 0;
}

int __attribute__((optimize("O0"))) main(){
    char input[10000] = "";
    int congruences[2][2];
    int index = 0;
    printf("Zpravy:\n");
    while(index<2){
        fgets(input, 10000, stdin);
        if(strToMsg(input,congruences,index++)){
            printf("Nespravny vstup.\n");
            return 1;
        }
    }
    int solution[2];
    if(solve(congruences[0],congruences[1],solution)){
        printf("Nelze dosahnout.\n");
        return 0;
    }
//     for multiple congruences run this
//    for(int i = 2; i<index;i++){
//        solve(solution,congruences[i],solution);
//    }
    printf("Synchronizace za: %d\n",solution[0]);
    return 0;
}
