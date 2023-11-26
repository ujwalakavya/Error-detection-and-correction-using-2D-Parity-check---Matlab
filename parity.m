prompt = 'Enter the number of rows';
m = input(prompt);

prompt = 'Enter the number of columns';
n = input(prompt);

a =zeros(m+1,n+1); 
x =sprintf('Enter %d * %d matrix',m,n);
disp(x)

for i= 1:(m)
    for j= 1:(n)
       a(i,j) = mod(randi(100),2);
    end
end

disp('Original matrix')
disp(a(1:m,1:n))

%Add parity bit for rows(even parity )
for i= 1:(m)
	count = 0;
	for j= 1:(n)
		if a(i,j)==1
			count=count+1;
  end 
 end
    %disp(count)
		if mod(count,2) == 0

			a(i,(n+1)) = 0;

		else

			a(i,(n+1)) = 1;
  end 
end


%Add parity bit for columns(even parity )
for i= 1:(n+1)
	count = 0;
	for j= 1:(m)
		if a(j,i)==1
			count=count+1;
  end 
 end
    %disp(count)
		if mod(count,2) == 0

			a((m+1),i) = 0;

		else

			a((m+1),i) = 1;
  end 
end
disp('After adding parity bits')
disp(a)

% 1 or 2 bit error %
for i= 1:randi(2)

k1 = randi(m);
k2 = randi(n); 

    if a(k1,k2)==0

         a(k1,k2)=1;
    else
        a(k1,k2)=0;
    end 
 x = sprintf('Error at line %d',k1);
 disp(x)
end
 disp(a)
 
% Error detection code 
row_er=0;
colm_er=0;
for i=1:m
 rcount = 0;
 ccount = 0;
 
	for j=1:n
		if a(i,j) == 1
			rcount=rcount+1;
  end            
                if a(j,i) == 1
                    ccount= ccount+1;
                end
 end
 %x= sprintf('i= %d \n rcount= %d \t ccount=%d',i,rcount,ccount);
 %disp(x)
                if ((mod(rcount,2) == 0 && a(i,(n+1)) ~= 0) ||(mod(rcount,2) == 1 && a(i,(n+1)) ~= 1))
                    row_er=row_er+1;
                    r=i;
                        x=sprintf('Error in row %d',i);
                    disp(x);
                end
   
                   if ((mod(ccount,2) == 0 && a((m+1),i) ~= 0) || (mod(ccount,2) == 1 && a((m+1),i) ~= 1))
                       colm_er=colm_er+1;
                       c=i;
                        x=sprintf('Error in column %d',i);
                        disp(x);     
                    end
end

% 1 bit Error correction code 
if( row_er==1)&&(colm_er==1)
    y=sprintf('Error at row %d \t column %d',r,c);
    disp(y);
   if a(r,c)==1
       a(r,c)=0;
   else
       a(r,c)=1;
   end
   disp("Error corrected !")
    disp(a)
else 
    y1=sprintf('Number of bits corrupted = 2');
    disp(y1);
end


