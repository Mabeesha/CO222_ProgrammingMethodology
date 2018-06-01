/*
   CO222 lab03
   E/14/390
   Wijekoon D.W.M.M.p   
*/



#include<stdio.h>

int main()
{
	int c,r,i,j,R,G,B,o,k;
	scanf("%d %d",&c,&r);
	printf("%d %d\n",c,r);
    
	for(i=0;i<r;i++)
	{
		for(j=0;j<c;j++)
		{
			scanf("%d %d %d",&R,&G,&B);
			R = 255 - R;
			G = 255 - G;
			B = 255 - B;
			printf("%d %d %d\n",R,G,B);
		}

		int p = 4 - ((c*3)%4);
		//printf("Pad = %d\n",p);

		for(k=0;k<p;k++)
		{
			scanf("%d",&o);
			printf("%d\n",o);
		}


	}





}