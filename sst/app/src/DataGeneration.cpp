#include <vector>
#include <cassert>
#include <algorithm>
#include <iostream>
using namespace std;

#include "PrettyPrinting.h"

vector< vector< int> > generateFakeData(int r, int c, int ub) {
	vector< vector < int> > data;

	for (int i=0; i<r; ++i) {

		data.push_back(vector<int>(c));
		generate_n(data[i].begin(), c, rand);

		for (int j=0; j<c; ++j)
			data[i][j] %= ub;

	}

	return data;
}

/* ****************************************************************************** */
/* ****************************************************************************** */

vector< vector< vector<int> > > genData3x2() {

	vector< vector< vector<int> > > tmp;
	vector<int> used;

	for (unsigned int a=1; a<=6; ++a) {
		used.push_back(a);

		for (unsigned int b=1; b<=6; ++b) {
			if (a>b) {
				used.push_back(b);

				for (unsigned int c=1; c<=6; ++c) {
					if (find(used.begin(), used.end(), c) == used.end()) {
						used.push_back(c);

						for (unsigned int d=1; d<=6; ++d) {
							if ((c>d) && (find(used.begin(), used.end(), d) == used.end())) {
								used.push_back(d);

								for (unsigned int e=1; e<=6; ++e) {
									if (find(used.begin(), used.end(), e) == used.end()) {

										used.push_back(e);
										for (unsigned int f=1; f<=6; ++f) {
											if ((e>f) && (find(used.begin(), used.end(), f) == used.end())) {
												/*
												 * 												cout << "((" << a << "," << b << ")," << "(" << c << "," << d << "),"
												 << "(" << e << "," << f << "))" << endl;
												 */
												vector< vector<int> > outer(3);

												vector<int> inner1(2);
												vector<int> inner2(2);
												vector<int> inner3(2);

												inner1[0] = a;
												inner1[1] = b;
												outer[0] = inner1;
												inner2[0] = c;
												inner2[1] = d;
												outer[1] = inner2;
												inner3[0] = e;
												inner3[1] = f;
												outer[2] = inner3;
												tmp.push_back(outer);
											}
										}

										used.pop_back();
									}
								}
								used.pop_back();
							}
						}
						used.pop_back();
					}
				}
				used.pop_back();
			}
		}
		used.pop_back();
	}

	return tmp;
}

/* ****************************************************************************** */
/* ****************************************************************************** */

vector< vector< vector<int> > > genData3x3() {

	vector< vector< vector<int> > > tmp;
	vector<int> used;

	for (unsigned int a=1; a<=9; ++a) {
		used.push_back(a);

		for (unsigned int b=1; b<=9; ++b) {
			if (a>b) {
				used.push_back(b);

				for (unsigned int c=1; c<=9; ++c) {
					if (b>c) {
						used.push_back(c);

						for (unsigned int d=1; d<=9; ++d) {
							if ((find(used.begin(), used.end(), d) == used.end())) {
								used.push_back(d);

								for (unsigned int e=1; e<=9; ++e) {
									if ((d>e) && find(used.begin(), used.end(), e) == used.end()) {
										used.push_back(e);

										for (unsigned int f=1; f<=9; ++f) {
											if ((e>f) && (find(used.begin(), used.end(), f) == used.end())) {
												used.push_back(f);

												for (unsigned int g=1; g<=9; ++g) {
													if (find(used.begin(), used.end(), g) == used.end()) {
														used.push_back(g);

														for (unsigned int h=1; h<=9; ++h) {
															if ((g>h) && find(used.begin(), used.end(), h)
																	== used.end()) {
																used.push_back(h);

																for (unsigned int i=1; i<=9; ++i) {
																	if ((h>i) && (find(used.begin(), used.end(), i)
																			== used.end())) {
																		/*
																		 cout << "((" << a << "," << b << "," << c
																		 << ")," << "(" << d << "," << e << ","
																		 << f << ")," << "(" << g << "," << h
																		 << "," << i << "))" << endl;
																		 */
																		vector< vector<int> > outer(3);

																		vector<int> inner1(3);
																		vector<int> inner2(3);
																		vector<int> inner3(3);

																		inner1[0] = a;
																		inner1[1] = b;
																		inner1[2] = c;
																		outer[0] = inner1;
																		inner2[0] = d;
																		inner2[1] = e;
																		inner2[2] = f;
																		outer[1] = inner2;
																		inner3[0] = g;
																		inner3[1] = h;
																		inner3[2] = i;
																		outer[2] = inner3;
																		tmp.push_back(outer);
																	}
																}
																used.pop_back();
															}
														}
														used.pop_back();
													}
												}
												used.pop_back();
											}
										}

										used.pop_back();
									}
								}
								used.pop_back();
							}
						}
						used.pop_back();
					}
				}
				used.pop_back();
			}
		}
		used.pop_back();
	}

	return tmp;
}

/* ****************************************************************************** */
/* ****************************************************************************** */

vector< vector< vector<int> > > genData4x4() {

	vector< vector< vector<int> > > tmp;
	vector<int> used;

	///////////////////////////
	// start code for row 0

	for (unsigned int a=1; a<=16; ++a) {
		used.push_back(a);

		for (unsigned int b=1; b<=16; ++b) {
			if (a>b) {
				used.push_back(b);

				for (unsigned int c=1; c<=16; ++c) {
					if (b>c) {
						used.push_back(c);

						for (unsigned int d=1; d<=16; ++d) {
							if (c>d) {
								used.push_back(d);

								///////////////////////////
								// start code for row 1

								for (unsigned int e=1; e<=16; ++e) {
									if ((find(used.begin(), used.end(), e) == used.end())) {
										used.push_back(e);

										for (unsigned int f=1; f<=16; ++f) {
											if ((e>f) && find(used.begin(), used.end(), f) == used.end()) {
												used.push_back(f);

												for (unsigned int g=1; g<=16; ++g) {
													if ((f>g) && (find(used.begin(), used.end(), g) == used.end())) {
														used.push_back(g);

														for (unsigned int h=1; h<=16; ++h) {
															if ((g>h) && (find(used.begin(), used.end(), h)
																	== used.end())) {
																used.push_back(h);

																///////////////////////////
																// start code for row 2

																for (unsigned int i=1; i<=16; ++i) {
																	if (find(used.begin(), used.end(), i) == used.end()) {
																		used.push_back(i);

																		for (unsigned int j=1; j<=16; ++j) {
																			if ((i>j) && find(used.begin(), used.end(),
																					j) == used.end()) {
																				used.push_back(j);

																				for (unsigned int k=1; k<=16; ++k) {
																					if ((j>k) && (find(used.begin(),
																							used.end(), k)
																							== used.end())) {
																						used.push_back(k);

																						for (unsigned int l=1; l<=16; ++l) {
																							if ((k>l) && (find(
																									used.begin(),
																									used.end(), l)
																									== used.end())) {
																								used.push_back(l);

																								///////////////////////////
																								// start code for row 3

																								for (unsigned int m=1; m
																										<=16; ++m) {
																									if ((find(
																											used.begin(),
																											used.end(),
																											m)
																											== used.end())) {
																										used.push_back(m);

																										for (unsigned int
																												n=1; n
																												<=16; ++n) {
																											if ((m>n)
																													&& find(
																															used.begin(),
																															used.end(),
																															n)
																															== used.end()) {
																												used.push_back(n);

																												for (unsigned int
																														o=
																																1; o
																														<=16; ++o) {
																													if ((n
																															>o)
																															&& (find(
																																	used.begin(),
																																	used.end(),
																																	o)
																																	== used.end())) {
																														used.push_back(o);

																														for (unsigned int
																																p=
																																		1; p
																																<=16; ++p) {
																															if ((o
																																	>p)
																																	&& (find(
																																			used.begin(),
																																			used.end(),
																																			p)
																																			== used.end())) {

																																///////////////////////////
																																// populate the data

																																vector< vector<int> >
																																		outer(4);

																																vector<int>
																																		inner1(4);
																																vector<int>
																																		inner2(4);
																																vector<int>
																																		inner3(4);
																																vector<int>
																																		inner4(4);

																																inner1[0]
																																		= a;
																																inner1[1]
																																		= b;
																																inner1[2]
																																		= c;
																																inner1[3]
																																		= d;
																																outer[0]
																																		= inner1;
																																inner2[0]
																																		= e;
																																inner2[1]
																																		= f;
																																inner2[2]
																																		= g;
																																inner2[3]
																																		= h;
																																outer[1]
																																		= inner2;
																																inner3[0]
																																		= i;
																																inner3[1]
																																		= j;
																																inner3[2]
																																		= k;
																																inner3[3]
																																		= l;
																																outer[2]
																																		= inner3;
																																inner4[0]
																																		= m;
																																inner4[1]
																																		= n;
																																inner4[2]
																																		= o;
																																inner4[3]
																																		= p;
																																outer[3]
																																		= inner4;
																																tmp.push_back(outer);

																																///////////////////////////
																																// done with the data

																																cout << toString(outer) << endl;
																																
																															}
																														}
																														used.pop_back();

																													}
																												}
																												used.pop_back();
																											}
																										}
																										used.pop_back();
																									}
																								}
																								used.pop_back();

																								///////////////////////////
																								//done with row 3

																							}
																						}
																						used.pop_back();

																					}
																				}
																				used.pop_back();
																			}
																		}
																		used.pop_back();
																	}
																}
																used.pop_back();

																///////////////////////////
																// done with row 2

															}
														}
														used.pop_back();

													}
												}

												used.pop_back();
											}
										}
										used.pop_back();
									}
								}
								used.pop_back();

								///////////////////////////
								// done with row 1

							}
						}
						used.pop_back();

					}
				}
				used.pop_back();
			}
		}
		used.pop_back();
	}

	///////////////////////////
	// done with row 0

	return tmp;
}

/* ****************************************************************************** */
/* ****************************************************************************** */

vector< vector< vector<int> > > genData(unsigned int r, unsigned int c) {

	if (r == 3 && c == 2)
		return genData3x2();
	if (r == 3 && c == 3)
		return genData3x3();
	if (r==4 && c == 4)
		return genData4x4();

	assert(false);
}

/* ****************************************************************************** */
/* ****************************************************************************** */
