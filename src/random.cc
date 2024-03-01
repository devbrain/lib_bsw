//
// Created by igor on 3/1/24.
//
#include <ctime>
#include <bsw/exception.hh>
#include <bsw/random.hh>
#include <bsw/io/random_stream.hh>

#define    TYPE_0        0        /* linear congruential */
#define    BREAK_0        8
#define    DEG_0        0
#define    SEP_0        0

#define    TYPE_1        1        /* x**7 + x**3 + 1 */
#define    BREAK_1        32
#define    DEG_1        7
#define    SEP_1        3

#define    TYPE_2        2        /* x**15 + x + 1 */
#define    BREAK_2        64
#define    DEG_2        15
#define    SEP_2        1

#define    TYPE_3        3        /* x**31 + x**3 + 1 */
#define    BREAK_3        128
#define    DEG_3        31
#define    SEP_3        3

#define    TYPE_4        4        /* x**63 + x + 1 */
#define    BREAK_4        256
#define    DEG_4        63
#define    SEP_4        1

namespace bsw {
	random::random (int stateSize) {
		ENFORCE (BREAK_0 <= stateSize && stateSize <= BREAK_4);

		_pBuffer = new char[stateSize];

		initState ((uint32_t)std::time (NULL), _pBuffer, stateSize);

	}

	random::~random () {
		delete[] _pBuffer;
	}

/*
 * Compute x = (7^5 * x) mod (2^31 - 1)
 * wihout overflowing 31 bits:
 *      (2^31 - 1) = 127773 * (7^5) + 2836
 * From "Random number generators: good ones are hard to find",
 * Park and Miller, Communications of the ACM, vol. 31, no. 10,
 * October 1988, p. 1195.
 */
	inline uint32_t random::goodRand (int32_t x) {
		int32_t hi, lo;

		if (x == 0) { x = 123459876; }
		hi = x / 127773;
		lo = x % 127773;
		x = 16807 * lo - 2836 * hi;
		if (x < 0) { x += 0x7FFFFFFF; }

		return x;
	}

/*
 * Initialize the random number generator based on the given seed.  If the
 * type is the trivial no-state-information type, just remember the seed.
 * Otherwise, initializes state[] based on the given "seed" via a linear
 * congruential generator.  Then, the pointers are set to known locations
 * that are exactly rand_sep places apart.  Lastly, it cycles the state
 * information a given number of times to get rid of any initial dependencies
 * introduced by the L.C.R.N.G.  Note that the initialization of randtbl[]
 * for default usage relies on values produced by this routine.
 */
	void random::seed (uint32_t x) {
		int i, lim;

		_state[0] = x;
		if (_randType == TYPE_0) {
			lim = NSHUFF;
		} else {
			for (i = 1; i < _randDeg; i++) {
				_state[i] = goodRand (_state[i - 1]);
			}
			_fptr = &_state[_randSep];
			_rptr = &_state[0];
			lim = 10 * _randDeg;
		}
		for (i = 0; i < lim; i++) {
			next ();
		}
	}

/*
 * Many programs choose the seed value in a totally predictable manner.
 * This often causes problems.  We seed the generator using the much more
 * secure random(4) interface.  Note that this particular seeding
 * procedure can generate states which are impossible to reproduce by
 * calling srandom() with any value, since the succeeding terms in the
 * state buffer are no longer derived from the LC algorithm applied to
 * a fixed seed.
 */
	void random::seed () {
		std::streamsize len;

		if (_randType == TYPE_0) {
			len = sizeof _state[0];
		} else {
			len = _randDeg * sizeof _state[0];
		}

		io::random_input_stream rstr;
		rstr.read ((char*)_state, len);
	}

/*
 * Initialize the state information in the given array of n bytes for future
 * random number generation.  Based on the number of bytes we are given, and
 * the break values for the different R.N.G.'s, we choose the best (largest)
 * one we can and set things up for it.  srandom() is then called to
 * initialize the state information.
 *
 * Note that on return from srandom(), we set state[-1] to be the type
 * multiplexed with the current value of the rear pointer; this is so
 * successive calls to initstate() won't lose this information and will be
 * able to restart with setstate().
 *
 * Note: the first thing we do is save the current state, if any, just like
 * setstate() so that it doesn't matter when initstate is called.
 *
 * Returns a pointer to the old state.
 *
 * Note: The Sparc platform requires that arg_state begin on an int
 * word boundary; otherwise a bus error will occur. Even so, lint will
 * complain about mis-alignment, but you should disregard these messages.
 */
	void random::initState (uint32_t s, char* argState, int32_t n) {
		auto* intArgState = (uint32_t*)argState;

		if (n < BREAK_0) {
			RAISE_EX("not enough state");
		}
		if (n < BREAK_1) {
			_randType = TYPE_0;
			_randDeg = DEG_0;
			_randSep = SEP_0;
		} else if (n < BREAK_2) {
			_randType = TYPE_1;
			_randDeg = DEG_1;
			_randSep = SEP_1;
		} else if (n < BREAK_3) {
			_randType = TYPE_2;
			_randDeg = DEG_2;
			_randSep = SEP_2;
		} else if (n < BREAK_4) {
			_randType = TYPE_3;
			_randDeg = DEG_3;
			_randSep = SEP_3;
		} else {
			_randType = TYPE_4;
			_randDeg = DEG_4;
			_randSep = SEP_4;
		}
		_state = intArgState + 1; /* first location */
		_endPtr = &_state[_randDeg];    /* must set end_ptr before seed */
		seed (s);
		if (_randType == TYPE_0) {
			intArgState[0] = _randType;
		} else {
			intArgState[0] = MAX_TYPES * (int)(_rptr - _state) + _randType;
		}
	}

/*
 * Next:
 *
 * If we are using the trivial TYPE_0 R.N.G., just do the old linear
 * congruential bit.  Otherwise, we do our fancy trinomial stuff, which is
 * the same in all the other cases due to all the global variables that have
 * been set up.  The basic operation is to add the number at the rear pointer
 * into the one at the front pointer.  Then both pointers are advanced to
 * the next location cyclically in the table.  The value returned is the sum
 * generated, reduced to 31 bits by throwing away the "least random" low bit.
 *
 * Note: the code takes advantage of the fact that both the front and
 * rear pointers can't wrap on the same call by not testing the rear
 * pointer if the front one has wrapped.
 *
 * Returns a 31-bit random number.
 */
	uint32_t random::next () {
		uint32_t i;
		uint32_t* f, * r;

		if (_randType == TYPE_0) {
			i = _state[0];
			_state[0] = i = goodRand (i) & 0x7FFFFFFF;
		} else {
			/*
			 * Use local variables rather than static variables for speed.
			 */
			f = _fptr;
			r = _rptr;
			*f += *r;
			i = (*f >> 1) & 0x7FFFFFFF;    /* chucking least random bit */
			if (++f >= _endPtr) {
				f = _state;
				++r;
			} else if (++r >= _endPtr) {
				r = _state;
			}

			_fptr = f;
			_rptr = r;
		}
		return i;
	}

}