#ifndef PREFIX_HPP
#define PREFIX_HPP

/*
 * Takes any integer iterable and tests its prefix sums for non-negativity
 */
template<class I>
bool non_neg_prefix_sum(const I& lst)
{
    int sum = 0;
    for (const int& i : lst) {
        sum += i;
        if (sum < 0) {
            return false;
        }
    }
    return true;
}

/*
 * Takes any integer iterable and tests its prefix sums for non-positivity
 */
template<class I>
bool non_pos_prefix_sum(const I& lst)
{
    int sum = 0;
    for (const int& i : lst) {
        sum += i;
        if (sum > 0) {
            return false;
        }
    }
    return true;
}

#endif
