// inside main():

int x1, x2;
    float y1, y2;
    float kx, ky, k, b;

    
    cout << "coordinates X1: & Y1: " << '\n';
    cin >> x1;
    cin >> y1;

    cout << "coordinates X2: & Y2: " << '\n';
    cin >> x2;
    cin >> y2;
    

    kx = x2 - x1;
    ky = y2 - y1;
    k = ky / kx;
    b = - (x1 * ky - y1 * kx) / kx;

    cout << k << '\n';
    cout << b << '\n';

    return 0;
