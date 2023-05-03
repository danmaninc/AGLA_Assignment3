#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
namespace MatrixUtils {
    class Matrix {
    public:
        int n;
        int m;
        vector<vector<double>> data;

        Matrix(int n, int m) {
            this->n = n;
            this->m = m;
            this->data = *new vector<vector<double>>(n, vector<double>(m));
        }

        Matrix operator+(Matrix b) const {
            if (this->n != b.n || this->m != b.m) {
                cout << "Error: the dimensional problem occurred" << endl;
                Matrix result = *new Matrix(0, 0);
                return result;
            }
            Matrix result = *new Matrix(n, m);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    result.data[i].at(j) = this->data[i].at(j) + b.data[i].at(j);
                }
            }
            return result;
        }
        Matrix operator-(Matrix b) const {
            if (this->n != b.n || this->m != b.m) {
                cout << "Error: the dimensional problem occurred" << endl;
                Matrix result = *new Matrix(0, 0);
                return result;
            }
            Matrix result = *new Matrix(n, m);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    result.data[i].at(j) = this->data[i].at(j) - b.data[i].at(j);
                }
            }
            return result;
        }
        Matrix operator*(Matrix b) const {
            if (this->m != b.n) {
                cout << "Error: the dimensional problem occurred" << endl;
                Matrix result = *new Matrix(0, 0);
                return result;
            }
            Matrix result = *new Matrix(this->n, b.m);
            for (int i = 0; i < result.n; i++) {
                for (int j = 0; j < result.m; j++) {
                    double sum = 0;
                    for (int k = 0; k < b.n; k++) {
                        sum += this->data[i].at(k) * b.data[k].at(j);
                    }
                    result.data[i].at(j) = sum;
                }
            }
            return result;
        }
        Matrix operator!() const {
            Matrix result = *new Matrix(this->m, this->n);
            for (int i = 0; i < result.n; i++) {
                for (int j = 0; j < result.m; j++) {
                    result.data[i].at(j) = this->data[j].at(i);
                }
            }
            return result;
        }
        void operator=(Matrix b) {
            this->n = b.n;
            this->m = b.m;
            this->data = b.data;
        }

        friend ostream& operator<<(ostream& out, Matrix mtrx) {
            for (int i = 0; i < mtrx.n; i++) {
                for (int j = 0; j < mtrx.m; j++) {
                    cout << fixed << mtrx.data[i].at(j) << " ";
                }
                cout << endl;
            }
            return cout;
        };

        friend Matrix operator<<(Matrix mtrx1, Matrix mtrx2) {
            int M_new = mtrx1.m + mtrx2.m;
            Matrix composed = *new Matrix(mtrx1.n, M_new);
            for (int i = 0; i < mtrx1.n; i++) {
                int k = 0;
                for (int j = 0; j < mtrx1.m; j++) {
                    composed.data[i].at(j) = mtrx1.data[i].at(j);
                    k++;
                }
                for (int j = 0; j < mtrx2.m; j++) {
                    composed.data[i].at(k) = mtrx2.data[i].at(j);
                    k++;
                }
            }
            return composed;
        }

        friend istream& operator>>(istream& in, Matrix& mtrx) {
            string num;
            for (int i = 0; i < mtrx.n; i++) {
                for (int j = 0; j < mtrx.m; j++) {
                    cin >> num;
                    mtrx.data[i].at(j) = (stod(num));
                }
            }
            return cin;
        };
    };
}
using namespace MatrixUtils;
using namespace std;
#ifdef WIN32
#define GNUPLOT_NAME "C:\\gnuplot\\bin\\gnuplot -persist"
#else
#define GNUPLOT_NAME "gnuplot -persist"
#endif
int main() {
#ifdef WIN32
    FILE* pipe = _popen(GNUPLOT_NAME, "w");
#else
    FILE* pipe = popen(GNUPLOT_NAME, "w");
#endif
    cout.precision(2);
    double v0, k0, a1, b1, a2, b2, T, N;
    cin >> v0;
    cin >> k0;
    cin >> a1;
    cin >> b1;
    cin >> a2;
    cin >> b2;
    cin >> T;
    cin >> N;


    double step = (double)T/(double)N;
    Matrix t = *new Matrix(1, N+1);
    double curTime = 0;
    for (int i = 0; i <= N; i++) {
        t.data[0].at(i) = curTime;
        curTime += step;
    }
    cout << "t:" << endl;
    cout << t;

    Matrix v = *new Matrix(1, N+1);
    v.data[0].at(0) = v0;

    Matrix k = *new Matrix(1, N+1);
    k.data[0].at(0) = k0;
    for (int i = 1; i <= N; i++) {
        v.data[0].at(i) = (v0 - (a2/b2))*(cos(sqrt(a1*a2)*t.data[0].at(i))) - (k0 - (a1/b1))*((sqrt(a2)*b1)/(b2*sqrt(a1)))*sin(sqrt(a1*a2)*t.data[0].at(i));
        k.data[0].at(i) = (v0 - (a2/b2))*((sqrt(a1)*b2)/(b1*sqrt(a2)))*sin(sqrt(a1*a2)*t.data[0].at(i)) + (k0 - (a1/b1))*(cos(sqrt(a1*a2)*t.data[0].at(i)));
    }
    Matrix V = *new Matrix(1, N+1);
    Matrix K = *new Matrix(1, N+1);
    K.data[0].at(0) = 0;
    V.data[0].at(0) = 0;
    for (int i = 1; i <= N; i++) {
        K.data[0].at(i) = (a1/b1);
        V.data[0].at(i) = (a2/b2);
    }
    v = v + V;
    k = k + K;
    cout << "v:" << endl;
    cout << v;

    cout << "k:" << endl;
    cout << k;

    fprintf(pipe, "plot '-' using 1:2 t 'v(t)' with lines, '' using 1:2 t 'k(t)' with lines\n");
    for (int i = 0; i <= N; i++) {
        fprintf(pipe, "%f\t%f\n", t.data[0].at(i), v.data[0].at(i));
    }
    fprintf(pipe, "e\n");
    for (int i = 0; i <= N; i++) {
        fprintf(pipe, "%f\t%f\n", t.data[0].at(i), k.data[0].at(i));
    }
    fprintf(pipe, "e\n");
    fflush(pipe);
#ifdef WIN32
    _pclose(pipe);
#else
    pclose(pipe);
#endif
    pipe = _popen(GNUPLOT_NAME, "w");
    fprintf(pipe, "plot '-' using 1:2 t 'v(k)' with lines\n");
    for (int i = 0; i <= N; i++) {
        fprintf(pipe, "%f\t%f\n", k.data[0].at(i), v.data[0].at(i));
    }
    fprintf(pipe, "e\n");
    fflush(pipe);
#ifdef WIN32
    _pclose(pipe);
#else
    pclose(pipe);
#endif
}
