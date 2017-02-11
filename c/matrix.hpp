

#ifndef MATRIX_HPP_INCLUDED
#define MATRIX_HPP_INCLUDED

#include <vector>
#include "type_traits.hpp"

namespace snipts {

template <typename Integer = int>
class ref_count
{
public:
	typedef Integer counter_type;

	ref_count() : _cnt(1) {}
	~ref_count() {}

	void ref()   { ++_cnt; }
	void deref()
	{
		if ( --_cnt == 1 )
		{
			delete this;
		}
	}

	Integer count() { return _cnt; }

private:
	Integer	_cnt;
};


namespace storage_layout {

	//! layout category
	struct coo {};	//! coordinate
	struct csr {};	//! row major
	struct csc {};	//! column major

    template <typename layout>
    const char* to_string(const layout* = 0)
    {
        if ( is_same<layout, coo>::value )
        {
            return "coordinate layout";
        }

        if ( is_same<layout, csr>::value )
        {
            return "row-major layout";
        }

        if ( is_same<layout, csc>::value )
        {
            return "column-major layout";
        }

        return "unknown layout";
    }
} //namespace storage_layout

struct coordinate
{
    typedef std::size_t size_type;

    const size_type x;
    const size_type y;

    coordinate(const size_type _x = -1, const size_type _y = -1)
    : x(_x), y(_y)
    {
    }

    void move(const size_type dx, const size_type dy)
    {
        const_cast<size_type&>(x) = x + dx;
        const_cast<size_type&>(y) = y + dy;
    }

    coordinate& swap()
    {
        const size_type t_x = x;
        const_cast<size_type&>(x) = y;
        const_cast<size_type&>(y) = t_x;
        return *this;
    }

    bool is_valid() const
    {
        return x != size_type(-1) && y != size_type(-1);
    }
};


namespace detail {


struct storage_base
{
	typedef std::size_t             size_type;
	typedef std::vector<size_type>  index_vector;

    storage_base(size_type n_rows, size_type n_cols, size_type n_nnzs);

	size_type	size()   const	{ return _n_rows * _n_cols; 	}
	size_type	n_rows() const	{ return _n_rows;				}
	size_type	n_cols() const	{ return _n_cols;				}
	size_type	n_nzs()  const	{ return _row_index[_n_rows];	}

protected:
	index_vector	_row_index;
	index_vector    _col_index;
	size_type	    _n_rows;
	size_type	    _n_cols;
};

//default is not implemented
template <typename eT, typename layout>
class storage : public storage_base
{
public:
	typedef layout      layout_type;
	typedef std::size_t size_type;
	typedef eT          elem_type;
	typedef	eT*         pointer_type;


	~storage();
	storage(size_type n_rows, size_type n_cols, size_type n_nnzs = 0);

	size_type 	lindex(size_type i, size_type j) const;
	coordinate  coord(size_type k) const;
	eT*			operator()(size_type i, size_type j);
	eT* 		insert(eT v, size_type i, size_type j);
	size_type 	resize(size_type n_rows, size_type n_cols);
	void        delete_zeros(); //performance care
};

template <typename eT>
class storage<eT, storage_layout::csr> : public storage_base
{
public:
	typedef storage_layout::csr					layout_type;
	typedef storage_base                        base;
	typedef typename storage_base::size_type	size_type;
	typedef eT 									elem_type;
	typedef	eT*									pointer_type;


	~storage();
	storage(size_type n_rows, size_type n_cols, size_type n_nnzs = 0);

	size_type 	lindex(size_type i, size_type j) const;
	coordinate  coord(size_type k) const;
	eT*			operator()(size_type i, size_type j);
	eT* 		insert(const eT v, size_type i, size_type j);
	size_type 	resize(size_type n_rows, size_type n_cols);

    void        delete_zeros(); //performance care

private:
    size_type   find_insert_index(size_type i, size_type j) const;

	std::vector<eT> _value;
};

template <typename eT>
class storage<eT, storage_layout::csc>
{
public:
	typedef storage_layout::csr					layout_type;
	typedef storage_base                        base;
	typedef typename storage_base::size_type	size_type;
	typedef eT 									elem_type;
	typedef	eT*									pointer_type;


	~storage();
	storage(size_type n_rows, size_type n_cols, size_type n_nnzs = 0);

	size_type 	lindex(size_type i, size_type j) const;
	coordinate  coord(size_type k) const;
	eT*			operator()(size_type i, size_type j);
	eT* 		insert(const eT v, size_type i, size_type j);
	size_type 	resize(size_type n_rows, size_type n_cols);

	size_type	size()   const	{ return _M.size(); 	}
	size_type	n_rows() const	{ return _M.n_cols();	}
	size_type	n_cols() const	{ return _M.n_rows();	}
	size_type	n_nzs()  const	{ return _M.n_nzs();	}

    void        delete_zeros(); //performance care

private:
	storage<eT, storage_layout::csr> _M; //csc = transpose(csr)
};



//matrix proxy
template <typename mat>
struct sp_proxy
{
	typedef typename mat::size_type	size_type;
	typedef typename mat::elem_type	elem_type;

	sp_proxy(const size_type in_row, const size_type in_col, mat& m, elem_type* val = 0);
	sp_proxy& operator=(const elem_type val);

	operator elem_type() const;

private:
	const size_type _in_row;
	const size_type _in_col;
	mat&			_m;
	elem_type*		_val;
};

} //namespace detail



template <typename eT, typename layout = storage_layout::csr>
class sparse_matrix
{
public:
	typedef sparse_matrix<eT, layout>			self_type;
	typedef detail::storage<eT, layout>	        storage_type;
	typedef typename storage_type::size_type    size_type;
	typedef typename storage_type::elem_type    elem_type;


	~sparse_matrix();
	sparse_matrix();

	sparse_matrix(size_type n_rows, size_type n_cols, size_type n_nnzs = 16);

    detail::sp_proxy<const self_type> operator()(size_type i, size_type j) const;
    detail::sp_proxy<self_type> 	  operator()(size_type i, size_type j);

    size_type   n_nzs() const;  //!number of nonzeros
    size_type   n_rows() const; //!number of rows
    size_type   n_cols() const; //!number of columns

    void 		resize(size_type n_rows, size_type n_cols);
    void		reshape(size_type n_rows, size_type n_cols);

    void 		set(const eT v, size_type i, size_type j);

    coordinate  to_coordinate(const size_type k) const;

    void        delete_zeros(); //performance care

private:
	storage_type* _M;
};


template <typename eT>
sparse_matrix<eT, storage_layout::csc>
to_csc_layout(const sparse_matrix<eT, storage_layout::csr>& m);

template <typename eT>
sparse_matrix<eT, storage_layout::coo>
to_coo_layout(const sparse_matrix<eT, storage_layout::csr>& m);


}

#include "matrix_impl.hpp"

#endif
