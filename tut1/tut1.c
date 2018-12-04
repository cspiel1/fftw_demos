#include <complex.h>
#include <fftw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

int main(int argc, char* argv[]) {
    fftw_complex *in, *out;
    fftw_plan p;
	int n = 128;
	int f = 440;
	int r = 16000;
	int m = 1;
	double a = 1.;

	/* Read input */
	int c;
	while (1) {
		c = getopt(argc, argv, "n:f:a:r:m:h");
		if (c==-1) break;

		switch (c) {
			case 'n':
				n = atoi(optarg);
				if (n<2)
					return 1;
				break;
			case 'f':
				f = atoi(optarg);
				if (f<0)
					return 1;
				break;
			case 'r':
				r = atoi(optarg);
				if (r<0)
					return 1;
				break;
			case 'm':
				m = atoi(optarg);
				if (m<0)
					return 1;
				break;
			case 'a':
				a = atof(optarg);
				if (a<1.0)
					return 1;
				break;
			case 'h':
				printf("usage: %s [options]\n"
						"	-n int     Integer which specifies the array length.\n"
						"	-f int     The frequency of the intput data.\n"
						"	-r int     The sample rate.\n"
						"	-a float   The amplitude.\n"
						, argv[0]);
				return 0;
		}
	}

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);

	/* Build input */
	double dt=1./r;
	double omega=2*f*M_PI;
	printf("Input:\n");
	int i;
	for (i=0; i<n; i++) {
		double t=i*dt;
		double y = a*sin(omega*t);
		in[i] = y;
/*        printf("%f %f %f\n", t, creal(in[i]), cimag(in[i]));*/
	}

    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n);
    p = fftw_plan_dft_1d(n, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

	for (int j=0; j<m; j++) {
		fftw_execute(p); /* repeat as needed */

		/* Print output */
		printf("Output Run %i:\n", j);
		for (i=0; i<n; i++) {
			int f=i*r/n;
			double a=creal(out[i])/n;
			double b=cimag(out[i])/n;
			double r=sqrt(a*a+b*b);
			printf("%d %f\n", f, r);
		}
	}

    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);

	return 0;
}
