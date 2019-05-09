//TOOM-COOK
#include <cstdlib>  
#include <iostream>  
#include <math.h>    
#include <stdio.h>   

#define S_MAX 729  



void init(int *a, int *b){
	for(int i = 0; i < S_MAX; i++){
		a[i] = rand() % 10;
		b[i] = rand() % 10;
	}
}

void normalMul(int * a, int *b, int *r, int length){
	for(int i = 0; i < length*2; i++ )	r[i] = 0;
	for(int i = 0; i < length; i++){
		for(int j = 0; j< length; j++){
			r[i+j] += a[i] * b[j];
		}
	}

}

void carry(int *a , int length){
	int carry = 0;
	for(int i = 0; i < length; i++){
		a[i] += carry;
		if(a[i] < 0){
			carry = -(-(a[i] + 1)/10 + 1);
		}else{
			carry = a[i] / 10;
		}
		a[i] -= carry * 10;
	}
}

void printRes(int *a, int *b, int *r, int length){

	int aSize = length;
	int bSize = length;
	int rSize = length *2;
	while(a[aSize-1] == 0) 	aSize--;
	while(b[bSize-1] == 0) 	bSize--;
	while(r[rSize-1] == 0) 	rSize--;

	//print intputs firsts
	std::cout << "x =  ";
	for(int i = aSize-1; i >= 0; i--){
		std::cout << a[i];
	}
	std::cout <<std::endl;
	std::cout << "y =  ";
	for(int i = bSize-1; i >= 0; i--){
		std::cout << b[i];
	}
	std::cout <<std::endl;

	//print result
	std::cout << "Result =  ";
	for(int i = (rSize)-1; i >= 0; i--){
		std::cout << r[i];
	}
	std::cout <<std::endl;

}


void tcMul(int * m, int *n, int *r, int length){


	int *m0 = &m[0];		//right
	int *m1 = &m[length/3];		//middle
	int *m2 = &m[length *2/3];	//left

	int *n0 = &n[0];		//right
	int *n1 = &n[length/3];		//middle
	int *n2 = &n[length *2/3];	//left

	int *r0 = &r[(length / 3) *  0];     
	int *r2 = &r[(length / 3) *  2];      
	int *r4 = &r[(length / 3) *  4];      
	int r1      [(length / 3) * 2];      
	int r3      [(length / 3) * 2];  
   
	int p_m2    [length / 3];          
	int p_m1    [length / 3];          
	int p_0     [length / 3];          
	int p_1     [length / 3];         
	int p_inf   [length / 3];   
      
	int q_m2    [length / 3];          
	int q_m1    [length / 3];          
	int q_0     [length / 3];          
	int q_1     [length / 3];         
	int q_inf   [length / 3];    
      
	int r_m2    [(length / 3) * 2];   
	int r_m1    [(length / 3) * 2];   
	int r_0     [(length / 3) * 2];    
	int r_1     [(length / 3) * 2];    
	int r_inf   [(length / 3) * 2];    


	if(length <= 9){
		normalMul(m,n,r,length);
		return;
	}

	//p(0) = m(0)  and q(0) = n(0);
	for(int i = 0; i < length/3; i++){
		p_0[i] = m0[i];
		q_0[i] = n0[i];
	}

	//p_inf = m(2)  and q_inf = n(2)
	for(int i = 0; i < length/3; i++){
		p_inf[i] = m2[i];
		q_inf[i] = n2[i];
	}

	//p_(1) = m0+m1+m2  
	for(int i = 0; i < length/3; i++){
		p_1[i] = m0[i] + m1[i] + m2[i];
		q_1[i] = n0[i] + n1[i] + n2[i];
	}

	//p_(-1) = m0-m1+m2  
	for(int i = 0; i < length/3; i++){
		p_m1[i] = m0[i] - m1[i] + m2[i];
		q_m1[i] = n0[i] - n1[i] + n2[i];
	}

	//p_(-2) = m0-2m1+4m2  
	for(int i = 0; i < length/3; i++){
		p_m2[i] = m0[i] - (2*m1[i]) + (4*m2[i]);
		q_m2[i] = n0[i] - (2*n1[i]) + (4*n2[i]);
	}

	//use recursion to solve the subproblems of r = p*q;  r(0) = p(0)*q(0) .......
	//r(0) = p(0)*q(0)
	tcMul(p_0, q_0, r_0,length/3);
	//r(1) = p(1)*q(1)
	tcMul(p_1, q_1, r_1,length/3);
	//r(2) = p(inf)*q(inf)
	tcMul(p_inf, q_inf, r_inf,length/3);
	//r(-1) = p(-1)*q(-1)
	tcMul(p_m1, q_m1, r_m1,length/3);
	//r(-2) = p(-2)*q(-2)
	tcMul(p_m2, q_m2, r_m2,length/3);

	//r4 = r(inf)
	for(int i = 0; i < (length/3)*2; i++){
		r4[i] = r_inf[i];
	}
	//r0 = r(0)
	for(int i = 0; i < (length/3)*2; i++){
		r0[i] = r_0[i];
	}

        // r3 = (r(-2) - r(1)) / 3
	for(int i = 0; i < (length/3)*2; i++){
		r3[i] = r_m2[i];
		r3[i] -= r_1[i];
		r3[i] /= 3;
	}
	//r1	←	(r(1) − r(−1))/2
	for(int i = 0; i < (length/3)*2; i++){
		r1[i] = r_1[i] - r_m1[i];
		r1[i] /= 2;
	}
	//r2	←	r(−1) − r(0)
	for(int i = 0; i < (length/3)*2; i++){
		r2[i] = r_m1[i] - r_0[i];
	}
	//r3	←	(r2 − r3)/2 + 2r(∞)	
	for(int i = 0; i < (length/3)*2; i++){
		r3[i] = r2[i] - r3[i];
		r3[i] /= 2;
		r3[i] += 2*r_inf[i] ;
	}
	//r2	←	r2 + r1 − r4
	for(int i = 0; i < (length/3)*2; i++){
		r2[i] = r2[i] + r1[i] - r4[i];
	}
	//r1	←	r1 − r3
	for(int i = 0; i < (length/3)*2; i++){
		r1[i] = r1[i] - r3[i];
	}

        //res = r4 * x^4 + r3 * x^3 + r2 * x^2 + r1 * x + r0
	for(int i = 0; i < (length / 3) * 2; i++){
		 r[i + length / 3] += r1[i];
	}
	for(int i = 0; i < (length / 3) * 2; i++){
		 r[i + (length/ 3) * 3] += r3[i];
	}

}



int main()
{
	int A[S_MAX];
	int B[S_MAX];
	int RES[S_MAX * 2];
	init(A, B);
	tcMul(A, B, RES, S_MAX);
	carry(RES, S_MAX * 2);
	printRes(A,B,RES,S_MAX);

    return 0;
}









