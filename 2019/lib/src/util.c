
int gcd(int a, int b)
{
    while (b != 0)
    {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

long min(long a, long b) { return (a < b ? a : b); }

long max(long a, long b) { return (a > b ? a : b); }
