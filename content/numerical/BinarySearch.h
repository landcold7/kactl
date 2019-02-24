
// Binary search on float numbers.
int binary_search(vector<double>& v, double x) {
    int lo = -1, hi = sz(v) - 1;
    while (hi - lo > 1) {
        int mid = lo + (hi - lo) / 2;
        if (v[mid] - x >= 0) {
            hi = mid;
        } else {
            lo = mid;
        }
    }
    return hi;
}
