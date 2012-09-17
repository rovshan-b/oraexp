#ifndef TRIPLE_H
#define TRIPLE_H

#ifndef QT_H
#endif // QT_H

template <class T1, class T2, class T3>
struct Triple
{
    typedef T1 first_type;
    typedef T2 second_type;
    typedef T3 third_type;

    Triple()
    : first( T1() ), second( T2() ), third( T3() )
    {}
    Triple( const T1& t1, const T2& t2, const T3& t3 )
    : first( t1 ), second( t2 ), third( t3 )
    {}

    T1 first;
    T2 second;
    T3 third;
};

template <class T1, class T2, class T3>
inline bool operator==( const Triple<T1, T2, T3>& x, const Triple<T1, T2, T3>& y )
{
    return x.first == y.first && x.second == y.second && x.third == y.third;
}

template <class T1, class T2, class T3>
inline bool operator<( const Triple<T1, T2, T3>& x, const Triple<T1, T2, T3>& y )
{
    // x < y is TRUE if:
    // x.first is less than y.first,
    // or if x.second is less than y.second and x.first is the same as y.first
    // or if x.third is less than y.third and both x.first and x.second are the same as y.first and y.second respectively
    return x.first < y.first || ( !( y.first < x.first ) && x.second < y.second ) ||
           (!( y.first < x.first) && !( y.second < x.second ) && x.third < y.third );
}

template <class T1, class T2, class T3>
inline Triple<T1, T2, T3> makeTriple( const T1& x, const T2& y, const T3& z )
{
    return Triple<T1, T2, T3>( x, y, z );
}

#endif
