
//---------------------------------------------------------------------------------------
//!	MyQuickSort function: sorting an array of strings
//!     1) Checking if array is sorted
//!     2) Checking if we need to sort only two elements
//!		3) Put the middle string (MS) in the beginning of array and compare each other string with it
//!         a) increase last (which responsible for border of elements who < MS and not checked (or checked, but >= MS))
//!         b) if string < MS => change it with the first element after others, which we already know they're < MS
//!     4) After checking all strings, change MS with the last element, which < MS
//!		5) Great! We have an array, where one part < MS, other one >= MS
//!     6) So we can recusively sort these parts and get fully sorted array in the end
//!
//! Optimisations:
//!     1) Three parted array (> p, == p, < p)
//!     2) At great depth of recursion, when small arrays we can call another (not recursion) sort. The best is InsertSort
//!     3) Strategy of choosing p (pivot)
//!         a) fixed (фиксированное) plfce in array
//!         b) random
//!         c) Medianofmedians
//!         d) Median
//!
//! @param [out] sarray     - pointer on the beginig of array with structures
//!	@param [in]  left       - left  border of sorting
//! @param [in]  right      - right border of sorting
//! @param [out] comparator - function, that compares two strings 
//!
//!	@return integer (0)
//!
//---------------------------------------------------------------------------------------
int MyQuickSort (int* sarray, int left, int right)
{
   
    if (right - left <= 0)
        return 0;
    if (right - left == 1)
        {
        if (sarray[left] > sarray[right])
            swap(sarray[left], sarray[right]);
        return 0;
        }
    
    
    swap( (sarray + left), (sarray + (left+right)/2) );
    
    int last = left;
    for (int i = left + 1; i <= right; i++)
        {
        if (Comp (sarray + left, sarray + i) > 0)
            {
            last++;
            Swap ((sarray + last), (sarray + i));
            }
        }
    
    Swap( (sarray + left), (sarray + last) );
    
    
    if (right - left <= 0)
        return 0;
    if (right - left == 1)
        {
        if (Comp (sarray + left, sarray + right) > 0)
            Swap ( (sarray + left),  (sarray + right));
        return 0;
        }
    
    MyQuickSort (sarray, left,     last,  Comp);
    MyQuickSort (sarray, last + 1, right, Comp);
        
}
