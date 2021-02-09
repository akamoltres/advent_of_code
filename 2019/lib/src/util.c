
long gcd(long a, long b)
{
    while (b != 0)
    {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

long lcm(long a, long b) { return (a * b / gcd(a, b)); }

long min(long a, long b) { return (a < b ? a : b); }

long max(long a, long b) { return (a > b ? a : b); }
