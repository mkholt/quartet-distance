#ifndef SODA_13_CALC_H
	#define SODA_13_CALC_H

	#include "int_stuff.h"
	#include "triplet_calc.h"
	#include "quartet_calc.h"
	#include "hdt.h"

	using namespace std;

	class Soda13Calc : public TripletCalc, public QuartetCalc
	{
		public:
			Soda13Calc();
			~Soda13Calc();
			INTTYPE_REST calculateTripletDistance(RootedTree *t1, RootedTree *t2);
			INTTYPE_N4 calculateQuartetDistance(UnrootedTree *t1, UnrootedTree *t2);
		private:
			HDTFactory *dummyHDTFactory;
			void countChildren(RootedTree *t);
			void count(RootedTree *v);
			//RootedTree* contract(RootedTree *v, int markedColor);
			RootedTree *t1, *t2;
			HDT *hdt;
			INTTYPE_REST resolvedTriplets, unresolvedTriplets;
#ifdef quartetsToo
			INTTYPE_N4 resolvedQuartetsAgree, resolvedQuartetsAgreeDiag, resolvedQuartetsDisagree, resolvedQuartetsDisagreeDiag;
			INTTYPE_N4 resolvedQuartetsAgreeUpper, resolvedQuartetsDisagreeUpper;
			INTTYPE_N4 unresolvedQuartets;
#endif
	};
#endif
