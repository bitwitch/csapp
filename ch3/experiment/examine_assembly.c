
void swap_style1(long *xp, long *yp, long *zp) {
    long x = *xp;
    long y = *yp;
    long z = *zp;

    *xp = z;
    *yp = x;
    *zp = y;
}

void swap_style2(long *xp, long *yp, long *zp) {
    long tmp = *zp;

    *zp = *yp;
    *yp = *xp;
    *xp = tmp;
}



int main() {

    long a = 1;
    long b = 2;
    long c = 3;

    long x = 3;
    long y = 6;
    long z = 9;

    swap_style1(&a, &b, &c);

    swap_style2(&x, &y, &z);

    return 0;
}
