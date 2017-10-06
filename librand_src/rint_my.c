long rint_my (double x) {
        if (x>=0.0) return  (long)x == (long)(x+0.5) ? (long)x : (long)(x+0.5);
        else        return  (long)x == (long)(x-0.5) ? (long)x : (long)(x-0.5);
}
