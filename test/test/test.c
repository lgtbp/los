
int test(int a, int b, int ff)
{
    int c = 1;
    c = ff + b;
    c++;
    return a + c;
}
extern void functions(int, int, int);
int main(void)
{
    unsigned int a = 4, b = 3;
    b = test(a, b, 6);
    switch (b)
    {
    case 0:
        a += 2;
        break;
    case 2:
        a += 8;
        break;
    default:
        a += 10;
        break;
    }
    if (a == 0)
    {
        b += 13;
    }
    else
    {
        a--;
        b += 10;
        b += 4;
        b /= 7;
    }
    printf("%d,%d,%d\n",b, b * 25, 26);
    return b;
}

