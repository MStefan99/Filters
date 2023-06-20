//
// Created by MStefan99 on 18.7.21.
//

#include "Tester.hpp"

#include "FixMath.hpp"
#include "Matrix.hpp"


using namespace fix_math;

int main() {
	FIXED_TYPE maxPos {(1ll << (INTEGER_BITS - 1)) - 1};
	FIXED_TYPE maxNeg {(1ll << (INTEGER_BITS - 1)) * -1};

	auto maxPosFloat = static_cast<float>((1ll << (INTEGER_BITS - 1)) - 1);
	auto maxNegFloat = static_cast<float>((1ll << (INTEGER_BITS - 1)) * -1);

	std::cout << "FixMath options: " << std::endl;
	std::cout << "  Total bits: " << TOTAL_BITS << std::endl;
	std::cout << "  Integer bits: " << INTEGER_BITS << std::endl;
	std::cout << "  Fractional bits: " << FRACTIONAL_BITS << std::endl << std::endl;

	describe("Basic tests", [&](TestSuite& suite) {
		suite.test("Integer values", [] {
			FIXED_TYPE values[] {0, 1, -1};

			for (size_t i {0}; i < sizeof(values) / sizeof(values[0]); ++i) {
				expect(fixed(values[i]).toRaw()).toEqual(values[i] * ONE);
			}
		});

		suite.test("Max integer values", [&] {
			expect(fixed(maxPos).toRaw()).toEqual(maxPos * ONE);
			expect(fixed(maxNeg).toRaw()).toEqual(maxNeg * ONE);
		});

		suite.test("Float values", [] {
			float values[] {0, 1, -1};

			for (size_t i {0}; i < sizeof(values) / sizeof(values[0]); ++i) {
				expect(fixed(values[i]).toRaw()).toEqual(
						static_cast<FIXED_TYPE>(values[i]) * ONE);
			}
		});

		suite.test("Max float values", [&] {
			expect(fixed(maxPosFloat).toRaw()).toEqual(
					static_cast<FIXED_TYPE>(maxPosFloat) * ONE);
			expect(fixed(maxNegFloat).toRaw()).toEqual(
					static_cast<FIXED_TYPE>(maxNegFloat) * ONE);
		});

		suite.test("Boolean operators", [] {
			expect(fixed(1) == fixed(1)).toBeTruthy();
			expect(fixed(1) != fixed(1)).toBeFalsy();

			expect(fixed(1) == fixed(0)).toBeFalsy();
			expect(fixed(1) != fixed(0)).toBeTruthy();

			expect(fixed(1) > fixed(0)).toBeTruthy();
			expect(fixed(0) > fixed(1)).toBeFalsy();
			expect(fixed(1) > fixed(1)).toBeFalsy();

			expect(fixed(1) >= fixed(0)).toBeTruthy();
			expect(fixed(0) >= fixed(1)).toBeFalsy();
			expect(fixed(1) >= fixed(1)).toBeTruthy();

			expect(fixed(1) < fixed(0)).toBeFalsy();
			expect(fixed(0) < fixed(1)).toBeTruthy();
			expect(fixed(1) < fixed(1)).toBeFalsy();

			expect(fixed(1) <= fixed(0)).toBeFalsy();
			expect(fixed(0) <= fixed(1)).toBeTruthy();
			expect(fixed(1) <= fixed(1)).toBeTruthy();

		});

		suite.test("Arithmetic operations", [] {
			expect(fixed(0) + fixed(0)).toEqual(fixed(0));
			expect(fixed(6) + fixed(2)).toEqual(fixed(8));
			expect(fixed(6) - fixed(2)).toEqual(fixed(4));
			expect(fixed(6) * fixed(2)).toEqual(fixed(12));
			expect(fixed(6) / fixed(2)).toEqual(fixed(3));

			expect(fixed(1.5) + fixed(0.5)).toEqual(fixed(2));
			expect(fixed(1.5) - fixed(0.5)).toEqual(fixed(1));
			expect(fixed(1.5) * fixed(0.5)).toEqual(fixed(0.75));
			expect(fixed(1.5) / fixed(0.5)).toEqual(fixed(3));
		});

		suite.test("Trigonometric functions", [] {
			for (int i {-64}; i < 64; ++i) {
				double val = M_PI / 8 * i;

				expect(static_cast<double>(sin(fixed(val)))).toBeCloseTo(std::sin(val));
				expect(static_cast<double>(cos(fixed(val)))).toBeCloseTo(std::cos(val));
			}

			for (int i {-85}; i < 85; ++i) {
				double valX = static_cast<double>(i) / 100;

				expect(static_cast<double>(tan(fixed(valX)))).toBeCloseTo(std::tan(valX));
				expect(static_cast<double>(asin(fixed(valX)))).toBeCloseTo(std::asin(valX));
				expect(static_cast<double>(acos(fixed(valX)))).toBeCloseTo(std::acos(valX));
				expect(static_cast<double>(atan(fixed(valX)))).toBeCloseTo(std::atan(valX));
			}

			for (int i {-99}; i < 100; ++i) {
				double valX = static_cast<double>(i) / 100;
				double valY = sqrt(1 - valX * valX);

				expect(static_cast<double>(atan2(fixed(valY), fixed(valX)))).toBeCloseTo(std::atan2(valY, valX), 0.05);
				expect(static_cast<double>(atan2(-fixed(valY), fixed(valX)))).toBeCloseTo(std::atan2(-valY, valX), 0.05);
			}
		});
	});

	return 0;
}
