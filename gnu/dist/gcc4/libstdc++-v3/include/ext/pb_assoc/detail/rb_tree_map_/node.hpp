// -*- C++ -*-

// Copyright (C) 2005 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
// USA.

// As a special exception, you may use this file as part of a free software
// library without restriction.  Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License.  This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.

// Copyright (C) 2004 Ami Tavory and Vladimir Dreizin, IBM-HRL.

// Permission to use, copy, modify, sell, and distribute this software
// is hereby granted without fee, provided that the above copyright
// notice appears in all copies, and that both that copyright notice and
// this permission notice appear in supporting documentation. None of
// the above authors, nor IBM Haifa Research Laboratories, make any
// representation about the suitability of this software for any
// purpose. It is provided "as is" without express or implied warranty.

/**
 * @file node.hpp
 * Contains an implementation for rb_tree_.
 */

#ifndef RB_TREE_NODE_HPP
#define RB_TREE_NODE_HPP

namespace pb_assoc
{

  namespace detail
  {

    template<class Value_Type, class Allocator>
    struct rb_tree_node_
    {
    public:
      typedef
      typename Allocator::template rebind<
	rb_tree_node_<
	Value_Type,
	Allocator> >::other::pointer
      node_pointer;

    public:
      inline bool
      special_dec_check() const
      {
	return (m_red);
      }

    public:
      node_pointer m_p_left;
      node_pointer m_p_right;

      node_pointer m_p_parent;

      typedef Value_Type value_type;

      value_type m_value;

      bool m_red;
    };

  } // namespace detail

} // namespace pb_assoc

#endif // #ifndef RB_TREE_NODE_HPP
