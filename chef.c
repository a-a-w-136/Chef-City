/* Solution to the Chef scenario.
To determine what the minimum amount of time it takes to get to a point in chef city the following questions
must be answered:
1. Is the point given a station?
2. If the point is not a station, is it quicker to catch a train to the next
   station and walk back or walk from the current station.

To determine 1, consider the sequence of numbers that represent stations on the number line:
                             1, 3, 6, 10, 15 ...
1 = Station 1, 3 = Station 2, 6 = Stations 3 ...

Asub_k = point of station k on the number line.

Asub_1 = 1
Asub_2 = 1 + 2 = 3
Asub_3 = 3 + 3 = 6

Therefore:
    Asub_k = Asub_kminus1 + k

Given a point X, iterate over the recurrence relation. Compare X with Asub_k. X is not a station IFF Asub_k > X .

To determine 2, it makes sense that chef should travel from the closet station to that point.

- Given 2 adjacent stations S1 and S2.
- S2 - S1 = Walking distance from S1 to S2 = N
- N can be even or odd

When N is even.
- The number of points between S1 and S2 is odd
- There is a middle point, M.
- Both S1 and S2 are equal distances away from M.
- Walking from S1 to M takes T minutes
- Walking from S2 to M takes T minutes
- To get to S2 chef must catch a train from S1, taking 1 minute
- To get to the mid point via S2 takes T + 1 minutes
- To get to point M - 1 from S1 takes T - 1 minutes
- To get to M - 1 via S2 takes T + 1 + 1
- To get to point M + 1 from S1 takes T + 1 minutes
- To get to point M + 1 via S2 takes T + 1 - 1 minutes
- Therefore when the number of points between stations is odd, chef should travel from the 
  closet station to that point. When the point is equal distances from both stations, chef
  should travel from closer of the two station, S1.

When N is odd.
- The number of points between S1 and S2 is even
- There is no middle point M
- There are N - 1 points between S1 and S2
- S1 + ([N - 1] / 2) = P1 = the furthest point from S1 without being closet to S2
- S1 + ([N - 1] / 2) + 1 = P2 = the furthest point from S2 without being closet to S1
- S1 + ([N - 1] / 2) + 2 = P3 = the 2nd furthest point from S2 without being closet to S1
- (N - 1) / 2 = T = time to get from S1 to P1
- (N - 1) / 2 = T = time to get from S2 to P2
- To get to S2 chef must catch a train from S1, taking 1 minute
- To get to P2 via S2 takes T + 1 minutes
- To get to P2 from S1 takes T + 1 minutes
- To get to P1 via S2 takes T + 1 + 1 minutes
- To get to P1 from S1 takes T minutes
- To get to P3 from S1 takes T + 1 + 1 minutes
- To get to P3 from S2 takes T + 1 - 1 minutes
- Therefore when the number of points between stations is even, chef should walk from 
  the station that is closet to that point. However when Pk = S1 + ([N - 1] / 2) + 1,
  chef has the option of travelling from either station.
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MIN_INPUT 1
#define XMAX 1e+09
#define TMAX 200
#define NUM_TEST_CASES_DIGITS 3
#define NUM_TEST_CASE_DIGITS 10

long input(long max_val, int max_digits);

int main(void) {
    unsigned short T;
    unsigned long *X;
    unsigned long *point;
    int cur = 0;
    unsigned long minutes = 1;    
    
    while(!(T = (short)input(TMAX , NUM_TEST_CASES_DIGITS))) { }        /*Get number of test cases*/
    
    X = calloc(T, sizeof(long));        /*Allocate space for each test case*/                 
    point = X;
    while(cur < T) {
        while(!(*point = (unsigned long)input(XMAX , NUM_TEST_CASE_DIGITS))) { } /*Get each test case*/ 
        point++;
        cur++;
    }
    
    point = X;
    cur = 0;
    while(cur < T) {        /*Iterate test cases*/
        unsigned int Asub_1 = 1;
        unsigned long Asub_kminus1 = Asub_1;
        unsigned long Asub_k = Asub_1;
        unsigned int k = 1;
        while(Asub_k < *point) {        /*Iterate over each recurrence relation*/
            k++;
            Asub_k = Asub_kminus1 + k;    /*Asub_k is the point on the number line of station k*/
            if(Asub_k > XMAX) {     /*Asub_k is past XMAX. Therefore calculate minutes from Asub_kminus1.*/
                minutes += *point - Asub_kminus1;
                break;
            }
            if(Asub_k > *point) {       /*Asub_k is past the test point. Determine if k is even or odd*/
                if((k % 2) == 0) {      
                    if((Asub_k - *point) >= ceil((float)k/2)){
                        minutes += *point - Asub_kminus1;
                    }
                    else {
                        minutes += (Asub_k - *point) + 1;
                    }
                    break;
                }  
                else {
                    if((Asub_k - *point) > k/2) {
                        minutes += *point - Asub_kminus1;
                    }
                    else {
                        minutes += (Asub_k - *point) + 1;
                    }
                    break;
                }
            }
            minutes++;
            Asub_kminus1 = Asub_k;
        }
        printf("%lu\n", minutes);
        minutes = 1;
        ++point;
        ++cur;
    }
free(X);
}

long input(long max_val, int max_digits) {
    char keys[12] = {0};
    char *ptr = NULL;
    long ret;
    char key;
    unsigned short num_keys_pressed = 0;
    unsigned short chars_less_than_three = 0;
    
    /*Get user input*/
    for(unsigned short count = 0; count < max_digits + 1; count++) {
        key = getchar();
        ++num_keys_pressed;
        keys[count] = key;
        if(key == '\n') {
            chars_less_than_three = 1;
            break;
        }
    }
    
    /*Test for at least one potential valid character*/
    if(keys[0] == '\n') {      
        printf("Min: 1\n");
        return 0;
    }
    
    /*Test that the user hasn't entered too many characters*/
    if(!(chars_less_than_three)) {
        printf("Max: %d\n", max_digits);
        int c;
        while((c = getchar()) != '\n') { }      /*Clear the buffer*/
        return 0;
    }
    
    /*Test for valid characters. The last key is necessarily 'enter'.*/
    for(unsigned short count = 0; count < num_keys_pressed - 1; count++) {
        if(keys[count] < '0' || keys[count] > '9') {
            printf("Illegal Digit Used: 0 - 9 Only\n");
            return 0;
        }
    }
    
    /*Convert to type long. Test the number is within bounds*/
    ret = strtol(keys, &ptr, 10);
    if (ret < MIN_INPUT || ret > max_val) {
        printf("Min: 1 : Max: %ld\n", max_val);
        return 0;
    }
    return ret;
}     