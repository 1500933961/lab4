//test domain of global var define and local define
int a = 3;
int b = 5;

int MyAdd(int x, int y)
{
    int z = x + y;
	return z;
}

int main(){
	double aa = 1.1;
    int a = 5;
    int c = 10;
    int d = a + c * 10;
    if(a <= c)
    {
        a = a + 1;
        a = a * b;
    }
	int e = -MyAdd(a+MyAdd(1,2), MyAdd(3,4));
    return 0;
}
