#include <stdio.h>
#include <math.h>
#define PI 3.14159
int main(){
	double left=-1.0*PI;
	double right=PI;
	double mid=0.0;
	double k;
	scanf("%lf",&k);

	while(fabs(right-left)>0.000000000001){
		mid=(left+right)/2;
		double aa=sin(mid)+tan(mid);
		if(aa>k){
			right=mid;
		}
		else{
			left=mid;
		}
	}

	printf("%.3lf\n", right);
}
