// algorithm standard header

#ifndef ALGORITHM
#define ALGORITHM

namespace MyNamespace
{
	template< typename RanIt, typename Ty, typename Pr >
	inline void pushHeapByIndex(RanIt first, ptrdiff_t hole, ptrdiff_t top, Ty& val, Pr pred)
	{
		// top => root 
		// pred => 比較大小
		// hole = currentNode 
		// index = currentNode / 2 

		// 這裡-1 是因為是從0開始放
		// 上課的簡報是從1開始放

		while (top != hole)
		{
			if (pred(first[(hole - 1) / 2], val))
			{
				first[hole] = first[(hole - 1) / 2];
				hole = (hole - 1) / 2;
			}
			else
				break;
		}

		first[hole] = val;
	}

	// push *(last - 1) onto heap at [first, last - 1), using pred
	template< typename RanIt, typename Pr >
	inline void push_heap(RanIt first, RanIt last, Pr pred)
	{
		ptrdiff_t count = last - first;
		if (count >= 2)
		{
			--last;
			typename RanIt::value_type val = *last;
			--count;
			pushHeapByIndex(first, count, 0, val, pred);
		}
	}

	template< typename RanIt, typename Ty, typename Pr >
	inline void popHeapHoleByIndex(RanIt first, ptrdiff_t hole, ptrdiff_t bottom, Ty& val, Pr pred)
	{
		ptrdiff_t idx = hole * 2 + 1;
		while (idx < bottom) {
			idx = (idx + 1 < bottom && !pred(first[idx + 1], first[idx])) ? idx + 1 : idx;
			if (pred(val, first[idx])) {
				first[hole] = first[idx];
				hole = idx;
				idx = idx * 2 + 1;
			}
			else break;
		}
		first[hole] = val;

	}

	// pop *first to *(last - 1) and reheap, using pred
	template< typename RanIt, typename Pr >
	inline void pop_heap(RanIt first, RanIt last, Pr pred)
	{
		if (last - first >= 2)
		{
			--last;
			typename RanIt::value_type val = *last;
			popHeapHoleByIndex(first, 0, last - first, val, pred);
		}
	}

	// make [first, last) into a heap, using pred
	template< typename RanIt, typename Pr >
	inline void make_heap(RanIt first, RanIt last, Pr pred)
	{
		ptrdiff_t bottom = last - first;
		ptrdiff_t hole = bottom / 2;
		while (hole > 0)
		{
			// reheap top half, bottom to top
			--hole;
			typename RanIt::value_type val = *(first + hole);
			popHeapHoleByIndex(first, hole, bottom, val, pred);
		}
	}
}

#endif // ALGORITHM