//matrix_impl.hpp
#include <algorithm>

namespace snipts {
namespace detail {

storage_base::storage_base(size_type n_rows, size_type n_cols, size_type n_nnzs)
: _row_index(n_rows + 1, 0), _col_index(n_nnzs, 0)
{
    _n_rows    = n_rows;
    _n_cols    = n_cols;
}

template <typename eT>
storage<eT, storage_layout::csr>::~storage()
{
}

template <typename eT>
storage<eT, storage_layout::csr>::storage(size_type n_rows, size_type n_cols, size_type n_nnzs)
: storage_base(n_rows, n_cols, n_nnzs), _value(n_nnzs, 0)
{

}

template <typename eT>
typename storage<eT, storage_layout::csr>::size_type
storage<eT, storage_layout::csr>::lindex(size_type i, size_type j) const
{
    const size_type in_row   = _row_index[i];
    const size_type next_row = _row_index[i+1];

    for (size_type k = in_row; k < next_row; ++k)
    {
        const size_type col_index = _col_index[k];
        if ( col_index > j )
        {
            return -1; //do not exist
        }
        else if( col_index == j )
        {
            return k;
        }
    }

    //do not exist
    return -1;
}

template <typename eT>
typename storage<eT, storage_layout::csr>::size_type
storage<eT, storage_layout::csr>::find_insert_index(size_type i, size_type j) const
{
    const size_type in_row   = _row_index[i];
    const size_type next_row = _row_index[i+1];

    for (size_type k = in_row; k < next_row; ++k)
    {
        const size_type col_index = _col_index[k];
        if ( col_index > j )
        {
            return k;
        }
    }

    return next_row;
}

template <typename eT>
eT*
storage<eT, storage_layout::csr>::operator()(size_type i, size_type j)
{
    const size_type k = lindex(i, j);
    if ( k == size_type(-1) )
    {
        //do no exist
        return 0;
    }
    else
    {
        return &_value[k];
    }
}


template <typename eT>
eT*
storage<eT, storage_layout::csr>::insert(const eT v, size_type i, size_type j)
{
    size_type k = lindex(i, j);
    if ( k != size_type(-1) )
    {
        _value[k] = v;
        return &_value[k];
    }

    k = find_insert_index(i, j);

    _value.insert(_value.begin() + k, v); //compiler cannot deduce insert(k, v)
    _col_index.insert(_col_index.begin() + k, j); //compiler cannot deduce insert(k, j)

    //updates the row_index
    for (size_type x = i + 1; x <= _n_rows; ++x)
    {
        ++_row_index[x];
    }

    return &_value[k];
}


template <typename mat>
sp_proxy<mat>::sp_proxy(const size_type in_row, const size_type in_col, mat& m, elem_type* val)
: _in_row(in_row), _in_col(in_col), _m(m), _val(val)
{
}

template <typename mat>
sp_proxy<mat>& sp_proxy<mat>::operator=(const elem_type val)
{
    if ( _val )
    {
        *_val = val;
    }

    //only insert non zero value
    if ( val != elem_type() )
    {
        _m.set(val, _in_row, _in_col);
    }

    return *this;
}

template <typename mat>
sp_proxy<mat>::operator elem_type() const
{
    if ( _val )
    {
        return *_val;
    }
    else
    {
        return elem_type();
    }
}

}//namespace detail


template <typename eT, typename layout>
sparse_matrix<eT, layout>::~sparse_matrix()
{
    if ( _M )
    {
        delete _M;
    }
}

template <typename eT, typename layout>
sparse_matrix<eT, layout>::sparse_matrix()
: _M(new storage_type(0, 0, 0))
{

}

template <typename eT, typename layout>
sparse_matrix<eT, layout>::sparse_matrix(size_type n_rows, size_type n_cols, size_type n_nnzs)
: _M(new storage_type(n_rows, n_cols, n_nnzs))
{

}

template <typename eT, typename layout>
detail::sp_proxy<const sparse_matrix<eT, layout> >
sparse_matrix<eT, layout>::operator()(size_type i, size_type j) const
{
    return detail::sp_proxy<const self_type>(i, j, *this, (*_M)(i, j));
}

template <typename eT, typename layout>
detail::sp_proxy<sparse_matrix<eT, layout> >
sparse_matrix<eT, layout>::operator()(size_type i, size_type j)
{
    return detail::sp_proxy<self_type>(i, j, *this, (*_M)(i, j));
}

template <typename eT, typename layout>
void 
sparse_matrix<eT, layout>::set(const eT val, size_type i, size_type j)
{
    eT* v = (*_M)(i, j);

    //!TODO: remove zeros values
    //update values
    if ( v )
    {
        *v = val;
    }
    else
    {
        //insert value
        _M->insert(val, i, j);
    }
}

template <typename eT, typename layout>
typename sparse_matrix<eT, layout>::size_type
sparse_matrix<eT, layout>::n_nzs() const
{
    return _M->n_nzs();
}

template <typename eT, typename layout>
typename sparse_matrix<eT, layout>::size_type
sparse_matrix<eT, layout>::n_rows() const
{
    return _M->n_rows();
}

template <typename eT, typename layout>
typename sparse_matrix<eT, layout>::size_type
sparse_matrix<eT, layout>::n_cols() const
{
    return _M->n_cols();
}

} //namespace snipts

