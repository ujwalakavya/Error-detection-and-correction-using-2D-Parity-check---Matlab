#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include<omp.h>
#define s 50

int m,n,a[50][50];

void parity()
{
    int count=0,i,j;    
//Add parity bit for rows(even parity )
    for(i=0;i<m;i++)
    {
        count=0;
	# pragma omp parallel for num_threads(2)
        for(j=0;j<=n;j++)
        {
            if(j<(n))
            {
                if(a[i][j]== 1)
		#pragma omp atomic
                count++;
            }

        }
       //printf("%d\n",count);
            if((count%2) == 0)
            {
                a[i][n]= 0;
            }
            else
            {
                a[i][n]=1;
            }

    }
//Add parity bit for columns (even parity)
    for(i=0;i<=n;i++)
    {
        count=0;
	# pragma omp parallel for num_threads(2)
        for(j=0;j<m;j++)
        {
            if(a[j][i]==1)
		#pragma omp atomic 
                count++;
        }
        //printf("%d\n",count);
            if((count%2) == 0)
                a[m][i] = 0;
            else
                a[m][i] = 1;
    }
    printf("After adding parity bits\n");
    for(i=0;i<=m;i++)
    {
        for(j=0;j<=n;j++)
            printf("%d \t",a[i][j]);
        printf("\n");
    }

}

void induce()
{
    int k1,k2,l,i,j;
    l=rand()%3;
    printf("error=%d\n",l);
    #pragma omg parallel for num_threads(4)
    for(i=0;i<l;i++)
    {
        k1=rand()%(m-1);
        k2=rand()%(n-1);
        if(a[k1][k2]==0)
            a[k1][k2]=1;
        else
              a[k1][k2]=0;
	#pragma omp barrier
        printf("Error at line %d\n",k1);
    }
    printf("After inducing Error\n");
    for(i=0;i<=m;i++)
    {
       for(j=0;j<=n;j++)
            printf("%d\t",a[i][j]);
        printf("\n");
    }

}

void detection()
{
    int rcount,ccount,row_er,colm_er;
    int i,j,r,c;
    row_er=0;
    colm_er=0;
    #pragma omp parallel shared(rcount)
    {
    for(i=0;i<m;i++)
    {
        rcount=0;
        for(j=0;j<n;j++)
        {
            if(a[i][j]==1)
                rcount++;
        }
        //printf("i= %d \n rcount= %d \t ccount=%d\n",i,rcount,ccount);
        if (((rcount%2) == 0 && a[i][(n)] != 0) ||((rcount%2) == 1 && a[i][(n)] != 1))
        {
            row_er++;
            r = i;
            printf("Error in row %d\n",i);
        }
    }
    }

    #pragma omp num_threads(2) shared(ccount)
    {
    for(i=0;i<n;i++)
    {
        ccount=0;
        for(j=0;j<m;j++)
        {
            if(a[j][i]==1)
                ccount++;
        }
        //printf(" i= %d \t rcount= %d \t ccount=%d \n",i,rcount,ccount);
        if(((ccount%2) == 0 && a[(m)][i] != 0) ||((ccount%2) == 1 && a[(m)][i] != 1))
        {
            colm_er++;
            c = i;
            printf("Error in column %d\n",i);
        }
    }
    }

//1 bit Error correction code
    if( (row_er==1)&&(colm_er==1))
    {
        //printf("\nError at row %d \t column %d\n",r,c);
       if(a[r][c]==1)
           a[r][c]=0;
       else
           a[r][c]=1;
       printf("Error corrected !\n");
       for(i=0;i<=m;i++)
       {
            for(j=0;j<=n;j++)
                printf("%d \t",a[i][j]);
            printf("\n");
       }
    }
    else
        printf("Number of bits corrupted = 2\n");
	
}
int main()
{
    int i,j;
    double b,e;    	

    printf("Enter the number of rows\n");
    scanf("%d",&m);
    printf("Enter the number of columns\n");
    scanf("%d",&n);
    for(i=0;i<=m;i++)
        for(j=0;j<=n;j++)
            a[i][j]=1;

   for(i=0;i<m;i++)
   {
   	#pragma omg parallel for num_threads(2)
	for(j=0;j<n;j++)
	{
	    #pragma omp critical 
            a[i][j]=(rand()%100)%2;
	}
   }

    printf("The original matrix Before calculating Parity\n");
    for(i=0;i<m;i++)
    {
       for(j=0;j<n;j++)
            printf("%d\t",a[i][j]);
        printf("\n");
    }

    b = omp_get_wtime();
    parity();//Add row and column parity 
    #pragma omp critical 
    induce();//Induces 1 or 2 bit error 
    detection();// Error detection code and correction
    e = omp_get_wtime();
    printf ("Elapsed time is: %f s\n\n", e - b);	

    return 0;
}
