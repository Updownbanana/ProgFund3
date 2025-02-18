#include <iostream>
#include <cassert>
#include <algorithm>

// assume arr is sorted
int binarySearch(const int arr[], int start, int end, int target) {
	int mid = start + (end - start) / 2;

	if (end < start) {
		return -1;
	}

	if (arr[mid] == target) {
		return arr[mid];
	}

	if (target < arr[mid]) {
		return binarySearch(arr, start, mid - 1, target);
	}
	else {
		return binarySearch(arr, mid + 1, end, target);
	}
}

void testBinarySearch() {
	int arr_00[1] = {};
	assert(binarySearch(arr_00, 0, -1, 10) == -1);

	int arr_01[] = { 1 };
	assert(binarySearch(arr_01, 0, 0, 1) == 1);

	int arr_02[] = { 1, 5, 8, 10, 30 };
	assert(binarySearch(arr_02, 0, 4, 10) == 10);

	int arr_03[] = { 4, 6, 8, 9 };
	assert(binarySearch(arr_03, 0, 3, 6) == 6);
}

// assume arr is not empty
int findMax(const int arr[], int start, int end) {
	if (start == end) {
		return arr[start];
	}

	int mid = start + (end - start) / 2;

	return std::max(
		findMax(arr, start, mid),
		findMax(arr, mid + 1, end)
	);
}

void testFindMax() {
	int arr_00[] = { 1 };
	assert(findMax(arr_00, 0, 0) == 1);

	int arr_01[] = { 4, 19, 38, 364, 38 };
	assert(findMax(arr_01, 0, 4) == 364);
}

// assume month > 1
int rabbit(int month) {
	if (month <= 2) {
		return 1;
	}
	else {
		return rabbit(month - 1) + rabbit(month - 2);
	}
}

void main() {
	testFindMax();
}